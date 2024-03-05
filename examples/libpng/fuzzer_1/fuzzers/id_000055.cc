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
//<ID> 2740
//<Prompt> ["png_get_user_height_max","png_get_interlace_type","png_get_x_pixels_per_inch","png_process_data_pause","png_set_sRGB"]
/*<Combination>: [png_uint_32 png_get_user_height_max(png_const_structrp png_ptr),
    png_byte png_get_interlace_type(png_const_structrp png_ptr, png_const_inforp info_ptr),
    png_uint_32 png_get_x_pixels_per_inch(png_const_structrp png_ptr, png_const_inforp info_ptr),
    size_t png_process_data_pause(png_structrp , int save),
    void png_set_sRGB(png_const_structrp png_ptr, png_inforp info_ptr, int srgb_intent)
*/
//<score> 24, nr_unique_branch: 2
//<Quality> {"density":12,"unique_branches":{"png_get_x_pixels_per_meter":[[131,8,131,46,0,0,4,0],[136,14,136,62,0,0,4,0]]},"library_calls":["png_create_read_struct","png_create_info_struct","png_destroy_read_struct","png_set_longjmp_fn","png_destroy_read_struct","png_init_io","png_read_info","png_get_user_height_max","png_get_interlace_type","png_get_x_pixels_per_inch","png_process_data_pause","png_set_sRGB","png_destroy_read_struct"],"critical_calls":["png_create_read_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_read_info","png_get_user_height_max","png_get_interlace_type","png_get_x_pixels_per_inch","png_process_data_pause","png_set_sRGB","png_destroy_read_struct"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_55(const uint8_t* f_data, size_t f_size) {
	if(f_size<21) return 0;

	FUZZ_FILE_INIT();
	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
	//fuzzer shim end}




  // Create FILE pointer for reading input data
  FILE *in_file = fmemopen((void *)data, size, "rb");
	FUZZ_FILE_PUSH(in_file)
  if (!in_file) {
    assert_file_closed(&in_file);
	return 0;
  }
	FUZZ_FILENAME_PUSH("output_file.png")

  // Create FILE pointer for writing output data
  FILE *out_file = fopen("output_file.png", "wb");
	FUZZ_FILE_PUSH(out_file)
  if (!out_file) {
    assert_file_closed(&in_file);;
    assert_file_name_closed("output_file.png");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
  }

  // Create libpng read struct
  png_structp png_ptr = png_create_read_struct(fuzz_str_1, NULL, NULL, NULL);
  if (!png_ptr) {
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    assert_file_name_closed("output_file.png");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
  }

  // Create libpng info struct
  png_infop info_ptr = png_create_info_struct(png_ptr);
  if (!info_ptr) {
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    png_destroy_read_struct(&png_ptr, (png_infopp)NULL, (png_infopp)NULL);
    assert_file_name_closed("output_file.png");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
  }

  // Set up error handling
  if (setjmp(png_jmpbuf(png_ptr))) {
	FUZZ_FILE_CLOSE();
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);
    assert_file_name_closed("output_file.png");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
  }

  // Initialize libpng IO
  png_init_io(png_ptr, in_file);

  // Read PNG file header
  png_read_info(png_ptr, info_ptr);

  // Get user height max
  png_uint_32 user_height_max = png_get_user_height_max(png_ptr);
  std::cout << "User Height Max: " << user_height_max << std::endl;

  // Get interlace type
  png_byte interlace_type = png_get_interlace_type(png_ptr, info_ptr);
  std::cout << "Interlace Type: " << static_cast<int>(interlace_type) << std::endl;

  // Get x pixels per inch
  png_uint_32 x_pixels_per_inch = png_get_x_pixels_per_inch(png_ptr, info_ptr);
  std::cout << "X Pixels Per Inch: " << x_pixels_per_inch << std::endl;

  // Perform custom event - pause processing data
  size_t pause_result = png_process_data_pause(png_ptr, fuzz_int32_t_2);
  std::cout << "Pause Result: " << pause_result << std::endl;

  // Set sRGB
  png_set_sRGB(png_ptr, info_ptr, fuzz_int32_t_3);

  // Clean up
  assert_file_closed(&in_file);;
  assert_file_closed(&out_file);;
  png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);

  assert_file_name_closed("output_file.png");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}