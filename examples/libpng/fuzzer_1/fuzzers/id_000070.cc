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
//<ID> 3481
//<Prompt> ["png_set_invert_mono","png_get_image_height","png_set_chunk_malloc_max","png_image_write_to_file","png_set_gray_to_rgb","png_set_swap"]
/*<Combination>: [void png_set_invert_mono(png_structrp png_ptr),
    png_uint_32 png_get_image_height(png_const_structrp png_ptr, png_const_inforp info_ptr),
    void png_set_chunk_malloc_max(png_structrp png_ptr, png_alloc_size_t user_chunk_cache_max),
    int png_image_write_to_file(png_imagep image, const char * file, int convert_to_8bit, const void * buffer, png_int_32 row_stride, const void * colormap),
    void png_set_gray_to_rgb(png_structrp png_ptr),
    void png_set_swap(png_structrp png_ptr)
*/
//<score> 33, nr_unique_branch: 3
//<Quality> {"density":11,"unique_branches":{"png_image_write_to_file":[[2347,8,2347,21,0,0,4,1],[2400,13,2400,26,0,0,4,1]],"png_set_swap":[[42,8,42,32,0,0,4,1]]},"library_calls":["png_create_read_struct","png_create_info_struct","png_destroy_read_struct","png_init_io","png_set_write_fn","png_set_invert_mono","png_get_image_height","png_set_chunk_malloc_max","png_image_write_to_file","png_set_gray_to_rgb","png_set_swap","png_destroy_read_struct"],"critical_calls":["png_create_read_struct","png_create_info_struct","png_init_io","png_set_write_fn","png_set_invert_mono","png_get_image_height","png_set_chunk_malloc_max","png_image_write_to_file","png_set_gray_to_rgb","png_set_swap","png_destroy_read_struct"],"visited":0}
/*Here is the C++ program that uses the libpng library APIs to achieve the specified event:

*/


extern "C" int LLVMFuzzerTestOneInput_70(const uint8_t* f_data, size_t f_size) {
	if(f_size<43) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
	//fuzzer shim end}




    // Step 4: Open input file as FILE *
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file == nullptr) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Step 5: Get file descriptor for reading
    int in_fd = fuzz_fileno(in_file);

    // Step 6: Set output file name
    const char *out_file_name = "output_file";

    // Open output file as FILE *
    FILE *out_file = fopen(out_file_name, "wb");
    if (out_file == nullptr) {
        assert_file_closed(&in_file);;
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
    }

    // Get file descriptor for writing
    int out_fd = fuzz_fileno(out_file);

    // Step 2: Create png_struct and png_info objects
    png_structp png_ptr = png_create_read_struct(fuzz_str_1, nullptr, nullptr, nullptr);
    if (png_ptr == nullptr) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == nullptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }

    // Set input and output files
    png_init_io(png_ptr, in_file);
    png_set_write_fn(png_ptr, &out_fd, nullptr, nullptr);

    // Step 1: Use libpng APIs
    png_set_invert_mono(png_ptr);
    png_uint_32 image_height = png_get_image_height(png_ptr, info_ptr);
    png_set_chunk_malloc_max(png_ptr, fuzz_uint64_t_2);
    png_image_write_to_file(nullptr, fuzz_str_3, fuzz_int32_t_4, nullptr, fuzz_int32_t_5, nullptr);
    png_set_gray_to_rgb(png_ptr);
    png_set_swap(png_ptr);

    // Cleanup
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;

    assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}