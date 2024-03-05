#include "FDSan.h"
#include <png.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 4953
//<Prompt> ["png_set_hIST","png_get_tIME","png_convert_from_struct_tm","png_set_cHRM_XYZ","png_set_rows"]
/*<Combination>: [void png_set_hIST(png_const_structrp png_ptr, png_inforp info_ptr, png_const_uint_16p hist),
    png_uint_32 png_get_tIME(png_const_structrp png_ptr, png_inforp info_ptr, png_timep * mod_time),
    void png_convert_from_struct_tm(png_timep ptime, const struct tm * ttime),
    void png_set_cHRM_XYZ(png_const_structrp png_ptr, png_inforp info_ptr, double red_X, double red_Y, double red_Z, double green_X, double green_Y, double green_Z, double blue_X, double blue_Y, double blue_Z),
    void png_set_rows(png_const_structrp png_ptr, png_inforp info_ptr, png_bytepp row_pointers)
*/
//<score> 24, nr_unique_branch: 2
//<Quality> {"density":8,"unique_branches":{"png_XYZ_normalize":[[1564,8,1564,21,0,0,4,1]],"png_set_hIST":[[214,8,214,34,0,0,4,0]]},"library_calls":["png_create_read_struct","png_create_info_struct","png_destroy_read_struct","png_set_progressive_read_fn","png_set_hIST","png_set_cHRM_XYZ","png_set_rows","png_destroy_read_struct"],"critical_calls":["png_create_read_struct","png_create_info_struct","png_set_progressive_read_fn","png_set_hIST","png_set_cHRM_XYZ","png_set_rows","png_destroy_read_struct"],"visited":0}
/**/

extern "C" int LLVMFuzzerTestOneInput_106(const uint8_t *data, size_t size) {
	if(size<0) return 0;

    // Step 4: Create an input file
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file == nullptr) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Step 5: Get the file descriptor for reading
    int in_fd = fuzz_fileno(in_file);

    // Step 6: Prepare the output file
    FILE *out_file = fopen("output_file.png", "wb");
    if (out_file == nullptr) {
        assert_file_closed(&in_file);;
        assert_file_name_closed("output_file.png");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
    }

    // Step 5: Get the file descriptor for writing
    int out_fd = fuzz_fileno(out_file);

    // Step 1: Initialize libpng structures
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (png_ptr == nullptr) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_name_closed("output_file.png");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == nullptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_name_closed("output_file.png");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }

    // Step 2: Set progressive read function
    png_set_progressive_read_fn(png_ptr, nullptr, nullptr, nullptr, nullptr);

    // Step 3: Set other information
    png_set_hIST(png_ptr, info_ptr, nullptr);

    // Step 4: Set cHRM_XYZ
    png_set_cHRM_XYZ(png_ptr, info_ptr, 0.4124, 0.2126, 0.0193, 0.3576, 0.7152, 0.1192, 0.1805, 0.0722, 0.9504);

    // Step 5: Set row pointers
    png_bytepp row_pointers = nullptr;
    png_set_rows(png_ptr, info_ptr, row_pointers);

    // Step 7: Release resources
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;

    assert_file_name_closed("output_file.png");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}