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
//<ID> 2529
//<Prompt> ["png_malloc_default","png_get_int_32","png_create_info_struct","png_get_pixel_aspect_ratio_fixed","png_set_compression_mem_level","png_get_color_type","png_get_gAMA","png_image_finish_read"]
/*<Combination>: [png_voidp png_malloc_default(png_const_structrp png_ptr, png_alloc_size_t size),
    png_int_32 png_get_int_32(png_const_bytep buf),
    png_infop png_create_info_struct(png_const_structrp png_ptr),
    png_fixed_point png_get_pixel_aspect_ratio_fixed(png_const_structrp png_ptr, png_const_inforp info_ptr),
    void png_set_compression_mem_level(png_structrp png_ptr, int mem_level),
    png_byte png_get_color_type(png_const_structrp png_ptr, png_const_inforp info_ptr),
    png_uint_32 png_get_gAMA(png_const_structrp png_ptr, png_const_inforp info_ptr, double * file_gamma),
    int png_image_finish_read(png_imagep image, png_const_colorp background, void * buffer, png_int_32 row_stride, void * colormap)
*/
//<score> 8, nr_unique_branch: 3
//<Quality> {"density":2,"unique_branches":{"png_malloc_default":[[197,8,197,23,0,0,4,0]],"png_create_info_struct":[[360,8,360,23,0,0,4,0]],"png_free_default":[[251,8,251,23,0,0,4,0]]},"library_calls":["png_malloc_default","png_create_info_struct","png_free_default"],"critical_calls":["png_malloc_default","png_create_info_struct","png_free_default"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_51(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    // Step 4: Open input file using fmemopen
    FILE *in_file = fmemopen((void *)data, size, "rb");
  
    // Step 6: Set input file name
    const char *input_file = "input_file";
  
    // Step 1: Allocate memory using png_malloc_default
    png_voidp ptr = png_malloc_default(NULL, sizeof(int));
  
    // Step 2: Get int value from byte stream using png_get_int_32
    const png_bytep byte_stream = (const png_bytep)data;
    png_int_32 value = png_get_int_32(byte_stream);
  
    // Step 3: Create png_info struct using png_create_info_struct
    png_const_structrp png_ptr = NULL; // Initialize png_ptr
    png_infop info_ptr = png_create_info_struct(png_ptr);
  
    // Step 5: Get file descriptor using fuzz_fileno
    int fd_input = fuzz_fileno(in_file);
  
    // Step 7: Release resources
    png_free_default(NULL, ptr);
    assert_file_closed(&in_file);;
  
    assert_file_name_closed("input_file");
	assert_file_closed(&in_file);
	assert_fd_closed(fd_input);
	return 0;
}