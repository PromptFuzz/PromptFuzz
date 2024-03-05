#include "FDSan.h"
#include <png.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 4810
//<Prompt> ["png_set_longjmp_fn","png_get_gAMA","png_get_valid","png_get_sCAL","png_get_user_height_max","png_set_rgb_to_gray","png_set_swap_alpha"]
/*<Combination>: [jmp_buf *png_set_longjmp_fn(png_structrp png_ptr, png_longjmp_ptr longjmp_fn, size_t jmp_buf_size),
    png_uint_32 png_get_gAMA(png_const_structrp png_ptr, png_const_inforp info_ptr, double * file_gamma),
    png_uint_32 png_get_valid(png_const_structrp png_ptr, png_const_inforp info_ptr, png_uint_32 flag),
    png_uint_32 png_get_sCAL(png_const_structrp png_ptr, png_const_inforp info_ptr, int * unit, double * width, double * height),
    png_uint_32 png_get_user_height_max(png_const_structrp png_ptr),
    void png_set_rgb_to_gray(png_structrp png_ptr, int error_action, double red, double green),
    void png_set_swap_alpha(png_structrp png_ptr)
*/
//<score> 15, nr_unique_branch: 0
//<Quality> {"density":15,"unique_branches":{},"library_calls":["png_create_read_struct","png_create_info_struct","png_destroy_read_struct","png_set_longjmp_fn","png_destroy_read_struct","png_set_longjmp_fn","png_destroy_read_struct","png_init_io","png_read_info","png_get_gAMA","png_get_valid","png_get_sCAL","png_get_user_height_max","png_set_rgb_to_gray","png_set_swap_alpha","png_destroy_read_struct"],"critical_calls":["png_create_read_struct","png_create_info_struct","png_set_longjmp_fn","png_set_longjmp_fn","png_init_io","png_read_info","png_get_gAMA","png_get_valid","png_get_sCAL","png_get_user_height_max","png_set_rgb_to_gray","png_set_swap_alpha","png_destroy_read_struct"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_104(const uint8_t *data, size_t size) {
	if(size<0) return 0;

	FUZZ_FILE_INIT();
    // Create a PNG read struct
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (png_ptr == NULL) {
        return 0;
    }
  
    // Create a PNG info struct
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == NULL) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return 0;
    }
  
    // Setup error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
	FUZZ_FILE_CLOSE();
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return 0;
    }
  
    // Set custom longjmp function
    jmp_buf jmpbuf;
    png_set_longjmp_fn(png_ptr, longjmp, sizeof(jmp_buf));
  
    // Set the input file
    FILE *in_file = fmemopen((void *)data, size, "rb");
	FUZZ_FILE_PUSH(in_file)
    if (in_file == NULL) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        assert_file_closed(&in_file);
	return 0;
    }
    png_init_io(png_ptr, in_file);
  
    // Read the PNG header
    png_read_info(png_ptr, info_ptr);
  
    // Get the gAMA value
    double file_gamma;
    if (png_get_gAMA(png_ptr, info_ptr, &file_gamma)) {
        // Do something with the gamma value
    }
  
    // Get valid flags
    png_uint_32 valid_flags = png_get_valid(png_ptr, info_ptr, PNG_INFO_gAMA | PNG_INFO_sCAL);
    if (valid_flags & PNG_INFO_gAMA) {
        // Do something with the gAMA flag
    }
    if (valid_flags & PNG_INFO_sCAL) {
        // Do something with the sCAL flag
    }
  
    // Get the sCAL value
    int unit;
    double width, height;
    if (png_get_sCAL(png_ptr, info_ptr, &unit, &width, &height)) {
        // Do something with the sCAL value
    }
  
    // Get the user height max
    png_uint_32 user_height_max = png_get_user_height_max(png_ptr);
  
    // Set rgb to gray
    png_set_rgb_to_gray(png_ptr, PNG_ERROR_ACTION_NONE, 0.3, 0.59);

    // Set swap alpha
    png_set_swap_alpha(png_ptr);

    // Perform additional operations as needed

    // Clean up
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    assert_file_closed(&in_file);;

    assert_file_closed(&in_file);
	return 0;
}