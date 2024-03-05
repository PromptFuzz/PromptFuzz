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
//<ID> 4618
//<Prompt> ["png_save_uint_32","png_get_sPLT","png_get_pHYs","png_set_cHRM_XYZ","png_process_data"]
/*<Combination>: [void png_save_uint_32(png_bytep buf, png_uint_32 i),
    int png_get_sPLT(png_const_structrp png_ptr, png_inforp info_ptr, png_sPLT_tpp entries),
    png_uint_32 png_get_pHYs(png_const_structrp png_ptr, png_const_inforp info_ptr, png_uint_32 * res_x, png_uint_32 * res_y, int * unit_type),
    void png_set_cHRM_XYZ(png_const_structrp png_ptr, png_inforp info_ptr, double red_X, double red_Y, double red_Z, double green_X, double green_Y, double green_Z, double blue_X, double blue_Y, double blue_Z),
    void png_process_data(png_structrp png_ptr, png_inforp info_ptr, png_bytep buffer, size_t buffer_size)
*/
//<score> 18, nr_unique_branch: 3
//<Quality> {"density":9,"unique_branches":{"png_get_sPLT":[[778,8,778,23,0,0,4,0],[778,27,778,43,0,0,4,0],[778,47,778,64,0,0,4,0]]},"library_calls":["png_create_read_struct","png_create_info_struct","png_destroy_read_struct","png_set_longjmp_fn","png_destroy_read_struct","png_init_io","png_read_info","png_save_uint_32","png_get_sPLT","png_get_pHYs","png_destroy_read_struct"],"critical_calls":["png_create_read_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_read_info","png_save_uint_32","png_get_sPLT","png_get_pHYs","png_destroy_read_struct"],"visited":0}
/**/



extern "C" int LLVMFuzzerTestOneInput_97(const uint8_t* f_data, size_t f_size) {
	if(f_size<17) return 0;

	FUZZ_FILE_INIT();
	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





  // Step 4: Reading input data from a memory buffer
  FILE *in_file = fmemopen((void *)data, size, "rb");
	FUZZ_FILE_PUSH(in_file)
  if (!in_file) {
    assert_file_closed(&in_file);
	return 0;
  }
	FUZZ_FILENAME_PUSH("input_file")

  // Step 6: Specifying the input file name
  const char *input_file = "input_file";

  // Initialize libpng structures
  png_structp png_ptr = png_create_read_struct(fuzz_str_1, nullptr, nullptr, nullptr);
  if (!png_ptr) {
    assert_file_closed(&in_file);;
    assert_file_name_closed("input_file");
	assert_file_closed(&in_file);
	return 0;
  }

  png_infop info_ptr = png_create_info_struct(png_ptr);
  if (!info_ptr) {
    png_destroy_read_struct(&png_ptr, nullptr, nullptr);
    assert_file_closed(&in_file);;
    assert_file_name_closed("input_file");
	assert_file_closed(&in_file);
	return 0;
  }

  if (setjmp(png_jmpbuf(png_ptr))) {
	FUZZ_FILE_CLOSE();
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    assert_file_closed(&in_file);;
    assert_file_name_closed("input_file");
	assert_file_closed(&in_file);
	return 0;
  }

  // Step 3: Reading the input data using libpng
  png_init_io(png_ptr, in_file);
  png_read_info(png_ptr, info_ptr);

  // Step 1: Calling png_save_uint_32 API
  png_byte buf[4];
  png_save_uint_32(buf, fuzz_uint32_t_2);

  // Step 2: Calling png_get_sPLT API
  png_sPLT_t *entries;
  int num_entries = png_get_sPLT(png_ptr, info_ptr, &entries);
  if (num_entries > 0) {
    // Process the sPLT entries
  }

  // Step 5: Getting physical resolution using png_get_pHYs API
  png_uint_32 res_x, res_y;
  int unit_type;
  png_get_pHYs(png_ptr, info_ptr, &res_x, &res_y, &unit_type);

  // Step 7: Cleaning up resources
  png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
  assert_file_closed(&in_file);;

  assert_file_name_closed("input_file");
	assert_file_closed(&in_file);
	return 0;
}