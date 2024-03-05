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
//<ID> 1105
//<Prompt> ["png_set_compression_mem_level","png_set_sRGB_gAMA_and_cHRM","png_progressive_combine_row","png_set_oFFs","png_get_gAMA","png_data_freer","png_get_pHYs"]
/*<Combination>: [void png_set_compression_mem_level(png_structrp png_ptr, int mem_level),
    void png_set_sRGB_gAMA_and_cHRM(png_const_structrp png_ptr, png_inforp info_ptr, int srgb_intent),
    void png_progressive_combine_row(png_const_structrp png_ptr, png_bytep old_row, png_const_bytep new_row),
    void png_set_oFFs(png_const_structrp png_ptr, png_inforp info_ptr, png_int_32 offset_x, png_int_32 offset_y, int unit_type),
    png_uint_32 png_get_gAMA(png_const_structrp png_ptr, png_const_inforp info_ptr, double * file_gamma),
    void png_data_freer(png_const_structrp png_ptr, png_inforp info_ptr, int freer, png_uint_32 mask),
    png_uint_32 png_get_pHYs(png_const_structrp png_ptr, png_const_inforp info_ptr, png_uint_32 * res_x, png_uint_32 * res_y, int * unit_type)
*/
//<score> 90, nr_unique_branch: 12
//<Quality> {"density":15,"unique_branches":{"png_get_gAMA":[[714,7,714,68,0,0,4,0],[715,7,715,25,0,0,4,0]],"png_set_compression_mem_level":[[1187,8,1187,23,0,0,4,1]],"png_progressive_combine_row":[[1061,8,1061,23,0,0,4,1],[1068,8,1068,23,0,0,4,1]],"png_set_sRGB_gAMA_and_cHRM":[[658,8,658,23,0,0,4,1],[658,27,658,43,0,0,4,1],[661,8,662,25,0,0,4,0],[661,8,662,25,0,0,4,1]],"png_data_freer":[[457,8,457,23,0,0,4,1],[457,27,457,43,0,0,4,1],[460,8,460,43,0,0,4,0]]},"library_calls":["png_create_read_struct","png_create_info_struct","png_destroy_read_struct","png_set_longjmp_fn","png_destroy_read_struct","png_destroy_read_struct","png_init_io","png_read_info","png_set_compression_mem_level","png_set_sRGB_gAMA_and_cHRM","png_progressive_combine_row","png_set_oFFs","png_get_gAMA","png_data_freer","png_get_pHYs","png_destroy_read_struct"],"critical_calls":["png_create_read_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_read_info","png_set_compression_mem_level","png_set_sRGB_gAMA_and_cHRM","png_progressive_combine_row","png_set_oFFs","png_get_gAMA","png_data_freer","png_get_pHYs","png_destroy_read_struct"],"visited":0}
/*Here is the C++ program that achieves the event using the libpng APIs:

*/


extern "C" int LLVMFuzzerTestOneInput_16(const uint8_t* f_data, size_t f_size) {
	if(f_size<41) return 0;

	FUZZ_FILE_INIT();
	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_7, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_8, fdp);
	//fuzzer shim end}




    // Step 1: Create a PNG read structure
    png_structp png_ptr = png_create_read_struct(fuzz_str_1, NULL, NULL, NULL);
    if (png_ptr == NULL) {
        return 0;
    }

    // Step 2: Create a PNG info structure
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == NULL) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return 0;
    }

    // Step 3: Set up error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
	FUZZ_FILE_CLOSE();
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return 0;
    }

    // Step 4: Set the input file using a FILE pointer
    FILE *in_file = fmemopen((void *)data, size, "rb");
	FUZZ_FILE_PUSH(in_file)
    if (in_file == NULL) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        assert_file_closed(&in_file);
	return 0;
    }
    png_init_io(png_ptr, in_file);

    // Step 5: Read the PNG header
    png_read_info(png_ptr, info_ptr);

    // Step 6: Set compression memory level
    png_set_compression_mem_level(png_ptr, fuzz_int32_t_2);

    // Step 7: Set sRGB, gAMA, and cHRM values
    png_set_sRGB_gAMA_and_cHRM(png_ptr, info_ptr, fuzz_int32_t_3);

    // Step 8: Combine row data
    png_bytep old_row = NULL; // Replace with actual row data if needed
    png_bytep new_row = NULL; // Replace with actual row data if needed
    png_progressive_combine_row(png_ptr, old_row, new_row);

    // Step 9: Set image offset
    png_set_oFFs(png_ptr, info_ptr, fuzz_int32_t_4, fuzz_int32_t_5, fuzz_int32_t_6);

    // Step 10: Get gAMA value
    double file_gamma;
    png_get_gAMA(png_ptr, info_ptr, &file_gamma);

    // Step 11: Free data
    png_data_freer(png_ptr, info_ptr, fuzz_int32_t_7, fuzz_uint32_t_8);

    // Step 12: Get pHYs value
    png_uint_32 res_x, res_y;
    int unit_type;
    png_get_pHYs(png_ptr, info_ptr, &res_x, &res_y, &unit_type);

    // Step 13: Clean up
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    assert_file_closed(&in_file);;

    assert_file_closed(&in_file);
	return 0;
}