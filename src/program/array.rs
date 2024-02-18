use crate::ast::helper::CommomHelper;
use crate::ast::{Clang, InitListExpr, Node};
use crate::program::transform::Transformer;
use eyre::Result;
/// Parse the array stmts in AST.
/// Impl of Transformation (generate and mutate) of array arguments.
use std::str::FromStr;

use super::serde::Deserializer;

impl InitListExpr {

    pub fn is_mutable_list(&self, node: &Node) -> bool {
        for child in &node.inner {
            let child = child.ignore_cast().ignore_parent();
            if !matches!(&child.kind,  Clang::StringLiteral(_)
            | Clang::CharacterLiteral(_)
            | Clang::FloatingLiteral(_)
            | Clang::IntegerLiteral(_)
            | Clang::InitListExpr(_)
            | Clang::UnaryOperator(_)) {
                return false;
            }
        }
        true
    }

    pub fn get_childs<'a>(&self, node: &'a Node) -> Vec<&'a Node> {
        let mut childs: Vec<&Node> = Vec::new();
        for child in &node.inner {
            let child = child.ignore_cast().ignore_parent();
            match &child.kind {
                Clang::StringLiteral(_)
                | Clang::CharacterLiteral(_)
                | Clang::FloatingLiteral(_)
                | Clang::IntegerLiteral(_)
                | Clang::InitListExpr(_) => childs.push(child),
                Clang::UnaryOperator(_) => childs.push(child.get_child().ignore_cast()),
                _ => unimplemented!("{child:?} is unimplemented for InitListExpr::get_childs."),
            }
        }
        childs
    }

    /// get the char * array declared in InitListExpr, e.g., char *[] = {"hello", "world"}
    pub fn get_char_star_array_literal(
        &self,
        node: &Node,
        len: Option<usize>,
    ) -> Result<Vec<String>> {
        let mut strings: Vec<String> = Vec::new();
        for child in self.get_childs(node) {
            if let Clang::StringLiteral(sl) = &child.kind {
                strings.push(sl.get_eval_value());
            } else {
                eyre::bail!("child should be StringLiteral: {child:?}")
            }
        }
        if let Some(len) = len {
            //assert_eq!(strings.len(), len);
            if strings.is_empty() {
                for _ in 0..len {
                    strings.push("str".to_string());
                }
            }
        }
        Ok(strings)
    }
}

/// impl fns to get array of literals for each literal type.
macro_rules! impl_array_literal {
    ($name:ident, $literal_ty:ident) => {
        impl InitListExpr {
            /// get the literal array declared in InitListExpr, e.g., {'a', 'b', 'c'}
            pub fn $name<T: FromStr + Default>(&self, node: &Node, len: Option<usize>) -> Result<Vec<T>>
            where
                <T as std::str::FromStr>::Err: std::fmt::Debug,
            {
                let mut literals: Vec<T> = Vec::new();
                for child in self.get_childs(node) {
                    if let Clang::$literal_ty(il) = &child.kind {
                        literals.push(il.get_value()?);
                    }
                    // consider the integreral cast
                    else if let Clang::IntegerLiteral(il) = &child.kind {
                        literals.push(il.get_value()?);
                    } else {
                        eyre::bail!("cannot get {} from: {node:?}", stringify!($literal_ty))
                    }
                }

                if let Some(len) = len {
                    // if this list initialized by default value
                    if literals.is_empty() {
                        for _ in 0..len {
                            literals.push("0".parse::<T>().unwrap())
                        }
                    }
                    if literals.len() != len {
                        eyre::bail!(
                            "get `{}` literals, need `{len}` literals. {node:?}",
                            literals.len()
                        )
                    }
                }
                Ok(literals)
            }
        }
    };
}

/// impl fns to get two demension matrix literals for each literal type.
macro_rules! impl_matrix_literal {
    ($name:ident, $literal_ty_fn:ident) => {
        impl InitListExpr {
            /// get the matrix declared in InitListExpr, e.g., char[][2] = {{'a', 'b'}, {'c', 'd'}}
            pub fn $name<T: FromStr + Default>(
                &self,
                node: &Node,
                row: Option<usize>,
                col: Option<usize>,
            ) -> Result<Vec<Vec<T>>>
            where
                <T as std::str::FromStr>::Err: std::fmt::Debug,
            {
                let mut matrix: Vec<Vec<T>> = Vec::new();
                for child in self.get_childs(node) {
                    if let Clang::InitListExpr(ile) = &child.kind {
                        let literals = ile.$literal_ty_fn(child, col)?;
                        matrix.push(literals);
                    } else {
                        eyre::bail!("child should be InitListExpr: {child:?}")
                    }
                }
                if let Some(row) = row {
                    assert_eq!(matrix.len(), row);
                }
                Ok(matrix)
            }
        }
    };
}
impl_array_literal!(get_char_array_literal, CharacterLiteral);
impl_matrix_literal!(get_char_matrix_literal, get_char_array_literal);
// integeral can be cast to floating, since need to impl manually.
impl_array_literal!(get_floating_array_literal, FloatingLiteral);
impl_matrix_literal!(get_floating_matrix_literal, get_floating_array_literal);
impl_array_literal!(get_integer_array_literal, IntegerLiteral);
impl_matrix_literal!(get_integer_matrix_literal, get_integer_array_literal);

/// Type of array/list to generate. Currently only two dimension support.
pub enum InitListType {
    // char [row][col]
    CharList(usize, usize),
    // char *[col]
    CharStarList(usize, usize),
    UCharList(usize, usize),
    UCharStarList(usize, usize),
    FloatList(usize, usize),
    FloatStarList(usize, usize),
    DoubleList(usize, usize),
    DoubleStarList(usize, usize),
    I8List(usize, usize),
    I8StarList(usize, usize),
    I16List(usize, usize),
    I16StarList(usize, usize),
    I32List(usize, usize),
    I32StarList(usize, usize),
    I64List(usize, usize),
    I64StarList(usize, usize),
    U8List(usize, usize),
    U8StarList(usize, usize),
    U16List(usize, usize),
    U16StarList(usize, usize),
    U32List(usize, usize),
    U32StarList(usize, usize),
    U64List(usize, usize),
    U64StarList(usize, usize),
    PointerList,
    /// Over two dimensions
    Others,
}

/// return InitListType enums according the given row and col
macro_rules! type_match {
    ($list:ident, $array_list:ident, $row:expr, $col:expr) => {
        if $row == usize::MAX {
            return InitListType::$array_list($col, $col);
        } else {
            return InitListType::$list($row, $col);
        }
    };
}

impl InitListType {
    pub fn new(ty: &str) -> Self {
        let mut row_num = usize::MAX;
        let mut col_num = 0;

        let mut de = Deserializer::from_input(ty);
        if de.is_next_token("const") {
            de.eat_token("const").unwrap();
        }

        let idx = usize::min(
            de.input.find('*').unwrap_or(usize::MAX),
            de.input.find('[').unwrap_or(usize::MAX),
        );
        if idx == usize::MAX {
            unreachable!()
        }
        let ty_token = de.input[..idx].trim();
        let input = &de.input[idx..];
        let mut de = Deserializer::from_input(input);
        let next_char = de.next_char().unwrap();

        // StarList: *[3]
        if next_char == '*' {
            if de.is_next_token("const") {
                de.eat_token("const").unwrap();
            }
            de.next_char()
                .unwrap_or_else(|_| panic!("CharStarList: next char should be '[' here."));
            col_num = de.parse_number().unwrap();
            de.eat_token("]").unwrap();
            if de.next_char().is_ok() {
                return Self::Others;
            }
        // Fixed List: [3] or [3][4]
        } else if next_char == '[' {
            row_num = de.parse_number().unwrap();
            de.eat_token("]").unwrap();
            if let Ok(_next_char) = de.next_char() {
                col_num = de.parse_number().unwrap();
                de.eat_token("]").unwrap();
                if de.next_char().is_ok() {
                    return Self::Others;
                }
            }
        } else {
            unreachable!()
        }

        let canonical_ty = super::gadget::ctype::retrieve_canonical_type(ty_token);

        match canonical_ty.as_str() {
            "char" => type_match!(CharList, CharStarList, row_num, col_num),
            "unsigned char" => type_match!(UCharList, UCharStarList, row_num, col_num),
            "float" => type_match!(FloatList, FloatStarList, row_num, col_num),
            "double" => type_match!(DoubleList, DoubleStarList, row_num, col_num),
            "int" => type_match!(I32List, I32StarList, row_num, col_num),
            "int8_t" => type_match!(I8List, I8StarList, row_num, col_num),
            "uint8_t" => type_match!(U8List, U8StarList, row_num, col_num),
            "int16_t" | "short" => type_match!(I16List, I16StarList, row_num, col_num),
            "uint16_t" | "unsigned short" => type_match!(U16List, U16StarList, row_num, col_num),
            "int32_t" => type_match!(I32List, I32StarList, row_num, col_num),
            "uint32_t" | "unsigned int" => type_match!(U32List, U32StarList, row_num, col_num),
            "int64_t" | "long" | "long long" => type_match!(I64List, I64StarList, row_num, col_num),
            "uint64_t" | "unsigned long" | "unsigned long long" => {
                type_match!(U64List, U64StarList, row_num, col_num)
            }
            _ => Self::PointerList,
        }
    }

    pub fn get_array_shape(&self) -> (&usize, &usize) {
        match self {
            InitListType::CharList(row, col) => (row, col),
            InitListType::CharStarList(row, col) => (row, col),
            InitListType::UCharList(row, col) => (row, col),
            InitListType::UCharStarList(row, col) => (row, col),
            InitListType::FloatList(row, col) => (row, col),
            InitListType::FloatStarList(row, col) => (row, col),
            InitListType::DoubleList(row, col) => (row, col),
            InitListType::DoubleStarList(row, col) => (row, col),
            InitListType::I8List(row, col) => (row, col),
            InitListType::I8StarList(row, col) => (row, col),
            InitListType::I16List(row, col) => (row, col),
            InitListType::I16StarList(row, col) => (row, col),
            InitListType::I32List(row, col) => (row, col),
            InitListType::I32StarList(row, col) => (row, col),
            InitListType::I64List(row, col) => (row, col),
            InitListType::I64StarList(row, col) => (row, col),
            InitListType::U8List(row, col) => (row, col),
            InitListType::U8StarList(row, col) => (row, col),
            InitListType::U16List(row, col) => (row, col),
            InitListType::U16StarList(row, col) => (row, col),
            InitListType::U32List(row, col) => (row, col),
            InitListType::U32StarList(row, col) => (row, col),
            InitListType::U64List(row, col) => (row, col),
            InitListType::U64StarList(row, col) => (row, col),
            InitListType::Others => (&0, &0),
            InitListType::PointerList => (&0, &0),
        }
    }
}

/// impl create sized fuzzer vars for each possisble var type.
macro_rules! impl_create_literal_list_fuzzer_var {
    ($ty:ty, $name:ident, $ile_ty_name:ident, $fuzzer_fn:ident) => {
        impl<'a> Transformer<'a> {
            paste::expr! {
                pub fn [<create_ $name _list_fuzzer_var>](&mut self, ile: &InitListExpr, node: &Node, row: usize, col: usize) -> Result<()> {
                    if col == 0 {
                        let literarls = ile.[<get_ $ile_ty_name _array_literal>]::<$ty>(node, Some(row))?;
                        self.fuzzer_shim.$fuzzer_fn(literarls);
                    } else {
                        let matrix = ile.[<get_ $ile_ty_name _matrix_literal>]::<$ty>(node, Some(row), Some(col))?;
                        for literals in matrix {
                            self.fuzzer_shim.$fuzzer_fn(literals);
                        }
                    }
                    Ok(())
                }
            }
        }

    };
}

impl_create_literal_list_fuzzer_var!(u8, char, char, append_char_vars);
impl_create_literal_list_fuzzer_var!(u8, uchar, char, append_uchar_vars);
impl_create_literal_list_fuzzer_var!(f32, float, floating, append_float_vars);
impl_create_literal_list_fuzzer_var!(f64, double, floating, append_double_vars);
impl_create_literal_list_fuzzer_var!(i8, int8_t, integer, append_integer_vars);
impl_create_literal_list_fuzzer_var!(u8, uint8_t, integer, append_integer_vars);
impl_create_literal_list_fuzzer_var!(i16, int16_t, integer, append_integer_vars);
impl_create_literal_list_fuzzer_var!(u16, uint16_t, integer, append_integer_vars);
impl_create_literal_list_fuzzer_var!(i32, int32_t, integer, append_integer_vars);
impl_create_literal_list_fuzzer_var!(u32, uint32_t, integer, append_integer_vars);
impl_create_literal_list_fuzzer_var!(i64, int64_t, integer, append_integer_vars);
impl_create_literal_list_fuzzer_var!(u64, uint64_t, integer, append_integer_vars);

/// impl create unlimit sized fuzzer vars for each possisble var type.
macro_rules! impl_create_unlimit_literal_list_fuzzer_var {
    ($ty:ty, $name:ident, $ile_ty_name:ident, $fuzzer_fn:ident) => {
        impl<'a> Transformer<'a> {
            paste::expr! {
                pub fn [<create_unlimit_ $name _list_fuzzer_var>](&mut self, ile: &InitListExpr, node: &Node, row: usize) -> Result<()> {
                    let literarls = ile.[<get_ $ile_ty_name _array_literal>]::<$ty>(node, Some(row))?;
                    self.fuzzer_shim.$fuzzer_fn(literarls);
                    Ok(())
                }
            }
        }

    };
}

impl_create_unlimit_literal_list_fuzzer_var!(u8, char, char, append_char_array);
impl_create_unlimit_literal_list_fuzzer_var!(u8, uchar, char, append_uchar_array);
impl_create_unlimit_literal_list_fuzzer_var!(f32, float, floating, append_float_array);
impl_create_unlimit_literal_list_fuzzer_var!(f64, double, floating, append_double_array);
impl_create_unlimit_literal_list_fuzzer_var!(i8, int8_t, integer, append_integer_array);
impl_create_unlimit_literal_list_fuzzer_var!(u8, uint8_t, integer, append_integer_array);
impl_create_unlimit_literal_list_fuzzer_var!(i16, int16_t, integer, append_integer_array);
impl_create_unlimit_literal_list_fuzzer_var!(u16, uint16_t, integer, append_integer_array);
impl_create_unlimit_literal_list_fuzzer_var!(i32, int32_t, integer, append_integer_array);
impl_create_unlimit_literal_list_fuzzer_var!(u32, uint32_t, integer, append_integer_array);
impl_create_unlimit_literal_list_fuzzer_var!(i64, int64_t, integer, append_integer_array);
impl_create_unlimit_literal_list_fuzzer_var!(u64, uint64_t, integer, append_integer_array);
