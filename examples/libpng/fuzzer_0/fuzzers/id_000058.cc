#include "FDSan.h"
#include <png.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 2837
//<Prompt> ["png_set_swap_alpha","png_write_info","png_set_background","png_get_sCAL_s","png_chunk_warning","png_set_write_user_transform_fn","png_permit_mng_features","png_reset_zstream","png_get_x_offset_inches_fixed"]
/*<Combination>: [void png_set_swap_alpha(png_structrp png_ptr),
    void png_write_info(png_structrp png_ptr, png_const_inforp info_ptr),
    void png_set_background(png_structrp png_ptr, png_const_color_16p background_color, int background_gamma_code, int need_expand, double background_gamma),
    png_uint_32 png_get_sCAL_s(png_const_structrp png_ptr, png_const_inforp info_ptr, int * unit, png_charpp swidth, png_charpp sheight),
    void png_chunk_warning(png_const_structrp png_ptr, png_const_charp warning_message),
    void png_set_write_user_transform_fn(png_structrp png_ptr, png_user_transform_ptr write_user_transform_fn),
    png_uint_32 png_permit_mng_features(png_structrp png_ptr, png_uint_32 mng_features_permitted),
    int png_reset_zstream(png_structrp png_ptr),
    png_fixed_point png_get_x_offset_inches_fixed(png_const_structrp png_ptr, png_const_inforp info_ptr)
*/
//<score> 176, nr_unique_branch: 14
//<Quality> {"density":16,"unique_branches":{"png_init_io":[[707,8,707,23,0,0,4,0]],"png_set_write_user_transform_fn":[[1346,8,1346,23,0,0,4,0]],"png_write_info":[[200,8,200,23,0,0,4,0]],"png_get_sCAL_s":[[974,8,974,23,0,0,4,1]],"png_write_info_before_PLTE":[[97,11,97,56,0,0,4,1],[133,11,133,69,0,0,4,1],[145,14,145,72,0,0,4,1],[163,14,163,72,0,0,4,1],[176,14,176,72,0,0,4,1]],"png_chunk_warning":[[500,8,500,23,0,0,4,0]],"png_reset_zstream":[[976,8,976,23,0,0,4,0]],"png_set_background_fixed":[[143,8,143,37,0,0,4,0]],"png_read_info":[[100,8,100,23,0,0,4,0]],"png_write_data":[[36,8,36,38,0,0,4,1]]},"library_calls":["png_create_read_struct","png_create_info_struct","png_destroy_read_struct","png_set_longjmp_fn","png_destroy_read_struct","png_init_io","png_read_info","png_set_swap_alpha","png_write_info","png_set_background","png_get_sCAL_s","png_chunk_warning","png_set_write_user_transform_fn","png_permit_mng_features","png_reset_zstream","png_get_x_offset_inches_fixed","png_destroy_read_struct"],"critical_calls":["png_create_read_struct","png_create_info_struct","png_set_longjmp_fn","png_destroy_read_struct","png_init_io","png_read_info","png_set_swap_alpha","png_write_info","png_set_background","png_get_sCAL_s","png_chunk_warning","png_set_write_user_transform_fn","png_permit_mng_features","png_reset_zstream","png_get_x_offset_inches_fixed","png_destroy_read_struct"],"visited":0}
/**/

extern "C" int LLVMFuzzerTestOneInput_58(const uint8_t *data, size_t size) {
	if(size<0) return 0;

	FUZZ_FILE_INIT();
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

    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
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
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    if (setjmp(png_jmpbuf(png_ptr))) {
	FUZZ_FILE_CLOSE();
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
    }

    png_init_io(png_ptr, in_file);
    png_read_info(png_ptr, info_ptr);
    png_set_swap_alpha(png_ptr);
    png_write_info(png_ptr, info_ptr);

    png_color_16 background_color;
    background_color.index = 0;
    background_color.red = 255;
    background_color.green = 255;
    background_color.blue = 255;
    background_color.gray = 0;
    png_set_background(png_ptr, &background_color, PNG_BACKGROUND_GAMMA_SCREEN, 0, 0.45);

    int unit;
    png_charp swidth, sheight;
    png_get_sCAL_s(png_ptr, info_ptr, &unit, &swidth, &sheight);

    png_chunk_warning(png_ptr, "Warning Message");

    png_user_transform_ptr write_user_transform_fn = [](png_structp png_ptr, png_row_infop row_info, png_bytep data) {
        // User-defined transformation function
        // Modify the pixel data if needed
    };
    png_set_write_user_transform_fn(png_ptr, write_user_transform_fn);

    png_uint_32 mng_features_permitted = png_permit_mng_features(png_ptr, 0xABCDEF);
    int reset_status = png_reset_zstream(png_ptr);

    png_fixed_point x_offset_inches_fixed = png_get_x_offset_inches_fixed(png_ptr, info_ptr);

    // Perform further operations with the libpng API

    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;

    assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}