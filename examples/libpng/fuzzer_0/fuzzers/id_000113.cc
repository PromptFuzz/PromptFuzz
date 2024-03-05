#include "FDSan.h"
#include <png.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 5529
//<Prompt> ["png_get_channels","png_set_user_limits","png_set_unknown_chunk_location","png_set_strip_alpha","png_set_flush","png_save_int_32"]
/*<Combination>: [png_byte png_get_channels(png_const_structrp png_ptr, png_const_inforp info_ptr),
    void png_set_user_limits(png_structrp png_ptr, png_uint_32 user_width_max, png_uint_32 user_height_max),
    void png_set_unknown_chunk_location(png_const_structrp png_ptr, png_inforp info_ptr, int chunk, int location),
    void png_set_strip_alpha(png_structrp png_ptr),
    void png_set_flush(png_structrp png_ptr, int nrows),
    void png_save_int_32(png_bytep buf, png_int_32 i)
*/
//<score> 11, nr_unique_branch: 0
//<Quality> {"density":11,"unique_branches":{},"library_calls":["png_create_read_struct","png_create_info_struct","png_set_longjmp_fn","png_destroy_read_struct","png_init_io","png_read_info","png_get_channels","png_set_user_limits","png_set_unknown_chunk_location","png_set_strip_alpha","png_set_flush","png_save_int_32","png_destroy_read_struct"],"critical_calls":["png_create_read_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_read_info","png_get_channels","png_set_user_limits","png_set_unknown_chunk_location","png_set_strip_alpha","png_set_flush","png_save_int_32","png_destroy_read_struct"],"visited":0}
/**/

extern "C" int LLVMFuzzerTestOneInput_113(const uint8_t* data, size_t size) {
	if(size<0) return 0;

	FUZZ_FILE_INIT();
    // Variables for input and output files
    FILE* in_file = fmemopen((void*)data, size, "rb");
	FUZZ_FILENAME_PUSH("output_file")
	FUZZ_FILE_PUSH(in_file)
    FILE* out_file = fopen("output_file", "wb");
	FUZZ_FILE_PUSH(out_file)

    if (!in_file || !out_file) {
        // Failed to open input or output file
        if (in_file) assert_file_closed(&in_file);;
        if (out_file) assert_file_closed(&out_file);;
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Create png_struct and png_info variables
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    png_infop info_ptr = png_create_info_struct(png_ptr);

     // Set the custom error handling function
    if (setjmp(png_jmpbuf(png_ptr))) {
	FUZZ_FILE_CLOSE();
        // Error occurred during reading the PNG file
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Set input file stream
    png_init_io(png_ptr, in_file);

    // Read the PNG file information
    png_read_info(png_ptr, info_ptr);

    // Get channels
    int channels = png_get_channels(png_ptr, info_ptr);
    std::cout << "Number of channels: " << channels << std::endl;

    // Set user limits for width and height
    png_set_user_limits(png_ptr, 1000, 1000);

    // Set unknown chunk location
    png_set_unknown_chunk_location(png_ptr, info_ptr, 1, PNG_AFTER_IDAT);

    // Strip alpha channel
    png_set_strip_alpha(png_ptr);

    // Flush the data - for testing purposes only
    png_set_flush(png_ptr, 10);

    // Save int_32 value to a byte buffer
    png_byte buf[4];
    png_save_int_32(buf, 123456789);

    // Write the byte buffer to the output file
    fwrite(buf, sizeof(png_byte), 4, out_file);

    // Clean up resources
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;

    assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}