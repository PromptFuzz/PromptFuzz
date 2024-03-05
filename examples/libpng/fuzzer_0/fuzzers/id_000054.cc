#include "FDSan.h"
#include <png.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 2734
//<Prompt> ["png_get_sCAL_s","png_get_sRGB","png_set_alpha_mode","png_get_user_transform_ptr","png_set_check_for_invalid_index","png_get_gAMA","png_set_strip_16","png_set_rgb_to_gray_fixed","png_get_image_width"]
/*<Combination>: [png_uint_32 png_get_sCAL_s(png_const_structrp png_ptr, png_const_inforp info_ptr, int * unit, png_charpp swidth, png_charpp sheight),
    png_uint_32 png_get_sRGB(png_const_structrp png_ptr, png_const_inforp info_ptr, int * file_srgb_intent),
    void png_set_alpha_mode(png_structrp png_ptr, int mode, double output_gamma),
    png_voidp png_get_user_transform_ptr(png_const_structrp png_ptr),
    void png_set_check_for_invalid_index(png_structrp png_ptr, int allowed),
    png_uint_32 png_get_gAMA(png_const_structrp png_ptr, png_const_inforp info_ptr, double * file_gamma),
    void png_set_strip_16(png_structrp png_ptr),
    void png_set_rgb_to_gray_fixed(png_structrp png_ptr, int error_action, png_fixed_point red, png_fixed_point green),
    png_uint_32 png_get_image_width(png_const_structrp png_ptr, png_const_inforp info_ptr)
*/
//<score> 68, nr_unique_branch: 16
//<Quality> {"density":17,"unique_branches":{"png_rtran_ok":[[118,16,118,25,0,0,4,0],[118,29,118,65,0,0,4,1]],"png_set_rgb_to_gray_fixed":[[977,8,977,37,0,0,4,1],[982,7,982,33,0,0,4,0],[986,7,986,33,0,0,4,1],[990,7,990,34,0,0,4,1],[994,7,994,14,0,0,4,1],[998,8,998,53,0,0,4,0],[998,8,998,53,0,0,4,1],[1013,11,1013,19,0,0,4,0],[1013,23,1013,33,0,0,4,0],[1013,37,1013,60,0,0,4,0]],"png_get_sCAL_s":[[974,8,974,23,0,0,4,0],[974,27,974,43,0,0,4,0],[975,8,975,46,0,0,4,0],[975,8,975,46,0,0,4,1]]},"library_calls":["png_create_read_struct","png_create_info_struct","png_destroy_read_struct","png_set_longjmp_fn","png_destroy_read_struct","png_destroy_read_struct","png_init_io","png_read_info","png_get_image_width","png_get_sCAL_s","png_get_sRGB","png_set_alpha_mode","png_get_user_transform_ptr","png_set_check_for_invalid_index","png_get_gAMA","png_set_strip_16","png_set_rgb_to_gray_fixed","png_destroy_read_struct"],"critical_calls":["png_create_read_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_read_info","png_get_image_width","png_get_sCAL_s","png_get_sRGB","png_set_alpha_mode","png_get_user_transform_ptr","png_set_check_for_invalid_index","png_get_gAMA","png_set_strip_16","png_set_rgb_to_gray_fixed","png_destroy_read_struct"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_54(const uint8_t *data, size_t size) {
	if(size<0) return 0;

	FUZZ_FILE_INIT();
    // Create a PNG read struct and info struct
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        return 0;
    }
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return 0;
    }

    // Set up error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
	FUZZ_FILE_CLOSE();
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return 0;
    }

    // Create a FILE pointer from the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
	FUZZ_FILE_PUSH(in_file)
    if (!in_file) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        assert_file_closed(&in_file);
	return 0;
    }

    // Set up PNG input as file stream
    png_init_io(png_ptr, in_file);

    // Read the PNG header and get image width
    png_read_info(png_ptr, info_ptr);
    int width = png_get_image_width(png_ptr, info_ptr);

    // Do some operations using the libpng APIs
    int unit = 0;
    char *swidth = NULL;
    char *sheight = NULL;
    png_uint_32 sCAL_s = png_get_sCAL_s(png_ptr, info_ptr, &unit, &swidth, &sheight);

    int file_srgb_intent = 0;
    png_uint_32 sRGB = png_get_sRGB(png_ptr, info_ptr, &file_srgb_intent);

    png_set_alpha_mode(png_ptr, PNG_ALPHA_STANDARD, 1.0);
    png_voidp user_transform_ptr = png_get_user_transform_ptr(png_ptr);

    png_set_check_for_invalid_index(png_ptr, 1);

    double file_gamma = 0.0;
    png_uint_32 gAMA = png_get_gAMA(png_ptr, info_ptr, &file_gamma);

    png_set_strip_16(png_ptr);

    png_fixed_point red = 0;
    png_fixed_point green = 0;
    png_set_rgb_to_gray_fixed(png_ptr, PNG_ERROR_ACTION_NONE, red, green);

    // Clean up
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    assert_file_closed(&in_file);;

    assert_file_closed(&in_file);
	return 0;
}