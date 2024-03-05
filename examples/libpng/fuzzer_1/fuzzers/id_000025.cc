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
//<ID> 1531
//<Prompt> ["png_get_pHYs_dpi","png_set_text_compression_window_bits","png_set_flush","png_start_read_image","png_set_user_limits","png_get_x_offset_microns","png_image_free","png_set_sCAL_fixed","png_get_gAMA"]
/*<Combination>: [png_uint_32 png_get_pHYs_dpi(png_const_structrp png_ptr, png_const_inforp info_ptr, png_uint_32 * res_x, png_uint_32 * res_y, int * unit_type),
    void png_set_text_compression_window_bits(png_structrp png_ptr, int window_bits),
    void png_set_flush(png_structrp png_ptr, int nrows),
    void png_start_read_image(png_structrp png_ptr),
    void png_set_user_limits(png_structrp png_ptr, png_uint_32 user_width_max, png_uint_32 user_height_max),
    png_int_32 png_get_x_offset_microns(png_const_structrp png_ptr, png_const_inforp info_ptr),
    void png_image_free(png_imagep image),
    void png_set_sCAL_fixed(png_const_structrp png_ptr, png_inforp info_ptr, int unit, png_fixed_point width, png_fixed_point height),
    png_uint_32 png_get_gAMA(png_const_structrp png_ptr, png_const_inforp info_ptr, double * file_gamma)
*/
//<score> 75, nr_unique_branch: 20
//<Quality> {"density":15,"unique_branches":{"png_set_sCAL_fixed":[[522,8,522,18,0,0,4,1],[525,13,525,24,0,0,4,1]],"png_ascii_from_fixed":[[3239,8,3239,17,0,0,4,0],[3244,11,3244,17,0,0,4,1],[3251,11,3251,28,0,0,4,0],[3256,17,3256,20,0,0,4,0],[3256,17,3256,20,0,0,4,1],[3265,17,3265,28,0,0,4,0],[3265,32,3265,39,0,0,4,0],[3270,14,3270,25,0,0,4,0],[3272,20,3272,31,0,0,4,1],[3277,17,3277,27,0,0,4,0],[3285,23,3285,34,0,0,4,0],[3285,23,3285,34,0,0,4,1],[3289,23,3289,39,0,0,4,0],[3289,23,3289,39,0,0,4,1]],"png_get_pHYs_dpi":[[437,8,437,23,0,0,4,0],[437,27,437,43,0,0,4,0],[438,8,438,46,0,0,4,1]],"png_check_fp_number":[[2754,19,2754,50,0,0,4,1]]},"library_calls":["png_create_read_struct","png_create_info_struct","png_destroy_read_struct","png_set_longjmp_fn","png_destroy_read_struct","png_init_io","png_set_write_fn","png_get_pHYs_dpi","png_set_text_compression_window_bits","png_set_flush","png_start_read_image","png_set_user_limits","png_get_x_offset_microns","png_image_free","png_set_sCAL_fixed","png_get_gAMA","png_destroy_read_struct"],"critical_calls":["png_create_read_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_set_write_fn","png_get_pHYs_dpi","png_set_text_compression_window_bits","png_set_flush","png_start_read_image","png_set_user_limits","png_get_x_offset_microns","png_image_free","png_set_sCAL_fixed","png_get_gAMA","png_destroy_read_struct"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_25(const uint8_t* f_data, size_t f_size) {
	if(f_size<41) return 0;

	FUZZ_FILE_INIT();
	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_4, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_7, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_8, fdp);
	//fuzzer shim end}




    // Step 4: open input file using fmemopen
    FILE *in_file = fmemopen((void *)data, size, "rb");
	FUZZ_FILE_PUSH(in_file)
    if (in_file == NULL) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Step 5: get file descriptor for reading
    int in_fd = fuzz_fileno(in_file);
	FUZZ_FILENAME_PUSH("output_file")
	FUZZ_FD_PUSH(in_fd)

    // Step 6: create output file
    FILE *out_file = fopen("output_file", "wb");
	FUZZ_FILE_PUSH(out_file)
    if (out_file == NULL) {
        assert_file_closed(&in_file);;
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
    }

    // Step 7: get file descriptor for writing
    int out_fd = fuzz_fileno(out_file);
	FUZZ_FD_PUSH(out_fd)

    // Create libpng structures
    png_structp png_ptr = png_create_read_struct(fuzz_str_1, NULL, NULL, NULL);
    if (png_ptr == NULL) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == NULL) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }

    // Setup libpng error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
	FUZZ_FILE_CLOSE();
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }

    // Set input/output streams
    png_init_io(png_ptr, in_file);
    png_set_write_fn(png_ptr, &out_fd, NULL, NULL);

    // Call libpng APIs
    png_uint_32 res_x, res_y;
    int unit_type;
    png_get_pHYs_dpi(png_ptr, info_ptr, &res_x, &res_y, &unit_type);
    png_set_text_compression_window_bits(png_ptr, fuzz_int32_t_2);
    png_set_flush(png_ptr, fuzz_int32_t_3);
    png_start_read_image(png_ptr);
    png_set_user_limits(png_ptr, fuzz_uint32_t_4, fuzz_uint32_t_5);
    png_int_32 x_offset = png_get_x_offset_microns(png_ptr, info_ptr);
    png_image image;
    memset(&image, 0, sizeof(image));
    png_image_free(&image);
    png_set_sCAL_fixed(png_ptr, info_ptr, fuzz_int32_t_6, fuzz_int32_t_7, fuzz_int32_t_8);
    double file_gamma;
    png_get_gAMA(png_ptr, info_ptr, &file_gamma);

    // Release resources
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

    assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}