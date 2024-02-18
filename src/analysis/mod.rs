use std::collections::VecDeque;

pub mod adg;
pub mod callgraph;
pub mod cfg;
pub mod dfa;
pub mod fdsan;
pub mod header;

pub struct WorkList<T> {
    stmts: VecDeque<T>,
}

impl<T> Default for WorkList<T> {
    fn default() -> Self {
        Self::new()
    }
}

impl<T> WorkList<T> {
    pub fn new() -> Self {
        Self {
            stmts: VecDeque::<T>::new(),
        }
    }

    pub fn empty(&self) -> bool {
        self.stmts.is_empty()
    }

    /// Stack pop
    pub fn pop(&mut self) -> T {
        self.stmts.pop_back().expect("expect non-empty work list")
    }

    /// Queue pop
    pub fn pop_front(&mut self) -> T {
        self.stmts.pop_front().expect("expect non-empty work list")
    }

    /// Stack push
    pub fn push(&mut self, stmt: T) {
        self.stmts.push_back(stmt);
    }

    /// Save nodes that enable to Pre-order traversal of AST.
    pub fn push_childs(&mut self, childs: Vec<T>) {
        let mut tempstack = Self::new();
        for child in childs {
            tempstack.push(child);
        }
        while !tempstack.empty() {
            self.push(tempstack.pop());
        }
    }
}
