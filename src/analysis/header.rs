//! Parse the header dependency
//! For header files of a library, typically there are the dependency between them.
//! For example, libpng, the header file "pnglibconf.h" record the configuration of the library and "png.h" rely on it.
//! Such config-like files should be include by other headers and should not be included directly, otherwise the gadgets (apis, types and macros) parsed from headers files might be inconsisent with the built binary library.
//! The get_include_lib_headers() returns the top-level header files that a program should include.
//! The get_include_sys_headers() returns the system header files used in this library.

use crate::{config::get_library_name, deopt::Deopt, execution::Executor};
use eyre::Result;
use once_cell::sync::OnceCell;
use std::{
    path::Path,
    process::{Command, Stdio},
};

use super::WorkList;

#[derive(Debug)]
struct TreeNode {
    name: String,
    children: Vec<TreeNode>,
}

impl TreeNode {
    fn new(name: String) -> Self {
        let name = name.replace("./", "");
        Self {
            name,
            children: Vec::new(),
        }
    }

    fn new_invalid() -> Self {
        Self {
            name: "invalid".to_string(),
            children: Vec::new(),
        }
    }

    fn is_invalid(&self) -> bool {
        self.name == "invalid"
    }

    fn set_name(&mut self, name: String) {
        self.name = name;
    }

    fn add_child(&mut self, child: TreeNode) {
        self.children.push(child)
    }

    fn get_name(&self) -> &str {
        &self.name
    }

    fn get_clean_root(&mut self, deopt: &Deopt) {
        let binding = deopt.get_library_build_header_path().unwrap();
        let include_path = binding.to_str().unwrap();
        let mut work_list = WorkList::new();
        work_list.push(self);
        while !work_list.empty() {
            let node = work_list.pop();
            let name = node.get_name();
            if name.starts_with(include_path) {
                let name = name.strip_prefix(include_path).unwrap();
                let name = name.strip_prefix('/').unwrap();
                node.set_name(name.to_string());
            }
            for child in &mut node.children {
                work_list.push(child);
            }
        }
    }
}

impl Executor {
    fn extract_header_dependency(&self, header: &Path) -> Result<TreeNode> {
        let header_path = self.deopt.get_library_build_header_path()?;
        let output = Command::new("clang++")
            .current_dir(&header_path)
            .arg("-fsyntax-only")
            .arg("-H")
            .arg("-I.")
            .arg(header)
            .stdout(Stdio::null())
            .stderr(Stdio::piped())
            .output()
            .expect("fail to extract header dependency");
        if !output.status.success() {
            log::trace!("{}", String::from_utf8_lossy(&output.stderr));
            return Ok(TreeNode::new_invalid());
        }
        let base_name = header
            .to_str()
            .unwrap()
            .strip_prefix(header_path.to_str().unwrap())
            .unwrap();
        let base_name = [".", base_name].concat();
        let output = String::from_utf8_lossy(&output.stderr).to_string();
        let mut tree = parse_dependency_tree(&output, &base_name)?;
        tree.get_clean_root(&self.deopt);
        Ok(tree)
    }
}

fn get_layer_child(layered_nodes: Vec<(usize, &str)>, depth: usize) -> Vec<TreeNode> {
    let mut seq_each_layer = Vec::new();
    let mut layer_seqs = Vec::new();
    for (layer, node) in layered_nodes {
        if layer == depth {
            seq_each_layer.push(layer_seqs);
            layer_seqs = Vec::new();
        }
        layer_seqs.push((layer, node));
    }
    seq_each_layer.push(layer_seqs);
    seq_each_layer.retain(|x| !x.is_empty());

    let mut childs = Vec::new();
    for seq in seq_each_layer {
        let root = seq[0];
        let mut root = TreeNode::new(root.1.to_string());
        for child in get_layer_child(seq[1..].to_vec(), depth + 1) {
            root.add_child(child);
        }
        childs.push(root);
    }
    childs
}

fn parse_dependency_tree(output: &str, base_name: &str) -> Result<TreeNode> {
    let mut node_layer: Vec<(usize, &str)> = Vec::new();
    for line in output.lines() {
        let sep = line
            .find(' ')
            .ok_or_else(|| eyre::eyre!("Expect an spece in line: {line}"))?;
        let layer = sep;
        let header = line[sep..].trim();
        node_layer.push((layer, header));
    }
    let mut tree = TreeNode::new(base_name.to_owned());
    for child in get_layer_child(node_layer, 1) {
        tree.add_child(child);
    }
    Ok(tree)
}

fn is_header_in_tree(tree: &TreeNode, name: &str) -> bool {
    let mut worklist = WorkList::new();
    worklist.push(tree);
    while !worklist.empty() {
        let node = worklist.pop();
        if node.get_name() == name {
            return true;
        }
        for child in &node.children {
            worklist.push(child);
        }
    }
    false
}

fn get_independent_headers(trees: &[TreeNode]) -> Result<Vec<&str>> {
    let mut indep_headers = Vec::new();
    for (i, tree) in trees.iter().enumerate() {
        let name = tree.get_name();
        let mut find = false;
        for (j, j_tree) in trees.iter().enumerate() {
            if j == i {
                continue;
            }
            if is_header_in_tree(j_tree, name) {
                find = true;
                break;
            }
        }
        if !find {
            indep_headers.push(name);
        }
    }
    Ok(indep_headers)
}

fn is_a_lib_header(name: &str) -> bool {
    !name.starts_with('/')
}

fn get_included_sys_header(tree: &TreeNode) -> Vec<&str> {
    let mut worklist = WorkList::new();
    worklist.push(tree);
    let mut sys_headers = Vec::new();
    while !worklist.empty() {
        let node = worklist.pop();
        let name = node.get_name();
        // the names not start with '/' are lib headers
        if is_a_lib_header(name) {
            for child in &node.children {
                let child_name = child.get_name();
                if is_a_lib_header(child_name) {
                    continue;
                }
                sys_headers.push(child_name);
            }
        }
        for child in &node.children {
            worklist.push(child);
        }
    }
    sys_headers
}

fn get_library_dep_trees(deopt: &Deopt) -> &'static Vec<TreeNode> {
    static TREES: OnceCell<Vec<TreeNode>> = OnceCell::new();
    TREES.get_or_init(|| {
        let executor = Executor::new(deopt).unwrap();
        let header_dir = deopt.get_library_build_header_path().unwrap();
        let headers = crate::deopt::utils::read_all_files_in_dir(&header_dir).unwrap();
        let mut header_trees = Vec::new();
        for header in headers {
            let ext = header.extension();
            if ext.is_none() {
                continue;
            }
            let ext = ext.unwrap().to_string_lossy().to_string();
            if ext != "h" && ext != "hpp" && ext != "hxx" {
                continue;
            }
            let tree = executor.extract_header_dependency(&header).unwrap();
            if tree.is_invalid() {
                continue;
            }
            header_trees.push(tree);
        }
        header_trees
    })
}

pub fn get_include_lib_headers(deopt: &Deopt) -> Result<Vec<String>> {
    let header_trees = get_library_dep_trees(deopt);
    let header_strs: Vec<String> = get_independent_headers(header_trees)?
        .iter()
        .map(|x| x.to_string())
        .collect();
    Ok(header_strs)
}

pub fn get_include_sys_headers(deopt: &Deopt) -> &'static Vec<String> {
    static SYS_HEADERS: OnceCell<Vec<String>> = OnceCell::new();
    SYS_HEADERS.get_or_init(|| {
        let header_trees = get_library_dep_trees(deopt);
        let header_strs: Vec<String> = get_independent_headers(header_trees)
            .unwrap()
            .iter()
            .map(|x| x.to_string())
            .collect();
        let mut sys_headers = Vec::new();
        for tree in header_trees {
            let name = tree.get_name();
            if header_strs.contains(&name.to_string()) {
                let headers = get_included_sys_header(tree);
                // remove the prefix of sys headers
                for header in headers {
                    if let Some(idx) = header.rfind("/include/") {
                        let header = header[idx + "/include/".len()..].to_string();
                        if !sys_headers.contains(&header) {
                            sys_headers.push(header);
                        }
                    }
                }
            }
        }
        sys_headers
    })
}

pub fn get_include_sys_headers_str() -> String {
    let deopt = Deopt::new(get_library_name()).unwrap();
    let headers = get_include_sys_headers(&deopt);
    headers.join("\n")
}

#[test]
fn test_library_headers() {
    let deopt = Deopt::new("c-ares".to_string()).unwrap();
    let headers = get_include_lib_headers(&deopt).unwrap();
    let sys_headers = get_include_sys_headers(&deopt);
    assert_eq!(
        headers,
        vec!["aom/aomdx.h", "aom/aom_decoder.h", "aom/aomcx.h"]
    );
    assert_eq!(sys_headers, &vec!["stddef.h", "stdint.h", "inttypes.h"]);
}

#[test]
fn test_library_header() {
    crate::config::Config::init_test("libtiff");
    let deopt = Deopt::new("libtiff".to_string()).unwrap();
    let headers = get_include_lib_headers(&deopt).unwrap();
    println!("{headers:?}");
}
