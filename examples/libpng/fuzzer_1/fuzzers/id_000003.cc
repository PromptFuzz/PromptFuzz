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
//<ID> 252
//<Prompt> ["png_set_cHRM_XYZ_fixed","png_process_data","png_set_cHRM_fixed","png_image_free","png_permit_mng_features","png_set_compression_strategy","png_set_swap","png_get_eXIf","png_set_invert_mono"]
/*<Combination>: [void png_set_cHRM_XYZ_fixed(png_const_structrp png_ptr, png_inforp info_ptr, png_fixed_point int_red_X, png_fixed_point int_red_Y, png_fixed_point int_red_Z, png_fixed_point int_green_X, png_fixed_point int_green_Y, png_fixed_point int_green_Z, png_fixed_point int_blue_X, png_fixed_point int_blue_Y, png_fixed_point int_blue_Z),
    void png_process_data(png_structrp png_ptr, png_inforp info_ptr, png_bytep buffer, size_t buffer_size),
    void png_set_cHRM_fixed(png_const_structrp png_ptr, png_inforp info_ptr, png_fixed_point int_white_x, png_fixed_point int_white_y, png_fixed_point int_red_x, png_fixed_point int_red_y, png_fixed_point int_green_x, png_fixed_point int_green_y, png_fixed_point int_blue_x, png_fixed_point int_blue_y),
    void png_image_free(png_imagep image),
    png_uint_32 png_permit_mng_features(png_structrp png_ptr, png_uint_32 mng_features_permitted),
    void png_set_compression_strategy(png_structrp png_ptr, int strategy),
    void png_set_swap(png_structrp png_ptr),
    png_uint_32 png_get_eXIf(png_const_structrp png_ptr, png_inforp info_ptr, png_bytep * exif),
    void png_set_invert_mono(png_structrp png_ptr)
*/
//<score> 9, nr_unique_branch: 9
//<Quality> {"density":1,"unique_branches":{"png_set_cHRM_XYZ_fixed":[[80,8,80,23,0,0,4,0]],"png_set_invert_mono":[[254,8,254,23,0,0,4,0]],"png_set_compression_strategy":[[1198,8,1198,23,0,0,4,0]],"png_warning":[[219,8,219,23,0,0,4,1],[234,8,234,23,0,0,4,1]],"png_set_cHRM_fixed":[[49,8,49,23,0,0,4,0]],"png_process_data":[[39,8,39,23,0,0,4,0]],"png_set_swap":[[39,8,39,23,0,0,4,0]],"png_permit_mng_features":[[1324,8,1324,23,0,0,4,0]]},"library_calls":["png_set_cHRM_XYZ_fixed","png_process_data","png_set_cHRM_fixed","png_image_free","png_permit_mng_features","png_set_compression_strategy","png_set_swap","png_get_eXIf","png_set_invert_mono"],"critical_calls":["png_set_cHRM_XYZ_fixed","png_process_data","png_set_cHRM_fixed","png_image_free","png_permit_mng_features","png_set_compression_strategy","png_set_swap","png_get_eXIf","png_set_invert_mono"],"visited":0}
/*Here is the C++ program that implements the steps mentioned above:

*/


extern "C" int LLVMFuzzerTestOneInput_3(const uint8_t* f_data, size_t f_size) {
	if(f_size<84) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_7, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_8, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_9, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_10, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_11, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_12, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_13, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_14, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_15, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_16, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_17, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_18, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_19, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_20, fdp);
	//fuzzer shim end}




  // Step 4: Open the input file
  FILE *in_file = fmemopen((void *)data, size, "rb");
  if (in_file == NULL) {
    assert_file_closed(&in_file);
	return 0;
  }

  // Step 5: Get the file descriptor for reading
  int in_fd = fuzz_fileno(in_file);

  // Step 6: Set the output file name
  const char *out_file_name = "output_file";

  // Step 1: Use libpng APIs to achieve the desired event
  png_image image;
  memset(&image, 0, sizeof(image));

  // Step 2: Call libpng APIs
  png_set_cHRM_XYZ_fixed(NULL, NULL, fuzz_int32_t_1, fuzz_int32_t_2, fuzz_int32_t_3, fuzz_int32_t_4, fuzz_int32_t_5, fuzz_int32_t_6, fuzz_int32_t_7, fuzz_int32_t_8, fuzz_int32_t_9);
  png_process_data(NULL, NULL, NULL, fuzz_uint64_t_10);
  png_set_cHRM_fixed(NULL, NULL, fuzz_int32_t_11, fuzz_int32_t_12, fuzz_int32_t_13, fuzz_int32_t_14, fuzz_int32_t_15, fuzz_int32_t_16, fuzz_int32_t_17, fuzz_int32_t_18);
  png_image_free(&image);
  png_permit_mng_features(NULL, fuzz_uint32_t_19);
  png_set_compression_strategy(NULL, fuzz_int32_t_20);
  png_set_swap(NULL);
  png_get_eXIf(NULL, NULL, NULL);
  png_set_invert_mono(NULL);

  // Step 7: Release resources
  assert_file_closed(&in_file);;

  assert_file_name_closed("output_file");
	assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
}