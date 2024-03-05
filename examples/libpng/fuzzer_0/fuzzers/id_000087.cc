#include "FDSan.h"
#include <png.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 4115
//<Prompt> ["png_set_compression_buffer_size","png_get_cHRM_XYZ","png_create_read_struct_2","png_create_write_struct","png_set_invalid","png_get_x_pixels_per_meter","png_set_gamma"]
/*<Combination>: [void png_set_compression_buffer_size(png_structrp png_ptr, size_t size),
    png_uint_32 png_get_cHRM_XYZ(png_const_structrp png_ptr, png_const_inforp info_ptr, double * red_X, double * red_Y, double * red_Z, double * green_X, double * green_Y, double * green_Z, double * blue_X, double * blue_Y, double * blue_Z),
    png_structp png_create_read_struct_2(png_const_charp user_png_ver, png_voidp error_ptr, png_error_ptr error_fn, png_error_ptr warn_fn, png_voidp mem_ptr, png_malloc_ptr malloc_fn, png_free_ptr free_fn),
    png_structp png_create_write_struct(png_const_charp user_png_ver, png_voidp error_ptr, png_error_ptr error_fn, png_error_ptr warn_fn),
    void png_set_invalid(png_const_structrp png_ptr, png_inforp info_ptr, int mask),
    png_uint_32 png_get_x_pixels_per_meter(png_const_structrp png_ptr, png_const_inforp info_ptr),
    void png_set_gamma(png_structrp png_ptr, double screen_gamma, double override_file_gamma)
*/
//<score> 40, nr_unique_branch: 10
//<Quality> {"density":8,"unique_branches":{"png_set_invalid":[[1619,27,1619,43,0,0,4,1]],"png_get_x_pixels_per_meter":[[130,27,130,43,0,0,4,1]],"png_set_compression_buffer_size":[[1566,8,1566,49,0,0,4,1],[1574,8,1574,49,0,0,4,0],[1576,11,1576,31,0,0,4,1],[1588,11,1588,29,0,0,4,1],[1596,11,1596,19,0,0,4,0],[1596,11,1596,19,0,0,4,1],[1607,11,1607,40,0,0,4,0]],"png_get_cHRM_XYZ":[[574,27,574,43,0,0,4,1]]},"library_calls":["png_create_read_struct_2","png_create_write_struct","png_destroy_read_struct","png_init_io","png_init_io","png_set_compression_buffer_size","png_read_info","png_get_cHRM_XYZ","png_set_invalid","png_get_x_pixels_per_meter","png_set_gamma","png_destroy_read_struct","png_destroy_write_struct"],"critical_calls":["png_create_read_struct_2","png_create_write_struct","png_init_io","png_init_io","png_set_compression_buffer_size","png_read_info","png_get_cHRM_XYZ","png_set_invalid","png_get_x_pixels_per_meter","png_set_gamma","png_destroy_read_struct","png_destroy_write_struct"],"visited":0}
/**/

extern "C" int LLVMFuzzerTestOneInput_87(const uint8_t *data, size_t size) {
	if(size<0) return 0;

    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file == NULL) {
        assert_file_closed(&in_file);
	return 0;
    }

    FILE *out_file = fopen("output_file", "wb");
    if (out_file == NULL) {
        assert_file_closed(&in_file);;
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    int in_file_fd = fuzz_fileno(in_file);
    int out_file_fd = fuzz_fileno(out_file);

    // Initialize libpng structures
    png_structp png_read_ptr = png_create_read_struct_2(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL, NULL, NULL, NULL);
    if (png_read_ptr == NULL) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_file_fd);
	assert_fd_closed(in_file_fd);
	return 0;
    }

    png_structp png_write_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (png_write_ptr == NULL) {
        png_destroy_read_struct(&png_read_ptr, NULL, NULL);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_file_fd);
	assert_fd_closed(in_file_fd);
	return 0;
    }

    // Set the input and output files
    png_init_io(png_read_ptr, in_file);
    png_init_io(png_write_ptr, out_file);

    // Set compression buffer size
    png_set_compression_buffer_size(png_write_ptr, size);

    // Read image information
    png_read_info(png_read_ptr, NULL);

    // Get XYZ values of cHRM chunk
    double red_X, red_Y, red_Z, green_X, green_Y, green_Z, blue_X, blue_Y, blue_Z;
    png_get_cHRM_XYZ(png_read_ptr, NULL, &red_X, &red_Y, &red_Z, &green_X, &green_Y, &green_Z, &blue_X, &blue_Y, &blue_Z);

    // Set invalid flag
    png_set_invalid(png_read_ptr, NULL, 1);

    // Get x pixels per meter
    png_uint_32 x_pixels_per_meter = png_get_x_pixels_per_meter(png_read_ptr, NULL);

    // Set gamma
    png_set_gamma(png_write_ptr, 1.0, 1.0);

    // Clean up
    png_destroy_read_struct(&png_read_ptr, NULL, NULL);
    png_destroy_write_struct(&png_write_ptr, NULL);

    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;

    assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_file_fd);
	assert_fd_closed(in_file_fd);
	return 0;
}