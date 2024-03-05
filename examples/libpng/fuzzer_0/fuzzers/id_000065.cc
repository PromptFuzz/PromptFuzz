#include "FDSan.h"
#include <png.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 3169
//<Prompt> ["png_get_image_height","png_get_header_version","png_get_tRNS","png_set_compression_strategy","png_get_x_offset_pixels","png_build_grayscale_palette","png_set_sPLT","png_set_IHDR"]
/*<Combination>: [png_uint_32 png_get_image_height(png_const_structrp png_ptr, png_const_inforp info_ptr),
    png_const_charp png_get_header_version(png_const_structrp png_ptr),
    png_uint_32 png_get_tRNS(png_const_structrp png_ptr, png_inforp info_ptr, png_bytep * trans_alpha, int * num_trans, png_color_16p * trans_color),
    void png_set_compression_strategy(png_structrp png_ptr, int strategy),
    png_int_32 png_get_x_offset_pixels(png_const_structrp png_ptr, png_const_inforp info_ptr),
    void png_build_grayscale_palette(int bit_depth, png_colorp palette),
    void png_set_sPLT(png_const_structrp png_ptr, png_inforp info_ptr, png_const_sPLT_tp entries, int nentries),
    void png_set_IHDR(png_const_structrp png_ptr, png_inforp info_ptr, png_uint_32 width, png_uint_32 height, int bit_depth, int color_type, int interlace_method, int compression_method, int filter_method)
*/
//<score> 72, nr_unique_branch: 7
//<Quality> {"density":12,"unique_branches":{"png_set_sPLT":[[1066,8,1066,23,0,0,4,1],[1066,27,1066,43,0,0,4,0]],"png_get_tRNS":[[1108,27,1108,43,0,0,4,1]],"png_get_x_offset_pixels":[[290,8,290,23,0,0,4,0],[290,27,290,43,0,0,4,1]],"png_get_image_height":[[74,27,74,43,0,0,4,1]],"png_set_IHDR":[[253,27,253,43,0,0,4,0]]},"library_calls":["png_create_read_struct","png_destroy_read_struct","png_init_io","png_read_info","png_get_image_height","png_get_header_version","png_get_tRNS","png_set_compression_strategy","png_get_x_offset_pixels","png_build_grayscale_palette","png_set_sPLT","png_set_IHDR","png_destroy_read_struct"],"critical_calls":["png_create_read_struct","png_init_io","png_read_info","png_get_image_height","png_get_header_version","png_get_tRNS","png_set_compression_strategy","png_get_x_offset_pixels","png_build_grayscale_palette","png_set_sPLT","png_set_IHDR","png_destroy_read_struct"],"visited":0}
/*Below is the implementation of the C++ program that uses libpng library APIs to achieve the given event:

*/


extern "C" int LLVMFuzzerTestOneInput_65(const uint8_t *data, size_t size) {
	if(size<0) return 0;

	FUZZ_FILE_INIT();
    // Declare necessary variables
    png_structp png_ptr = nullptr;
    png_infop info_ptr = nullptr;
    FILE *in_file = nullptr;
    FILE *out_file = nullptr;
    jmp_buf jmpbuf;
  
    // Initialize libpng error handling
    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (setjmp(jmpbuf)) {
	FUZZ_FILE_CLOSE();
        // If an error occurs, jump back here and clean up
        if (png_ptr != nullptr && info_ptr != nullptr) {
            png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        }
        if (in_file != nullptr) {
            assert_file_closed(&in_file);;
        }
        if (out_file != nullptr) {
            assert_file_closed(&out_file);;
        }
        return 0;
    }
    png_init_io(png_ptr, in_file);

    // Read PNG header
    png_read_info(png_ptr, info_ptr);

    // Use libpng APIs to achieve the desired event
    png_uint_32 image_height = png_get_image_height(png_ptr, info_ptr);
    png_const_charp header_version = png_get_header_version(png_ptr);
    png_bytep trans_alpha;
    int num_trans;
    png_color_16p trans_color;
    png_get_tRNS(png_ptr, info_ptr, &trans_alpha, &num_trans, &trans_color);

    png_set_compression_strategy(png_ptr, 3);

    png_int_32 x_offset_pixels = png_get_x_offset_pixels(png_ptr, info_ptr);

    png_color palette[256];
    png_build_grayscale_palette(8, palette);

    png_sPLT_entry sPLT_entries[3];
    png_sPLT_t sPLT;
    sPLT_entries[0].red = 0;
    sPLT_entries[0].green = 0;
    sPLT_entries[0].blue = 0;
    sPLT_entries[0].alpha = 0;
    sPLT_entries[0].frequency = 1;
    sPLT.nentries = 1;
    sPLT.entries = sPLT_entries;
    png_set_sPLT(png_ptr, info_ptr, &sPLT, 1);

    png_set_IHDR(png_ptr, info_ptr, 100, 100, 8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

    // Clean up and release resources
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    return 0;
}