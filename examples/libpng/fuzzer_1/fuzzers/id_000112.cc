#include "FDSan.h"
#include "FuzzedDataProvider.h"
#include <png.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 5499
//<Prompt> ["png_set_check_for_invalid_index","png_get_pCAL","png_write_chunk_start","png_set_keep_unknown_chunks","png_create_write_struct","png_get_user_transform_ptr"]
/*<Combination>: [void png_set_check_for_invalid_index(png_structrp png_ptr, int allowed),
    png_uint_32 png_get_pCAL(png_const_structrp png_ptr, png_inforp info_ptr, png_charp * purpose, png_int_32 * X0, png_int_32 * X1, int * type, int * nparams, png_charp * units, png_charpp * params),
    void png_write_chunk_start(png_structrp png_ptr, png_const_bytep chunk_name, png_uint_32 length),
    void png_set_keep_unknown_chunks(png_structrp png_ptr, int keep, png_const_bytep chunk_list, int num_chunks),
    png_structp png_create_write_struct(png_const_charp user_png_ver, png_voidp error_ptr, png_error_ptr error_fn, png_error_ptr warn_fn),
    png_voidp png_get_user_transform_ptr(png_const_structrp png_ptr)
*/
//<score> 20, nr_unique_branch: 2
//<Quality> {"density":10,"unique_branches":{"png_get_pCAL":[[908,8,908,23,0,0,4,0],[908,27,908,43,0,0,4,1]]},"library_calls":["png_create_write_struct","png_init_io","png_set_write_fn","png_set_longjmp_fn","png_destroy_write_struct","png_set_check_for_invalid_index","png_get_pCAL","png_write_chunk_start","png_set_keep_unknown_chunks","png_get_user_transform_ptr","png_destroy_write_struct"],"critical_calls":["png_create_write_struct","png_init_io","png_set_write_fn","png_set_longjmp_fn","png_set_check_for_invalid_index","png_get_pCAL","png_write_chunk_start","png_set_keep_unknown_chunks","png_get_user_transform_ptr","png_destroy_write_struct"],"visited":0}
/**/

extern "C" int LLVMFuzzerTestOneInput_112(const uint8_t* f_data, size_t f_size) {
	if(f_size<25) return 0;

	FUZZ_FILE_INIT();
	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
	//fuzzer shim end}




    // Create a FILE pointer for reading the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
	FUZZ_FILE_PUSH(in_file)
    if (in_file == nullptr) {
        assert_file_closed(&in_file);
	return 0;
    }
	FUZZ_FILENAME_PUSH("output_file")
    
    // Create a FILE pointer for writing output data
    FILE *out_file = fopen("output_file", "wb");
	FUZZ_FILE_PUSH(out_file)
    if (out_file == nullptr) {
        assert_file_closed(&in_file);;
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Create a png_struct to handle the PNG file
    png_structp png_ptr = png_create_write_struct(fuzz_str_1, NULL, NULL, NULL);
    if (png_ptr == nullptr) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Set the input and output file streams
    png_init_io(png_ptr, in_file);
    png_set_write_fn(png_ptr, out_file, NULL, NULL);

    // Set the error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
	FUZZ_FILE_CLOSE();
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        png_destroy_write_struct(&png_ptr, NULL);
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Set the check for invalid index
    png_set_check_for_invalid_index(png_ptr, fuzz_int32_t_2);

    // Get the pCAL information
    png_charp purpose;
    png_int_32 X0, X1;
    int type, nparams;
    png_charp units;
    png_charpp params;
    png_get_pCAL(png_ptr, NULL, &purpose, &X0, &X1, &type, &nparams, &units, &params);

    // Write a PNG chunk
    png_byte chunk_data[] = {0x00, 0x01, 0x02, 0x03};
    png_write_chunk_start(png_ptr, chunk_data, sizeof(chunk_data));

    // Set the keep unknown chunks
    png_set_keep_unknown_chunks(png_ptr, fuzz_int32_t_3, NULL, fuzz_int32_t_4);

    // Get the user transform pointer
    png_voidp user_transform_ptr = png_get_user_transform_ptr(png_ptr);

    // Clean up and release resources
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    png_destroy_write_struct(&png_ptr, NULL);

    assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}