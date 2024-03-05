#include "FDSan.h"
#include <png.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 2310
//<Prompt> ["png_warning","png_set_cHRM_fixed","png_set_interlace_handling","png_permit_mng_features","png_set_eXIf"]
/*<Combination>: [void png_warning(png_const_structrp png_ptr, png_const_charp warning_message),
    void png_set_cHRM_fixed(png_const_structrp png_ptr, png_inforp info_ptr, png_fixed_point int_white_x, png_fixed_point int_white_y, png_fixed_point int_red_x, png_fixed_point int_red_y, png_fixed_point int_green_x, png_fixed_point int_green_y, png_fixed_point int_blue_x, png_fixed_point int_blue_y),
    int png_set_interlace_handling(png_structrp png_ptr),
    png_uint_32 png_permit_mng_features(png_structrp png_ptr, png_uint_32 mng_features_permitted),
    void png_set_eXIf(png_const_structrp png_ptr, png_inforp info_ptr, png_bytep exif)
*/
//<score> 11, nr_unique_branch: 0
//<Quality> {"density":11,"unique_branches":{},"library_calls":["png_create_read_struct","png_create_info_struct","png_set_longjmp_fn","png_destroy_read_struct","png_init_io","png_read_info","png_warning","png_set_cHRM_fixed","png_set_interlace_handling","png_permit_mng_features","png_set_eXIf","png_destroy_read_struct"],"critical_calls":["png_create_read_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_read_info","png_warning","png_set_cHRM_fixed","png_set_interlace_handling","png_permit_mng_features","png_set_eXIf","png_destroy_read_struct"],"visited":0}
/*Here is the C++ program that achieves the event by utilizing the given libpng APIs:

*/


extern "C" int LLVMFuzzerTestOneInput_48(const uint8_t *data, size_t size) {
	if(size<0) return 0;

	FUZZ_FILE_INIT();
    // Create file variables
    FILE *in_file = fmemopen((void *)data, size, "rb");
	FUZZ_FILENAME_PUSH("output_file")
	FUZZ_FILE_PUSH(in_file)
    FILE *out_file = fopen("output_file", "wb");
	FUZZ_FILE_PUSH(out_file)

    // Check if file variables are valid
    if (in_file == NULL || out_file == NULL) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Create libpng structures
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    png_infop info_ptr = png_create_info_struct(png_ptr);

    // Set error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
	FUZZ_FILE_CLOSE();
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Set input source
    png_init_io(png_ptr, in_file);

    // Read PNG image information
    png_read_info(png_ptr, info_ptr);

    // Perform specific event using libpng APIs
    png_warning(png_ptr, "Example warning message");
    
    png_fixed_point white_x = 0;
    png_fixed_point white_y = 0;
    png_fixed_point red_x = 0;
    png_fixed_point red_y = 0;
    png_fixed_point green_x = 0;
    png_fixed_point green_y = 0;
    png_fixed_point blue_x = 0;
    png_fixed_point blue_y = 0;
    png_set_cHRM_fixed(png_ptr, info_ptr, white_x, white_y, red_x, red_y, green_x, green_y, blue_x, blue_y);
    
    png_set_interlace_handling(png_ptr);
    
    png_permit_mng_features(png_ptr, PNG_FLAG_MNG_FILTER_64);
    
    png_byte exif[10];
    png_set_eXIf(png_ptr, info_ptr, exif);

    // Clean up and release resources
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;

    assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}