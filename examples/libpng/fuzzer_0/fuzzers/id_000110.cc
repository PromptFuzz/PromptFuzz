#include "FDSan.h"
#include <png.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 5316
//<Prompt> ["png_get_image_height","png_set_text_compression_mem_level","png_get_uint_16","png_set_strip_alpha","png_warning","png_sig_cmp","png_set_oFFs"]
/*<Combination>: [png_uint_32 png_get_image_height(png_const_structrp png_ptr, png_const_inforp info_ptr),
    void png_set_text_compression_mem_level(png_structrp png_ptr, int mem_level),
    png_uint_16 png_get_uint_16(png_const_bytep buf),
    void png_set_strip_alpha(png_structrp png_ptr),
    void png_warning(png_const_structrp png_ptr, png_const_charp warning_message),
    int png_sig_cmp(png_const_bytep sig, size_t start, size_t num_to_check),
    void png_set_oFFs(png_const_structrp png_ptr, png_inforp info_ptr, png_int_32 offset_x, png_int_32 offset_y, int unit_type)
*/
//<score> 36, nr_unique_branch: 2
//<Quality> {"density":12,"unique_branches":{"png_read_sig":[[126,8,126,31,0,0,4,0]],"png_check_IHDR":[[2683,11,2683,56,0,0,4,1]]},"library_calls":["png_sig_cmp","png_create_read_struct","png_create_info_struct","png_destroy_read_struct","png_set_longjmp_fn","png_destroy_read_struct","png_init_io","png_set_sig_bytes","png_read_info","png_get_image_width","png_get_image_height","png_get_bit_depth","png_get_color_type","png_destroy_read_struct"],"critical_calls":["png_sig_cmp","png_create_read_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_set_sig_bytes","png_read_info","png_get_image_width","png_get_image_height","png_get_bit_depth","png_get_color_type","png_destroy_read_struct"],"visited":0}
/*Here is the C++ program that achieves the event using the libpng APIs:

*/


extern "C" int LLVMFuzzerTestOneInput_110(const uint8_t *data, size_t size) {
	if(size<0) return 0;

	FUZZ_FILE_INIT();
    png_structp png_ptr;
    png_infop info_ptr;
    png_uint_32 width, height;
    int bit_depth, color_type;
    FILE *in_file = fmemopen((void *)data, size, "rb");
	FUZZ_FILE_PUSH(in_file)
    
    if (in_file == NULL) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    png_byte sig[8];
    fread(sig, 1, 8, in_file);
    
    if (png_sig_cmp(sig, 0, 8) != 0) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    
    if (png_ptr == NULL) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    info_ptr = png_create_info_struct(png_ptr);
    
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
    png_set_sig_bytes(png_ptr, 8);
    png_read_info(png_ptr, info_ptr);
    
    width = png_get_image_width(png_ptr, info_ptr);
    height = png_get_image_height(png_ptr, info_ptr);
    bit_depth = png_get_bit_depth(png_ptr, info_ptr);
    color_type = png_get_color_type(png_ptr, info_ptr);
    
    // Use other libpng APIs as needed
    
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    assert_file_closed(&in_file);;
    
    assert_file_closed(&in_file);
	return 0;
}