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
//<ID> 4693
//<Prompt> ["png_set_compression_buffer_size","png_get_hIST","png_get_rows","png_malloc_default","png_get_gAMA","png_set_eXIf","png_set_filter_heuristics","png_set_text_compression_level"]
/*<Combination>: [void png_set_compression_buffer_size(png_structrp png_ptr, size_t size),
    png_uint_32 png_get_hIST(png_const_structrp png_ptr, png_inforp info_ptr, png_uint_16p * hist),
    png_bytepp png_get_rows(png_const_structrp png_ptr, png_const_inforp info_ptr),
    png_voidp png_malloc_default(png_const_structrp png_ptr, png_alloc_size_t size),
    png_uint_32 png_get_gAMA(png_const_structrp png_ptr, png_const_inforp info_ptr, double * file_gamma),
    void png_set_eXIf(png_const_structrp png_ptr, png_inforp info_ptr, png_bytep exif),
    void png_set_filter_heuristics(png_structrp png_ptr, int heuristic_method, int num_weights, png_const_doublep filter_weights, png_const_doublep filter_costs),
    void png_set_text_compression_level(png_structrp png_ptr, int level)
*/
//<score> 13, nr_unique_branch: 0
//<Quality> {"density":13,"unique_branches":{},"library_calls":["png_create_read_struct","png_create_info_struct","png_set_longjmp_fn","png_destroy_read_struct","png_set_compression_buffer_size","png_get_hIST","png_get_rows","png_get_gAMA","png_set_eXIf","png_set_filter_heuristics","png_set_text_compression_level","png_init_io","png_read_png","png_destroy_read_struct"],"critical_calls":["png_create_read_struct","png_create_info_struct","png_set_longjmp_fn","png_set_compression_buffer_size","png_get_hIST","png_get_rows","png_get_gAMA","png_set_eXIf","png_set_filter_heuristics","png_set_text_compression_level","png_init_io","png_read_png","png_destroy_read_struct"],"visited":0}
/*Here is the implementation of the C++ program that follows the given instructions:

*/


extern "C" int LLVMFuzzerTestOneInput_102(const uint8_t* f_data, size_t f_size) {
	if(f_size<82) return 0;

	FUZZ_FILE_INIT();
	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeDoublingArray(fuzz_double_array_1, fuzz_double_array_size_1, fdp);
		FDPConsumeDoublingArray(fuzz_double_array_2, fuzz_double_array_size_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
	
		png_const_doublep fuzzer_var_png_set_filter_heuristics_9_3 = fuzz_double_array_1;
		int fuzzer_size_png_set_filter_heuristics_9_3 = static_cast<int>(fuzz_double_array_size_1);
		png_const_doublep fuzzer_var_png_set_filter_heuristics_9_4 = fuzz_double_array_2;//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    // Step 4: Create a FILE * variable to read the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
	FUZZ_FILENAME_PUSH("input_file")
	FUZZ_FILE_PUSH(in_file)
    
    // Step 6: Define the input file name
    const char *input_file = "input_file";
    
    // Initialize libpng structures and variables
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    png_infop info_ptr = png_create_info_struct(png_ptr);

    // Set error handling if something goes wrong
    if (setjmp(png_jmpbuf(png_ptr))) {
	FUZZ_FILE_CLOSE();
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        assert_file_closed(&in_file);;
        assert_file_name_closed("input_file");
	assert_file_closed(&in_file);
	return 0;
    }

    // Step 5: Get the file descriptor for reading
    int in_fd = fuzz_fileno(in_file);
	FUZZ_FD_PUSH(in_fd)
    
    // Step 2: Call libpng APIs to achieve the desired event
    png_set_compression_buffer_size(png_ptr, size);
    
    png_uint_16p hist;
    png_get_hIST(png_ptr, info_ptr, &hist);
    
    png_bytepp rows = png_get_rows(png_ptr, info_ptr);
    
    png_voidp png_default_malloc(png_const_structrp png_ptr, png_alloc_size_t size);
    
    double file_gamma;
    png_get_gAMA(png_ptr, info_ptr, &file_gamma);
    
    png_bytep exif;
    png_set_eXIf(png_ptr, info_ptr, exif);
    
    double filter_weights[4] = {1.0, 2.0, 3.0, 4.0};
    double filter_costs[4] = {0.1, 0.2, 0.3, 0.4};
    png_set_filter_heuristics(png_ptr, fuzz_int32_t_3, fuzzer_size_png_set_filter_heuristics_9_3, fuzzer_var_png_set_filter_heuristics_9_3, fuzzer_var_png_set_filter_heuristics_9_4);
    
    png_set_text_compression_level(png_ptr, fuzz_int32_t_4);

    // Read the image data from the input file
    png_init_io(png_ptr, in_file);
    png_read_png(png_ptr, info_ptr, fuzz_int32_t_5, NULL);

    // Clean up and release resources
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    assert_file_closed(&in_file);;

    assert_file_name_closed("input_file");
	assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
}