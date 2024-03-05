#include "FDSan.h"
#include <png.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 1394
//<Prompt> ["png_set_iCCP","png_set_compression_mem_level","png_set_keep_unknown_chunks","png_get_y_offset_pixels","png_set_filter","png_get_PLTE"]
/*<Combination>: [void png_set_iCCP(png_const_structrp png_ptr, png_inforp info_ptr, png_const_charp name, int compression_type, png_const_bytep profile, png_uint_32 proflen),
    void png_set_compression_mem_level(png_structrp png_ptr, int mem_level),
    void png_set_keep_unknown_chunks(png_structrp png_ptr, int keep, png_const_bytep chunk_list, int num_chunks),
    png_int_32 png_get_y_offset_pixels(png_const_structrp png_ptr, png_const_inforp info_ptr),
    void png_set_filter(png_structrp png_ptr, int method, int filters),
    png_uint_32 png_get_PLTE(png_const_structrp png_ptr, png_inforp info_ptr, png_colorp * palette, int * num_palette)
*/
//<score> 33, nr_unique_branch: 14
//<Quality> {"density":11,"unique_branches":{"png_set_filter":[[1018,8,1018,23,0,0,4,1],[1022,8,1022,71,0,0,4,1],[1027,8,1027,38,0,0,4,0],[1032,10,1032,16,0,0,4,1],[1033,10,1033,16,0,0,4,1],[1034,10,1034,16,0,0,4,1],[1037,10,1037,36,0,0,4,1],[1041,10,1041,35,0,0,4,1],[1044,10,1044,34,0,0,4,1],[1047,10,1047,35,0,0,4,1],[1050,10,1050,37,0,0,4,1],[1053,10,1053,17,0,0,4,0],[1075,11,1075,35,0,0,4,1]],"png_set_iCCP":[[686,47,686,59,0,0,4,0]]},"library_calls":["png_create_read_struct","png_create_info_struct","png_destroy_read_struct","png_set_iCCP","png_set_compression_mem_level","png_set_keep_unknown_chunks","png_get_y_offset_pixels","png_set_filter","png_get_PLTE","png_destroy_read_struct","png_destroy_read_struct"],"critical_calls":["png_create_read_struct","png_create_info_struct","png_set_iCCP","png_set_compression_mem_level","png_set_keep_unknown_chunks","png_get_y_offset_pixels","png_set_filter","png_get_PLTE","png_destroy_read_struct"],"visited":0}
/**/

extern "C" int LLVMFuzzerTestOneInput_20(const uint8_t *data, size_t size) {
	if(size<0) return 0;

	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);

    // Step 4: Create a FILE * to read input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file == nullptr) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Step 6: Define the input file name
    const char *input_file = "input_file";

    // Step 7: Save the input file
    FILE *input = fopen(input_file, "wb");
    if (input == nullptr) {
        assert_file_closed(&in_file);;
        assert_file_name_closed("input_file");
	assert_file_closed(&input);
	assert_file_closed(&in_file);
	return 0;
    }
    fwrite(data, sizeof(uint8_t), size, input);
    assert_file_closed(&input);;

    // Step 2: Create a png_struct and png_info
    png_struct *png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (png_ptr == nullptr) {
        assert_file_closed(&in_file);;
        assert_file_name_closed("input_file");
	assert_file_closed(&input);
	assert_file_closed(&in_file);
	return 0;
    }
    png_info *info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == nullptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        assert_file_closed(&in_file);;
        assert_file_name_closed("input_file");
	assert_file_closed(&input);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Step 5: Get the file descriptor for reading
    int fd = fuzz_fileno(in_file);

    // Step 1: Set iCCP, compression level, keep unknown chunks, y offset pixels, filter, and get PLTE
    png_set_iCCP(png_ptr, info_ptr, nullptr, 0, nullptr, 0);
    png_set_compression_mem_level(png_ptr, 1);
    png_bytep chunk_list = nullptr;
    int num_chunks = 0;
    png_set_keep_unknown_chunks(png_ptr, 0, chunk_list, num_chunks);
    png_int_32 y_offset = png_get_y_offset_pixels(png_ptr, info_ptr);
    png_set_filter(png_ptr, PNG_FILTER_TYPE_BASE, PNG_FILTER_NONE);
    png_colorp palette = nullptr;
    int num_palette = 0;
    png_get_PLTE(png_ptr, info_ptr, &palette, &num_palette);

    // Step 6: Define the output file name
    const char *output_file = "output_file";

    // Step 7: Save the output data
    FILE *output = fopen(output_file, "wb");
    if (output == nullptr) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        assert_file_closed(&in_file);;
        assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_closed(&output);
	assert_file_closed(&input);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    fwrite(data, sizeof(uint8_t), size, output);
    assert_file_closed(&output);;

    // Step 7: Release resources
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    assert_file_closed(&in_file);;

    assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_closed(&output);
	assert_file_closed(&input);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
}