#include "FDSan.h"
#include <png.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 4003
//<Prompt> ["png_get_pixel_aspect_ratio_fixed","png_set_user_limits","png_write_info_before_PLTE","png_get_color_type","png_access_version_number","png_set_gAMA_fixed","png_get_pixels_per_meter"]
/*<Combination>: [png_fixed_point png_get_pixel_aspect_ratio_fixed(png_const_structrp png_ptr, png_const_inforp info_ptr),
    void png_set_user_limits(png_structrp png_ptr, png_uint_32 user_width_max, png_uint_32 user_height_max),
    void png_write_info_before_PLTE(png_structrp png_ptr, png_const_inforp info_ptr),
    png_byte png_get_color_type(png_const_structrp png_ptr, png_const_inforp info_ptr),
    png_uint_32 png_access_version_number(),
    void png_set_gAMA_fixed(png_const_structrp png_ptr, png_inforp info_ptr, png_fixed_point int_file_gamma),
    png_uint_32 png_get_pixels_per_meter(png_const_structrp png_ptr, png_const_inforp info_ptr)
*/
//<score> 5, nr_unique_branch: 4
//<Quality> {"density":1,"unique_branches":{"png_set_gAMA_fixed":[[185,8,185,23,0,0,4,0]],"png_write_info_before_PLTE":[[88,8,88,23,0,0,4,0]],"png_get_pixel_aspect_ratio_fixed":[[220,8,220,23,0,0,4,1]],"png_get_color_type":[[92,8,92,23,0,0,4,1]]},"library_calls":["png_get_pixel_aspect_ratio_fixed","png_set_user_limits","png_write_info_before_PLTE","png_get_color_type","png_access_version_number","png_set_gAMA_fixed","png_get_pixels_per_meter"],"critical_calls":["png_get_pixel_aspect_ratio_fixed","png_set_user_limits","png_write_info_before_PLTE","png_get_color_type","png_access_version_number","png_set_gAMA_fixed","png_get_pixels_per_meter"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_86(const uint8_t *data, size_t size) {
	if(size<0) return 0;

	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);

    // Step 4: Open input file
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file == NULL) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Step 5: Get file descriptor for reading
    int in_fd = fuzz_fileno(in_file);

    // Step 6: Set input file name
    const char *in_file_name = "input_file";

    // Step 4: Open output file
    FILE *out_file = fopen("output_file", "wb");
    if (out_file == NULL) {
        assert_file_closed(&in_file);;
        assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
    }

    // Step 5: Get file descriptor for writing
    int out_fd = fuzz_fileno(out_file);

    // Step 6: Set output file name
    const char *out_file_name = "output_file";

    // Step 1: Get pixel aspect ratio fixed
    png_fixed_point pixel_aspect_ratio_fixed = png_get_pixel_aspect_ratio_fixed(NULL, NULL);

    // Step 1: Set user limits
    png_set_user_limits(NULL, 100, 100);

    // Step 1: Write info before PLTE
    png_write_info_before_PLTE(NULL, NULL);

    // Step 1: Get color type
    png_byte color_type = png_get_color_type(NULL, NULL);

    // Step 1: Get PNG library version number
    png_uint_32 version_number = png_access_version_number();

    // Step 1: Set gamma fixed
    png_set_gAMA_fixed(NULL, NULL, 1);

    // Step 1: Get pixels per meter
    png_uint_32 pixels_per_meter = png_get_pixels_per_meter(NULL, NULL);

    // Step 7: Release resources
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;

    assert_file_name_closed("output_file");
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}