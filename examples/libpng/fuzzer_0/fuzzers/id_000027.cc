#include "FDSan.h"
#include <png.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 1674
//<Prompt> ["png_get_current_row_number","png_set_iCCP","png_set_sBIT","png_get_eXIf","png_get_bit_depth"]
/*<Combination>: [png_uint_32 png_get_current_row_number(png_const_structrp ),
    void png_set_iCCP(png_const_structrp png_ptr, png_inforp info_ptr, png_const_charp name, int compression_type, png_const_bytep profile, png_uint_32 proflen),
    void png_set_sBIT(png_const_structrp png_ptr, png_inforp info_ptr, png_const_color_8p sig_bit),
    png_uint_32 png_get_eXIf(png_const_structrp png_ptr, png_inforp info_ptr, png_bytep * exif),
    png_byte png_get_bit_depth(png_const_structrp png_ptr, png_const_inforp info_ptr)
*/
//<score> 13, nr_unique_branch: 0
//<Quality> {"density":13,"unique_branches":{},"library_calls":["png_create_read_struct","png_create_info_struct","png_destroy_read_struct","png_set_longjmp_fn","png_destroy_read_struct","png_destroy_read_struct","png_init_io","png_read_info","png_get_current_row_number","png_set_iCCP","png_set_sBIT","png_get_eXIf","png_get_bit_depth","png_destroy_read_struct"],"critical_calls":["png_create_read_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_read_info","png_get_current_row_number","png_set_iCCP","png_set_sBIT","png_get_eXIf","png_get_bit_depth","png_destroy_read_struct"],"visited":0}
/**/



extern "C" int LLVMFuzzerTestOneInput_27(const uint8_t *data, size_t size) {
	if(size<0) return 0;

	FUZZ_FILE_INIT();
    // Step 1: Declare variables and initialize libpng structs
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        // Handle error
        return 0;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        // Handle error
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return 0;
    }

    // Step 2: Set up error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
	FUZZ_FILE_CLOSE();
        // Handle error
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return 0;
    }

    // Step 3: Use the input data to create a FILE pointer for reading
    FILE *in_file = fmemopen((void *)data, size, "rb");
	FUZZ_FILE_PUSH(in_file)
    if (!in_file) {
        // Handle error
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        assert_file_closed(&in_file);
	return 0;
    }

    // Step 4: Set up libpng to read from the input file
    png_init_io(png_ptr, in_file);

    // Step 5: Read the PNG file information
    png_read_info(png_ptr, info_ptr);

    // Step 6: Use the libpng APIs to achieve the desired event
    png_uint_32 current_row_number = png_get_current_row_number(png_ptr);
    png_set_iCCP(png_ptr, info_ptr, "ICC profile", PNG_COMPRESSION_TYPE_BASE, NULL, 0);

    png_color_8 sig_bit;
    sig_bit.red = 8;
    sig_bit.green = 8;
    sig_bit.blue = 8;
    sig_bit.gray = 8;
    sig_bit.alpha = 8;
    png_set_sBIT(png_ptr, info_ptr, &sig_bit);

    png_byte *exif_data = NULL;
    png_uint_32 exif_length = png_get_eXIf(png_ptr, info_ptr, &exif_data);
    if (exif_data) {
        // Process exif data
        free(exif_data);
    }

    png_byte bit_depth = png_get_bit_depth(png_ptr, info_ptr);

    // Step 7: Clean up and release resources
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    assert_file_closed(&in_file);;

    assert_file_closed(&in_file);
	return 0;
}