#include "FDSan.h"
#include <png.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 1768
//<Prompt> ["png_get_hIST","png_get_pixels_per_inch","png_set_scale_16","png_set_compression_buffer_size","png_get_cHRM","png_init_io","png_get_error_ptr","png_set_background"]
/*<Combination>: [png_uint_32 png_get_hIST(png_const_structrp png_ptr, png_inforp info_ptr, png_uint_16p * hist),
    png_uint_32 png_get_pixels_per_inch(png_const_structrp png_ptr, png_const_inforp info_ptr),
    void png_set_scale_16(png_structrp png_ptr),
    void png_set_compression_buffer_size(png_structrp png_ptr, size_t size),
    png_uint_32 png_get_cHRM(png_const_structrp png_ptr, png_const_inforp info_ptr, double * white_x, double * white_y, double * red_x, double * red_y, double * green_x, double * green_y, double * blue_x, double * blue_y),
    void png_init_io(png_structrp png_ptr, png_FILE_p fp),
    png_voidp png_get_error_ptr(png_const_structrp png_ptr),
    void png_set_background(png_structrp png_ptr, png_const_color_16p background_color, int background_gamma_code, int need_expand, double background_gamma)
*/
//<score> 30, nr_unique_branch: 2
//<Quality> {"density":15,"unique_branches":{"png_set_background_fixed":[[143,41,143,65,0,0,4,1],[146,8,146,61,0,0,4,0]]},"library_calls":["png_create_read_struct","png_create_info_struct","png_destroy_read_struct","png_set_longjmp_fn","png_destroy_read_struct","png_init_io","png_get_hIST","png_get_pixels_per_inch","png_get_pixels_per_inch","png_set_scale_16","png_set_compression_buffer_size","png_get_cHRM","png_get_error_ptr","png_set_background","png_destroy_read_struct","png_destroy_read_struct"],"critical_calls":["png_create_read_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_get_hIST","png_get_pixels_per_inch","png_get_pixels_per_inch","png_set_scale_16","png_set_compression_buffer_size","png_get_cHRM","png_get_error_ptr","png_set_background","png_destroy_read_struct"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_30(const uint8_t *data, size_t size) {
	if(size<0) return 0;

	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);

	FUZZ_FILE_INIT();
    // Step 4: Create a FILE * to read the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
	FUZZ_FILE_PUSH(in_file)
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
	FUZZ_FILENAME_PUSH("input_file")

    // Step 6: Specify the file name for the input file
    const char *input_file = "input_file";

    // Step 2: Initialize the libpng structures
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
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

    // Step 5: Get the file descriptor for reading
    int in_fd = fuzz_fileno(in_file);
	FUZZ_FD_PUSH(in_fd)

    // Step 7: Set up error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
	FUZZ_FILE_CLOSE();
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        assert_file_closed(&in_file);;
        assert_file_name_closed("input_file");
	assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
    }

    // Step 3: Set the input function to read from the file descriptor
    png_init_io(png_ptr, in_file);

    // Step 1: Call the libpng APIs to achieve the desired event
    // png_get_hIST
    png_uint_16p hist;
    png_uint_32 num_hist = png_get_hIST(png_ptr, info_ptr, &hist);

    // png_get_pixels_per_inch
    png_uint_32 xppi, yppi;
    xppi = png_get_pixels_per_inch(png_ptr, info_ptr);
    yppi = png_get_pixels_per_inch(png_ptr, info_ptr);

    // png_set_scale_16
    png_set_scale_16(png_ptr);

    // png_set_compression_buffer_size
    png_set_compression_buffer_size(png_ptr, size);

    // png_get_cHRM
    double white_x, white_y, red_x, red_y, green_x, green_y, blue_x, blue_y;
    png_get_cHRM(png_ptr, info_ptr, &white_x, &white_y, &red_x, &red_y, &green_x, &green_y, &blue_x, &blue_y);

    // png_get_error_ptr
    png_voidp error_ptr = png_get_error_ptr(png_ptr);

    // png_set_background
    png_color_16 background_color;
    int background_gamma_code, need_expand;
    double background_gamma;
    png_set_background(png_ptr, &background_color, background_gamma_code, need_expand, background_gamma);
	FUZZ_FILENAME_PUSH("output_file")

    // Step 6: Specify the file name for the output file
    const char *output_file = "output_file";

    // Step 4: Create a FILE * to write the output data
    FILE *out_file = fopen(output_file, "wb");
	FUZZ_FILE_PUSH(out_file)
    if (!out_file) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        assert_file_closed(&in_file);;
        assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
    }

    // Step 5: Get the file descriptor for writing
    int out_fd = fuzz_fileno(out_file);
	FUZZ_FD_PUSH(out_fd)
  
    // Release resources
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;

    assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}