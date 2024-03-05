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
//<ID> 2460
//<Prompt> ["png_get_text","png_set_PLTE","png_get_cHRM_fixed","png_set_sPLT","png_get_y_pixels_per_meter"]
/*<Combination>: [int png_get_text(png_const_structrp png_ptr, png_inforp info_ptr, png_textp * text_ptr, int * num_text),
    void png_set_PLTE(png_structrp png_ptr, png_inforp info_ptr, png_const_colorp palette, int num_palette),
    png_uint_32 png_get_cHRM_fixed(png_const_structrp png_ptr, png_const_inforp info_ptr, png_fixed_point * int_white_x, png_fixed_point * int_white_y, png_fixed_point * int_red_x, png_fixed_point * int_red_y, png_fixed_point * int_green_x, png_fixed_point * int_green_y, png_fixed_point * int_blue_x, png_fixed_point * int_blue_y),
    void png_set_sPLT(png_const_structrp png_ptr, png_inforp info_ptr, png_const_sPLT_tp entries, int nentries),
    png_uint_32 png_get_y_pixels_per_meter(png_const_structrp png_ptr, png_const_inforp info_ptr)
*/
//<score> 42, nr_unique_branch: 16
//<Quality> {"density":14,"unique_branches":{"png_get_y_pixels_per_meter":[[152,27,152,43,0,0,4,0],[153,8,153,46,0,0,4,0],[153,8,153,46,0,0,4,1],[158,11,158,59,0,0,4,0]],"png_get_cHRM_fixed":[[659,8,659,23,0,0,4,0],[659,27,659,43,0,0,4,0],[660,7,660,72,0,0,4,0],[660,7,660,72,0,0,4,1],[662,11,662,26,0,0,4,0],[664,11,664,26,0,0,4,0],[666,11,666,24,0,0,4,0],[668,11,668,24,0,0,4,0],[670,11,670,26,0,0,4,0],[672,11,672,26,0,0,4,0],[674,11,674,25,0,0,4,0],[676,11,676,25,0,0,4,0]]},"library_calls":["png_create_read_struct","png_create_info_struct","png_destroy_read_struct","png_set_longjmp_fn","png_destroy_read_struct","png_destroy_read_struct","png_init_io","png_read_info","png_get_text","png_destroy_read_struct","png_init_io","png_set_PLTE","png_get_cHRM_fixed","png_get_y_pixels_per_meter","png_destroy_read_struct"],"critical_calls":["png_create_read_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_read_info","png_get_text","png_init_io","png_set_PLTE","png_get_cHRM_fixed","png_get_y_pixels_per_meter","png_destroy_read_struct"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_50(const uint8_t* f_data, size_t f_size) {
	if(f_size<13) return 0;

	FUZZ_FILE_INIT();
	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
	//fuzzer shim end}




    // Create png_struct and png_info variables
    png_structp png_ptr = png_create_read_struct(fuzz_str_1, NULL, NULL, NULL);
    if (!png_ptr) {
        return 0;
    }
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return 0;
    }

    // Set error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
	FUZZ_FILE_CLOSE();
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return 0;
    }

    // Set up input file
    FILE *in_file = fmemopen((void *)data, size, "rb");
	FUZZ_FILE_PUSH(in_file)
    if (!in_file) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        assert_file_closed(&in_file);
	return 0;
    }
    png_init_io(png_ptr, in_file);

    // Read PNG header
    png_read_info(png_ptr, info_ptr);

    // Get and print text data
    png_textp text_ptr = NULL;
    int num_text = 0;
    png_get_text(png_ptr, info_ptr, &text_ptr, &num_text);
    for (int i = 0; i < num_text; i++) {
        std::cout << "Text: " << text_ptr[i].text << std::endl;
    }
	FUZZ_FILENAME_PUSH("output_file")

    // Set up output file
    FILE *out_file = fopen("output_file", "wb");
	FUZZ_FILE_PUSH(out_file)
    if (!out_file) {
        assert_file_closed(&in_file);;
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    png_init_io(png_ptr, out_file);

    // Set up PLTE (palette)
    png_color palette[256];
    int num_palette = 256;
    png_set_PLTE(png_ptr, info_ptr, palette, num_palette);

    // Get cHRM (chromaticity) fixed point values
    png_fixed_point int_white_x, int_white_y, int_red_x, int_red_y, int_green_x, int_green_y, int_blue_x, int_blue_y;
    png_get_cHRM_fixed(png_ptr, info_ptr, &int_white_x, &int_white_y, &int_red_x, &int_red_y,
                       &int_green_x, &int_green_y, &int_blue_x, &int_blue_y);

    // Get y pixels per meter value
    png_uint_32 y_pixels_per_meter = png_get_y_pixels_per_meter(png_ptr, info_ptr);

    // Clean up
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}