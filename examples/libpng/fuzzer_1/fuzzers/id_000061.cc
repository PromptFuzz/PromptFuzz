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
//<ID> 2903
//<Prompt> ["png_set_text_compression_strategy","png_set_iCCP","png_set_bgr","png_get_io_ptr","png_permit_mng_features","png_get_copyright"]
/*<Combination>: [void png_set_text_compression_strategy(png_structrp png_ptr, int strategy),
    void png_set_iCCP(png_const_structrp png_ptr, png_inforp info_ptr, png_const_charp name, int compression_type, png_const_bytep profile, png_uint_32 proflen),
    void png_set_bgr(png_structrp png_ptr),
    png_voidp png_get_io_ptr(png_const_structrp png_ptr),
    png_uint_32 png_permit_mng_features(png_structrp png_ptr, png_uint_32 mng_features_permitted),
    png_const_charp png_get_copyright(png_const_structrp png_ptr)
*/
//<score> 45, nr_unique_branch: 9
//<Quality> {"density":9,"unique_branches":{"png_set_bgr":[[25,8,25,23,0,0,4,1]],"png_colorspace_set_ICC":[[2446,8,2446,57,0,0,4,1],[2449,8,2449,72,0,0,4,0],[2449,8,2449,72,0,0,4,1],[2450,8,2451,28,0,0,4,1]],"png_icc_check_header":[[2026,8,2026,30,0,0,4,0]],"png_set_iCCP":[[686,63,686,78,0,0,4,1],[689,8,689,53,0,0,4,1],[705,11,705,22,0,0,4,0]]},"library_calls":["png_create_read_struct","png_create_info_struct","png_set_text_compression_strategy","png_set_iCCP","png_set_bgr","png_get_io_ptr","png_permit_mng_features","png_get_copyright","png_destroy_read_struct"],"critical_calls":["png_create_read_struct","png_create_info_struct","png_set_text_compression_strategy","png_set_iCCP","png_set_bgr","png_get_io_ptr","png_permit_mng_features","png_get_copyright","png_destroy_read_struct"],"visited":0}
/*Here is the completed code implementing the steps you provided:

*/


extern "C" int LLVMFuzzerTestOneInput_61(const uint8_t* f_data, size_t f_size) {
	if(f_size<36) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_4, fdp);
	//fuzzer shim end}




    // Step 4: Create input file
    FILE *in_file = fmemopen((void *)data, size, "rb");
    
    // Step 6: Create output file
    FILE *out_file = fopen("output_file", "wb");
    
    // Step 5: Get file descriptors
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);
    
    // Step 1: Use libpng APIs
    png_structp png_ptr = png_create_read_struct(fuzz_str_1, NULL, NULL, NULL);
    png_infop info_ptr = png_create_info_struct(png_ptr);
    
    png_set_text_compression_strategy(png_ptr, fuzz_int32_t_2);
    
    png_set_iCCP(png_ptr, info_ptr, fuzz_str_3, PNG_COMPRESSION_TYPE_BASE, data, size);
    
    png_set_bgr(png_ptr);
    
    png_voidp io_ptr = png_get_io_ptr(png_ptr);
    
    png_permit_mng_features(png_ptr, fuzz_uint32_t_4);
    
    png_const_charp copyright = png_get_copyright(png_ptr);
    
    // Step 2: Use more libpng APIs
    
    // Step 7: Free resources
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}