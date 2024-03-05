#include "FDSan.h"
#include <png.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 1890
//<Prompt> ["png_get_cHRM","png_save_uint_16","png_get_pixel_aspect_ratio","png_permit_mng_features","png_destroy_read_struct","png_get_x_pixels_per_inch","png_get_y_offset_pixels","png_write_chunk"]
/*<Combination>: [png_uint_32 png_get_cHRM(png_const_structrp png_ptr, png_const_inforp info_ptr, double * white_x, double * white_y, double * red_x, double * red_y, double * green_x, double * green_y, double * blue_x, double * blue_y),
    void png_save_uint_16(png_bytep buf, unsigned int i),
    float png_get_pixel_aspect_ratio(png_const_structrp png_ptr, png_const_inforp info_ptr),
    png_uint_32 png_permit_mng_features(png_structrp png_ptr, png_uint_32 mng_features_permitted),
    void png_destroy_read_struct(png_structpp png_ptr_ptr, png_infopp info_ptr_ptr, png_infopp end_info_ptr_ptr),
    png_uint_32 png_get_x_pixels_per_inch(png_const_structrp png_ptr, png_const_inforp info_ptr),
    png_int_32 png_get_y_offset_pixels(png_const_structrp png_ptr, png_const_inforp info_ptr),
    void png_write_chunk(png_structrp png_ptr, png_const_bytep chunk_name, png_const_bytep data, size_t length)
*/
//<score> 99, nr_unique_branch: 24
//<Quality> {"density":11,"unique_branches":{"png_do_read_transformations":[[4926,8,4926,53,0,0,4,0],[4975,8,4975,50,0,0,4,0]],"png_do_chop":[[2454,8,2454,33,0,0,4,0],[2454,8,2454,33,0,0,4,1],[2460,14,2460,21,0,0,4,0],[2460,14,2460,21,0,0,4,1]],"png_do_unpack":[[2156,8,2156,31,0,0,4,1]],"png_read_transform_info":[[2011,14,2011,59,0,0,4,0],[2074,8,2074,50,0,0,4,0],[2075,8,2075,33,0,0,4,1]],"png_set_packing":[[57,8,57,30,0,0,4,1]],"png_read_png":[[1085,8,1085,50,0,0,4,0],[1105,8,1105,49,0,0,4,0],[1127,8,1127,48,0,0,4,0],[1193,8,1193,53,0,0,4,0]],"png_read_start_row":[[4450,8,4450,50,0,0,4,0],[4450,54,4450,76,0,0,4,0],[4539,11,4539,59,0,0,4,1],[4550,14,4550,34,0,0,4,0],[4550,14,4550,34,0,0,4,1],[4552,17,4552,64,0,0,4,1],[4559,19,4559,66,0,0,4,0],[4559,19,4559,66,0,0,4,1]],"png_do_gray_to_rgb":[[2868,8,2868,58,0,0,4,1]]},"library_calls":["png_create_read_struct","png_create_info_struct","png_destroy_read_struct","png_set_longjmp_fn","png_destroy_read_struct","png_init_io","png_read_png","png_get_rows","png_get_image_width","png_get_image_height","png_get_channels","png_destroy_read_struct"],"critical_calls":["png_create_read_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_read_png","png_get_rows","png_get_image_width","png_get_image_height","png_get_channels","png_destroy_read_struct"],"visited":0}
/**/



extern "C" int LLVMFuzzerTestOneInput_35(const uint8_t *data, size_t size)
{
	if(size<0) return 0;

	FUZZ_FILE_INIT();
    FILE *in_file = fmemopen((void *)data, size, "rb");
	FUZZ_FILE_PUSH(in_file)
    if (in_file == NULL) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Read PNG image
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (png_ptr == NULL) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == NULL) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // Set up error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
	FUZZ_FILE_CLOSE();
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // Initialize PNG I/O
    png_init_io(png_ptr, in_file);

    // Read PNG data
    png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_STRIP_16 | PNG_TRANSFORM_PACKING | PNG_TRANSFORM_EXPAND | PNG_TRANSFORM_GRAY_TO_RGB, NULL);

    // Access PNG data
    png_bytep *row_pointers = png_get_rows(png_ptr, info_ptr);
    int width = png_get_image_width(png_ptr, info_ptr);
    int height = png_get_image_height(png_ptr, info_ptr);
    int channels = png_get_channels(png_ptr, info_ptr);

    // Process PNG data
    // TODO: Add your code here to process the PNG data

    // Clean up
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    assert_file_closed(&in_file);;

    assert_file_closed(&in_file);
	return 0;
}