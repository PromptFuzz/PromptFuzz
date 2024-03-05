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
//<ID> 3152
//<Prompt> ["png_set_gAMA_fixed","png_set_IHDR","png_get_cHRM_XYZ","png_set_sCAL_fixed","png_get_chunk_malloc_max","png_read_update_info","png_set_compression_strategy","png_destroy_read_struct"]
/*<Combination>: [void png_set_gAMA_fixed(png_const_structrp png_ptr, png_inforp info_ptr, png_fixed_point int_file_gamma),
    void png_set_IHDR(png_const_structrp png_ptr, png_inforp info_ptr, png_uint_32 width, png_uint_32 height, int bit_depth, int color_type, int interlace_method, int compression_method, int filter_method),
    png_uint_32 png_get_cHRM_XYZ(png_const_structrp png_ptr, png_const_inforp info_ptr, double * red_X, double * red_Y, double * red_Z, double * green_X, double * green_Y, double * green_Z, double * blue_X, double * blue_Y, double * blue_Z),
    void png_set_sCAL_fixed(png_const_structrp png_ptr, png_inforp info_ptr, int unit, png_fixed_point width, png_fixed_point height),
    png_alloc_size_t png_get_chunk_malloc_max(png_const_structrp png_ptr),
    void png_read_update_info(png_structrp png_ptr, png_inforp info_ptr),
    void png_set_compression_strategy(png_structrp png_ptr, int strategy),
    void png_destroy_read_struct(png_structpp png_ptr_ptr, png_infopp info_ptr_ptr, png_infopp end_info_ptr_ptr)
*/
//<score> 33, nr_unique_branch: 2
//<Quality> {"density":11,"unique_branches":{"png_colorspace_set_gamma":[[1130,8,1130,17,0,0,4,0]],"png_set_compression_strategy":[[1198,8,1198,23,0,0,4,1]]},"library_calls":["png_create_read_struct","png_create_info_struct","png_init_io","png_read_update_info","png_set_gAMA_fixed","png_set_IHDR","png_get_cHRM_XYZ","png_set_sCAL_fixed","png_get_chunk_malloc_max","png_set_compression_strategy","png_destroy_read_struct","png_destroy_read_struct"],"critical_calls":["png_create_read_struct","png_create_info_struct","png_init_io","png_read_update_info","png_set_gAMA_fixed","png_set_IHDR","png_get_cHRM_XYZ","png_set_sCAL_fixed","png_get_chunk_malloc_max","png_set_compression_strategy","png_destroy_read_struct"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_64(const uint8_t* f_data, size_t f_size) {
	if(f_size<17) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
	//fuzzer shim end}




    // Declare necessary variables
    png_structp png_ptr = nullptr;
    png_infop info_ptr = nullptr;
    FILE *in_file = nullptr;
    FILE *out_file = nullptr;
    jmp_buf jmpbuf;
    int result = setjmp(jmpbuf);

    if (result == 0) {
        // Step 1: Create the input file using fmemopen
        in_file = fmemopen((void *)data, size, "rb");
        
        // Step 2: Create the output file
        out_file = fopen("output_file", "wb");

        // Step 3: Create the PNG read and info structures
        png_ptr = png_create_read_struct(fuzz_str_1, nullptr, nullptr, nullptr);
        if (!png_ptr) {
            longjmp(jmpbuf, 1);
        }

        info_ptr = png_create_info_struct(png_ptr);
        if (!info_ptr) {
            longjmp(jmpbuf, 1);
        }

        // Step 4: Initialize the PNG IO
        png_init_io(png_ptr, in_file);

        // Step 5: Read the PNG image and update the info
        png_read_update_info(png_ptr, info_ptr);

        // Step 6: Set the required PNG attributes
        png_set_gAMA_fixed(png_ptr, info_ptr, 1.0);
        png_set_IHDR(png_ptr, info_ptr, 100, 100, 8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE,
                     PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
        double red_X, red_Y, red_Z, green_X, green_Y, green_Z, blue_X, blue_Y, blue_Z;
        png_get_cHRM_XYZ(png_ptr, info_ptr, &red_X, &red_Y, &red_Z, &green_X, &green_Y, &green_Z,
                         &blue_X, &blue_Y, &blue_Z);
        png_set_sCAL_fixed(png_ptr, info_ptr, PNG_SCALE_METER, 1.0, 1.0);
        png_alloc_size_t malloc_max = png_get_chunk_malloc_max(png_ptr);

        // Step 7: Set the compression strategy
        png_set_compression_strategy(png_ptr, fuzz_int32_t_2);

        // Step 8: Destroy the PNG read and info structures
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);

        // Step 9: Close the input and output files
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
    }
    else {
        // Handle error
        if (png_ptr != nullptr) {
            png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        }
        if (in_file != nullptr) {
            assert_file_closed(&in_file);;
        }
        if (out_file != nullptr) {
            assert_file_closed(&out_file);;
        }
    }

    return 0;
}