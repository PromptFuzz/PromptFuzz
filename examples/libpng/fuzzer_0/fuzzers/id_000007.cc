#include "FDSan.h"
#include <png.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 384
//<Prompt> ["png_get_hIST","png_convert_from_time_t","png_get_user_height_max","png_permit_mng_features","png_set_unknown_chunk_location","png_get_error_ptr","png_free","png_get_y_offset_inches_fixed"]
/*<Combination>: [png_uint_32 png_get_hIST(png_const_structrp png_ptr, png_inforp info_ptr, png_uint_16p * hist),
    void png_convert_from_time_t(png_timep ptime, time_t ttime),
    png_uint_32 png_get_user_height_max(png_const_structrp png_ptr),
    png_uint_32 png_permit_mng_features(png_structrp png_ptr, png_uint_32 mng_features_permitted),
    void png_set_unknown_chunk_location(png_const_structrp png_ptr, png_inforp info_ptr, int chunk, int location),
    png_voidp png_get_error_ptr(png_const_structrp png_ptr),
    void png_free(png_const_structrp png_ptr, png_voidp ptr),
    png_fixed_point png_get_y_offset_inches_fixed(png_const_structrp png_ptr, png_const_inforp info_ptr)
*/
//<score> 27, nr_unique_branch: 4
//<Quality> {"density":9,"unique_branches":{"png_get_user_height_max":[[1214,12,1214,19,0,0,4,0]],"png_get_hIST":[[824,8,824,23,0,0,4,0],[824,27,824,43,0,0,4,0],[825,8,825,46,0,0,4,1]]},"library_calls":["png_create_read_struct","png_create_info_struct","png_destroy_read_struct","png_set_longjmp_fn","png_destroy_read_struct","png_init_io","png_read_png","png_get_user_height_max","png_get_hIST","png_destroy_read_struct"],"critical_calls":["png_create_read_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_read_png","png_get_user_height_max","png_get_hIST","png_destroy_read_struct"],"visited":0}
/**/

extern "C" int LLVMFuzzerTestOneInput_7(const uint8_t *data, size_t size) {
	if(size<0) return 0;

	FUZZ_FILE_INIT();
    // Step 4: Create a FILE * variable for reading input data
    FILE *in_file = fmemopen((void*)data, size, "rb");
	FUZZ_FILE_PUSH(in_file)
    if (in_file == nullptr) {
        assert_file_closed(&in_file);
	return 0; // Return if unable to open input file
    }

    // Step 5: Get file descriptor for reading
    int in_fd = fuzz_fileno(in_file);
	FUZZ_FD_PUSH(in_fd)

    // Initialize libpng structures
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (png_ptr == nullptr) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0; // Return if unable to create read struct
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == nullptr) {
        assert_file_closed(&in_file);;
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0; // Return if unable to create info struct
    }

    // Initialize the libpng error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
	FUZZ_FILE_CLOSE();
        assert_file_closed(&in_file);;
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0; // Return if there is an error during read
    }

    // Step 6: Read input file using libpng
    png_init_io(png_ptr, in_file);
    png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, nullptr);

    // Step 1: Use png_get_user_height_max to get the maximum height
    png_uint_32 max_height = png_get_user_height_max(png_ptr);
    std::cout << "Maximum height: " << max_height << std::endl;

    // Step 2: Use png_get_hIST to get the histogram
    png_uint_16p hist;
    png_uint_32 num_palette = png_get_hIST(png_ptr, info_ptr, &hist);
    std::cout << "Number of palette entries: " << num_palette << std::endl;
    for (int i = 0; i < num_palette; i++) {
        std::cout << "Entry " << i << ": " << hist[i] << std::endl;
    }

    // Step 3: Consume the input data - not required for this example

    // Step 7: Release resources
    assert_file_closed(&in_file);;
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);

    assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
}