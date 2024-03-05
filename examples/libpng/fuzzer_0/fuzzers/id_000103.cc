#include "FDSan.h"
#include <png.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 4803
//<Prompt> ["png_get_compression_type","png_get_chunk_cache_max","png_set_rgb_to_gray_fixed","png_set_alpha_mode","png_get_filter_type"]
/*<Combination>: [png_byte png_get_compression_type(png_const_structrp png_ptr, png_const_inforp info_ptr),
    png_uint_32 png_get_chunk_cache_max(png_const_structrp png_ptr),
    void png_set_rgb_to_gray_fixed(png_structrp png_ptr, int error_action, png_fixed_point red, png_fixed_point green),
    void png_set_alpha_mode(png_structrp png_ptr, int mode, double output_gamma),
    png_byte png_get_filter_type(png_const_structrp png_ptr, png_const_inforp info_ptr)
*/
//<score> 12, nr_unique_branch: 0
//<Quality> {"density":12,"unique_branches":{},"library_calls":["png_create_read_struct","png_create_info_struct","png_destroy_read_struct","png_set_longjmp_fn","png_destroy_read_struct","png_init_io","png_read_info","png_get_compression_type","png_get_chunk_cache_max","png_set_rgb_to_gray_fixed","png_set_alpha_mode","png_get_filter_type","png_destroy_read_struct"],"critical_calls":["png_create_read_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_read_info","png_get_compression_type","png_get_chunk_cache_max","png_set_rgb_to_gray_fixed","png_set_alpha_mode","png_get_filter_type","png_destroy_read_struct"],"visited":0}
/*Here is the completed program:

*/


extern "C" int LLVMFuzzerTestOneInput_103(const uint8_t *data, size_t size) {
	if(size<0) return 0;

	FUZZ_FILE_INIT();
    // Open input file
    FILE *in_file = fmemopen((void *)data, size, "rb");
	FUZZ_FILE_PUSH(in_file)
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Read PNG file
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    if (setjmp(png_jmpbuf(png_ptr))) {
	FUZZ_FILE_CLOSE();
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    png_init_io(png_ptr, in_file);
    png_read_info(png_ptr, info_ptr);

    // Get compression type
    png_byte compression_type = png_get_compression_type(png_ptr, info_ptr);
    std::cout << "Compression Type: " << (int)compression_type << std::endl;

    // Get chunk cache max
    png_uint_32 chunk_cache_max = png_get_chunk_cache_max(png_ptr);
    std::cout << "Chunk Cache Max: " << chunk_cache_max << std::endl;

    // Set RGB to Gray
    png_set_rgb_to_gray_fixed(png_ptr, PNG_ERROR_ACTION_NONE, 100, 200);

    // Set alpha mode
    png_set_alpha_mode(png_ptr, PNG_ALPHA_PNG, 2.2);

    // Get filter type
    png_byte filter_type = png_get_filter_type(png_ptr, info_ptr);
    std::cout << "Filter Type: " << (int)filter_type << std::endl;

    // Clean up
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    assert_file_closed(&in_file);;

    assert_file_closed(&in_file);
	return 0;
}