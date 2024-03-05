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
//<ID> 4471
//<Prompt> ["png_get_palette_max","png_get_user_transform_ptr","png_get_pixels_per_inch","png_get_user_chunk_ptr","png_set_invalid","png_set_quantize","png_get_int_32"]
/*<Combination>: [int png_get_palette_max(png_const_structp png_ptr, png_const_infop info_ptr),
    png_voidp png_get_user_transform_ptr(png_const_structrp png_ptr),
    png_uint_32 png_get_pixels_per_inch(png_const_structrp png_ptr, png_const_inforp info_ptr),
    png_voidp png_get_user_chunk_ptr(png_const_structrp png_ptr),
    void png_set_invalid(png_const_structrp png_ptr, png_inforp info_ptr, int mask),
    void png_set_quantize(png_structrp png_ptr, png_colorp palette, int num_palette, int maximum_colors, png_const_uint_16p histogram, int full_quantize),
    png_int_32 png_get_int_32(png_const_bytep buf)
*/
//<score> 24, nr_unique_branch: 2
//<Quality> {"density":12,"unique_branches":{"png_get_palette_max":[[1252,8,1252,23,0,0,4,0],[1252,27,1252,43,0,0,4,0]]},"library_calls":["png_create_read_struct","png_create_info_struct","png_destroy_read_struct","png_set_longjmp_fn","png_destroy_read_struct","png_init_io","png_read_info","png_get_palette_max","png_get_user_transform_ptr","png_get_pixels_per_inch","png_get_user_chunk_ptr","png_set_invalid","png_destroy_read_struct"],"critical_calls":["png_create_read_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_read_info","png_get_palette_max","png_get_user_transform_ptr","png_get_pixels_per_inch","png_get_user_chunk_ptr","png_set_invalid","png_destroy_read_struct"],"visited":0}
/*Here is the C++ code that achieves the given instructions:

*/


extern "C" int LLVMFuzzerTestOneInput_93(const uint8_t* f_data, size_t f_size) {
	if(f_size<13) return 0;

	FUZZ_FILE_INIT();
	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
	//fuzzer shim end}




    FILE *in_file = fmemopen((void *)data, size, "rb");
	FUZZ_FILE_PUSH(in_file)
    if (in_file == NULL) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Create a PNG read struct
    png_structp png_ptr = png_create_read_struct(fuzz_str_1, NULL, NULL, NULL);
    if (png_ptr == NULL) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Create a PNG info struct
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == NULL) {
        assert_file_closed(&in_file);;
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Set error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
	FUZZ_FILE_CLOSE();
        assert_file_closed(&in_file);;
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Set up IO
    png_init_io(png_ptr, in_file);
    
    // Read PNG header
    png_read_info(png_ptr, info_ptr);
    
    // Use the libpng APIs and achieve the event
    int palette_max = png_get_palette_max(png_ptr, info_ptr);
    png_voidp user_transform_ptr = png_get_user_transform_ptr(png_ptr);
    png_uint_32 pixels_per_inch = png_get_pixels_per_inch(png_ptr, info_ptr);
    png_voidp user_chunk_ptr = png_get_user_chunk_ptr(png_ptr);
    png_set_invalid(png_ptr, info_ptr, palette_max);
    
    // Clean up
    assert_file_closed(&in_file);;
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    
    assert_file_closed(&in_file);
	return 0;
}