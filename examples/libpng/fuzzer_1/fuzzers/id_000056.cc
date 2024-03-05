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
//<ID> 2761
//<Prompt> ["png_get_cHRM","png_save_uint_32","png_get_bit_depth","png_create_info_struct","png_save_int_32","png_get_pHYs_dpi"]
/*<Combination>: [png_uint_32 png_get_cHRM(png_const_structrp png_ptr, png_const_inforp info_ptr, double * white_x, double * white_y, double * red_x, double * red_y, double * green_x, double * green_y, double * blue_x, double * blue_y),
    void png_save_uint_32(png_bytep buf, png_uint_32 i),
    png_byte png_get_bit_depth(png_const_structrp png_ptr, png_const_inforp info_ptr),
    png_infop png_create_info_struct(png_const_structrp png_ptr),
    void png_save_int_32(png_bytep buf, png_int_32 i),
    png_uint_32 png_get_pHYs_dpi(png_const_structrp png_ptr, png_const_inforp info_ptr, png_uint_32 * res_x, png_uint_32 * res_y, int * unit_type)
*/
//<score> 30, nr_unique_branch: 16
//<Quality> {"density":10,"unique_branches":{"png_get_cHRM":[[534,7,534,72,0,0,4,0],[538,11,538,26,0,0,4,0],[541,11,541,26,0,0,4,0],[544,11,544,24,0,0,4,0],[547,11,547,24,0,0,4,0],[550,11,550,26,0,0,4,0],[553,11,553,26,0,0,4,0],[556,11,556,25,0,0,4,0],[559,11,559,25,0,0,4,0]],"png_get_pHYs_dpi":[[438,8,438,46,0,0,4,0],[442,11,442,24,0,0,4,0],[448,11,448,24,0,0,4,0],[454,11,454,28,0,0,4,0],[459,14,459,29,0,0,4,0],[461,17,461,30,0,0,4,0],[462,17,462,30,0,0,4,0]]},"library_calls":["png_create_read_struct","png_create_info_struct","png_set_longjmp_fn","png_destroy_read_struct","png_init_io","png_read_png","png_get_cHRM","png_save_uint_32","png_get_pHYs_dpi","png_get_bit_depth","png_destroy_read_struct"],"critical_calls":["png_create_read_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_read_png","png_get_cHRM","png_save_uint_32","png_get_pHYs_dpi","png_get_bit_depth","png_destroy_read_struct"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_56(const uint8_t* f_data, size_t f_size) {
	if(f_size<4) return 0;

	FUZZ_FILE_INIT();
	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
	//fuzzer shim end}




    // Step 4: Create input file
    FILE *in_file = fmemopen((void *)data, size, "rb");
	FUZZ_FILENAME_PUSH("output_file")
	FUZZ_FILE_PUSH(in_file)
    
    // Step 6: Create output file
    FILE *out_file = fopen("output_file", "wb");
	FUZZ_FILE_PUSH(out_file)
    
    // Step 7: Check if file creation was successful
    if (in_file == nullptr || out_file == nullptr) {
        // Release resources and return
        if (in_file != nullptr) {
            assert_file_closed(&in_file);;
        }
        if (out_file != nullptr) {
            assert_file_closed(&out_file);;
        }
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Step 5: Get file descriptor
    int in_fd = fuzz_fileno(in_file);
	FUZZ_FD_PUSH(in_fd)
    int out_fd = fuzz_fileno(out_file);
	FUZZ_FD_PUSH(out_fd)
    
    // Initialize libpng structs
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    png_infop info_ptr = png_create_info_struct(png_ptr);
    
    // Set error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
	FUZZ_FILE_CLOSE();
        // Error handling
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Set input/output
    png_init_io(png_ptr, in_file);
    
    // Read the PNG image
    png_read_png(png_ptr, info_ptr, fuzz_int32_t_1, nullptr);
    
    // Step 1: Call the required libpng APIs
    double white_x, white_y, red_x, red_y, green_x, green_y, blue_x, blue_y;
    png_uint_32 res_x, res_y;
    int unit_type;
    
    png_get_cHRM(png_ptr, info_ptr, &white_x, &white_y, &red_x, &red_y, &green_x, &green_y, &blue_x, &blue_y);
    png_save_uint_32((png_bytep)&res_x, png_get_pHYs_dpi(png_ptr, info_ptr, &res_x, &res_y, &unit_type));
    
    png_byte bit_depth = png_get_bit_depth(png_ptr, info_ptr);
    
    // Step 2: Use the values obtained from the APIs
    std::cout << "cHRM values: white_x=" << white_x 
              << ", white_y=" << white_y 
              << ", red_x=" << red_x 
              << ", red_y=" << red_y 
              << ", green_x=" << green_x 
              << ", green_y=" << green_y 
              << ", blue_x=" << blue_x 
              << ", blue_y=" << blue_y << std::endl;
    
    std::cout << "pHYs values: res_x=" << res_x 
              << ", res_y=" << res_y 
              << ", unit_type=" << unit_type << std::endl;
    
    std::cout << "bit_depth=" << (int)bit_depth << std::endl;
    
    // Step 3: Consume the input data
    
    // Release resources
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}