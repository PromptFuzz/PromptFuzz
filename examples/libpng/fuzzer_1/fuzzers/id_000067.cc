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
//<ID> 3189
//<Prompt> ["png_set_strip_alpha","png_get_pixel_aspect_ratio","png_set_eXIf_1","png_set_text","png_get_unknown_chunks","png_set_progressive_read_fn","png_init_io","png_get_cHRM_fixed","png_get_x_offset_inches_fixed"]
/*<Combination>: [void png_set_strip_alpha(png_structrp png_ptr),
    float png_get_pixel_aspect_ratio(png_const_structrp png_ptr, png_const_inforp info_ptr),
    void png_set_eXIf_1(png_const_structrp png_ptr, png_inforp info_ptr, png_uint_32 num_exif, png_bytep exif),
    void png_set_text(png_const_structrp png_ptr, png_inforp info_ptr, png_const_textp text_ptr, int num_text),
    int png_get_unknown_chunks(png_const_structrp png_ptr, png_inforp info_ptr, png_unknown_chunkpp entries),
    void png_set_progressive_read_fn(png_structrp png_ptr, png_voidp progressive_ptr, png_progressive_info_ptr info_fn, png_progressive_row_ptr row_fn, png_progressive_end_ptr end_fn),
    void png_init_io(png_structrp png_ptr, png_FILE_p fp),
    png_uint_32 png_get_cHRM_fixed(png_const_structrp png_ptr, png_const_inforp info_ptr, png_fixed_point * int_white_x, png_fixed_point * int_white_y, png_fixed_point * int_red_x, png_fixed_point * int_red_y, png_fixed_point * int_green_x, png_fixed_point * int_green_y, png_fixed_point * int_blue_x, png_fixed_point * int_blue_y),
    png_fixed_point png_get_x_offset_inches_fixed(png_const_structrp png_ptr, png_const_inforp info_ptr)
*/
//<score> 7, nr_unique_branch: 6
//<Quality> {"density":1,"unique_branches":{"png_get_cHRM_fixed":[[659,8,659,23,0,0,4,1]],"png_set_eXIf_1":[[155,8,155,23,0,0,4,0]],"png_set_progressive_read_fn":[[1078,8,1078,23,0,0,4,0]],"png_set_text_2":[[769,8,769,23,0,0,4,0]],"png_get_pixel_aspect_ratio":[[196,8,196,23,0,0,4,1]],"png_set_strip_alpha":[[214,8,214,37,0,0,4,0]]},"library_calls":["png_set_strip_alpha","png_get_pixel_aspect_ratio","png_set_eXIf_1","png_set_text","png_get_unknown_chunks","png_set_progressive_read_fn","png_init_io","png_get_cHRM_fixed","png_get_x_offset_inches_fixed"],"critical_calls":["png_set_strip_alpha","png_get_pixel_aspect_ratio","png_set_eXIf_1","png_set_text","png_get_unknown_chunks","png_set_progressive_read_fn","png_init_io","png_get_cHRM_fixed","png_get_x_offset_inches_fixed"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_67(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    // Step 4: Create input and output FILE pointers
    FILE *in_file = fmemopen((void *)data, size, "rb");
    FILE *out_file = fopen("output_file", "wb");

    // Step 5: Get file descriptors for input and output
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);

    // Step 6: Define input and output file names
    const char *in_file_name = "input_file";
    const char *out_file_name = "output_file";

    // Step 1: Set strip alpha
    png_set_strip_alpha(nullptr);

    // Step 2: Get pixel aspect ratio
    float aspect_ratio = png_get_pixel_aspect_ratio(nullptr, nullptr);

    // Step 3: Set exif data
    png_byte exif_data[] = { /* exif data */ };
    png_set_eXIf_1(nullptr, nullptr, sizeof(exif_data), exif_data);

    // Step 3: Set text information
    png_text text_info[] = { /* text information */ };
    int num_text = sizeof(text_info) / sizeof(png_text);
    png_set_text(nullptr, nullptr, text_info, num_text);

    // Step 3: Get unknown chunks
    png_unknown_chunkpp unknown_chunks = nullptr;
    int num_unknown_chunks = png_get_unknown_chunks(nullptr, nullptr, unknown_chunks);

    // Step 3: Set progressive read function
    png_set_progressive_read_fn(nullptr, nullptr, nullptr, nullptr, nullptr);

    // Step 3: Init IO
    png_init_io(nullptr, in_file);

    // Step 3: Get cHRM fixed points
    png_fixed_point int_white_x, int_white_y, int_red_x, int_red_y, int_green_x, int_green_y, int_blue_x, int_blue_y;
    png_get_cHRM_fixed(nullptr, nullptr, &int_white_x, &int_white_y, &int_red_x, &int_red_y, &int_green_x, &int_green_y, &int_blue_x, &int_blue_y);

    // Step 3: Get X offset in inches
    png_fixed_point x_offset_inches_fixed = png_get_x_offset_inches_fixed(nullptr, nullptr);

    // Step 7: Free resources
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