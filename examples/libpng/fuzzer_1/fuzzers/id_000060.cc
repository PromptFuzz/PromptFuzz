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
//<ID> 2897
//<Prompt> ["png_destroy_info_struct","png_get_interlace_type","png_process_data_pause","png_get_pixel_aspect_ratio_fixed","png_image_begin_read_from_stdio","png_get_user_transform_ptr"]
/*<Combination>: [void png_destroy_info_struct(png_const_structrp png_ptr, png_infopp info_ptr_ptr),
    png_byte png_get_interlace_type(png_const_structrp png_ptr, png_const_inforp info_ptr),
    size_t png_process_data_pause(png_structrp , int save),
    png_fixed_point png_get_pixel_aspect_ratio_fixed(png_const_structrp png_ptr, png_const_inforp info_ptr),
    int png_image_begin_read_from_stdio(png_imagep image, FILE * file),
    png_voidp png_get_user_transform_ptr(png_const_structrp png_ptr)
*/
//<score> 18, nr_unique_branch: 2
//<Quality> {"density":9,"unique_branches":{"png_process_data_pause":[[58,11,58,20,0,0,4,1],[69,14,69,51,0,0,4,1]]},"library_calls":["png_create_read_struct","png_create_info_struct","png_destroy_info_struct","png_get_interlace_type","png_process_data_pause","png_process_data_pause","png_get_pixel_aspect_ratio_fixed","png_image_begin_read_from_stdio","png_get_user_transform_ptr","png_destroy_read_struct"],"critical_calls":["png_create_read_struct","png_create_info_struct","png_destroy_info_struct","png_get_interlace_type","png_process_data_pause","png_process_data_pause","png_get_pixel_aspect_ratio_fixed","png_image_begin_read_from_stdio","png_get_user_transform_ptr","png_destroy_read_struct"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_60(const uint8_t* f_data, size_t f_size) {
	if(f_size<21) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
	//fuzzer shim end}




    // Step 4: Read the input data from a memory buffer using fmemopen
    FILE *in_file = fmemopen((void *)data, size, "rb");
    
    // Step 6: Specify the input and output file names
    const char *input_file = "input.png";
    const char *output_file = "output.png";
    
    // Step 2: Initialize libpng structures
    png_structp png_ptr = png_create_read_struct(fuzz_str_1, NULL, NULL, NULL);
    png_infop info_ptr = png_create_info_struct(png_ptr);
    
    // Step 5: Get the file descriptor for reading and writing
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(fopen(output_file, "wb"));
    
    // Step 1: Destroy libpng structs and get the interlace type
    png_destroy_info_struct(png_ptr, &info_ptr);
    png_byte interlace_type = png_get_interlace_type(png_ptr, info_ptr);
    
    // Step 3: Process the data by pausing and saving it
    png_process_data_pause(png_ptr, fuzz_int32_t_2);
    png_process_data_pause(png_ptr, fuzz_int32_t_3);
    
    // Step 2: Get the pixel aspect ratio from libpng
    png_fixed_point pixel_aspect_ratio = png_get_pixel_aspect_ratio_fixed(png_ptr, info_ptr);
    
    // Step 2: Begin reading the image from a file using stdio
    png_image image;
    png_image_begin_read_from_stdio(&image, in_file);
    
    // Step 2: Get the user transform pointer from libpng
    png_voidp user_transform_ptr = png_get_user_transform_ptr(png_ptr);
    
    // Step 7: Release resources
    assert_file_closed(&in_file);;
    fclose(fopen(output_file, "wb"));
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    
    assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}