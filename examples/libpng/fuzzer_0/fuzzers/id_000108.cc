#include "FDSan.h"
#include <png.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 5068
//<Prompt> ["png_reset_zstream","png_get_eXIf_1","png_set_write_user_transform_fn","png_set_eXIf","png_create_info_struct","png_set_iCCP"]
/*<Combination>: [int png_reset_zstream(png_structrp png_ptr),
    png_uint_32 png_get_eXIf_1(png_const_structrp png_ptr, png_const_inforp info_ptr, png_uint_32 * num_exif, png_bytep * exif),
    void png_set_write_user_transform_fn(png_structrp png_ptr, png_user_transform_ptr write_user_transform_fn),
    void png_set_eXIf(png_const_structrp png_ptr, png_inforp info_ptr, png_bytep exif),
    png_infop png_create_info_struct(png_const_structrp png_ptr),
    void png_set_iCCP(png_const_structrp png_ptr, png_inforp info_ptr, png_const_charp name, int compression_type, png_const_bytep profile, png_uint_32 proflen)
*/
//<score> 32, nr_unique_branch: 1
//<Quality> {"density":16,"unique_branches":{"png_set_write_user_transform_fn":[[1346,8,1346,23,0,0,4,1]]},"library_calls":["png_create_read_struct","png_create_info_struct","png_destroy_read_struct","png_destroy_read_struct","png_destroy_read_struct","png_create_write_struct","png_create_info_struct","png_destroy_write_struct","png_reset_zstream","png_get_eXIf_1","png_set_write_user_transform_fn","png_set_eXIf","png_reset_zstream","png_set_iCCP","png_destroy_read_struct","png_destroy_write_struct"],"critical_calls":["png_create_read_struct","png_create_info_struct","png_create_write_struct","png_create_info_struct","png_reset_zstream","png_get_eXIf_1","png_set_write_user_transform_fn","png_set_eXIf","png_reset_zstream","png_set_iCCP","png_destroy_read_struct","png_destroy_write_struct"],"visited":0}
/*Here is the complete C++ program that implements the steps mentioned above:

*/



extern "C" int LLVMFuzzerTestOneInput_108(const uint8_t *data, size_t size) {
	if(size<0) return 0;

	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);

    // Step 4: Open input file
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Step 5: Get file descriptor for reading
    int in_fd = fuzz_fileno(in_file);
    if (in_fd < 0) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Step 6: Set input file name
    const char* input_file = "input_file";
    
    // Initialize PNG structures
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        assert_file_closed(&in_file);;
        assert_file_name_closed("input_file");
	assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        assert_file_closed(&in_file);;
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        assert_file_name_closed("input_file");
	assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Step 4: Open output file
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Step 5: Get file descriptor for writing
    int out_fd = fuzz_fileno(out_file);
    if (out_fd < 0) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Step 6: Set output file name
    const char* output_file = "output_file";
    
    // Initialize PNG structures
    png_structp png_out_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_out_ptr) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_name_closed("output_file");
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    png_infop info_out_ptr = png_create_info_struct(png_out_ptr);
    if (!info_out_ptr) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        png_destroy_write_struct(&png_out_ptr, NULL);
        assert_file_name_closed("output_file");
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Step 1: Reset the zstream
    png_reset_zstream(png_ptr);
    
    // Step 2: Get eXIf metadata
    png_uint_32 num_exif = 0;
    png_bytep exif = NULL;
    png_get_eXIf_1(png_ptr, info_ptr, &num_exif, &exif);
    
    // Step 3: Set write user transform function
    png_set_write_user_transform_fn(png_out_ptr, NULL);
    
    // Step 3: Set eXIf metadata
    png_set_eXIf(png_out_ptr, info_out_ptr, exif);
    
    // Step 1: Reset the zstream
    png_reset_zstream(png_out_ptr);
    
    // Step 3: Set iCCP metadata
    png_set_iCCP(png_out_ptr, info_out_ptr, "icc_profile", PNG_COMPRESSION_TYPE_BASE, exif, num_exif);
    
    // Step 7: Clean up and release resources
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    png_destroy_write_struct(&png_out_ptr, &info_out_ptr);
    
    assert_file_name_closed("output_file");
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}