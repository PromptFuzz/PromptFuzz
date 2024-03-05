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
//<ID> 4673
//<Prompt> ["png_set_pHYs","png_get_io_ptr","png_set_cHRM_XYZ_fixed","png_create_read_struct","png_get_x_offset_inches_fixed"]
/*<Combination>: [void png_set_pHYs(png_const_structrp png_ptr, png_inforp info_ptr, png_uint_32 res_x, png_uint_32 res_y, int unit_type),
    png_voidp png_get_io_ptr(png_const_structrp png_ptr),
    void png_set_cHRM_XYZ_fixed(png_const_structrp png_ptr, png_inforp info_ptr, png_fixed_point int_red_X, png_fixed_point int_red_Y, png_fixed_point int_red_Z, png_fixed_point int_green_X, png_fixed_point int_green_Y, png_fixed_point int_green_Z, png_fixed_point int_blue_X, png_fixed_point int_blue_Y, png_fixed_point int_blue_Z),
    png_structp png_create_read_struct(png_const_charp user_png_ver, png_voidp error_ptr, png_error_ptr error_fn, png_error_ptr warn_fn),
    png_fixed_point png_get_x_offset_inches_fixed(png_const_structrp png_ptr, png_const_inforp info_ptr)
*/
//<score> 11, nr_unique_branch: 0
//<Quality> {"density":11,"unique_branches":{},"library_calls":["png_create_read_struct","png_create_info_struct","png_destroy_read_struct","png_set_longjmp_fn","png_destroy_read_struct","png_init_io","png_read_info","png_set_pHYs","png_get_io_ptr","png_set_cHRM_XYZ_fixed","png_get_x_offset_inches_fixed","png_destroy_read_struct"],"critical_calls":["png_create_read_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_read_info","png_set_pHYs","png_get_io_ptr","png_set_cHRM_XYZ_fixed","png_get_x_offset_inches_fixed","png_destroy_read_struct"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_98(const uint8_t* f_data, size_t f_size) {
	if(f_size<61) return 0;

	FUZZ_FILE_INIT();
	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_7, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_8, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_9, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_10, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_11, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_12, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_13, fdp);
	//fuzzer shim end}




    FILE *in_file = fmemopen((void *)data, size, "rb");
	FUZZ_FILE_PUSH(in_file)
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
	FUZZ_FILENAME_PUSH("output_file")

    FILE *out_file = fopen("output_file", "wb");
	FUZZ_FILE_PUSH(out_file)
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    png_structp png_ptr = png_create_read_struct(fuzz_str_1, nullptr, nullptr, nullptr);
    if (!png_ptr) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    if (setjmp(png_jmpbuf(png_ptr))) {
	FUZZ_FILE_CLOSE();
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    png_init_io(png_ptr, in_file);
    png_read_info(png_ptr, info_ptr);

    // Use png_set_pHYs API
    png_set_pHYs(png_ptr, info_ptr, fuzz_uint32_t_2, fuzz_uint32_t_3, fuzz_int32_t_4);

    // Use png_get_io_ptr API
    png_voidp io_ptr = png_get_io_ptr(png_ptr);

    // Use png_set_cHRM_XYZ_fixed API
    png_fixed_point int_red_X = 0;
    png_fixed_point int_red_Y = 0;
    png_fixed_point int_red_Z = 0;
    png_fixed_point int_green_X = 0;
    png_fixed_point int_green_Y = 0;
    png_fixed_point int_green_Z = 0;
    png_fixed_point int_blue_X = 0;
    png_fixed_point int_blue_Y = 0;
    png_fixed_point int_blue_Z = 0;
    png_set_cHRM_XYZ_fixed(png_ptr, info_ptr, fuzz_int32_t_5, fuzz_int32_t_6, fuzz_int32_t_7, fuzz_int32_t_8, fuzz_int32_t_9, fuzz_int32_t_10, fuzz_int32_t_11, fuzz_int32_t_12, fuzz_int32_t_13);

    // Use png_get_x_offset_inches_fixed API
    png_fixed_point x_offset_inches = png_get_x_offset_inches_fixed(png_ptr, info_ptr);

    // Perform other operations with libpng APIs

    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);

    assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}