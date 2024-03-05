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
//<ID> 4598
//<Prompt> ["png_set_expand_16","png_get_rowbytes","png_destroy_write_struct","png_set_PLTE","png_malloc_warn","png_convert_to_rfc1123_buffer","png_write_row","png_set_rgb_to_gray_fixed","png_get_mem_ptr"]
/*<Combination>: [void png_set_expand_16(png_structrp png_ptr),
    size_t png_get_rowbytes(png_const_structrp png_ptr, png_const_inforp info_ptr),
    void png_destroy_write_struct(png_structpp png_ptr_ptr, png_infopp info_ptr_ptr),
    void png_set_PLTE(png_structrp png_ptr, png_inforp info_ptr, png_const_colorp palette, int num_palette),
    png_voidp png_malloc_warn(png_const_structrp png_ptr, png_alloc_size_t size),
    int png_convert_to_rfc1123_buffer(char out[29], png_const_timep ptime),
    void png_write_row(png_structrp png_ptr, png_const_bytep row),
    void png_set_rgb_to_gray_fixed(png_structrp png_ptr, int error_action, png_fixed_point red, png_fixed_point green),
    png_voidp png_get_mem_ptr(png_const_structrp png_ptr)
*/
//<score> 55, nr_unique_branch: 4
//<Quality> {"density":11,"unique_branches":{"png_malloc_warn":[[218,8,218,23,0,0,4,1]],"png_convert_to_rfc1123_buffer":[[743,8,743,19,0,0,4,0]],"png_write_row":[[717,8,717,23,0,0,4,0]],"png_set_PLTE":[[569,8,569,23,0,0,4,0]]},"library_calls":["png_create_read_struct","png_create_info_struct","png_destroy_read_struct","png_set_expand_16","png_get_rowbytes","png_destroy_write_struct","png_set_PLTE","png_malloc_warn","png_convert_to_rfc1123_buffer","png_write_row","png_set_rgb_to_gray_fixed","png_get_mem_ptr"],"critical_calls":["png_create_read_struct","png_create_info_struct","png_set_expand_16","png_get_rowbytes","png_destroy_write_struct","png_set_PLTE","png_malloc_warn","png_convert_to_rfc1123_buffer","png_write_row","png_set_rgb_to_gray_fixed","png_get_mem_ptr"],"visited":0}
/*Here is the code for the C++ program that follows the instructions given:

*/



extern "C" int LLVMFuzzerTestOneInput_96(const uint8_t* f_data, size_t f_size) {
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





    // Step 4: Create a FILE * variable for reading input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Step 5: Get file descriptor for reading
    int in_fd = fuzz_fileno(in_file);

    // Step 6: Create a string for file name
    const char *in_filename = "input_file";

    // Step 7: Initialize libpng structures
    png_structp png_ptr = png_create_read_struct(fuzz_str_1, NULL, NULL, NULL);
    if (!png_ptr) {
        assert_file_closed(&in_file);;
        assert_file_name_closed("input_file");
	assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        assert_file_closed(&in_file);;
        assert_file_name_closed("input_file");
	assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
    }

    // Step 2: Call libpng APIs
    png_set_expand_16(png_ptr);
    size_t rowbytes = png_get_rowbytes(png_ptr, info_ptr);
    png_destroy_write_struct(&png_ptr, &info_ptr);
    png_color palette[256];
    int num_palette = 256;
    png_set_PLTE(png_ptr, info_ptr, palette, num_palette);
    png_voidp mem_ptr = png_malloc_warn(png_ptr, size);
    png_convert_to_rfc1123_buffer(NULL, NULL);
    png_write_row(png_ptr, NULL);
    png_set_rgb_to_gray_fixed(png_ptr, fuzz_int32_t_2, fuzz_int32_t_3, fuzz_int32_t_4);
    png_get_mem_ptr(png_ptr);

    // Step 8: Release resources
    assert_file_closed(&in_file);;

    assert_file_name_closed("input_file");
	assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
}