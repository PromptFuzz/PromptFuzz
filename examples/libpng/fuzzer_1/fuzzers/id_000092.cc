#include "FDSan.h"
#include "FuzzedDataProvider.h"
#include <png.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 4444
//<Prompt> ["png_set_text_compression_method","png_get_cHRM_XYZ_fixed","png_get_uint_16","png_set_shift","png_set_crc_action","png_set_write_status_fn"]
/*<Combination>: [void png_set_text_compression_method(png_structrp png_ptr, int method),
    png_uint_32 png_get_cHRM_XYZ_fixed(png_const_structrp png_ptr, png_const_inforp info_ptr, png_fixed_point * int_red_X, png_fixed_point * int_red_Y, png_fixed_point * int_red_Z, png_fixed_point * int_green_X, png_fixed_point * int_green_Y, png_fixed_point * int_green_Z, png_fixed_point * int_blue_X, png_fixed_point * int_blue_Y, png_fixed_point * int_blue_Z),
    png_uint_16 png_get_uint_16(png_const_bytep buf),
    void png_set_shift(png_structrp png_ptr, png_const_color_8p true_bits),
    void png_set_crc_action(png_structrp png_ptr, int crit_action, int ancil_action),
    void png_set_write_status_fn(png_structrp png_ptr, png_write_status_ptr write_row_fn)
*/
//<score> 18, nr_unique_branch: 2
//<Quality> {"density":9,"unique_branches":{"png_set_crc_action":[[49,7,49,28,0,0,4,0],[54,7,54,29,0,0,4,1]]},"library_calls":["png_create_read_struct","png_set_text_compression_method","png_create_info_struct","png_destroy_read_struct","png_get_cHRM_XYZ_fixed","png_set_shift","png_set_crc_action","png_set_write_status_fn","png_destroy_read_struct"],"critical_calls":["png_create_read_struct","png_set_text_compression_method","png_create_info_struct","png_get_cHRM_XYZ_fixed","png_set_shift","png_set_crc_action","png_set_write_status_fn","png_destroy_read_struct"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_92(const uint8_t* f_data, size_t f_size) {
	if(f_size<25) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    // Step 4: Read input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Step 6: Set input file name
    const char *in_file_name = "input_file";

    // Step 5: Get file descriptor
    int in_fd = fuzz_fileno(in_file);

    // Step 4: Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
    }

    // Step 6: Set output file name
    const char *out_file_name = "output_file";

    // Step 5: Get file descriptor
    int out_fd = fuzz_fileno(out_file);

    // Step 1: Set text compression method
    png_structp png_ptr = png_create_read_struct(fuzz_str_1, NULL, NULL, NULL);
    if (!png_ptr) {
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
    png_set_text_compression_method(png_ptr, fuzz_int32_t_2);

    // Step 1: Get cHRM XYZ fixed
    png_info *info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
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
    png_fixed_point int_red_X, int_red_Y, int_red_Z, int_green_X, int_green_Y, int_green_Z;
    png_fixed_point int_blue_X, int_blue_Y, int_blue_Z;
    png_get_cHRM_XYZ_fixed(png_ptr, info_ptr, &int_red_X, &int_red_Y, &int_red_Z,
                           &int_green_X, &int_green_Y, &int_green_Z,
                           &int_blue_X, &int_blue_Y, &int_blue_Z);

    // Step 1: Get uint16 value
    png_byte buf[sizeof(png_uint_16)];
    png_uint_16 value = png_get_uint_16(buf);

    // Step 1: Set shift
    png_color_8 true_bits;
    png_set_shift(png_ptr, &true_bits);

    // Step 1: Set CRC action
    png_set_crc_action(png_ptr, fuzz_int32_t_3, fuzz_int32_t_4);

    // Step 1: Set write status function
    png_write_status_ptr write_row_fn;
    png_set_write_status_fn(png_ptr, write_row_fn);

    // Cleanup
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
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