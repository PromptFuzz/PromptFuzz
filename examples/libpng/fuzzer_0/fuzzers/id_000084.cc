#include "FDSan.h"
#include <png.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 3918
//<Prompt> ["png_image_write_to_file","png_set_shift","png_set_compression_strategy","png_set_cHRM_fixed","png_set_keep_unknown_chunks","png_set_rows","png_set_PLTE"]
/*<Combination>: [int png_image_write_to_file(png_imagep image, const char * file, int convert_to_8bit, const void * buffer, png_int_32 row_stride, const void * colormap),
    void png_set_shift(png_structrp png_ptr, png_const_color_8p true_bits),
    void png_set_compression_strategy(png_structrp png_ptr, int strategy),
    void png_set_cHRM_fixed(png_const_structrp png_ptr, png_inforp info_ptr, png_fixed_point int_white_x, png_fixed_point int_white_y, png_fixed_point int_red_x, png_fixed_point int_red_y, png_fixed_point int_green_x, png_fixed_point int_green_y, png_fixed_point int_blue_x, png_fixed_point int_blue_y),
    void png_set_keep_unknown_chunks(png_structrp png_ptr, int keep, png_const_bytep chunk_list, int num_chunks),
    void png_set_rows(png_const_structrp png_ptr, png_inforp info_ptr, png_bytepp row_pointers),
    void png_set_PLTE(png_structrp png_ptr, png_inforp info_ptr, png_const_colorp palette, int num_palette)
*/
//<score> 2, nr_unique_branch: 6
//<Quality> {"density":1,"unique_branches":{"png_image_write_to_file":[[2349,32,2349,46,0,0,4,0],[2353,14,2353,24,0,0,4,0],[2355,17,2356,43,0,0,4,0],[2361,20,2361,35,0,0,4,0],[2361,39,2361,54,0,0,4,0],[2363,23,2363,38,0,0,4,0]]},"library_calls":["png_image_write_to_file"],"critical_calls":["png_image_write_to_file"],"visited":0}
/**/

extern "C" int LLVMFuzzerTestOneInput_84(const uint8_t *data, size_t size) {
	if(size<0) return 0;

    // Create a PNG image structure
    png_image image;
    memset(&image, 0, sizeof(image));

    // Set the version number
    image.version = PNG_IMAGE_VERSION;

    // Set the width and height of the image
    image.width = 100;
    image.height = 100;

    // Set the format of the image
    image.format = PNG_FORMAT_RGBA;

    // Allocate memory for the image buffer
    size_t bufsize = PNG_IMAGE_SIZE(image);
    char *buffer = (char *)malloc(bufsize);
    if (!buffer) {
        return 0;
    }

    // Copy the input data to the image buffer
    memcpy(buffer, data, size);

    // Write the PNG image to a file
    png_image_write_to_file(&image, "output.png", 0, buffer, 0, NULL);

    // Free the image buffer
    free(buffer);

    // Return 0 to indicate successful execution
    return 0;
}