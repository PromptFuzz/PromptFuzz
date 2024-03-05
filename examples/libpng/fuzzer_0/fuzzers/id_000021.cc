#include "FDSan.h"
#include <png.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 1440
//<Prompt> ["png_set_palette_to_rgb","png_set_rgb_to_gray_fixed","png_get_uint_32","png_get_pixels_per_inch","png_get_error_ptr","png_get_x_offset_inches_fixed","png_image_begin_read_from_file","png_set_sPLT"]
/*<Combination>: [void png_set_palette_to_rgb(png_structrp png_ptr),
    void png_set_rgb_to_gray_fixed(png_structrp png_ptr, int error_action, png_fixed_point red, png_fixed_point green),
    png_uint_32 png_get_uint_32(png_const_bytep buf),
    png_uint_32 png_get_pixels_per_inch(png_const_structrp png_ptr, png_const_inforp info_ptr),
    png_voidp png_get_error_ptr(png_const_structrp png_ptr),
    png_fixed_point png_get_x_offset_inches_fixed(png_const_structrp png_ptr, png_const_inforp info_ptr),
    int png_image_begin_read_from_file(png_imagep image, const char * file_name),
    void png_set_sPLT(png_const_structrp png_ptr, png_inforp info_ptr, png_const_sPLT_tp entries, int nentries)
*/
//<score> 12, nr_unique_branch: 10
//<Quality> {"density":2,"unique_branches":{"png_get_error_ptr":[[858,8,858,23,0,0,4,0]],"png_image_free_function":[[4536,11,4536,30,0,0,4,0],[4542,14,4542,24,0,0,4,0]],"png_set_palette_to_rgb":[[906,8,906,37,0,0,4,0]],"png_image_begin_read_from_file":[[1504,8,1504,21,0,0,4,0],[1504,25,1504,60,0,0,4,0],[1506,11,1506,28,0,0,4,0],[1510,14,1510,24,0,0,4,0],[1512,17,1512,48,0,0,4,0]],"png_set_sPLT":[[1066,8,1066,23,0,0,4,0]]},"library_calls":["png_set_palette_to_rgb","png_set_rgb_to_gray_fixed","png_get_pixels_per_inch","png_get_error_ptr","png_get_x_offset_inches_fixed","png_image_begin_read_from_file","png_set_sPLT","png_image_free"],"critical_calls":["png_set_palette_to_rgb","png_set_rgb_to_gray_fixed","png_get_pixels_per_inch","png_get_error_ptr","png_get_x_offset_inches_fixed","png_image_begin_read_from_file","png_set_sPLT","png_image_free"],"visited":0}
/**/

extern "C" int LLVMFuzzerTestOneInput_21(const uint8_t *data, size_t size) {
	if(size<0) return 0;

	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);

    // Step 4: Create input file
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Step 6: Define file name
    const char *file_name = "input_file";

    // Step 2: Initialize libpng structures
    png_image image;
    memset(&image, 0, sizeof(image));
    image.version = PNG_IMAGE_VERSION;

    // Step 5: Get file descriptor
    int fd = fuzz_fileno(in_file);

    // Step 1: Set up libpng options
    png_set_palette_to_rgb(NULL);
    png_set_rgb_to_gray_fixed(NULL, 0, 0, 0);
    png_uint_32 uint32_data = png_get_uint_32((png_const_bytep)data);
    png_uint_32 pixels_per_inch = png_get_pixels_per_inch(NULL, NULL);
    png_voidp error_ptr = png_get_error_ptr(NULL);
    png_fixed_point x_offset_inches_fixed = png_get_x_offset_inches_fixed(NULL, NULL);
    int begin_read_result = png_image_begin_read_from_file(&image, file_name);
    png_sPLT_t entries;
    memset(&entries, 0, sizeof(entries));
    entries.name = "sPLT";
    entries.depth = 8;
    png_set_sPLT(NULL, NULL, &entries, 1);

    // Step 7: Release resources
    png_image_free(&image);
    assert_file_closed(&in_file);;

    assert_file_name_closed("input_file");
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
}