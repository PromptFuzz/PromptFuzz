#include "FDSan.h"
#include <png.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 1762
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
//<score> 56, nr_unique_branch: 5
//<Quality> {"density":14,"unique_branches":{"png_get_compression_buffer_size":[[1182,8,1182,23,0,0,4,1],[1186,8,1186,49,0,0,4,0]],"png_get_error_ptr":[[858,8,858,23,0,0,4,1]],"png_set_background_fixed":[[143,8,143,37,0,0,4,1],[143,41,143,65,0,0,4,0]]},"library_calls":["png_create_read_struct","png_create_info_struct","png_destroy_read_struct","png_set_longjmp_fn","png_destroy_read_struct","png_init_io","png_set_error_fn","png_get_error_ptr","png_get_hIST","png_get_pixels_per_inch","png_set_scale_16","png_get_compression_buffer_size","png_get_cHRM","png_set_background","png_destroy_read_struct"],"critical_calls":["png_create_read_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_set_error_fn","png_get_error_ptr","png_get_hIST","png_get_pixels_per_inch","png_set_scale_16","png_get_compression_buffer_size","png_get_cHRM","png_set_background","png_destroy_read_struct"],"visited":0}
/*Here is the implementation of the C++ program:

*/


extern "C" int LLVMFuzzerTestOneInput_29(const uint8_t *data, size_t size) {
	if(size<0) return 0;

	FUZZ_FILE_INIT();
    FILE *in_file = fmemopen((void *)data, size, "rb");
	FUZZ_FILE_PUSH(in_file)
    if (in_file == NULL) {
        assert_file_closed(&in_file);
	return 0;
    }
  
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (png_ptr == NULL) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
  
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == NULL) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
  
    if (setjmp(png_jmpbuf(png_ptr))) {
	FUZZ_FILE_CLOSE();
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
  
    png_init_io(png_ptr, in_file);
    png_set_error_fn(png_ptr, png_get_error_ptr(png_ptr), NULL, NULL);

    // Call the required libpng APIs
    png_uint_16p hist;
    png_uint_32 hIST_ret = png_get_hIST(png_ptr, info_ptr, &hist);

    png_uint_32 pixels_per_inch = png_get_pixels_per_inch(png_ptr, info_ptr);
  
    png_set_scale_16(png_ptr);
  
    size_t buffer_size = png_get_compression_buffer_size(png_ptr);

    double white_x, white_y, red_x, red_y, green_x, green_y, blue_x, blue_y;
    png_uint_32 cHRM_ret = png_get_cHRM(png_ptr, info_ptr, &white_x, &white_y, &red_x, &red_y, &green_x, &green_y, &blue_x, &blue_y);

    png_set_background(png_ptr, NULL, 0, 0, 0.0);
  
    // Clean up resources
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    assert_file_closed(&in_file);;
  
    assert_file_closed(&in_file);
	return 0;
}