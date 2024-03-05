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
//<ID> 4548
//<Prompt> ["png_set_PLTE","png_get_eXIf","png_create_write_struct","png_read_image","png_get_image_width","png_get_header_ver","png_get_y_pixels_per_meter"]
/*<Combination>: [void png_set_PLTE(png_structrp png_ptr, png_inforp info_ptr, png_const_colorp palette, int num_palette),
    png_uint_32 png_get_eXIf(png_const_structrp png_ptr, png_inforp info_ptr, png_bytep * exif),
    png_structp png_create_write_struct(png_const_charp user_png_ver, png_voidp error_ptr, png_error_ptr error_fn, png_error_ptr warn_fn),
    void png_read_image(png_structrp png_ptr, png_bytepp image),
    png_uint_32 png_get_image_width(png_const_structrp png_ptr, png_const_inforp info_ptr),
    png_const_charp png_get_header_ver(png_const_structrp png_ptr),
    png_uint_32 png_get_y_pixels_per_meter(png_const_structrp png_ptr, png_const_inforp info_ptr)
*/
//<score> 45, nr_unique_branch: 4
//<Quality> {"density":9,"unique_branches":{"png_get_image_width":[[65,27,65,43,0,0,4,1]],"png_get_y_pixels_per_meter":[[152,27,152,43,0,0,4,1]],"png_read_image":[[712,8,712,49,0,0,4,0]],"png_set_PLTE":[[569,27,569,43,0,0,4,0]]},"library_calls":["png_create_read_struct","png_create_write_struct","png_destroy_read_struct","png_set_longjmp_fn","png_destroy_read_struct","png_destroy_write_struct","png_set_PLTE","png_get_eXIf","png_read_image","png_get_image_width","png_get_header_ver","png_get_y_pixels_per_meter","png_destroy_read_struct","png_destroy_write_struct"],"critical_calls":["png_create_read_struct","png_create_write_struct","png_set_longjmp_fn","png_set_PLTE","png_get_eXIf","png_read_image","png_get_image_width","png_get_header_ver","png_get_y_pixels_per_meter","png_destroy_read_struct","png_destroy_write_struct"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_94(const uint8_t* f_data, size_t f_size) {
	if(f_size<26) return 0;

	FUZZ_FILE_INIT();
	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
	//fuzzer shim end}




  // Step 4: Open the input file
  FILE *in_file = fmemopen((void *)data, size, "rb");
	FUZZ_FILE_PUSH(in_file)
  if (!in_file) {
    assert_file_closed(&in_file);
	return 0;
  }
	FUZZ_FILENAME_PUSH("output_file")

  // Step 6: Prepare the output file
  FILE *out_file = fopen("output_file", "wb");
	FUZZ_FILE_PUSH(out_file)
  if (!out_file) {
    assert_file_closed(&in_file);;
    assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
  }

  // Step 7: Create libpng read and write structures
  png_structp read_png_ptr = png_create_read_struct(fuzz_str_1, NULL, NULL, NULL);
  if (!read_png_ptr) {
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
  }

  png_structp write_png_ptr = png_create_write_struct(fuzz_str_2, NULL, NULL, NULL);
  if (!write_png_ptr) {
    png_destroy_read_struct(&read_png_ptr, NULL, NULL);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
  }

  // Initialize the libpng error handling
  if (setjmp(png_jmpbuf(read_png_ptr))) {
	FUZZ_FILE_CLOSE();
    png_destroy_read_struct(&read_png_ptr, NULL, NULL);
    png_destroy_write_struct(&write_png_ptr, NULL);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
  }

  // Step 1: Set the palette
  png_color palette[256];
  int num_palette = 256;
  png_set_PLTE(write_png_ptr, NULL, palette, num_palette);

  // Step 2: Get the EXIF data
  png_bytep exif;
  png_uint_32 exif_length = png_get_eXIf(read_png_ptr, NULL, &exif);

  // Step 3: Read the image
  png_bytepp image = NULL;
  png_read_image(read_png_ptr, image);

  // Step 5: Get the image width
  png_uint_32 width = png_get_image_width(read_png_ptr, NULL);

  // Step 6: Get the header version
  png_const_charp header_ver = png_get_header_ver(read_png_ptr);

  // Step 7: Get the Y pixels per meter
  png_uint_32 y_pixels_per_meter = png_get_y_pixels_per_meter(read_png_ptr, NULL);

  // Cleanup
  png_destroy_read_struct(&read_png_ptr, NULL, NULL);
  png_destroy_write_struct(&write_png_ptr, NULL);
  assert_file_closed(&in_file);;
  assert_file_closed(&out_file);;

  assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}