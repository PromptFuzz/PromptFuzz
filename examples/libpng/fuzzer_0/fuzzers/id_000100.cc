#include "FDSan.h"
#include <png.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 4677
//<Prompt> ["png_set_pHYs","png_get_io_ptr","png_set_cHRM_XYZ_fixed","png_create_read_struct","png_get_x_offset_inches_fixed"]
/*<Combination>: [void png_set_pHYs(png_const_structrp png_ptr, png_inforp info_ptr, png_uint_32 res_x, png_uint_32 res_y, int unit_type),
    png_voidp png_get_io_ptr(png_const_structrp png_ptr),
    void png_set_cHRM_XYZ_fixed(png_const_structrp png_ptr, png_inforp info_ptr, png_fixed_point int_red_X, png_fixed_point int_red_Y, png_fixed_point int_red_Z, png_fixed_point int_green_X, png_fixed_point int_green_Y, png_fixed_point int_green_Z, png_fixed_point int_blue_X, png_fixed_point int_blue_Y, png_fixed_point int_blue_Z),
    png_structp png_create_read_struct(png_const_charp user_png_ver, png_voidp error_ptr, png_error_ptr error_fn, png_error_ptr warn_fn),
    png_fixed_point png_get_x_offset_inches_fixed(png_const_structrp png_ptr, png_const_inforp info_ptr)
*/
//<score> 8, nr_unique_branch: 0
//<Quality> {"density":8,"unique_branches":{},"library_calls":["png_create_read_struct","png_init_io","png_set_write_fn","png_set_pHYs","png_get_io_ptr","png_set_cHRM_XYZ_fixed","png_get_x_offset_inches_fixed","png_destroy_read_struct"],"critical_calls":["png_create_read_struct","png_init_io","png_set_write_fn","png_set_pHYs","png_get_io_ptr","png_set_cHRM_XYZ_fixed","png_get_x_offset_inches_fixed","png_destroy_read_struct"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_100(const uint8_t *data, size_t size) {
	if(size<0) return 0;

    // Create a FILE pointer to read the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Create a FILE pointer to write the output data
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Create the read structure
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Set the input and output handles
    png_init_io(png_ptr, in_file);
    png_set_write_fn(png_ptr, out_file, NULL, NULL);
    
    // Set the physical pixel resolution
    png_set_pHYs(png_ptr, NULL, 300, 300, PNG_RESOLUTION_METER);
    
    // Get the input handle
    png_voidp io_ptr = png_get_io_ptr(png_ptr);
    
    // Set the cHRM_XYZ values
    png_fixed_point int_red_X = 0x1234;
    png_fixed_point int_red_Y = 0x5678;
    png_fixed_point int_red_Z = 0x9abc;
    png_fixed_point int_green_X = 0xdef0;
    png_fixed_point int_green_Y = 0x1234;
    png_fixed_point int_green_Z = 0x5678;
    png_fixed_point int_blue_X = 0x9abc;
    png_fixed_point int_blue_Y = 0xdef0;
    png_fixed_point int_blue_Z = 0x1234;
    png_set_cHRM_XYZ_fixed(png_ptr, NULL, int_red_X, int_red_Y, int_red_Z, int_green_X, int_green_Y, int_green_Z, int_blue_X, int_blue_Y, int_blue_Z);
    
    // Get the X offset in inches
    png_fixed_point x_offset_inches_fixed = png_get_x_offset_inches_fixed(png_ptr, NULL);
    
    // Clean up and release resources
    png_destroy_read_struct(&png_ptr, NULL, NULL);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}