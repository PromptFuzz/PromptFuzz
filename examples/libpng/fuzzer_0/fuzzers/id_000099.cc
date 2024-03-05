#include "FDSan.h"
#include <png.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 4676
//<Prompt> ["png_set_pHYs","png_get_io_ptr","png_set_cHRM_XYZ_fixed","png_create_read_struct","png_get_x_offset_inches_fixed"]
/*<Combination>: [void png_set_pHYs(png_const_structrp png_ptr, png_inforp info_ptr, png_uint_32 res_x, png_uint_32 res_y, int unit_type),
    png_voidp png_get_io_ptr(png_const_structrp png_ptr),
    void png_set_cHRM_XYZ_fixed(png_const_structrp png_ptr, png_inforp info_ptr, png_fixed_point int_red_X, png_fixed_point int_red_Y, png_fixed_point int_red_Z, png_fixed_point int_green_X, png_fixed_point int_green_Y, png_fixed_point int_green_Z, png_fixed_point int_blue_X, png_fixed_point int_blue_Y, png_fixed_point int_blue_Z),
    png_structp png_create_read_struct(png_const_charp user_png_ver, png_voidp error_ptr, png_error_ptr error_fn, png_error_ptr warn_fn),
    png_fixed_point png_get_x_offset_inches_fixed(png_const_structrp png_ptr, png_const_inforp info_ptr)
*/
//<score> 14, nr_unique_branch: 1
//<Quality> {"density":7,"unique_branches":{"png_muldiv":[[3355,21,3355,31,0,0,4,0]]},"library_calls":["png_create_read_struct","png_create_info_struct","png_destroy_read_struct","png_set_pHYs","png_get_io_ptr","png_set_cHRM_XYZ_fixed","png_destroy_read_struct"],"critical_calls":["png_create_read_struct","png_create_info_struct","png_set_pHYs","png_get_io_ptr","png_set_cHRM_XYZ_fixed","png_destroy_read_struct"],"visited":0}
/**/

extern "C" int LLVMFuzzerTestOneInput_99(const uint8_t *data, size_t size) {
	if(size<0) return 0;

    // Step 4: Open input file
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        std::cerr << "Error opening input file" << std::endl;
        assert_file_closed(&in_file);
	return 0;
    }

    // Step 5: Get file descriptor for reading
    int in_fd = fuzz_fileno(in_file);

    // Step 6: Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        std::cerr << "Error opening output file" << std::endl;
        assert_file_closed(&in_file);;
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
    }

    // Step 5: Get file descriptor for writing
    int out_fd = fuzz_fileno(out_file);

    // Step 1: Set pHYs values
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        std::cerr << "Error creating PNG read structure" << std::endl;
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        std::cerr << "Error creating PNG info structure" << std::endl;
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }

    png_uint_32 res_x = 300;
    png_uint_32 res_y = 300;
    int unit_type = PNG_RESOLUTION_METER;

    png_set_pHYs(png_ptr, info_ptr, res_x, res_y, unit_type);

    // Step 3: Get IO pointer
    png_voidp io_ptr = png_get_io_ptr(png_ptr);

    // Step 2: Set cHRM_XYZ_fixed values
    png_fixed_point int_red_X = 0x1234;
    png_fixed_point int_red_Y = 0x2345;
    png_fixed_point int_red_Z = 0x3456;
    png_fixed_point int_green_X = 0x4567;
    png_fixed_point int_green_Y = 0x5678;
    png_fixed_point int_green_Z = 0x6789;
    png_fixed_point int_blue_X = 0x789a;
    png_fixed_point int_blue_Y = 0x89ab;
    png_fixed_point int_blue_Z = 0x9abc;

    png_set_cHRM_XYZ_fixed(png_ptr, info_ptr, int_red_X, int_red_Y, int_red_Z, int_green_X, int_green_Y, int_green_Z, int_blue_X, int_blue_Y, int_blue_Z);

    // Step 4: Cleanup
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

    assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}