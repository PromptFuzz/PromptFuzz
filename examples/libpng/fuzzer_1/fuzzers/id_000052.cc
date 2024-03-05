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
//<ID> 2555
//<Prompt> ["png_get_eXIf","png_info_init_3","png_get_filter_type","png_get_copyright","png_get_current_row_number"]
/*<Combination>: [png_uint_32 png_get_eXIf(png_const_structrp png_ptr, png_inforp info_ptr, png_bytep * exif),
    void png_info_init_3(png_infopp info_ptr, size_t png_info_struct_size),
    png_byte png_get_filter_type(png_const_structrp png_ptr, png_const_inforp info_ptr),
    png_const_charp png_get_copyright(png_const_structrp png_ptr),
    png_uint_32 png_get_current_row_number(png_const_structrp )
*/
//<score> 28, nr_unique_branch: 2
//<Quality> {"density":14,"unique_branches":{"png_get_filter_type":[[101,8,101,23,0,0,4,0],[101,27,101,43,0,0,4,0]]},"library_calls":["png_create_read_struct","png_create_info_struct","png_destroy_read_struct","png_set_longjmp_fn","png_destroy_read_struct","png_set_error_fn","png_destroy_read_struct","png_init_io","png_read_png","png_get_eXIf","png_get_filter_type","png_get_copyright","png_get_current_row_number","png_free_data","png_destroy_read_struct"],"critical_calls":["png_create_read_struct","png_create_info_struct","png_set_longjmp_fn","png_set_error_fn","png_init_io","png_read_png","png_get_eXIf","png_get_filter_type","png_get_copyright","png_get_current_row_number","png_free_data","png_destroy_read_struct"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_52(const uint8_t* f_data, size_t f_size) {
	if(f_size<25) return 0;

	FUZZ_FILE_INIT();
	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
	//fuzzer shim end}




  // Initialize libpng structures
  png_structp png_ptr =
      png_create_read_struct(fuzz_str_1, NULL, NULL, NULL);
  if (!png_ptr) {
    return 0;
  }
  png_infop info_ptr = png_create_info_struct(png_ptr);
  if (!info_ptr) {
    png_destroy_read_struct(&png_ptr, NULL, NULL);
    return 0;
  }
  if (setjmp(png_jmpbuf(png_ptr))) {
	FUZZ_FILE_CLOSE();
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    return 0;
  }

  // Set up error handling
  png_set_error_fn(png_ptr, NULL, NULL, NULL);

  // Create a file stream from the input data
  FILE *in_file = fmemopen((void *)data, size, "rb");
	FUZZ_FILE_PUSH(in_file)
  if (!in_file) {
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    assert_file_closed(&in_file);
	return 0;
  }

  // Set the input file stream
  png_init_io(png_ptr, in_file);

  // Read the PNG image
  png_read_png(png_ptr, info_ptr, fuzz_int32_t_2, NULL);

  // Get the Exif data
  png_bytep exif_data = NULL;
  png_uint_32 exif_size =
      png_get_eXIf(png_ptr, info_ptr, &exif_data);

  // Get the filter type
  png_byte filter_type = png_get_filter_type(png_ptr, info_ptr);

  // Get the copyright information
  png_const_charp copyright =
      png_get_copyright(png_ptr);

  // Get the current row number
  png_uint_32 current_row = png_get_current_row_number(png_ptr);

  // Clean up
  assert_file_closed(&in_file);;
  png_free_data(png_ptr, info_ptr, fuzz_uint32_t_3, fuzz_int32_t_4);
  png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

  assert_file_closed(&in_file);
	return 0;
}