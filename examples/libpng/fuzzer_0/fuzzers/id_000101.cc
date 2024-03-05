#include "FDSan.h"
#include <png.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 4686
//<Prompt> ["png_set_compression_strategy","png_get_libpng_ver","png_convert_from_time_t","png_set_chunk_cache_max","png_get_x_offset_inches","png_image_begin_read_from_file"]
/*<Combination>: [void png_set_compression_strategy(png_structrp png_ptr, int strategy),
    png_const_charp png_get_libpng_ver(png_const_structrp png_ptr),
    void png_convert_from_time_t(png_timep ptime, time_t ttime),
    void png_set_chunk_cache_max(png_structrp png_ptr, png_uint_32 user_chunk_cache_max),
    float png_get_x_offset_inches(png_const_structrp png_ptr, png_const_inforp info_ptr),
    int png_image_begin_read_from_file(png_imagep image, const char * file_name)
*/
//<score> 9, nr_unique_branch: 0
//<Quality> {"density":9,"unique_branches":{},"library_calls":["png_create_read_struct","png_create_info_struct","png_destroy_read_struct","png_set_longjmp_fn","png_destroy_read_struct","png_destroy_read_struct","png_init_io","png_read_png","png_get_IHDR","png_destroy_read_struct"],"critical_calls":["png_create_read_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_read_png","png_get_IHDR","png_destroy_read_struct"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_101(const uint8_t *data, size_t size) {
	if(size<0) return 0;

	FUZZ_FILE_INIT();
    // Create a PNG structure
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr) {
        return 0;
    }

    // Create an info structure
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        return 0;
    }

    // Set up error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
	FUZZ_FILE_CLOSE();
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        return 0;
    }

    // Set the input function
    FILE *in_file = fmemopen((void *)data, size, "rb");
	FUZZ_FILE_PUSH(in_file)
    if (!in_file) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        assert_file_closed(&in_file);
	return 0;
    }
    png_init_io(png_ptr, in_file);

    // Read the PNG image
    png_read_png(png_ptr, info_ptr, 0, nullptr);

    // Retrieve image information
    png_uint_32 width, height;
    int bit_depth, color_type, interlace_type, compression_type, filter_type;
    png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type, &interlace_type,
                 &compression_type, &filter_type);

    // Print the image information
    printf("Image size: %d x %d\n", width, height);
    printf("Bit depth: %d\n", bit_depth);
    printf("Color type: %d\n", color_type);

    // Clean up resources
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    assert_file_closed(&in_file);;

    assert_file_closed(&in_file);
	return 0;
}