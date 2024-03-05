#include "FDSan.h"
#include <png.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 1769
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
//<score> 70, nr_unique_branch: 4
//<Quality> {"density":14,"unique_branches":{"png_get_hIST":[[824,27,824,43,0,0,4,1]],"png_read_info":[[100,27,100,43,0,0,4,0]],"png_get_pixels_per_meter":[[173,27,173,43,0,0,4,1]],"png_get_cHRM":[[533,27,533,43,0,0,4,1]]},"library_calls":["png_create_read_struct","png_destroy_read_struct","png_set_error_fn","png_init_io","png_read_info","png_get_hIST","png_get_pixels_per_inch","png_set_scale_16","png_set_compression_buffer_size","png_get_cHRM","png_init_io","png_get_error_ptr","png_set_background","png_destroy_read_struct"],"critical_calls":["png_create_read_struct","png_set_error_fn","png_init_io","png_read_info","png_get_hIST","png_get_pixels_per_inch","png_set_scale_16","png_set_compression_buffer_size","png_get_cHRM","png_init_io","png_get_error_ptr","png_set_background","png_destroy_read_struct"],"visited":0}
/**/

extern "C" int LLVMFuzzerTestOneInput_31(const uint8_t *data, size_t size)
{
	if(size<0) return 0;

	FUZZ_FILE_INIT();
    png_structp png_ptr;
    png_infop info_ptr;
    jmp_buf jmpbuf;

    FILE *in_file = fmemopen((void *)data, size, "rb");
	FUZZ_FILE_PUSH(in_file)
    if (in_file == NULL)
    {
        assert_file_closed(&in_file);
	return 0;
    }
	FUZZ_FILENAME_PUSH("output_file")

    FILE *out_file = fopen("output_file", "wb");
	FUZZ_FILE_PUSH(out_file)
    if (out_file == NULL)
    {
        assert_file_closed(&in_file);;
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    int in_fd = fuzz_fileno(in_file);
	FUZZ_FD_PUSH(in_fd)
    int out_fd = fuzz_fileno(out_file);
	FUZZ_FD_PUSH(out_fd)

    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (png_ptr == NULL)
    {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }

    if (setjmp(jmpbuf))
    {
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

    png_set_error_fn(png_ptr, (png_voidp)jmpbuf, NULL, NULL);

    png_init_io(png_ptr, in_file);
    png_read_info(png_ptr, info_ptr);

    png_uint_16p hist;
    png_get_hIST(png_ptr, info_ptr, &hist);
    png_get_pixels_per_inch(png_ptr, info_ptr);
    png_set_scale_16(png_ptr);
    png_set_compression_buffer_size(png_ptr, size);
    double white_x, white_y, red_x, red_y, green_x, green_y, blue_x, blue_y;
    png_get_cHRM(png_ptr, info_ptr, &white_x, &white_y, &red_x, &red_y, &green_x, &green_y, &blue_x, &blue_y);
    png_init_io(png_ptr, in_file);
    png_get_error_ptr(png_ptr);
    png_set_background(png_ptr, NULL, 0, 0, 0.0);

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