#include "FDSan.h"
#include <png.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 556
//<Prompt> ["png_set_unknown_chunk_location","png_get_rows","png_set_text_compression_mem_level","png_set_sCAL_s","png_longjmp","png_get_gAMA","png_get_libpng_ver"]
/*<Combination>: [void png_set_unknown_chunk_location(png_const_structrp png_ptr, png_inforp info_ptr, int chunk, int location),
    png_bytepp png_get_rows(png_const_structrp png_ptr, png_const_inforp info_ptr),
    void png_set_text_compression_mem_level(png_structrp png_ptr, int mem_level),
    void png_set_sCAL_s(png_const_structrp png_ptr, png_inforp info_ptr, int unit, png_const_charp swidth, png_const_charp sheight),
    void png_longjmp(png_const_structrp png_ptr, int val),
    png_uint_32 png_get_gAMA(png_const_structrp png_ptr, png_const_inforp info_ptr, double * file_gamma),
    png_const_charp png_get_libpng_ver(png_const_structrp png_ptr)
*/
//<score> 4, nr_unique_branch: 3
//<Quality> {"density":1,"unique_branches":{"png_get_rows":[[53,8,53,23,0,0,4,1]],"png_set_text_compression_mem_level":[[1273,8,1273,23,0,0,4,0]],"png_set_unknown_chunk_location":[[1298,8,1298,23,0,0,4,1]]},"library_calls":["png_set_unknown_chunk_location","png_get_rows","png_set_text_compression_mem_level","png_get_gAMA","png_get_libpng_ver"],"critical_calls":["png_set_unknown_chunk_location","png_get_rows","png_set_text_compression_mem_level","png_get_gAMA","png_get_libpng_ver"],"visited":0}
/*Here is the C++ program that achieves the event using the libpng APIs:

*/


extern "C" int LLVMFuzzerTestOneInput_11(const uint8_t *data, size_t size) {
	if(size<0) return 0;

	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);

    // Step 4: Create input and output FILE pointers
    FILE *in_file = fmemopen((void *)data, size, "rb");
    FILE *out_file = fopen("output_file", "wb");
  
    // Step 5: Get file descriptors
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);
  
    // Step 6: Define file names
    const char* in_file_name = "input_file";
    const char* out_file_name = "output_file";
  
    // Step 1: Set unknown chunk location
    png_set_unknown_chunk_location(NULL, NULL, 0, 1);
  
    // Step 2: Get rows
    png_bytepp rows = png_get_rows(NULL, NULL);
  
    // Step 3: Set text compression memory level
    png_set_text_compression_mem_level(NULL, 5);
  
    // Step 7: Get gAMA value
    double file_gamma;
    png_get_gAMA(NULL, NULL, &file_gamma);
  
    // Step 8: Get libpng version
    png_const_charp libpng_version = png_get_libpng_ver(NULL);
  
    // Step 9: Cleanup and release resources
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
  
    assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}