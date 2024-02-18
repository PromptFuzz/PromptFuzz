//! Precompiled headers (PCH)
//! Precompiled headers are a general approach employed by many compilers
//!     to reduce compilation time.
//! The underlying motivation of the approach is that it is common for the
//!     same (and often large) header files to be included by multiple source files.
//! Precompiled header files, which represent one of many ways to implement this
//!     optimization, are literally files that represent an on-disk cache that
//!     contains the vital information necessary to reduce some of the work needed
//!     to process a corresponding header file.
//!
//! Generating a PCH File:
//!     $ clang -x c-header test.h -o test.h.pch
//!
//! Using a PCH File:
//!     $ clang -include-pch test.h.pch test.c -o test
//!
//! See, https://clang.llvm.org/docs/UsersManual.html#precompiled-headers
//!      https://clang.llvm.org/docs/PCHInternals.html
//!
use std::{path::PathBuf, process::Command};

use crate::{program::Program, Deopt};
use eyre::Result;

use super::Executor;

impl Program {
    /// get the indexs of '#include' stmts
    pub fn get_include_stmt_idxs(&self) -> Vec<usize> {
        todo!()
    }

    /// comment out those include stmts, so that the pch files could work.
    /// See, https://clang.llvm.org/docs/UsersManual.html#precompiled-headers
    pub fn comment_include_stmts(&mut self) -> Result<()> {
        todo!()
    }
}

impl Executor {
    /// Using clang to generate pch file of this header, and store the pch file in pch dir.
    pub fn generate_pch_file(header: PathBuf) -> Result<()> {
        log::trace!("generate pch for {header:?}");
        let pch_dir = Deopt::get_pch_path()?;
        let header_name = header
            .file_name()
            .ok_or_else(|| eyre::eyre!("path `{header:?}` have not basename."))?;
        let header_pch_name = [header_name.to_str().unwrap(), ".pch"].join("");
        let mut out_pch_path: PathBuf = pch_dir;
        out_pch_path.push(&header_pch_name);
        let output = Command::new("clang")
            .arg("-x")
            .arg("c-header")
            .arg(header.clone())
            .arg("-o")
            .arg(out_pch_path)
            .output()?;
        if output.status.success() {
            let header_pchs = Deopt::get_pch_names_mut().unwrap();
            header_pchs.push(header_pch_name);
            Ok(())
        } else {
            eyre::bail!("generate pch of `{header:?}` failed! {output:#?}")
        }
    }
}

/// get system location of this header file.
pub fn get_sys_header_path(header: &str) -> PathBuf {
    let header_path: PathBuf = ["/usr/include", header].iter().collect();
    header_path
}

/// is this header file existed in your system.
pub fn is_sys_header_exist(header: &str) -> bool {
    let header_path = get_sys_header_path(header);
    header_path.exists()
}

/// is pch of this header existed in storage.
pub fn is_header_pch_exist(header: &str) -> bool {
    let header_pchs = Deopt::get_pch_names().unwrap();
    let header_pch_name: String = [header, ".pch"].join("");
    header_pchs.contains(&header_pch_name)
}

/// get pch path of this header.
pub fn get_header_pch_path(header: &str) -> PathBuf {
    let mut header_pch_path = Deopt::get_pch_path().unwrap();
    let header_pch_name: String = [header, ".pch"].join("");
    header_pch_path.push(header_pch_name);
    header_pch_path
}

/// get the list of pch paths that this program would use.
pub fn get_pch_list(program: &mut Program, deopt: &Deopt) -> Vec<PathBuf> {
    let mut pch_vec = vec![];
    for header in crate::deopt::utils::get_library_headers(deopt)
        .unwrap()
        .iter()
    {
        // if header pch exist, then push it
        if is_header_pch_exist(header) {
            let p = get_header_pch_path(header);
            pch_vec.push(p);
        }
        // if pch doesn't exist, but sys header exist, then generate pch.
        else if is_sys_header_exist(header) {
            let p = get_sys_header_path(header);
            Executor::generate_pch_file(p).unwrap();
            let p = get_header_pch_path(header);
            pch_vec.push(p);
        }
        // if both of pch and sys header don't exist, error.
        else {
            unreachable!("header {header} doesn't exist!")
        }
    }
    log::trace!("program: {:?}, pch list: {pch_vec:?}", program.id);
    pch_vec
}
