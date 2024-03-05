#include "FDSan.h"
#include <png.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 3890
//<Prompt> ["png_set_filler","png_get_user_chunk_ptr","png_get_cHRM_XYZ_fixed","png_set_rows","png_get_pixel_aspect_ratio","png_get_bKGD","png_save_uint_32","png_image_finish_read"]
/*<Combination>: [void png_set_filler(png_structrp png_ptr, png_uint_32 filler, int flags),
    png_voidp png_get_user_chunk_ptr(png_const_structrp png_ptr),
    png_uint_32 png_get_cHRM_XYZ_fixed(png_const_structrp png_ptr, png_const_inforp info_ptr, png_fixed_point * int_red_X, png_fixed_point * int_red_Y, png_fixed_point * int_red_Z, png_fixed_point * int_green_X, png_fixed_point * int_green_Y, png_fixed_point * int_green_Z, png_fixed_point * int_blue_X, png_fixed_point * int_blue_Y, png_fixed_point * int_blue_Z),
    void png_set_rows(png_const_structrp png_ptr, png_inforp info_ptr, png_bytepp row_pointers),
    float png_get_pixel_aspect_ratio(png_const_structrp png_ptr, png_const_inforp info_ptr),
    png_uint_32 png_get_bKGD(png_const_structrp png_ptr, png_inforp info_ptr, png_color_16p * background),
    void png_save_uint_32(png_bytep buf, png_uint_32 i),
    int png_image_finish_read(png_imagep image, png_const_colorp background, void * buffer, png_int_32 row_stride, void * colormap)
*/
//<score> 5, nr_unique_branch: 4
//<Quality> {"density":1,"unique_branches":{"png_get_user_chunk_ptr":[[1175,12,1175,19,0,0,4,1]],"png_set_filler":[[124,8,124,23,0,0,4,0]],"png_get_cHRM_XYZ_fixed":[[622,8,622,23,0,0,4,1]],"png_set_rows":[[1542,8,1542,23,0,0,4,0]]},"library_calls":["png_set_filler","png_get_user_chunk_ptr","png_get_cHRM_XYZ_fixed","png_set_rows","png_get_pixel_aspect_ratio","png_get_bKGD","png_save_uint_32","png_image_finish_read"],"critical_calls":["png_set_filler","png_get_user_chunk_ptr","png_get_cHRM_XYZ_fixed","png_set_rows","png_get_pixel_aspect_ratio","png_get_bKGD","png_save_uint_32","png_image_finish_read"],"visited":0}
/**/

extern "C" int LLVMFuzzerTestOneInput_82(const uint8_t *data, size_t size) {
	if(size<0) return 0;

	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);

    // Step 4: Create a FILE pointer for reading the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    
    // Check if the file pointer is valid
    if (in_file == nullptr) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Step 5: Get the file descriptor for reading
    int in_fd = fuzz_fileno(in_file);
    
    // Step 7: Close the file and release resources
    assert_file_closed(&in_file);;
    
    // Step 6: Use the filename directly
    const char *input_file = "input_file";
    
    // Step 4: Create a FILE pointer for writing the output data
    FILE *out_file = fopen("output_file", "wb");
    
    // Check if the file pointer is valid
    if (out_file == nullptr) {
        assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Step 5: Get the file descriptor for writing
    int out_fd = fuzz_fileno(out_file);
    
    // Step 7: Close the file and release resources
    assert_file_closed(&out_file);;

    // Step 1: Call the libpng APIs to achieve the desired event
    png_set_filler(nullptr, 0, 0);
    png_get_user_chunk_ptr(nullptr);
    png_fixed_point int_red_X, int_red_Y, int_red_Z, int_green_X, int_green_Y, int_green_Z, int_blue_X, int_blue_Y, int_blue_Z;
    png_get_cHRM_XYZ_fixed(nullptr, nullptr, &int_red_X, &int_red_Y, &int_red_Z, &int_green_X, &int_green_Y, &int_green_Z, &int_blue_X, &int_blue_Y, &int_blue_Z);
    png_set_rows(nullptr, nullptr, nullptr);
    png_get_pixel_aspect_ratio(nullptr, nullptr);
    png_color_16p background;
    png_get_bKGD(nullptr, nullptr, &background);
    png_byte buf[4];
    png_save_uint_32(buf, 0);
    png_image image;
    png_image_finish_read(&image, nullptr, nullptr, 0, nullptr);
    
    // Step 7: Release any allocated resources

    assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}