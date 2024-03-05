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
//<ID> 5477
//<Prompt> ["png_set_filter_heuristics_fixed","png_set_cHRM_XYZ","png_set_text_compression_method","png_get_header_version","png_get_sCAL","png_set_tIME","png_convert_to_rfc1123_buffer","png_set_unknown_chunk_location","png_set_interlace_handling"]
/*<Combination>: [void png_set_filter_heuristics_fixed(png_structrp png_ptr, int heuristic_method, int num_weights, png_const_fixed_point_p filter_weights, png_const_fixed_point_p filter_costs),
    void png_set_cHRM_XYZ(png_const_structrp png_ptr, png_inforp info_ptr, double red_X, double red_Y, double red_Z, double green_X, double green_Y, double green_Z, double blue_X, double blue_Y, double blue_Z),
    void png_set_text_compression_method(png_structrp png_ptr, int method),
    png_const_charp png_get_header_version(png_const_structrp png_ptr),
    png_uint_32 png_get_sCAL(png_const_structrp png_ptr, png_const_inforp info_ptr, int * unit, double * width, double * height),
    void png_set_tIME(png_const_structrp png_ptr, png_inforp info_ptr, png_const_timep mod_time),
    int png_convert_to_rfc1123_buffer(char out[29], png_const_timep ptime),
    void png_set_unknown_chunk_location(png_const_structrp png_ptr, png_inforp info_ptr, int chunk, int location),
    int png_set_interlace_handling(png_structrp png_ptr)
*/
//<score> 24, nr_unique_branch: 1
//<Quality> {"density":12,"unique_branches":{"png_convert_to_rfc1123_buffer":[[747,8,747,25,0,0,4,0]]},"library_calls":["png_create_read_struct","png_create_info_struct","png_set_longjmp_fn","png_destroy_read_struct","png_set_filter_heuristics_fixed","png_set_cHRM_XYZ","png_set_text_compression_method","png_get_header_version","png_get_sCAL","png_set_tIME","png_convert_to_rfc1123_buffer","png_set_unknown_chunk_location","png_set_interlace_handling","png_destroy_read_struct"],"critical_calls":["png_create_read_struct","png_create_info_struct","png_set_longjmp_fn","png_set_filter_heuristics_fixed","png_set_cHRM_XYZ","png_set_text_compression_method","png_get_header_version","png_get_sCAL","png_set_tIME","png_convert_to_rfc1123_buffer","png_set_unknown_chunk_location","png_set_interlace_handling","png_destroy_read_struct"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_111(const uint8_t* f_data, size_t f_size) {
	if(f_size<29) return 0;

	FUZZ_FILE_INIT();
	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    // Step 4: Use fmemopen to create a FILE* variable for reading the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
	FUZZ_FILE_PUSH(in_file)
    if (in_file == nullptr) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Step 5: Get the file descriptor for reading the input data
    int in_fd = fuzz_fileno(in_file);
	FUZZ_FILENAME_PUSH("input_file")
	FUZZ_FD_PUSH(in_fd)

    // Step 6: Create a string for the input file name
    const char *in_filename = "input_file";

    // Create a PNG read struct and info struct
    png_structp png_ptr = png_create_read_struct(fuzz_str_1, nullptr, nullptr, nullptr);
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (png_ptr == nullptr || info_ptr == nullptr) {
        assert_file_closed(&in_file);;
        assert_file_name_closed("input_file");
	assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
    }

    // Set up error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
	FUZZ_FILE_CLOSE();
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        assert_file_closed(&in_file);;
        assert_file_name_closed("input_file");
	assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
    }

    // Step 2: Call libpng APIs to achieve the desired event
    png_set_filter_heuristics_fixed(png_ptr, fuzz_int32_t_2, 0, nullptr, nullptr);
    png_set_cHRM_XYZ(png_ptr, info_ptr, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    png_set_text_compression_method(png_ptr, fuzz_int32_t_3);
    png_get_header_version(png_ptr);
    int unit;
    double width, height;
    png_get_sCAL(png_ptr, info_ptr, &unit, &width, &height);
    png_set_tIME(png_ptr, info_ptr, nullptr);
    char out[29];
    png_time mod_time;
    int ret = png_convert_to_rfc1123_buffer(out, &mod_time);
    png_set_unknown_chunk_location(png_ptr, info_ptr, fuzz_int32_t_4, fuzz_int32_t_5);
    png_set_interlace_handling(png_ptr);

    // Step 7: Clean up resources
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    assert_file_closed(&in_file);;

    assert_file_name_closed("input_file");
	assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
}