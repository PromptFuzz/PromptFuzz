#include "FDSan.h"
#include <png.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 2186
//<Prompt> ["png_warning","png_set_sCAL_fixed","png_get_cHRM_XYZ_fixed","png_get_cHRM","png_read_update_info"]
/*<Combination>: [void png_warning(png_const_structrp png_ptr, png_const_charp warning_message),
    void png_set_sCAL_fixed(png_const_structrp png_ptr, png_inforp info_ptr, int unit, png_fixed_point width, png_fixed_point height),
    png_uint_32 png_get_cHRM_XYZ_fixed(png_const_structrp png_ptr, png_const_inforp info_ptr, png_fixed_point * int_red_X, png_fixed_point * int_red_Y, png_fixed_point * int_red_Z, png_fixed_point * int_green_X, png_fixed_point * int_green_Y, png_fixed_point * int_green_Z, png_fixed_point * int_blue_X, png_fixed_point * int_blue_Y, png_fixed_point * int_blue_Z),
    png_uint_32 png_get_cHRM(png_const_structrp png_ptr, png_const_inforp info_ptr, double * white_x, double * white_y, double * red_x, double * red_y, double * green_x, double * green_y, double * blue_x, double * blue_y),
    void png_read_update_info(png_structrp png_ptr, png_inforp info_ptr)
*/
//<score> 26, nr_unique_branch: 3
//<Quality> {"density":13,"unique_branches":{"png_ascii_from_fixed":[[3265,32,3265,39,0,0,4,1],[3272,20,3272,31,0,0,4,0],[3277,17,3277,27,0,0,4,1]]},"library_calls":["png_create_read_struct","png_create_info_struct","png_destroy_read_struct","png_set_longjmp_fn","png_destroy_read_struct","png_set_error_fn","png_init_io","png_set_write_fn","png_read_update_info","png_warning","png_set_sCAL_fixed","png_get_cHRM_XYZ_fixed","png_get_cHRM","png_destroy_read_struct"],"critical_calls":["png_create_read_struct","png_create_info_struct","png_set_longjmp_fn","png_set_error_fn","png_init_io","png_set_write_fn","png_read_update_info","png_warning","png_set_sCAL_fixed","png_get_cHRM_XYZ_fixed","png_get_cHRM","png_destroy_read_struct"],"visited":0}
/**/


// Include libpng headers

// Custom error handling function for libpng
void  id_000045_png_error_handler(png_structp png_ptr, png_const_charp error_msg)
{
    longjmp(png_jmpbuf(png_ptr), 1);
}

extern "C" int LLVMFuzzerTestOneInput_45(const uint8_t *data, size_t size)
{
	if(size<0) return 0;

	FUZZ_FILE_INIT();
    // Create FILE pointer for input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
	FUZZ_FILE_PUSH(in_file)
    if (in_file == nullptr) {
        assert_file_closed(&in_file);
	return 0;
    }
	FUZZ_FILENAME_PUSH("output_file")

    // Create FILE pointer for output data
    FILE *out_file = fopen("output_file", "wb");
	FUZZ_FILE_PUSH(out_file)
    if (out_file == nullptr) {
        assert_file_closed(&in_file);;
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Create libpng structures
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (png_ptr == nullptr) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == nullptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Set custom error handling function
    if (setjmp(png_jmpbuf(png_ptr))) {
	FUZZ_FILE_CLOSE();
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    png_set_error_fn(png_ptr, nullptr,  id_000045_png_error_handler, nullptr);
    
    // Set input/output functions for libpng
    png_init_io(png_ptr, in_file);
    png_set_write_fn(png_ptr, out_file, nullptr, nullptr);

    // Read PNG header
    png_read_update_info(png_ptr, info_ptr);

    // Call libpng APIs to achieve desired event
    png_warning(png_ptr, "This is a warning message");

    png_fixed_point width = PNG_FP_1, height = PNG_FP_1;
    png_set_sCAL_fixed(png_ptr, info_ptr, PNG_SCALE_METER, width, height);

    png_fixed_point int_red_X, int_red_Y, int_red_Z, int_green_X, int_green_Y, int_green_Z, int_blue_X, int_blue_Y, int_blue_Z;
    png_get_cHRM_XYZ_fixed(png_ptr, info_ptr, &int_red_X, &int_red_Y, &int_red_Z, &int_green_X, &int_green_Y, &int_green_Z, &int_blue_X, &int_blue_Y, &int_blue_Z);

    double white_x, white_y, red_x, red_y, green_x, green_y, blue_x, blue_y;
    png_get_cHRM(png_ptr, info_ptr, &white_x, &white_y, &red_x, &red_y, &green_x, &green_y, &blue_x, &blue_y);

    // Clean up allocated resources
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;

    assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}