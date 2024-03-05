#include "FDSan.h"
#include <png.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 2040
//<Prompt> ["png_set_gamma","png_set_sRGB_gAMA_and_cHRM","png_get_io_chunk_type","png_get_iCCP","png_get_signature","png_get_sCAL","png_set_palette_to_rgb"]
/*<Combination>: [void png_set_gamma(png_structrp png_ptr, double screen_gamma, double override_file_gamma),
    void png_set_sRGB_gAMA_and_cHRM(png_const_structrp png_ptr, png_inforp info_ptr, int srgb_intent),
    png_uint_32 png_get_io_chunk_type(png_const_structrp png_ptr),
    png_uint_32 png_get_iCCP(png_const_structrp png_ptr, png_inforp info_ptr, png_charpp name, int * compression_type, png_bytepp profile, png_uint_32 * proflen),
    png_const_bytep png_get_signature(png_const_structrp png_ptr, png_const_inforp info_ptr),
    png_uint_32 png_get_sCAL(png_const_structrp png_ptr, png_const_inforp info_ptr, int * unit, double * width, double * height),
    void png_set_palette_to_rgb(png_structrp png_ptr)
*/
//<score> 22, nr_unique_branch: 3
//<Quality> {"density":11,"unique_branches":{"png_get_iCCP":[[753,8,753,23,0,0,4,0],[753,27,753,43,0,0,4,0],[754,8,754,46,0,0,4,1]]},"library_calls":["png_create_read_struct","png_create_info_struct","png_destroy_read_struct","png_set_gamma","png_set_sRGB_gAMA_and_cHRM","png_get_io_chunk_type","png_get_iCCP","png_get_signature","png_get_sCAL","png_set_palette_to_rgb","png_destroy_read_struct"],"critical_calls":["png_create_read_struct","png_create_info_struct","png_set_gamma","png_set_sRGB_gAMA_and_cHRM","png_get_io_chunk_type","png_get_iCCP","png_get_signature","png_get_sCAL","png_set_palette_to_rgb","png_destroy_read_struct"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_40(const uint8_t *data, size_t size) {
	if(size<0) return 0;

    // Step 4: Create input file
    FILE *in_file = fmemopen((void *)data, size, "rb");

    // Step 5: Get file descriptor for input file
    int in_fd = fuzz_fileno(in_file);

    // Step 6: Set the output file name
    const char *output_file = "output.png";

    // Variables for libpng
    png_struct *png_ptr = NULL;
    png_info *info_ptr = NULL;

    // Step 2: Initialize libpng
    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
    }

    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        assert_file_closed(&in_file);;
        png_destroy_read_struct(&png_ptr, (png_info **)&info_ptr, NULL);
        assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
    }

    // Fuzzing event

    // Step 1: Set gamma
    double screen_gamma = 0.8;
    double override_file_gamma = 1.2;
    png_set_gamma(png_ptr, screen_gamma, override_file_gamma);

    // Step 1: Set sRGB, gAMA, and cHRM
    int srgb_intent = 1;
    png_set_sRGB_gAMA_and_cHRM(png_ptr, info_ptr, srgb_intent);

    // Step 1: Get IO chunk type
    png_uint_32 io_chunk_type = png_get_io_chunk_type(png_ptr);

    // Step 1: Get iCCP
    png_charp name;
    int compression_type;
    png_bytep profile;
    png_uint_32 proflen;
    png_get_iCCP(png_ptr, info_ptr, &name, &compression_type, &profile, &proflen);

    // Step 1: Get signature
    png_const_bytep signature = png_get_signature(png_ptr, info_ptr);

    // Step 1: Get sCAL
    int unit;
    double width, height;
    png_get_sCAL(png_ptr, info_ptr, &unit, &width, &height);

    // Step 1: Set palette to RGB
    png_set_palette_to_rgb(png_ptr);

    // Step 7: Clean up resources
    assert_file_closed(&in_file);;
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

    assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
}