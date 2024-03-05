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
//<ID> 5685
//<Prompt> ["png_set_invalid","png_set_sCAL","png_get_pixels_per_meter","png_set_benign_errors","png_set_crc_action","png_get_io_chunk_type","png_start_read_image","png_get_unknown_chunks","png_get_int_32"]
/*<Combination>: [void png_set_invalid(png_const_structrp png_ptr, png_inforp info_ptr, int mask),
    void png_set_sCAL(png_const_structrp png_ptr, png_inforp info_ptr, int unit, double width, double height),
    png_uint_32 png_get_pixels_per_meter(png_const_structrp png_ptr, png_const_inforp info_ptr),
    void png_set_benign_errors(png_structrp png_ptr, int allowed),
    void png_set_crc_action(png_structrp png_ptr, int crit_action, int ancil_action),
    png_uint_32 png_get_io_chunk_type(png_const_structrp png_ptr),
    void png_start_read_image(png_structrp png_ptr),
    int png_get_unknown_chunks(png_const_structrp png_ptr, png_inforp info_ptr, png_unknown_chunkpp entries),
    png_int_32 png_get_int_32(png_const_bytep buf)
*/
//<score> 56, nr_unique_branch: 33
//<Quality> {"density":14,"unique_branches":{"png_set_sCAL":[[492,8,492,18,0,0,4,1],[495,13,495,24,0,0,4,1]],"png_pow10":[[2866,8,2866,17,0,0,4,1],[2872,8,2872,17,0,0,4,0],[2878,14,2878,23,0,0,4,0],[2878,14,2878,23,0,0,4,1],[2882,14,2882,23,0,0,4,0],[2882,14,2882,23,0,0,4,1],[2884,11,2884,21,0,0,4,1]],"png_ascii_from_fp":[[2911,8,2911,21,0,0,4,1],[2915,8,2915,29,0,0,4,1],[2919,8,2919,27,0,0,4,0],[2921,11,2921,17,0,0,4,1],[2928,11,2928,24,0,0,4,0],[2928,28,2928,41,0,0,4,0],[2949,17,2949,31,0,0,4,1],[2949,35,2949,44,0,0,4,0],[2949,35,2949,44,0,0,4,1],[2954,17,2954,32,0,0,4,0],[2971,17,2971,24,0,0,4,0],[2971,17,2971,24,0,0,4,1],[2989,17,2989,28,0,0,4,1],[3013,20,3013,53,0,0,4,0],[3085,20,3085,26,0,0,4,1],[3096,26,3096,35,0,0,4,1],[3114,23,3114,38,0,0,4,0],[3116,26,3116,38,0,0,4,1],[3126,20,3126,51,0,0,4,0],[3126,55,3126,67,0,0,4,1],[3137,17,3137,32,0,0,4,0],[3137,36,3137,48,0,0,4,0],[3146,23,3146,36,0,0,4,0],[3146,23,3146,36,0,0,4,1]]},"library_calls":["png_create_read_struct","png_create_info_struct","png_destroy_read_struct","png_init_io","png_set_write_fn","png_set_invalid","png_set_sCAL","png_get_pixels_per_meter","png_set_benign_errors","png_set_crc_action","png_get_io_chunk_type","png_start_read_image","png_get_unknown_chunks","png_destroy_read_struct"],"critical_calls":["png_create_read_struct","png_create_info_struct","png_init_io","png_set_write_fn","png_set_invalid","png_set_sCAL","png_get_pixels_per_meter","png_set_benign_errors","png_set_crc_action","png_get_io_chunk_type","png_start_read_image","png_get_unknown_chunks","png_destroy_read_struct"],"visited":0}
/**/

extern "C" int LLVMFuzzerTestOneInput_117(const uint8_t* f_data, size_t f_size) {
	if(f_size<29) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
	//fuzzer shim end}




    // Create a FILE * to read the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file == nullptr) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Create a FILE * to write the output data
    FILE *out_file = fopen("output_file", "wb");
    if (out_file == nullptr) {
        assert_file_closed(&in_file);;
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Create a png_struct and png_info objects
    png_structp png_ptr = png_create_read_struct(fuzz_str_1, nullptr, nullptr, nullptr);
    if (png_ptr == nullptr) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == nullptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Set the file to read from
    png_init_io(png_ptr, in_file);
    
    // Set the file to write to
    png_set_write_fn(png_ptr, out_file, nullptr, nullptr);
    
    // Set invalid flag
    png_set_invalid(png_ptr, info_ptr, fuzz_int32_t_2);
    
    // Set sCAL values
    png_set_sCAL(png_ptr, info_ptr, 1, 100.0, 200.0);
    
    // Get pixels per meter
    png_uint_32 ppm = png_get_pixels_per_meter(png_ptr, info_ptr);
    
    // Enable benign errors
    png_set_benign_errors(png_ptr, fuzz_int32_t_3);
    
    // Set CRC action
    png_set_crc_action(png_ptr, fuzz_int32_t_4, fuzz_int32_t_5);
    
    // Get IO chunk type
    png_uint_32 io_chunk_type = png_get_io_chunk_type(png_ptr);
    
    // Start reading the image
    png_start_read_image(png_ptr);
    
    // Get unknown chunks
    png_unknown_chunkpp chunks;
    int num_chunks = png_get_unknown_chunks(png_ptr, info_ptr, chunks);
    
    // Get int32 from input buffer
    png_int_32 value = png_get_int_32(data);
    
    // Release resources
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}