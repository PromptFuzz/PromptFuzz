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
//<ID> 5613
//<Prompt> ["png_get_text","png_create_info_struct","png_image_begin_read_from_file","png_get_x_pixels_per_inch","png_get_pCAL"]
/*<Combination>: [int png_get_text(png_const_structrp png_ptr, png_inforp info_ptr, png_textp * text_ptr, int * num_text),
    png_infop png_create_info_struct(png_const_structrp png_ptr),
    int png_image_begin_read_from_file(png_imagep image, const char * file_name),
    png_uint_32 png_get_x_pixels_per_inch(png_const_structrp png_ptr, png_const_inforp info_ptr),
    png_uint_32 png_get_pCAL(png_const_structrp png_ptr, png_inforp info_ptr, png_charp * purpose, png_int_32 * X0, png_int_32 * X1, int * type, int * nparams, png_charp * units, png_charpp * params)
*/
//<score> 9, nr_unique_branch: 0
//<Quality> {"density":9,"unique_branches":{},"library_calls":["png_create_read_struct","png_create_info_struct","png_destroy_read_struct","png_set_longjmp_fn","png_destroy_read_struct","png_destroy_read_struct","png_init_io","png_read_info","png_get_IHDR","png_destroy_read_struct"],"critical_calls":["png_create_read_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_read_info","png_get_IHDR","png_destroy_read_struct"],"visited":0}
/*Here is a C++ program that uses the libpng library APIs to achieve the event described in step 1:

*/


extern "C" int LLVMFuzzerTestOneInput_116(const uint8_t* f_data, size_t f_size) {
	if(f_size<13) return 0;

	FUZZ_FILE_INIT();
	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
	//fuzzer shim end}




    png_structp png_ptr;
    png_infop info_ptr;
    png_uint_32 width, height;
    int bit_depth, color_type, interlace_type;
    
    // Create png_struct and png_info objects
    png_ptr = png_create_read_struct(fuzz_str_1, NULL, NULL, NULL);
    if (!png_ptr) {
        return 0;
    }
    
    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return 0;
    }
    
    // Set error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
	FUZZ_FILE_CLOSE();
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return 0;
    }
    
    // Initialize input file stream
    FILE *in_file = fmemopen((void *)data, size, "rb");
	FUZZ_FILE_PUSH(in_file)
    if (!in_file) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Set the input source
    png_init_io(png_ptr, in_file);
    
    // Read the PNG header
    png_read_info(png_ptr, info_ptr);
    
    // Get image attributes
    png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type, &interlace_type, NULL, NULL);
    
    // Print image attributes
    printf("Image dimensions: %d x %d\n", width, height);
    printf("Bit depth: %d\n", bit_depth);
    printf("Color type: %d\n", color_type);
    printf("Interlace type: %d\n", interlace_type);
    
    // Release resources
    assert_file_closed(&in_file);;
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    
    assert_file_closed(&in_file);
	return 0;
}