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
//<ID> 1269
//<Prompt> ["png_build_grayscale_palette","png_process_data_pause","png_set_invert_alpha","png_get_pixels_per_meter","png_write_rows"]
/*<Combination>: [void png_build_grayscale_palette(int bit_depth, png_colorp palette),
    size_t png_process_data_pause(png_structrp , int save),
    void png_set_invert_alpha(png_structrp png_ptr),
    png_uint_32 png_get_pixels_per_meter(png_const_structrp png_ptr, png_const_inforp info_ptr),
    void png_write_rows(png_structrp png_ptr, png_bytepp row, png_uint_32 num_rows)
*/
//<score> 40, nr_unique_branch: 13
//<Quality> {"density":10,"unique_branches":{"png_push_save_buffer":[[466,8,466,38,0,0,4,1],[482,8,483,32,0,0,4,1],[512,8,512,36,0,0,4,1]],"png_process_data_pause":[[53,8,53,23,0,0,4,0],[58,11,58,20,0,0,4,0]],"png_build_grayscale_palette":[[884,8,884,23,0,0,4,1],[889,7,889,13,0,0,4,1],[894,7,894,13,0,0,4,1],[899,7,899,13,0,0,4,1],[904,7,904,13,0,0,4,0],[909,7,909,14,0,0,4,1],[915,23,915,38,0,0,4,0],[915,23,915,38,0,0,4,1]]},"library_calls":["png_create_read_struct","png_create_info_struct","png_destroy_read_struct","png_set_longjmp_fn","png_destroy_read_struct","png_build_grayscale_palette","png_process_data_pause","png_set_invert_alpha","png_init_io","png_set_sig_bytes","png_read_info","png_destroy_read_struct"],"critical_calls":["png_create_read_struct","png_create_info_struct","png_set_longjmp_fn","png_build_grayscale_palette","png_process_data_pause","png_set_invert_alpha","png_init_io","png_set_sig_bytes","png_read_info","png_destroy_read_struct"],"visited":0}
/**/



extern "C" int LLVMFuzzerTestOneInput_17(const uint8_t* f_data, size_t f_size) {
	if(f_size<21) return 0;

	FUZZ_FILE_INIT();
	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





  // Step 4: Read input data using `fmemopen`
  FILE *in_file = fmemopen((void *)data, size, "rb");
	FUZZ_FILE_PUSH(in_file)
  if (!in_file) {
    assert_file_closed(&in_file);
	return 0;
  }
	FUZZ_FILENAME_PUSH("input_file")

  // Step 6: Provide the input file name
  const char *input_file = "input_file";

  // Step 5: Get the file descriptor for reading
  int in_fd = fuzz_fileno(in_file);
	FUZZ_FD_PUSH(in_fd)

  // Initialize PNG structures
  png_struct *png_ptr = png_create_read_struct(fuzz_str_1, NULL, NULL, NULL);
  if (!png_ptr) {
    assert_file_closed(&in_file);;
    assert_file_name_closed("input_file");
	assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
  }

  png_info *info_ptr = png_create_info_struct(png_ptr);
  if (!info_ptr) {
    assert_file_closed(&in_file);;
    png_destroy_read_struct(&png_ptr, NULL, NULL);
    assert_file_name_closed("input_file");
	assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
  }

  // Initialize error handling
  if (setjmp(png_jmpbuf(png_ptr))) {
	FUZZ_FILE_CLOSE();
    assert_file_closed(&in_file);;
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    assert_file_name_closed("input_file");
	assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
  }

  // Step 1: Build grayscale palette
  png_color palette[256];
  png_build_grayscale_palette(8, palette);

  // Step 2: Process data pause
  png_process_data_pause(png_ptr, fuzz_int32_t_2);

  // Step 3: Invert alpha
  png_set_invert_alpha(png_ptr);

  // Set the input stream
  png_init_io(png_ptr, in_file);
  png_set_sig_bytes(png_ptr, fuzz_int32_t_3);

  // Read PNG image information
  png_read_info(png_ptr, info_ptr);

  // Step 7: Release allocated resources
  assert_file_closed(&in_file);;
  png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

  assert_file_name_closed("input_file");
	assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
}