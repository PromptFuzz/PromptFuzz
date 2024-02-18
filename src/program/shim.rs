use once_cell::sync::OnceCell;
/// The shim to deliver fuzzer bytes to fuzz-able arguments.
use std::collections::HashMap;

pub trait Integer {
    fn to_bytes(&self) -> Vec<u8>;
}

macro_rules! impl_integer_to_bytes {
    ($( $ty:ty ),*) => {
        $(impl Integer for $ty {
            fn to_bytes(&self) -> Vec<u8> {
                self.to_be_bytes().to_vec()
            }
        })*
    };
}

impl_integer_to_bytes!(i8, u8, i16, u16, i32, u32, i64, u64);

static RUST_TO_C_MAP: OnceCell<HashMap<&'static str, &'static str>> = OnceCell::new();

fn get_rust_to_c_map() -> &'static HashMap<&'static str, &'static str> {
    RUST_TO_C_MAP.get_or_init(|| {
        HashMap::from([
            ("i8", "int8_t"),
            ("u8", "uint8_t"),
            ("i16", "int16_t"),
            ("u16", "uint16_t"),
            ("i32", "int32_t"),
            ("u32", "uint32_t"),
            ("i64", "int64_t"),
            ("u64", "uint64_t"),
        ])
    })
}

pub struct FuzzerShim {
    /// fuzzer required fields
    fuzz_offset: usize,
    fuzz_seed: Vec<u8>,
    fuzz_stmts: Vec<String>,
    fuzz_var_id: usize,
    init_var_id: usize,
    magic_bytes: [u8; 3],
}

impl Default for FuzzerShim {
    fn default() -> Self {
        Self::new()
    }
}

impl FuzzerShim {
    pub fn new() -> Self {
        Self {
            fuzz_offset: 0,
            fuzz_seed: Vec::new(),
            fuzz_stmts: vec!["FuzzedDataProvider fdp(f_data, f_size);".to_string()],
            fuzz_var_id: 0,
            init_var_id: 0,
            magic_bytes: [0xCA, 0xFE, 0xBA],
        }
    }

    pub fn get_magic_bytes() -> Vec<u8> {
        vec![0xCA, 0xFE, 0xBA]
    }

    pub fn get_offset(&self) -> usize {
        self.fuzz_offset
    }

    pub fn get_seeds(&self) -> &[u8] {
        &self.fuzz_seed
    }

    pub fn get_fuzz_id_inc(&mut self) -> usize {
        self.fuzz_var_id += 1;
        self.fuzz_var_id
    }

    pub fn get_init_id_inc(&mut self) -> usize {
        self.init_var_id += 1;
        self.init_var_id
    }

    fn append_magic_bytes(&mut self) {
        self.fuzz_seed.extend_from_slice(&self.magic_bytes);
        self.fuzz_offset += 3;
    }

    fn append_fuzz_seeds(&mut self, bytes: &[u8]) {
        self.fuzz_offset += bytes.len();
        self.fuzz_seed.extend(bytes);
    }

    fn append_fuzz_seeds_with_magic(&mut self, bytes: &[u8]) {
        self.fuzz_offset += bytes.len();
        self.fuzz_seed.extend(bytes);
        self.append_magic_bytes()
    }

    pub fn append_string_var(&mut self, str_var: String, ty: &str) {
        let var = format!(
            "FDPConsumeRandomLengthString({ty}, fuzz_str_{0}, fuzz_str_sz_{0}, fdp);",
            self.get_fuzz_id_inc()
        );
        self.fuzz_stmts.push(var);
        self.append_fuzz_seeds(str_var.as_bytes());
        self.append_magic_bytes();
    }

    fn append_char_var(&mut self, char: u8) {
        let var = format!("FDPConsumeChar(fuzz_char_{}, fdp);", self.get_fuzz_id_inc());
        self.fuzz_stmts.push(var);
        self.append_fuzz_seeds(&[char]);
    }

    pub fn append_char_vars(&mut self, chars: Vec<u8>) {
        for char in chars {
            self.append_char_var(char);
        }
    }

    fn append_uchar_var(&mut self, char: u8) {
        let var = format!(
            "FDPConsumeUChar(fuzz_uchar_{}, fdp);",
            self.get_fuzz_id_inc()
        );
        self.fuzz_stmts.push(var);
        self.append_fuzz_seeds(&[char]);
    }

    pub fn append_uchar_vars(&mut self, chars: Vec<u8>) {
        for char in chars {
            self.append_uchar_var(char);
        }
    }

    pub fn append_char_array(&mut self, chars: Vec<u8>) {
        let var = format!(
            "FDPConsumeCharArray(char, fuzz_char_array_{0}, fuzz_char_array_size_{0}, fdp);",
            self.get_fuzz_id_inc()
        );
        self.fuzz_stmts.push(var);
        self.append_fuzz_seeds_with_magic(chars.as_slice());
    }

    pub fn append_uchar_array(&mut self, chars: Vec<u8>) {
        let var = format!("FDPConsumeCharArray(unsigned char, fuzz_char_array_{0}, fuzz_char_array_size_{0}, fdp);", self.get_fuzz_id_inc());
        self.fuzz_stmts.push(var);
        self.append_fuzz_seeds_with_magic(chars.as_slice());
    }

    fn append_float_var(&mut self, f: f32) {
        let var = format!(
            "FDPConsumeFloatingPoint(fuzz_float_{}, fdp);",
            self.get_fuzz_id_inc()
        );
        self.fuzz_stmts.push(var);
        let bytes = f.to_le_bytes();
        self.append_fuzz_seeds(&bytes);
    }

    pub fn append_float_vars(&mut self, floats: Vec<f32>) {
        for f in floats {
            self.append_float_var(f);
        }
    }

    pub fn append_float_array(&mut self, floats: Vec<f32>) {
        let var = format!(
            "FDPConsumeFloatingArray(fuzz_float_array_{0}, fuzz_float_array_size_{0}, fdp);",
            self.get_fuzz_id_inc()
        );
        self.fuzz_stmts.push(var);
        let mut bytes: Vec<u8> = Vec::new();
        for float in floats {
            bytes.extend(float.to_le_bytes());
        }
        self.append_fuzz_seeds_with_magic(&bytes);
    }

    fn append_double_var(&mut self, d: f64) {
        let var = format!(
            "FDPConsumeDoublingPoint(fuzz_double_{}, fdp);",
            self.get_fuzz_id_inc()
        );
        self.fuzz_stmts.push(var);
        let bytes = d.to_le_bytes();
        self.append_fuzz_seeds(&bytes);
    }

    pub fn append_double_vars(&mut self, doubles: Vec<f64>) {
        for d in doubles {
            self.append_double_var(d);
        }
    }

    pub fn append_double_array(&mut self, doubles: Vec<f64>) {
        let var = format!(
            "FDPConsumeDoublingArray(fuzz_double_array_{0}, fuzz_double_array_size_{0}, fdp);",
            self.get_fuzz_id_inc()
        );
        self.fuzz_stmts.push(var);
        let mut bytes: Vec<u8> = Vec::new();
        for double in doubles {
            bytes.extend(double.to_le_bytes());
        }
        self.append_fuzz_seeds_with_magic(&bytes);
    }

    pub fn append_integer_var<T: Integer>(&mut self, integer: T) {
        let ty_name = std::any::type_name::<T>();
        let binding = get_rust_to_c_map();
        let c_ty_name = binding
            .get(ty_name)
            .unwrap_or_else(|| panic!("cannot found {ty_name} from RUST_TO_C_MAP."));
        let var = format!(
            "FDPConsumeIntegral({c_ty_name}, fuzz_{c_ty_name}_{}, fdp);",
            self.get_fuzz_id_inc()
        );
        self.fuzz_stmts.push(var);
        let bytes = integer.to_bytes();
        self.append_fuzz_seeds(&bytes);
    }

    pub fn append_integer_vars<T: Integer>(&mut self, integers: Vec<T>) {
        for integer in integers {
            self.append_integer_var(integer);
        }
    }

    pub fn append_integer_array<T: Integer>(&mut self, integers: Vec<T>) {
        let ty_name = std::any::type_name::<T>();
        let binding = get_rust_to_c_map();
        let c_ty_name = binding
            .get(ty_name)
            .unwrap_or_else(|| panic!("cannot found {ty_name} from RUST_TO_C_MAP."));

        let var = format!("FDPConsumeIntegralArray({c_ty_name}, fuzz_{c_ty_name}_array_{0}, fuzz_{c_ty_name}_array_size_{0}, fdp);", self.get_fuzz_id_inc());
        self.fuzz_stmts.push(var);
        let mut bytes: Vec<u8> = Vec::new();
        for integer in integers {
            bytes.extend(integer.to_bytes());
        }
        self.append_fuzz_seeds_with_magic(&bytes);
    }

    pub fn append_fuzzer_stmt(&mut self, stmt: String) {
        self.fuzz_stmts.push(stmt);
    }

    pub fn serialize_fuzzer_stmts(&self) -> String {
        let mut stmts = String::new();
        stmts.push_str("\n\t//fuzzer vars shim {\n\t\t");
        let vars = self.fuzz_stmts.join("\n\t\t");
        stmts.push_str(&vars);
        stmts.push_str("\n\t//fuzzer shim end}\n\n");
        stmts
    }
}
