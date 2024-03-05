#include "FDSan.h"
#include <png.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 940
//<Prompt> ["png_get_pixels_per_inch","png_set_compression_window_bits","png_set_text","png_set_gAMA","png_set_swap_alpha","png_get_x_offset_microns","png_image_write_to_file","png_get_y_offset_microns","png_read_rows"]
/*<Combination>: [png_uint_32 png_get_pixels_per_inch(png_const_structrp png_ptr, png_const_inforp info_ptr),
    void png_set_compression_window_bits(png_structrp png_ptr, int window_bits),
    void png_set_text(png_const_structrp png_ptr, png_inforp info_ptr, png_const_textp text_ptr, int num_text),
    void png_set_gAMA(png_const_structrp png_ptr, png_inforp info_ptr, double file_gamma),
    void png_set_swap_alpha(png_structrp png_ptr),
    png_int_32 png_get_x_offset_microns(png_const_structrp png_ptr, png_const_inforp info_ptr),
    int png_image_write_to_file(png_imagep image, const char * file, int convert_to_8bit, const void * buffer, png_int_32 row_stride, const void * colormap),
    png_int_32 png_get_y_offset_microns(png_const_structrp png_ptr, png_const_inforp info_ptr),
    void png_read_rows(png_structrp png_ptr, png_bytepp row, png_bytepp display_row, png_uint_32 num_rows)
*/
//<score> 42, nr_unique_branch: 11
//<Quality> {"density":6,"unique_branches":{"png_read_rows":[[654,8,654,23,0,0,4,1],[659,8,659,18,0,0,4,1],[668,13,668,23,0,0,4,1],[676,13,676,23,0,0,4,1]],"png_get_x_offset_microns":[[250,8,250,23,0,0,4,1]],"png_get_pixels_per_meter":[[173,8,173,23,0,0,4,1]],"png_get_y_offset_microns":[[270,8,270,23,0,0,4,1]],"png_set_text":[[756,8,756,16,0,0,4,1]],"png_image_write_to_file":[[2347,8,2347,21,0,0,4,0],[2347,25,2347,60,0,0,4,1],[2400,13,2400,26,0,0,4,0]]},"library_calls":["png_get_pixels_per_inch","png_create_read_struct","png_set_compression_window_bits","png_create_info_struct","png_set_text","png_get_x_offset_microns","png_get_y_offset_microns","png_read_rows","png_image_write_to_file","png_destroy_read_struct"],"critical_calls":["png_get_pixels_per_inch","png_create_read_struct","png_set_compression_window_bits","png_create_info_struct","png_set_text","png_get_x_offset_microns","png_get_y_offset_microns","png_read_rows","png_image_write_to_file","png_destroy_read_struct"],"visited":0}
/**/

extern "C" int LLVMFuzzerTestOneInput_15(const uint8_t *data, size_t size) {
	if(size<0) return 0;

    // Step 4: Create input file
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
  
    // Step 5: Get file descriptor for reading
    int in_file_fd = fuzz_fileno(in_file);

    // Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(in_file_fd);
	return 0;
    }

    // Get file descriptor for writing
    int out_file_fd = fuzz_fileno(out_file);

    // Step 1: Get pixels per inch
    png_uint_32 pixels_per_inch = png_get_pixels_per_inch(NULL, NULL);

    // Step 2: Set compression window bits
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    png_set_compression_window_bits(png_ptr, 15);

    // Step 3: Set text info
    png_info* info_ptr = png_create_info_struct(png_ptr);
    png_text text;
    text.compression = PNG_TEXT_COMPRESSION_NONE;
    text.key = (png_charp)"Key";
    text.text = (png_charp)"Text";
    text.text_length = strlen(text.text);
    text.itxt_length = 0;
    text.lang = NULL;
    text.lang_key = NULL;
    png_set_text(png_ptr, info_ptr, &text, 1);

    // Step 6: Get x offset in microns
    png_int_32 x_offset_microns = png_get_x_offset_microns(NULL, NULL);

    // Step 7: Get y offset in microns
    png_int_32 y_offset_microns = png_get_y_offset_microns(NULL, NULL);

    // Read rows
    png_bytepp row_pointers;
    png_uint_32 num_rows = 10;
    png_read_rows(png_ptr, row_pointers, NULL, num_rows);

    // Write image to file
    png_image image;
    memset(&image, 0, sizeof(image));
    image.width = 1000;
    image.height = 1000;

    png_image_write_to_file(&image, "output_file", 1, NULL, 0, NULL);

    // Release resources
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

    assert_file_name_closed("output_file");
	assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_file_fd);
	assert_fd_closed(in_file_fd);
	return 0;
}