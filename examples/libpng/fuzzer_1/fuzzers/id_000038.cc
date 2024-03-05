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
//<ID> 2003
//<Prompt> ["png_write_chunk_start","png_get_int_32","png_set_text_compression_method","png_get_libpng_ver","png_get_cHRM_XYZ","png_set_compression_level","png_set_palette_to_rgb"]
/*<Combination>: [void png_write_chunk_start(png_structrp png_ptr, png_const_bytep chunk_name, png_uint_32 length),
    png_int_32 png_get_int_32(png_const_bytep buf),
    void png_set_text_compression_method(png_structrp png_ptr, int method),
    png_const_charp png_get_libpng_ver(png_const_structrp png_ptr),
    png_uint_32 png_get_cHRM_XYZ(png_const_structrp png_ptr, png_const_inforp info_ptr, double * red_X, double * red_Y, double * red_Z, double * green_X, double * green_Y, double * green_Z, double * blue_X, double * blue_Y, double * blue_Z),
    void png_set_compression_level(png_structrp png_ptr, int level),
    void png_set_palette_to_rgb(png_structrp png_ptr)
*/
//<score> 18, nr_unique_branch: 3
//<Quality> {"density":9,"unique_branches":{"png_get_IHDR":[[858,8,858,32,0,0,4,1],[861,8,861,27,0,0,4,1],[864,8,864,30,0,0,4,1]]},"library_calls":["png_create_read_struct","png_create_info_struct","png_destroy_read_struct","png_set_longjmp_fn","png_destroy_read_struct","png_destroy_read_struct","png_init_io","png_read_info","png_get_IHDR","png_destroy_read_struct"],"critical_calls":["png_create_read_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_read_info","png_get_IHDR","png_destroy_read_struct"],"visited":0}
/**/

extern "C" int LLVMFuzzerTestOneInput_38(const uint8_t* f_data, size_t f_size) {
	if(f_size<13) return 0;

	FUZZ_FILE_INIT();
	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
	//fuzzer shim end}




    // Step 1: Initialize libpng structures
    png_structp png_ptr = png_create_read_struct(fuzz_str_1, NULL, NULL, NULL);
    if (!png_ptr) {
        return 0;
    }
    
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return 0;
    }
    
    // Step 2: Set up error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
	FUZZ_FILE_CLOSE();
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return 0;
    }
    
    // Step 3: Set up I/O
    FILE *in_file = fmemopen((void *)data, size, "rb");
	FUZZ_FILE_PUSH(in_file)
    if (!in_file) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        assert_file_closed(&in_file);
	return 0;
    }
    
    png_init_io(png_ptr, in_file);
    
    // Step 4: Read the PNG image
    png_uint_32 width, height;
    int bit_depth, color_type;
    
    png_read_info(png_ptr, info_ptr);
    png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type, NULL, NULL, NULL);
    
    // Step 5: Print image information
    printf("Image width: %u\n", width);
    printf("Image height: %u\n", height);
    printf("Bit depth: %d\n", bit_depth);
    printf("Color type: %d\n", color_type);
    
    // Step 6: Clean up
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    assert_file_closed(&in_file);;
    
    assert_file_closed(&in_file);
	return 0;
}