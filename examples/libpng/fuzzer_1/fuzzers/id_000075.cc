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
//<ID> 3610
//<Prompt> ["png_get_text","png_set_gAMA","png_set_packing","png_get_cHRM_XYZ","png_set_alpha_mode_fixed","png_get_x_offset_pixels"]
/*<Combination>: [int png_get_text(png_const_structrp png_ptr, png_inforp info_ptr, png_textp * text_ptr, int * num_text),
    void png_set_gAMA(png_const_structrp png_ptr, png_inforp info_ptr, double file_gamma),
    void png_set_packing(png_structrp png_ptr),
    png_uint_32 png_get_cHRM_XYZ(png_const_structrp png_ptr, png_const_inforp info_ptr, double * red_X, double * red_Y, double * red_Z, double * green_X, double * green_Y, double * green_Z, double * blue_X, double * blue_Y, double * blue_Z),
    void png_set_alpha_mode_fixed(png_structrp png_ptr, int mode, png_fixed_point output_gamma),
    png_int_32 png_get_x_offset_pixels(png_const_structrp png_ptr, png_const_inforp info_ptr)
*/
//<score> 26, nr_unique_branch: 4
//<Quality> {"density":13,"unique_branches":{"png_get_x_offset_pixels":[[290,27,290,43,0,0,4,0],[291,8,291,46,0,0,4,0],[291,8,291,46,0,0,4,1],[295,11,295,57,0,0,4,1]]},"library_calls":["png_create_read_struct","png_create_info_struct","png_destroy_read_struct","png_set_longjmp_fn","png_destroy_read_struct","png_init_io","png_read_info","png_get_text","png_set_gAMA","png_set_packing","png_get_cHRM_XYZ","png_set_alpha_mode_fixed","png_get_x_offset_pixels","png_destroy_read_struct"],"critical_calls":["png_create_read_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_read_info","png_get_text","png_set_gAMA","png_set_packing","png_get_cHRM_XYZ","png_set_alpha_mode_fixed","png_get_x_offset_pixels","png_destroy_read_struct"],"visited":0}
/**/

extern "C" int LLVMFuzzerTestOneInput_75(const uint8_t* f_data, size_t f_size) {
	if(f_size<21) return 0;

	FUZZ_FILE_INIT();
	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
	//fuzzer shim end}




    FILE *in_file = fmemopen((void *)data, size, "rb");
	FUZZ_FILE_PUSH(in_file)
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    png_structp png_ptr = png_create_read_struct(fuzz_str_1, NULL, NULL, NULL);
    if (!png_ptr) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        assert_file_closed(&in_file);;
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        assert_file_closed(&in_file);
	return 0;
    }

    if (setjmp(png_jmpbuf(png_ptr))) {
	FUZZ_FILE_CLOSE();
        assert_file_closed(&in_file);;
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        assert_file_closed(&in_file);
	return 0;
    }

    png_init_io(png_ptr, in_file);
    png_read_info(png_ptr, info_ptr);

    png_textp text_ptr;
    int num_text;
    png_get_text(png_ptr, info_ptr, &text_ptr, &num_text);
    
    double file_gamma = 2.2;
    png_set_gAMA(png_ptr, info_ptr, file_gamma);
    
    png_set_packing(png_ptr);
    
    double red_X, red_Y, red_Z, green_X, green_Y, green_Z, blue_X, blue_Y, blue_Z;
    png_get_cHRM_XYZ(png_ptr, info_ptr, &red_X, &red_Y, &red_Z, &green_X, &green_Y, &green_Z, &blue_X, &blue_Y, &blue_Z);
    
    int mode = PNG_ALPHA_ASSOCIATED;
    png_fixed_point output_gamma = 10000;
    png_set_alpha_mode_fixed(png_ptr, fuzz_int32_t_2, fuzz_int32_t_3);
    
    png_int_32 x_offset_pixels = png_get_x_offset_pixels(png_ptr, info_ptr);

    assert_file_closed(&in_file);;
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    
    assert_file_closed(&in_file);
	return 0;
}