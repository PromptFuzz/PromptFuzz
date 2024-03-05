#include "FDSan.h"
#include <png.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 328
//<Prompt> ["png_set_tIME","png_destroy_info_struct","png_set_compression_method","png_write_sig","png_set_cHRM"]
/*<Combination>: [void png_set_tIME(png_const_structrp png_ptr, png_inforp info_ptr, png_const_timep mod_time),
    void png_destroy_info_struct(png_const_structrp png_ptr, png_infopp info_ptr_ptr),
    void png_set_compression_method(png_structrp png_ptr, int method),
    void png_write_sig(png_structrp png_ptr),
    void png_set_cHRM(png_const_structrp png_ptr, png_inforp info_ptr, double white_x, double white_y, double red_x, double red_y, double green_x, double green_y, double blue_x, double blue_y)
*/
//<score> 60, nr_unique_branch: 4
//<Quality> {"density":15,"unique_branches":{"png_set_cHRM_fixed":[[49,27,49,43,0,0,4,0]],"png_set_compression_method":[[1242,8,1242,23,0,0,4,1],[1248,8,1248,19,0,0,4,0]],"png_write_sig":[[64,8,64,30,0,0,4,1]]},"library_calls":["png_create_read_struct","png_create_info_struct","png_destroy_read_struct","png_set_longjmp_fn","png_destroy_read_struct","png_destroy_read_struct","png_destroy_read_struct","png_init_io","png_set_write_fn","png_set_sig_bytes","png_write_sig","png_set_tIME","png_destroy_info_struct","png_set_compression_method","png_set_cHRM","png_destroy_read_struct"],"critical_calls":["png_create_read_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_set_write_fn","png_set_sig_bytes","png_write_sig","png_set_tIME","png_destroy_info_struct","png_set_compression_method","png_set_cHRM","png_destroy_read_struct"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_6(const uint8_t *data, size_t size) {
	if(size<0) return 0;

	FUZZ_FILE_INIT();
    // Initialize libpng structures
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        return 0;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return 0;
    }

    // Set up error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
	FUZZ_FILE_CLOSE();
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return 0;
    }

    // Set file to read from
    FILE *in_file = fmemopen((void *)data, size, "rb");
	FUZZ_FILE_PUSH(in_file)
    if (!in_file) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        assert_file_closed(&in_file);
	return 0;
    }
    int in_fd = fuzz_fileno(in_file);
	FUZZ_FILENAME_PUSH("output_file")
	FUZZ_FD_PUSH(in_fd)

    // Set file to write to
    FILE *out_file = fopen("output_file", "wb");
	FUZZ_FILE_PUSH(out_file)
    if (!out_file) {
        assert_file_closed(&in_file);;
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
    }
    int out_fd = fuzz_fileno(out_file);
	FUZZ_FD_PUSH(out_fd)

    // Set the input and output streams
    png_init_io(png_ptr, in_file);
    png_set_write_fn(png_ptr, (png_voidp)out_fd, NULL, NULL);

    // Read the PNG signature
    png_set_sig_bytes(png_ptr, 8);
    png_write_sig(png_ptr);

    // Set other parameters and write the image
    png_set_tIME(png_ptr, info_ptr, NULL);
    png_destroy_info_struct(png_ptr, &info_ptr);
    png_set_compression_method(png_ptr, 0);
    png_set_cHRM(png_ptr, info_ptr, 1.0, 0.5, 0.3, 0.7, 0.2, 0.8, 0.1, 0.9);

    // Clean up and release resources
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    png_destroy_read_struct(&png_ptr, NULL, NULL);

    assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}