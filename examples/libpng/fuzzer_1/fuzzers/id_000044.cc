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
//<ID> 2181
//<Prompt> ["png_warning","png_set_sCAL_fixed","png_get_cHRM_XYZ_fixed","png_get_cHRM","png_read_update_info"]
/*<Combination>: [void png_warning(png_const_structrp png_ptr, png_const_charp warning_message),
    void png_set_sCAL_fixed(png_const_structrp png_ptr, png_inforp info_ptr, int unit, png_fixed_point width, png_fixed_point height),
    png_uint_32 png_get_cHRM_XYZ_fixed(png_const_structrp png_ptr, png_const_inforp info_ptr, png_fixed_point * int_red_X, png_fixed_point * int_red_Y, png_fixed_point * int_red_Z, png_fixed_point * int_green_X, png_fixed_point * int_green_Y, png_fixed_point * int_green_Z, png_fixed_point * int_blue_X, png_fixed_point * int_blue_Y, png_fixed_point * int_blue_Z),
    png_uint_32 png_get_cHRM(png_const_structrp png_ptr, png_const_inforp info_ptr, double * white_x, double * white_y, double * red_x, double * red_y, double * green_x, double * green_y, double * blue_x, double * blue_y),
    void png_read_update_info(png_structrp png_ptr, png_inforp info_ptr)
*/
//<score> 33, nr_unique_branch: 4
//<Quality> {"density":11,"unique_branches":{"png_get_cHRM_XYZ_fixed":[[622,8,622,23,0,0,4,0],[622,27,622,43,0,0,4,0],[623,7,623,72,0,0,4,1]],"png_set_sCAL_fixed":[[525,13,525,24,0,0,4,0]]},"library_calls":["png_create_read_struct","png_create_info_struct","png_destroy_read_struct","png_create_write_struct","png_destroy_read_struct","png_create_info_struct","png_destroy_read_struct","png_warning","png_set_sCAL_fixed","png_get_cHRM_XYZ_fixed","png_get_cHRM","png_read_update_info","png_destroy_write_struct","png_destroy_read_struct"],"critical_calls":["png_create_read_struct","png_create_info_struct","png_create_write_struct","png_create_info_struct","png_warning","png_set_sCAL_fixed","png_get_cHRM_XYZ_fixed","png_get_cHRM","png_read_update_info","png_destroy_write_struct","png_destroy_read_struct"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_44(const uint8_t* f_data, size_t f_size) {
	if(f_size<34) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    // Step 4: Create a FILE object to read input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    
    // Step 6: Define input file name
    const char *input_file = "input_file";
    
    // Step 7: Allocate memory for libpng structures
    png_structp png_ptr = png_create_read_struct(fuzz_str_1, NULL, NULL, NULL);
    if (!png_ptr) {
        assert_file_closed(&in_file);;
        assert_file_name_closed("input_file");
	assert_file_closed(&in_file);
	return 0;
    }
    
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        assert_file_closed(&in_file);;
        assert_file_name_closed("input_file");
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Step 4: Create a FILE object to write output data
    FILE *out_file = fopen("output_file", "wb");
    
    // Step 6: Define output file name
    const char *output_file = "output_file";
    
    // Step 7: Allocate memory for libpng structures
    png_structp out_png_ptr = png_create_write_struct(fuzz_str_2, NULL, NULL, NULL);
    if (!out_png_ptr) {
        assert_file_closed(&out_file);;
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        assert_file_closed(&in_file);;
        assert_file_name_closed("output_file");
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    png_infop out_info_ptr = png_create_info_struct(out_png_ptr);
    if (!out_info_ptr) {
        assert_file_closed(&out_file);;
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        assert_file_closed(&in_file);;
        assert_file_name_closed("output_file");
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Step 2: Call libpng APIs to achieve the desired event
    // Step 3: Consume the input data and size
    png_warning(png_ptr, "This is a warning message");
    
    int unit = PNG_RESOLUTION_UNKNOWN;
    png_fixed_point width = 0x12345678;
    png_fixed_point height = 0x9abcdef0;
    png_set_sCAL_fixed(png_ptr, info_ptr, fuzz_int32_t_3, fuzz_int32_t_4, height);
    
    png_fixed_point int_red_X, int_red_Y, int_red_Z;
    png_fixed_point int_green_X, int_green_Y, int_green_Z;
    png_fixed_point int_blue_X, int_blue_Y, int_blue_Z;
    png_get_cHRM_XYZ_fixed(png_ptr, info_ptr, &int_red_X, &int_red_Y, &int_red_Z, 
                           &int_green_X, &int_green_Y, &int_green_Z,
                           &int_blue_X, &int_blue_Y, &int_blue_Z);
    
    double white_x, white_y, red_x, red_y, green_x, green_y, blue_x, blue_y;
    png_get_cHRM(png_ptr, info_ptr, &white_x, &white_y, &red_x, &red_y, &green_x, &green_y, &blue_x, &blue_y);
    
    png_read_update_info(png_ptr, info_ptr);
    
    // Step 5: Get file descriptors for reading and writing
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);
    
    // Release resources
    assert_file_closed(&out_file);;
    png_destroy_write_struct(&out_png_ptr, &out_info_ptr);
    
    assert_file_closed(&in_file);;
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    
    assert_file_name_closed("output_file");
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}