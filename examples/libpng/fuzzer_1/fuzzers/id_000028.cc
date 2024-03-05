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
//<ID> 1740
//<Prompt> ["png_write_chunk","png_set_chunk_cache_max","png_get_pCAL","png_set_option","png_set_unknown_chunk_location","png_get_pixels_per_meter"]
/*<Combination>: [void png_write_chunk(png_structrp png_ptr, png_const_bytep chunk_name, png_const_bytep data, size_t length),
    void png_set_chunk_cache_max(png_structrp png_ptr, png_uint_32 user_chunk_cache_max),
    png_uint_32 png_get_pCAL(png_const_structrp png_ptr, png_inforp info_ptr, png_charp * purpose, png_int_32 * X0, png_int_32 * X1, int * type, int * nparams, png_charp * units, png_charpp * params),
    int png_set_option(png_structrp png_ptr, int option, int onoff),
    void png_set_unknown_chunk_location(png_const_structrp png_ptr, png_inforp info_ptr, int chunk, int location),
    png_uint_32 png_get_pixels_per_meter(png_const_structrp png_ptr, png_const_inforp info_ptr)
*/
//<score> 30, nr_unique_branch: 4
//<Quality> {"density":6,"unique_branches":{"png_set_chunk_cache_max":[[1645,8,1645,23,0,0,4,1]],"png_set_option":[[4334,8,4334,23,0,0,4,1]],"png_get_pCAL":[[908,8,908,23,0,0,4,1]],"png_write_complete_chunk":[[178,8,178,23,0,0,4,0]]},"library_calls":["png_set_chunk_cache_max","png_get_pCAL","png_set_option","png_set_unknown_chunk_location","png_get_pixels_per_meter","png_write_chunk"],"critical_calls":["png_set_chunk_cache_max","png_get_pCAL","png_set_option","png_set_unknown_chunk_location","png_get_pixels_per_meter","png_write_chunk"],"visited":0}
/**/

extern "C" int LLVMFuzzerTestOneInput_28(const uint8_t* f_data, size_t f_size) {
	if(f_size<33) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
		FDPConsumeRandomLengthString(unsigned char, fuzz_str_6, fuzz_str_sz_6, fdp);
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    png_structrp png_ptr;
    png_inforp info_ptr;
    png_bytep *row_pointers;
    png_FILE_p in_file = fmemopen((void*)data, size, "rb");
    FILE *out_file = fopen("output_file", "wb");
    int in_file_descriptor = fuzz_fileno(in_file);
    int out_file_descriptor = fuzz_fileno(out_file);
    const char* input_file_name = "input_file";
    const char* output_file_name = "output_file";
    
    png_const_bytep chunk_name = (png_const_bytep)"Chunk";
    png_const_bytep chunk_data = (png_const_bytep)"Chunk Data";
    size_t chunk_length = strlen((const char*)chunk_data);
    
    png_uint_32 user_chunk_cache_max = 1000;
    png_set_chunk_cache_max(png_ptr, fuzz_uint32_t_1);
    
    png_charp purpose;
    png_int_32 X0, X1;
    int type, nparams;
    png_charp units;
    png_charpp params;
    png_get_pCAL(png_ptr, info_ptr, &purpose, &X0, &X1, &type, &nparams, &units, &params);
    
    int option = 1;
    int onoff = 1;
    png_set_option(png_ptr, fuzz_int32_t_2, fuzz_int32_t_3);
    
    int chunk = 0;
    int location = 1;
    png_set_unknown_chunk_location(png_ptr, info_ptr, fuzz_int32_t_4, fuzz_int32_t_5);
    
    png_uint_32 pixels_per_meter = png_get_pixels_per_meter(png_ptr, info_ptr);
    
    // Write a PNG chunk
    png_write_chunk(png_ptr, chunk_name, fuzz_str_6, static_cast<size_t>(fuzz_str_sz_6));
    
    // Use the input file descriptor
    FILE* in_file_from_descriptor = fdopen(in_file_descriptor, "rb");
    
    // Use the output file descriptor
    FILE* out_file_from_descriptor = fdopen(out_file_descriptor, "wb");
    
    // Use the input file name
    FILE* in_file_from_name = fopen(input_file_name, "rb");
    
    // Use the output file name
    FILE* out_file_from_name = fopen(output_file_name, "wb");
    
    // Release resources
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    assert_file_closed(&in_file_from_descriptor);;
    assert_file_closed(&out_file_from_descriptor);;
    assert_file_closed(&in_file_from_name);;
    assert_file_closed(&out_file_from_name);;

    assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_name_closed("output_file");
	assert_file_closed(&out_file_from_name);
	assert_file_closed(&in_file_from_name);
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_file_descriptor);
	assert_fd_closed(in_file_descriptor);
	return 0;
}