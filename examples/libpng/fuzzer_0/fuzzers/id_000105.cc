#include "FDSan.h"
#include <png.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 4812
//<Prompt> ["png_set_longjmp_fn","png_get_gAMA","png_get_valid","png_get_sCAL","png_get_user_height_max","png_set_rgb_to_gray","png_set_swap_alpha"]
/*<Combination>: [jmp_buf *png_set_longjmp_fn(png_structrp png_ptr, png_longjmp_ptr longjmp_fn, size_t jmp_buf_size),
    png_uint_32 png_get_gAMA(png_const_structrp png_ptr, png_const_inforp info_ptr, double * file_gamma),
    png_uint_32 png_get_valid(png_const_structrp png_ptr, png_const_inforp info_ptr, png_uint_32 flag),
    png_uint_32 png_get_sCAL(png_const_structrp png_ptr, png_const_inforp info_ptr, int * unit, double * width, double * height),
    png_uint_32 png_get_user_height_max(png_const_structrp png_ptr),
    void png_set_rgb_to_gray(png_structrp png_ptr, int error_action, double red, double green),
    void png_set_swap_alpha(png_structrp png_ptr)
*/
//<score> 36, nr_unique_branch: 3
//<Quality> {"density":9,"unique_branches":{"png_get_sCAL":[[958,8,958,23,0,0,4,1]],"png_get_valid":[[23,8,23,23,0,0,4,1]],"png_get_user_height_max":[[1214,12,1214,19,0,0,4,1]]},"library_calls":["png_set_longjmp_fn","png_set_longjmp_fn","png_get_gAMA","png_get_valid","png_get_sCAL","png_get_user_height_max","png_set_rgb_to_gray","png_set_swap_alpha","png_destroy_read_struct"],"critical_calls":["png_set_longjmp_fn","png_set_longjmp_fn","png_get_gAMA","png_get_valid","png_get_sCAL","png_get_user_height_max","png_set_rgb_to_gray","png_set_swap_alpha","png_destroy_read_struct"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_105(const uint8_t *data, size_t size) {
	if(size<0) return 0;

    // Step 4: Create input file stream
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file == nullptr) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Step 5: Get file descriptor for input file
    int in_fd = fuzz_fileno(in_file);

    // Step 6: Define output file name
    const char *out_file_name = "output_file";

    // Initialize libpng variables
    png_structp png_ptr = nullptr;
    png_infop info_ptr = nullptr;
    jmp_buf jmpbuf;
    png_set_longjmp_fn(png_ptr, longjmp, sizeof(jmp_buf));
    
    // Step 2: Call libpng APIs
    if (setjmp(jmpbuf) == 0) {
        // Step 1: png_set_longjmp_fn
        png_set_longjmp_fn(png_ptr, longjmp, sizeof(jmp_buf));
        
        // Step 3: png_get_gAMA
        double file_gamma;
        png_get_gAMA(png_ptr, info_ptr, &file_gamma);
        
        // Step 3: png_get_valid
        png_uint_32 valid_flags = png_get_valid(png_ptr, info_ptr, PNG_INFO_gAMA);
        
        // Step 3: png_get_sCAL
        int unit;
        double width, height;
        png_get_sCAL(png_ptr, info_ptr, &unit, &width, &height);
        
        // Step 3: png_get_user_height_max
        png_uint_32 user_height_max = png_get_user_height_max(png_ptr);
        
        // Step 3: png_set_rgb_to_gray
        png_set_rgb_to_gray(png_ptr, PNG_ERROR_ACTION_NONE, 0.21, 0.72);
        
        // Step 3: png_set_swap_alpha
        png_set_swap_alpha(png_ptr);
    }
    
    // Release all allocated resources
    assert_file_closed(&in_file);;
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);

    assert_file_name_closed("output_file");
	assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
}