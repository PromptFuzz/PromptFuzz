#include "FDSan.h"
#include <png.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 1651
//<Prompt> ["png_set_quantize","png_chunk_warning","png_get_bKGD","png_get_user_width_max","png_get_sBIT"]
/*<Combination>: [void png_set_quantize(png_structrp png_ptr, png_colorp palette, int num_palette, int maximum_colors, png_const_uint_16p histogram, int full_quantize),
    void png_chunk_warning(png_const_structrp png_ptr, png_const_charp warning_message),
    png_uint_32 png_get_bKGD(png_const_structrp png_ptr, png_inforp info_ptr, png_color_16p * background),
    png_uint_32 png_get_user_width_max(png_const_structrp png_ptr),
    png_uint_32 png_get_sBIT(png_const_structrp png_ptr, png_inforp info_ptr, png_color_8p * sig_bit)
*/
//<score> 45, nr_unique_branch: 7
//<Quality> {"density":15,"unique_branches":{"png_get_valid":[[23,8,23,23,0,0,4,0],[23,27,23,43,0,0,4,0],[30,11,30,32,0,0,4,1]],"png_get_sBIT":[[1047,8,1047,23,0,0,4,0],[1047,27,1047,43,0,0,4,0],[1048,8,1048,46,0,0,4,0],[1048,50,1048,65,0,0,4,0]]},"library_calls":["png_create_read_struct","png_create_info_struct","png_destroy_read_struct","png_init_io","png_set_longjmp_fn","png_destroy_read_struct","png_read_info","png_get_image_width","png_get_image_height","png_get_color_type","png_get_bit_depth","png_get_channels","png_get_valid","png_get_bKGD","png_get_sBIT","png_destroy_read_struct"],"critical_calls":["png_create_read_struct","png_create_info_struct","png_init_io","png_set_longjmp_fn","png_read_info","png_get_image_width","png_get_image_height","png_get_color_type","png_get_bit_depth","png_get_channels","png_get_valid","png_get_bKGD","png_get_sBIT","png_destroy_read_struct"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_26(const uint8_t* data, size_t size) {
	if(size<0) return 0;

	FUZZ_FILE_INIT();
    FILE* in_file = fmemopen((void*)data, size, "rb");
	FUZZ_FILE_PUSH(in_file)
    if (!in_file) {
        // handle error
        assert_file_closed(&in_file);
	return 0;
    }

    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        // handle error
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        // handle error
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // Set the read function to read data from in_file
    png_init_io(png_ptr, in_file);

    if (setjmp(png_jmpbuf(png_ptr))) {
	FUZZ_FILE_CLOSE();
        // handle error
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // Read the PNG image header and update the info_ptr
    png_read_info(png_ptr, info_ptr);

    // Use png_get_image_width to get the width of the image
    png_uint_32 width = png_get_image_width(png_ptr, info_ptr);

    // Use png_get_image_height to get the height of the image
    png_uint_32 height = png_get_image_height(png_ptr, info_ptr);

    // Use png_get_color_type to get the color type of the image
    png_byte color_type = png_get_color_type(png_ptr, info_ptr);

    // Use png_get_bit_depth to get the bit depth of the image
    png_byte bit_depth = png_get_bit_depth(png_ptr, info_ptr);

    // Use png_get_channels to get the number of color channels in the image
    png_byte channels = png_get_channels(png_ptr, info_ptr);

    // Use png_get_valid to get the valid chunk data for the image
    png_uint_32 valid_chunks = png_get_valid(png_ptr, info_ptr, PNG_INFO_bKGD | PNG_INFO_sBIT);

    if (valid_chunks & PNG_INFO_bKGD) {
        png_color_16p background;
        png_get_bKGD(png_ptr, info_ptr, &background);
        // use background color
    }

    if (valid_chunks & PNG_INFO_sBIT) {
        png_color_8p sig_bit;
        png_get_sBIT(png_ptr, info_ptr, &sig_bit);
        // use significant bit
    }

    // Perform any other image processing or analysis as needed

    // Clean up resources
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    assert_file_closed(&in_file);;

    assert_file_closed(&in_file);
	return 0;
}