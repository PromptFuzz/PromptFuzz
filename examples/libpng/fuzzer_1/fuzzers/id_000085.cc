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
//<ID> 3974
//<Prompt> ["png_set_user_transform_info","png_calloc","png_get_pHYs_dpi","png_set_cHRM_fixed","png_get_user_height_max","png_set_bKGD","png_get_sCAL","png_set_check_for_invalid_index"]
/*<Combination>: [void png_set_user_transform_info(png_structrp png_ptr, png_voidp user_transform_ptr, int user_transform_depth, int user_transform_channels),
    png_voidp png_calloc(png_const_structrp png_ptr, png_alloc_size_t size),
    png_uint_32 png_get_pHYs_dpi(png_const_structrp png_ptr, png_const_inforp info_ptr, png_uint_32 * res_x, png_uint_32 * res_y, int * unit_type),
    void png_set_cHRM_fixed(png_const_structrp png_ptr, png_inforp info_ptr, png_fixed_point int_white_x, png_fixed_point int_white_y, png_fixed_point int_red_x, png_fixed_point int_red_y, png_fixed_point int_green_x, png_fixed_point int_green_y, png_fixed_point int_blue_x, png_fixed_point int_blue_y),
    png_uint_32 png_get_user_height_max(png_const_structrp png_ptr),
    void png_set_bKGD(png_const_structrp png_ptr, png_inforp info_ptr, png_const_color_16p background),
    png_uint_32 png_get_sCAL(png_const_structrp png_ptr, png_const_inforp info_ptr, int * unit, double * width, double * height),
    void png_set_check_for_invalid_index(png_structrp png_ptr, int allowed)
*/
//<score> 15, nr_unique_branch: 0
//<Quality> {"density":15,"unique_branches":{},"library_calls":["png_create_read_struct","png_create_info_struct","png_destroy_read_struct","png_set_longjmp_fn","png_destroy_read_struct","png_set_user_transform_info","png_calloc","png_destroy_read_struct","png_get_pHYs_dpi","png_set_cHRM_fixed","png_get_user_height_max","png_set_bKGD","png_get_sCAL","png_set_check_for_invalid_index","png_free_default","png_destroy_read_struct"],"critical_calls":["png_create_read_struct","png_create_info_struct","png_set_longjmp_fn","png_set_user_transform_info","png_calloc","png_get_pHYs_dpi","png_set_cHRM_fixed","png_get_user_height_max","png_set_bKGD","png_get_sCAL","png_set_check_for_invalid_index","png_free_default","png_destroy_read_struct"],"visited":0}
/*Here is the complete C++ program that achieves the event using the libpng APIs:
*/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_85(const uint8_t* f_data, size_t f_size) {
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
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    // Step 4: Create input file
    FILE *in_file = fmemopen((void *)data, size, "rb");
	FUZZ_FILE_PUSH(in_file)
    if (in_file == nullptr) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Step 5: Get file descriptor
    int in_fd = fuzz_fileno(in_file);
	FUZZ_FILENAME_PUSH("input_file")
	FUZZ_FD_PUSH(in_fd)

    // Step 6: Specify input file name
    const char *in_filename = "input_file";

    // Step 7: Initialize libpng structures
    png_structp png_ptr = png_create_read_struct(fuzz_str_1, nullptr, nullptr, nullptr);
    if (png_ptr == nullptr) {
        assert_file_closed(&in_file);;
        assert_file_name_closed("input_file");
	assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == nullptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        assert_file_closed(&in_file);;
        assert_file_name_closed("input_file");
	assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
    }

    // Initialize error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
	FUZZ_FILE_CLOSE();
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        assert_file_closed(&in_file);;
        assert_file_name_closed("input_file");
	assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
    }

    // Step 1: Set user transform info
    png_set_user_transform_info(png_ptr, nullptr, fuzz_int32_t_2, fuzz_int32_t_3);

    // Step 2: Allocate memory
    png_voidp ptr = png_calloc(png_ptr, size);
    if (ptr == nullptr) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        assert_file_closed(&in_file);;
        assert_file_name_closed("input_file");
	assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
    }

    // Step 3: Get pHYs dpi
    png_uint_32 res_x, res_y;
    int unit_type;
    png_get_pHYs_dpi(png_ptr, info_ptr, &res_x, &res_y, &unit_type);

    // Step 8: Set cHRM fixed
    png_fixed_point int_white_x, int_white_y, int_red_x, int_red_y, int_green_x, int_green_y, int_blue_x, int_blue_y;
    png_set_cHRM_fixed(png_ptr, info_ptr, int_white_x, int_white_y, int_red_x, int_red_y, int_green_x, int_green_y, int_blue_x, int_blue_y);

    // Step 9: Get user height max
    png_uint_32 user_height_max = png_get_user_height_max(png_ptr);

    // Step 10: Set bKGD
    png_color_16 background;
    png_set_bKGD(png_ptr, info_ptr, &background);

    // Step 11: Get sCAL
    int unit;
    double width, height;
    png_get_sCAL(png_ptr, info_ptr, &unit, &width, &height);

    // Step 12: Set check for invalid index
    png_set_check_for_invalid_index(png_ptr, fuzz_int32_t_4);

    // Clean up
    png_free_default(png_ptr, ptr);
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    assert_file_closed(&in_file);;

    assert_file_name_closed("input_file");
	assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
}