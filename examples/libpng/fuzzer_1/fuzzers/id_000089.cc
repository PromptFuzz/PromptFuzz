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
//<ID> 4188
//<Prompt> ["png_set_pCAL","png_set_read_status_fn","png_get_y_offset_inches","png_set_packswap","png_get_image_width","png_set_cHRM_XYZ_fixed","png_get_cHRM_XYZ_fixed","png_set_text_compression_strategy"]
/*<Combination>: [void png_set_pCAL(png_const_structrp png_ptr, png_inforp info_ptr, png_const_charp purpose, png_int_32 X0, png_int_32 X1, int type, int nparams, png_const_charp units, png_charpp params),
    void png_set_read_status_fn(png_structrp png_ptr, png_read_status_ptr read_row_fn),
    float png_get_y_offset_inches(png_const_structrp png_ptr, png_const_inforp info_ptr),
    void png_set_packswap(png_structrp png_ptr),
    png_uint_32 png_get_image_width(png_const_structrp png_ptr, png_const_inforp info_ptr),
    void png_set_cHRM_XYZ_fixed(png_const_structrp png_ptr, png_inforp info_ptr, png_fixed_point int_red_X, png_fixed_point int_red_Y, png_fixed_point int_red_Z, png_fixed_point int_green_X, png_fixed_point int_green_Y, png_fixed_point int_green_Z, png_fixed_point int_blue_X, png_fixed_point int_blue_Y, png_fixed_point int_blue_Z),
    png_uint_32 png_get_cHRM_XYZ_fixed(png_const_structrp png_ptr, png_const_inforp info_ptr, png_fixed_point * int_red_X, png_fixed_point * int_red_Y, png_fixed_point * int_red_Z, png_fixed_point * int_green_X, png_fixed_point * int_green_Y, png_fixed_point * int_green_Z, png_fixed_point * int_blue_X, png_fixed_point * int_blue_Y, png_fixed_point * int_blue_Z),
    void png_set_text_compression_strategy(png_structrp png_ptr, int strategy)
*/
//<score> 24, nr_unique_branch: 1
//<Quality> {"density":12,"unique_branches":{"png_set_pCAL":[[314,12,314,23,0,0,4,1]]},"library_calls":["png_create_read_struct","png_create_info_struct","png_set_longjmp_fn","png_destroy_read_struct","png_set_pCAL","png_set_read_status_fn","png_get_y_offset_inches","png_set_packswap","png_get_image_width","png_set_cHRM_XYZ_fixed","png_get_cHRM_XYZ_fixed","png_set_text_compression_strategy","png_destroy_read_struct"],"critical_calls":["png_create_read_struct","png_create_info_struct","png_set_longjmp_fn","png_set_pCAL","png_set_read_status_fn","png_get_y_offset_inches","png_set_packswap","png_get_image_width","png_set_cHRM_XYZ_fixed","png_get_cHRM_XYZ_fixed","png_set_text_compression_strategy","png_destroy_read_struct"],"visited":0}
/**/

extern "C" int LLVMFuzzerTestOneInput_89(const uint8_t* f_data, size_t f_size) {
	if(f_size<38) return 0;

	FUZZ_FILE_INIT();
	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_7, fdp);
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    // Step 4: Create file pointers for reading and writing
    FILE *in_file = fmemopen((void *)data, size, "rb");
	FUZZ_FILENAME_PUSH("output_file")
	FUZZ_FILE_PUSH(in_file)
    FILE *out_file = fopen("output_file", "wb");
	FUZZ_FILE_PUSH(out_file)

    if (!in_file || !out_file) {
        // Error handling
        if (in_file) assert_file_closed(&in_file);;
        if (out_file) assert_file_closed(&out_file);;
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Step 5: Get file descriptors
    int in_fd = fuzz_fileno(in_file);
	FUZZ_FD_PUSH(in_fd)
    int out_fd = fuzz_fileno(out_file);
	FUZZ_FILENAME_PUSH("input_file")
	FUZZ_FD_PUSH(out_fd)

    // Step 6: Define file names
    const char* input_file = "input_file";
	FUZZ_FILENAME_PUSH("output_file")
    const char* output_file = "output_file";

    // Step 1: Use libpng APIs
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    png_infop info_ptr = png_create_info_struct(png_ptr);

    if (setjmp(png_jmpbuf(png_ptr))) {
	FUZZ_FILE_CLOSE();
        // Error handling
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

    // Step 2: Call PNG APIs
    png_set_pCAL(png_ptr, info_ptr, fuzz_str_1, fuzz_int32_t_3, fuzz_int32_t_4, fuzz_int32_t_5, fuzz_int32_t_6, fuzz_str_2, NULL);
    png_set_read_status_fn(png_ptr, NULL);
    float y_offset = png_get_y_offset_inches(png_ptr, info_ptr);
    png_set_packswap(png_ptr);
    png_uint_32 image_width = png_get_image_width(png_ptr, info_ptr);
    png_fixed_point red_X, red_Y, red_Z, green_X, green_Y, green_Z, blue_X, blue_Y, blue_Z;
    png_set_cHRM_XYZ_fixed(png_ptr, info_ptr, red_X, red_Y, red_Z, green_X, green_Y, green_Z, blue_X, blue_Y, blue_Z);
    png_get_cHRM_XYZ_fixed(png_ptr, info_ptr, &red_X, &red_Y, &red_Z, &green_X, &green_Y, &green_Z, &blue_X, &blue_Y, &blue_Z);
    png_set_text_compression_strategy(png_ptr, fuzz_int32_t_7);

    // Step 7: Clean up resources
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