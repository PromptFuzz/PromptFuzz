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
//<ID> 212
//<Prompt> ["png_set_gAMA","png_get_mem_ptr","png_set_longjmp_fn","png_get_cHRM","png_longjmp"]
/*<Combination>: [void png_set_gAMA(png_const_structrp png_ptr, png_inforp info_ptr, double file_gamma),
    png_voidp png_get_mem_ptr(png_const_structrp png_ptr),
    jmp_buf *png_set_longjmp_fn(png_structrp png_ptr, png_longjmp_ptr longjmp_fn, size_t jmp_buf_size),
    png_uint_32 png_get_cHRM(png_const_structrp png_ptr, png_const_inforp info_ptr, double * white_x, double * white_y, double * red_x, double * red_y, double * green_x, double * green_y, double * blue_x, double * blue_y),
    void png_longjmp(png_const_structrp png_ptr, int val)
*/
//<score> 32, nr_unique_branch: 7
//<Quality> {"density":8,"unique_branches":{"png_set_longjmp_fn":[[613,8,613,36,0,0,4,1],[636,11,636,20,0,0,4,0],[639,14,639,61,0,0,4,1],[651,11,651,31,0,0,4,1]],"png_get_mem_ptr":[[281,8,281,23,0,0,4,1]],"png_set_gAMA_fixed":[[185,8,185,23,0,0,4,1],[185,27,185,43,0,0,4,1]]},"library_calls":["png_create_read_struct","png_create_info_struct","png_destroy_read_struct","png_set_longjmp_fn","png_destroy_read_struct","png_set_gAMA","png_get_mem_ptr","png_set_longjmp_fn","png_destroy_read_struct"],"critical_calls":["png_create_read_struct","png_create_info_struct","png_set_longjmp_fn","png_set_gAMA","png_get_mem_ptr","png_set_longjmp_fn","png_destroy_read_struct"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_2(const uint8_t* f_data, size_t f_size) {
	if(f_size<13) return 0;

	FUZZ_FILE_INIT();
	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    // Step 4: Create FILE variables for input and output data
    FILE *in_file = fmemopen((void *)data, size, "rb");
	FUZZ_FILENAME_PUSH("output_file")
	FUZZ_FILE_PUSH(in_file)
    FILE *out_file = fopen("output_file", "wb");
	FUZZ_FILE_PUSH(out_file)
    
    // Step 5: Get file descriptors for input and output data
    int in_fd = fuzz_fileno(in_file);
	FUZZ_FD_PUSH(in_fd)
    int out_fd = fuzz_fileno(out_file);
	FUZZ_FILENAME_PUSH("input_file")
	FUZZ_FD_PUSH(out_fd)
    
    // Step 6: Define file names as strings
    const char *input_file = "input_file";
	FUZZ_FILENAME_PUSH("output_file")
    const char *output_file = "output_file";
    
    // Step 1: Set gamma value in PNG
    png_structp png_ptr = png_create_read_struct(fuzz_str_1, NULL, NULL, NULL);
    if (png_ptr == NULL) {
        // Error handling
        std::cerr << "Error creating PNG read struct" << std::endl;
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
    
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == NULL) {
        // Error handling
        std::cerr << "Error creating PNG info struct" << std::endl;
        png_destroy_read_struct(&png_ptr, NULL, NULL);
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
    
    if (setjmp(png_jmpbuf(png_ptr))) {
	FUZZ_FILE_CLOSE();
        // Error handling
        std::cerr << "Error setting longjmp function" << std::endl;
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
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
    
    double gamma = 2.2;
    png_set_gAMA(png_ptr, info_ptr, gamma);
    
    // Step 2: Get memory pointer from PNG
    png_voidp mem_ptr = png_get_mem_ptr(png_ptr);
    
    // Step 3: Set longjmp function in PNG
    jmp_buf jmpbuf;
    png_set_longjmp_fn(png_ptr, (png_longjmp_ptr)longjmp, sizeof(jmp_buf));
    
    // Step 7: Release resources
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
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