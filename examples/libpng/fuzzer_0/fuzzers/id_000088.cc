#include "FDSan.h"
#include <png.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 4142
//<Prompt> ["png_set_rgb_to_gray_fixed","png_info_init_3","png_write_chunk_start","png_get_y_pixels_per_meter","png_get_eXIf","png_set_text_compression_method","png_write_chunk_data","png_get_y_pixels_per_inch","png_get_eXIf_1"]
/*<Combination>: [void png_set_rgb_to_gray_fixed(png_structrp png_ptr, int error_action, png_fixed_point red, png_fixed_point green),
    void png_info_init_3(png_infopp info_ptr, size_t png_info_struct_size),
    void png_write_chunk_start(png_structrp png_ptr, png_const_bytep chunk_name, png_uint_32 length),
    png_uint_32 png_get_y_pixels_per_meter(png_const_structrp png_ptr, png_const_inforp info_ptr),
    png_uint_32 png_get_eXIf(png_const_structrp png_ptr, png_inforp info_ptr, png_bytep * exif),
    void png_set_text_compression_method(png_structrp png_ptr, int method),
    void png_write_chunk_data(png_structrp png_ptr, png_const_bytep data, size_t length),
    png_uint_32 png_get_y_pixels_per_inch(png_const_structrp png_ptr, png_const_inforp info_ptr),
    png_uint_32 png_get_eXIf_1(png_const_structrp png_ptr, png_const_inforp info_ptr, png_uint_32 * num_exif, png_bytep * exif)
*/
//<score> 12, nr_unique_branch: 0
//<Quality> {"density":12,"unique_branches":{},"library_calls":["png_create_read_struct","png_create_info_struct","png_destroy_read_struct","png_set_longjmp_fn","png_destroy_read_struct","png_init_io","png_read_info","png_set_rows","png_set_rgb_to_gray_fixed","png_create_write_struct","png_destroy_read_struct","png_create_info_struct","png_destroy_write_struct","png_destroy_read_struct","png_set_longjmp_fn","png_destroy_write_struct","png_destroy_read_struct","png_init_io","png_destroy_read_struct","png_destroy_write_struct"],"critical_calls":["png_create_read_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_read_info","png_set_rows","png_set_rgb_to_gray_fixed","png_create_write_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_destroy_read_struct","png_destroy_write_struct"],"visited":0}
/**/

extern "C" int LLVMFuzzerTestOneInput_88(const uint8_t *data, size_t size) {
	if(size<0) return 0;

	FUZZ_FILE_INIT();
    // Step 4: Create input file
    FILE *in_file = fmemopen((void *)data, size, "rb");
	FUZZ_FILE_PUSH(in_file)
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
	FUZZ_FILENAME_PUSH("output_file")

    // Step 6: Create output file
    FILE *out_file = fopen("output_file", "wb");
	FUZZ_FILE_PUSH(out_file)
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Step 5: Get file descriptor for reading
    int in_fd = fuzz_fileno(in_file);
	FUZZ_FD_PUSH(in_fd)
    if (in_fd == -1) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
    }

    // Step 5: Get file descriptor for writing
    int out_fd = fuzz_fileno(out_file);
	FUZZ_FD_PUSH(out_fd)
    if (out_fd == -1) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }

    // Step 3: Create libpng structures
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }

    // Step 2: Initialize libpng structures
    if (setjmp(png_jmpbuf(png_ptr))) {
	FUZZ_FILE_CLOSE();
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }

    png_init_io(png_ptr, in_file);
    png_read_info(png_ptr, info_ptr);
    png_set_rows(png_ptr, info_ptr, NULL);

    // Step 1: Use libpng APIs
    png_set_rgb_to_gray_fixed(png_ptr, PNG_ERROR_ACTION_NONE, 0, 0);

    // Step 3: Create libpng structures for writing
    png_structp png_write_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_write_ptr) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }

    png_infop write_info_ptr = png_create_info_struct(png_write_ptr);
    if (!write_info_ptr) {
        png_destroy_write_struct(&png_write_ptr, NULL);
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }

    // Step 2: Initialize libpng structures for writing
    if (setjmp(png_jmpbuf(png_write_ptr))) {
	FUZZ_FILE_CLOSE();
        png_destroy_write_struct(&png_write_ptr, &write_info_ptr);
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }

    png_init_io(png_write_ptr, out_file);

    // Step 7: Release allocated resources
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    png_destroy_write_struct(&png_write_ptr, &write_info_ptr);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;

    assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}