#include "FDSan.h"
#include <png.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 3098
//<Prompt> ["png_set_swap_alpha","png_create_write_struct","png_get_bKGD","png_get_valid","png_image_begin_read_from_memory","png_get_header_ver","png_set_read_fn","png_get_iCCP"]
/*<Combination>: [void png_set_swap_alpha(png_structrp png_ptr),
    png_structp png_create_write_struct(png_const_charp user_png_ver, png_voidp error_ptr, png_error_ptr error_fn, png_error_ptr warn_fn),
    png_uint_32 png_get_bKGD(png_const_structrp png_ptr, png_inforp info_ptr, png_color_16p * background),
    png_uint_32 png_get_valid(png_const_structrp png_ptr, png_const_inforp info_ptr, png_uint_32 flag),
    int png_image_begin_read_from_memory(png_imagep image, png_const_voidp memory, size_t size),
    png_const_charp png_get_header_ver(png_const_structrp png_ptr),
    void png_set_read_fn(png_structrp png_ptr, png_voidp io_ptr, png_rw_ptr read_data_fn),
    png_uint_32 png_get_iCCP(png_const_structrp png_ptr, png_inforp info_ptr, png_charpp name, int * compression_type, png_bytepp profile, png_uint_32 * proflen)
*/
//<score> 8, nr_unique_branch: 1
//<Quality> {"density":4,"unique_branches":{"png_get_bKGD":[[502,27,502,43,0,0,4,1]]},"library_calls":["png_create_write_struct","png_get_bKGD","png_destroy_write_struct","png_destroy_write_struct"],"critical_calls":["png_create_write_struct","png_get_bKGD","png_destroy_write_struct"],"visited":0}
/*Here is a C++ program that follows the provided instructions and uses the libpng library APIs:

*/


extern "C" int LLVMFuzzerTestOneInput_63(const uint8_t *data, size_t size) {
	if(size<0) return 0;

    // Step 4: Create input file from data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Step 2: Create libpng write struct
    png_structp write_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!write_ptr) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // Step 3: Get bKGD information
    png_color_16p background;
    png_uint_32 bkgd_result = png_get_bKGD(write_ptr, nullptr, &background);
    if (!bkgd_result) {
        png_destroy_write_struct(&write_ptr, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // Step 7: Release allocated resources
    png_destroy_write_struct(&write_ptr, nullptr);
    assert_file_closed(&in_file);;

    assert_file_closed(&in_file);
	return 0;
}