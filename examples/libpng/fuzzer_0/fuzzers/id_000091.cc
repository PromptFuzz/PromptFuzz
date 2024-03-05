#include "FDSan.h"
#include <png.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 4441
//<Prompt> ["png_set_text_compression_method","png_get_cHRM_XYZ_fixed","png_get_uint_16","png_set_shift","png_set_crc_action","png_set_write_status_fn"]
/*<Combination>: [void png_set_text_compression_method(png_structrp png_ptr, int method),
    png_uint_32 png_get_cHRM_XYZ_fixed(png_const_structrp png_ptr, png_const_inforp info_ptr, png_fixed_point * int_red_X, png_fixed_point * int_red_Y, png_fixed_point * int_red_Z, png_fixed_point * int_green_X, png_fixed_point * int_green_Y, png_fixed_point * int_green_Z, png_fixed_point * int_blue_X, png_fixed_point * int_blue_Y, png_fixed_point * int_blue_Z),
    png_uint_16 png_get_uint_16(png_const_bytep buf),
    void png_set_shift(png_structrp png_ptr, png_const_color_8p true_bits),
    void png_set_crc_action(png_structrp png_ptr, int crit_action, int ancil_action),
    void png_set_write_status_fn(png_structrp png_ptr, png_write_status_ptr write_row_fn)
*/
//<score> 20, nr_unique_branch: 1
//<Quality> {"density":10,"unique_branches":{"png_set_crc_action":[[94,7,94,32,0,0,4,0]]},"library_calls":["png_create_read_struct","png_create_info_struct","png_destroy_read_struct","png_set_text_compression_method","png_get_cHRM_XYZ_fixed","png_set_shift","png_set_crc_action","png_set_write_status_fn","png_destroy_read_struct","png_destroy_read_struct"],"critical_calls":["png_create_read_struct","png_create_info_struct","png_set_text_compression_method","png_get_cHRM_XYZ_fixed","png_set_shift","png_set_crc_action","png_set_write_status_fn","png_destroy_read_struct"],"visited":0}
/**/

extern "C" int LLVMFuzzerTestOneInput_91(const uint8_t *data, size_t size) {
	if(size<0) return 0;

	FILE *input_file_ptr = fopen("input_file.png", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);

    // Step 4: Create a file stream for reading the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file == nullptr) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Step 6: Specify the input file name
    const char *input_file = "input_file.png";

    // Step 2: Initialize the libpng structures
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (png_ptr == nullptr) {
        assert_file_closed(&in_file);;
        assert_file_name_closed("input_file.png");
	assert_file_closed(&in_file);
	return 0;
    }
    
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == nullptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        assert_file_closed(&in_file);;
        assert_file_name_closed("input_file.png");
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Step 5: Get the file descriptor for reading
    int in_fd = fuzz_fileno(in_file);
    
    // Step 3: Use libpng APIs to achieve the desired event
    png_set_text_compression_method(png_ptr, 1);
    
    png_fixed_point red_X, red_Y, red_Z, green_X, green_Y, green_Z, blue_X, blue_Y, blue_Z;
    png_get_cHRM_XYZ_fixed(png_ptr, info_ptr, &red_X, &red_Y, &red_Z, &green_X, &green_Y, &green_Z, &blue_X, &blue_Y, &blue_Z);
    
    png_byte buf[2];
    png_get_uint_16(buf);
    
    png_color_8 true_bits;
    png_set_shift(png_ptr, &true_bits);
    
    png_set_crc_action(png_ptr, PNG_CRC_QUIET_USE, PNG_CRC_WARN_DISCARD);
    
    // Step 1: Set the write status callback function
    png_set_write_status_fn(png_ptr, [](png_structp png_ptr, png_uint_32 row_num, int pass) {
        std::cout << "Writing row number " << row_num << " in pass " << pass << std::endl;
    });
    
    // Step 4: Create a file stream for writing the output data
    FILE *out_file = fopen("output_file.png", "wb");
    if (out_file == nullptr) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        assert_file_closed(&in_file);;
        assert_file_name_closed("output_file.png");
	assert_file_name_closed("input_file.png");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Step 6: Specify the output file name
    const char *output_file = "output_file.png";
    
    // Step 5: Get the file descriptor for writing
    int out_fd = fuzz_fileno(out_file);
    
    // Step 3: Use libpng APIs to achieve the desired event
    
    // Step 7: Release allocated resources
    assert_file_closed(&out_file);;
    assert_file_closed(&in_file);;
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    
    assert_file_name_closed("output_file.png");
	assert_file_name_closed("output_file.png");
	assert_file_name_closed("input_file.png");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}