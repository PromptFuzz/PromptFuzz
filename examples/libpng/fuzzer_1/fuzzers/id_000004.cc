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
//<ID> 292
//<Prompt> ["png_set_strip_16","png_get_x_offset_microns","png_set_bKGD","png_get_rgb_to_gray_status","png_set_mem_fn","png_image_finish_read","png_write_chunk_data","png_set_expand_gray_1_2_4_to_8"]
/*<Combination>: [void png_set_strip_16(png_structrp png_ptr),
    png_int_32 png_get_x_offset_microns(png_const_structrp png_ptr, png_const_inforp info_ptr),
    void png_set_bKGD(png_const_structrp png_ptr, png_inforp info_ptr, png_const_color_16p background),
    png_byte png_get_rgb_to_gray_status(png_const_structrp png_ptr),
    void png_set_mem_fn(png_structrp png_ptr, png_voidp mem_ptr, png_malloc_ptr malloc_fn, png_free_ptr free_fn),
    int png_image_finish_read(png_imagep image, png_const_colorp background, void * buffer, png_int_32 row_stride, void * colormap),
    void png_write_chunk_data(png_structrp png_ptr, png_const_bytep data, size_t length),
    void png_set_expand_gray_1_2_4_to_8(png_structrp png_ptr)
*/
//<score> 39, nr_unique_branch: 4
//<Quality> {"density":13,"unique_branches":{"png_image_finish_read":[[4108,8,4108,21,0,0,4,0],[4108,25,4108,60,0,0,4,1],[4214,13,4214,26,0,0,4,0]],"png_set_strip_16":[[201,8,201,37,0,0,4,1]]},"library_calls":["png_create_read_struct","png_create_info_struct","png_destroy_read_struct","png_init_io","png_set_write_fn","png_set_strip_16","png_get_x_offset_microns","png_set_bKGD","png_get_rgb_to_gray_status","png_set_mem_fn","png_image_finish_read","png_write_chunk_data","png_set_expand_gray_1_2_4_to_8","png_destroy_read_struct"],"critical_calls":["png_create_read_struct","png_create_info_struct","png_init_io","png_set_write_fn","png_set_strip_16","png_get_x_offset_microns","png_set_bKGD","png_get_rgb_to_gray_status","png_set_mem_fn","png_image_finish_read","png_write_chunk_data","png_set_expand_gray_1_2_4_to_8","png_destroy_read_struct"],"visited":0}
/*Here is the updated code:

*/


extern "C" int LLVMFuzzerTestOneInput_4(const uint8_t* f_data, size_t f_size) {
	if(f_size<25) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_3, fdp);
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    png_structp png_ptr;
    png_infop info_ptr;

    // Create a FILE * variable for reading the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");

    // Create a FILE * variable for writing output data
    FILE *out_file = fopen("output_file", "wb");

    // Get the file descriptors
    int in_file_descriptor = fuzz_fileno(in_file);
    int out_file_descriptor = fuzz_fileno(out_file);

    // Create a string for file name
    const char *input_file = "input_file";
    const char *output_file = "output_file";

    // Initialize the libpng structures
    png_ptr = png_create_read_struct(fuzz_str_1, NULL, NULL, NULL);
    if (!png_ptr) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_file_descriptor);
	assert_fd_closed(in_file_descriptor);
	return 0;
    }

    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_file_descriptor);
	assert_fd_closed(in_file_descriptor);
	return 0;
    }

    // Set the input and output files
    png_init_io(png_ptr, in_file);
    png_set_write_fn(png_ptr, out_file, NULL, NULL);

    // Set the strip 16 option
    png_set_strip_16(png_ptr);

    // Get the x offset in microns
    png_int_32 x_offset = png_get_x_offset_microns(png_ptr, info_ptr);

    // Set the bKGD option
    png_color_16 background;
    png_set_bKGD(png_ptr, info_ptr, &background);

    // Get the RGB to gray status
    png_byte rgb_to_gray = png_get_rgb_to_gray_status(png_ptr);

    // Set custom memory functions
    png_set_mem_fn(png_ptr, NULL, NULL, NULL);

    // Finish reading the image
    png_image image;
    int ret = png_image_finish_read(&image, NULL, NULL, fuzz_int32_t_2, NULL);

    // Write chunk data
    png_write_chunk_data(png_ptr, NULL, fuzz_uint64_t_3);

    // Set the expand grayscale option
    png_set_expand_gray_1_2_4_to_8(png_ptr);

    // Clean up and release resources
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;

    assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_file_descriptor);
	assert_fd_closed(in_file_descriptor);
	return 0;
}