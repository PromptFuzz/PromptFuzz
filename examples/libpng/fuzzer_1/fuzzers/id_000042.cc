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
//<ID> 2135
//<Prompt> ["png_permit_mng_features","png_read_info","png_set_cHRM_fixed","png_get_user_transform_ptr","png_set_compression_window_bits","png_get_iCCP"]
/*<Combination>: [png_uint_32 png_permit_mng_features(png_structrp png_ptr, png_uint_32 mng_features_permitted),
    void png_read_info(png_structrp png_ptr, png_inforp info_ptr),
    void png_set_cHRM_fixed(png_const_structrp png_ptr, png_inforp info_ptr, png_fixed_point int_white_x, png_fixed_point int_white_y, png_fixed_point int_red_x, png_fixed_point int_red_y, png_fixed_point int_green_x, png_fixed_point int_green_y, png_fixed_point int_blue_x, png_fixed_point int_blue_y),
    png_voidp png_get_user_transform_ptr(png_const_structrp png_ptr),
    void png_set_compression_window_bits(png_structrp png_ptr, int window_bits),
    png_uint_32 png_get_iCCP(png_const_structrp png_ptr, png_inforp info_ptr, png_charpp name, int * compression_type, png_bytepp profile, png_uint_32 * proflen)
*/
//<score> 36, nr_unique_branch: 2
//<Quality> {"density":12,"unique_branches":{"png_check_IHDR":[[2668,8,2668,44,0,0,4,0]],"png_XYZ_from_xy":[[1509,8,1509,23,0,0,4,0]]},"library_calls":["png_create_read_struct","png_create_info_struct","png_destroy_read_struct","png_permit_mng_features","png_set_longjmp_fn","png_destroy_read_struct","png_init_io","png_read_info","png_set_cHRM_fixed","png_get_user_transform_ptr","png_set_compression_window_bits","png_get_iCCP","png_destroy_read_struct"],"critical_calls":["png_create_read_struct","png_create_info_struct","png_permit_mng_features","png_set_longjmp_fn","png_init_io","png_read_info","png_set_cHRM_fixed","png_get_user_transform_ptr","png_set_compression_window_bits","png_get_iCCP","png_destroy_read_struct"],"visited":0}
/**/

extern "C" int LLVMFuzzerTestOneInput_42(const uint8_t* f_data, size_t f_size) {
	if(f_size<53) return 0;

	FUZZ_FILE_INIT();
	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_7, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_8, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_9, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_10, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_11, fdp);
	//fuzzer shim end}




  // Step 4: Create an input file using fmemopen
  FILE *in_file = fmemopen((void *)data, size, "rb");
	FUZZ_FILE_PUSH(in_file)
  if (!in_file) {
    assert_file_closed(&in_file);
	return 0;
  }

  // Step 5: Get the file descriptor for reading
  int in_file_descriptor = fuzz_fileno(in_file);
	FUZZ_FILENAME_PUSH("output_file.png")
	FUZZ_FD_PUSH(in_file_descriptor)

  // Step 6: Create an output file
  FILE *out_file = fopen("output_file.png", "wb");
	FUZZ_FILE_PUSH(out_file)
  if (!out_file) {
    assert_file_closed(&in_file);;
    assert_file_name_closed("output_file.png");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(in_file_descriptor);
	return 0;
  }

  // Step 7: Get the file descriptor for writing
  int out_file_descriptor = fuzz_fileno(out_file);
	FUZZ_FD_PUSH(out_file_descriptor)

  // Initialize libpng structures
  png_structp png_ptr = png_create_read_struct(fuzz_str_1, NULL, NULL, NULL);
  if (!png_ptr) {
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    assert_file_name_closed("output_file.png");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_file_descriptor);
	assert_fd_closed(in_file_descriptor);
	return 0;
  }

  png_infop info_ptr = png_create_info_struct(png_ptr);
  if (!info_ptr) {
    png_destroy_read_struct(&png_ptr, NULL, NULL);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    assert_file_name_closed("output_file.png");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_file_descriptor);
	assert_fd_closed(in_file_descriptor);
	return 0;
  }

  // Step 1: png_permit_mng_features
  png_permit_mng_features(png_ptr, fuzz_uint32_t_2);

  // Set up error handling
  if (setjmp(png_jmpbuf(png_ptr))) {
	FUZZ_FILE_CLOSE();
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    assert_file_name_closed("output_file.png");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_file_descriptor);
	assert_fd_closed(in_file_descriptor);
	return 0;
  }

  // Step 2: png_read_info
  png_init_io(png_ptr, in_file);
  png_read_info(png_ptr, info_ptr);

  // Step 3: png_set_cHRM_fixed
  png_fixed_point white_x = 50000;
  png_fixed_point white_y = 50000;
  png_fixed_point red_x = 64000;
  png_fixed_point red_y = 33000;
  png_fixed_point green_x = 30000;
  png_fixed_point green_y = 60000;
  png_fixed_point blue_x = 15000;
  png_fixed_point blue_y = 6000;
  png_set_cHRM_fixed(png_ptr, info_ptr, fuzz_int32_t_3, fuzz_int32_t_4, fuzz_int32_t_5, fuzz_int32_t_6, fuzz_int32_t_7, fuzz_int32_t_8, fuzz_int32_t_9, fuzz_int32_t_10);

  // Step 4: png_get_user_transform_ptr
  png_voidp user_transform_ptr = png_get_user_transform_ptr(png_ptr);
  
  // Step 5: png_set_compression_window_bits
  int window_bits = 9;
  png_set_compression_window_bits(png_ptr, fuzz_int32_t_11);

  // Step 6: png_get_iCCP
  png_charpp name;
  int compression_type;
  png_bytepp profile;
  png_uint_32 proflen;
  png_get_iCCP(png_ptr, info_ptr, name, &compression_type, profile, &proflen);

  // Clean up
  png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
  assert_file_closed(&in_file);;
  assert_file_closed(&out_file);;

  assert_file_name_closed("output_file.png");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_file_descriptor);
	assert_fd_closed(in_file_descriptor);
	return 0;
}