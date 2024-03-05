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
//<ID> 5601
//<Prompt> ["png_get_pixel_aspect_ratio","png_chunk_warning","png_benign_error","png_image_write_to_memory","png_get_progressive_ptr","png_get_chunk_malloc_max","png_warning","png_set_gamma"]
/*<Combination>: [float png_get_pixel_aspect_ratio(png_const_structrp png_ptr, png_const_inforp info_ptr),
    void png_chunk_warning(png_const_structrp png_ptr, png_const_charp warning_message),
    void png_benign_error(png_const_structrp png_ptr, png_const_charp warning_message),
    int png_image_write_to_memory(png_imagep image, void * memory, png_alloc_size_t *__restrict memory_bytes, int convert_to_8_bit, const void * buffer, png_int_32 row_stride, const void * colormap),
    png_voidp png_get_progressive_ptr(png_const_structrp png_ptr),
    png_alloc_size_t png_get_chunk_malloc_max(png_const_structrp png_ptr),
    void png_warning(png_const_structrp png_ptr, png_const_charp warning_message),
    void png_set_gamma(png_structrp png_ptr, double screen_gamma, double override_file_gamma)
*/
//<score> 48, nr_unique_branch: 3
//<Quality> {"density":16,"unique_branches":{"png_get_pixel_aspect_ratio":[[197,8,197,46,0,0,4,0],[201,11,201,43,0,0,4,0]],"png_image_write_to_memory":[[2233,11,2233,31,0,0,4,1]]},"library_calls":["png_create_read_struct","png_create_info_struct","png_destroy_read_struct","png_set_longjmp_fn","png_destroy_read_struct","png_init_io","png_read_info","png_get_pixel_aspect_ratio","png_chunk_warning","png_benign_error","png_get_image_width","png_get_image_height","png_image_write_to_memory","png_get_progressive_ptr","png_get_chunk_malloc_max","png_warning","png_set_gamma","png_destroy_read_struct"],"critical_calls":["png_create_read_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_read_info","png_get_pixel_aspect_ratio","png_chunk_warning","png_benign_error","png_get_image_width","png_get_image_height","png_image_write_to_memory","png_get_progressive_ptr","png_get_chunk_malloc_max","png_warning","png_set_gamma","png_destroy_read_struct"],"visited":0}
/**/



extern "C" int LLVMFuzzerTestOneInput_115(const uint8_t* f_data, size_t f_size) {
	if(f_size<66) return 0;

	FUZZ_FILE_INIT();
	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
	//fuzzer shim end}




    // Create a FILE pointer to read the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
	FUZZ_FILE_PUSH(in_file)
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
	FUZZ_FILENAME_PUSH("output_file.png")

    // Create a FILE pointer to write output data
    FILE *out_file = fopen("output_file.png", "wb");
	FUZZ_FILE_PUSH(out_file)
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_name_closed("output_file.png");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Create png_struct and png_info pointers
    png_structp png_ptr = png_create_read_struct(fuzz_str_1, NULL, NULL, NULL);
    if (!png_ptr) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_name_closed("output_file.png");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_name_closed("output_file.png");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Set error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
	FUZZ_FILE_CLOSE();
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_name_closed("output_file.png");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Initialize IO
    png_init_io(png_ptr, in_file);

    // Read PNG header
    png_read_info(png_ptr, info_ptr);

    // Example: Get pixel aspect ratio
    float pixel_aspect_ratio = png_get_pixel_aspect_ratio(png_ptr, info_ptr);

    // Example: Generate warning
    png_chunk_warning(png_ptr, fuzz_str_2);

    // Example: Generate benign error
    png_benign_error(png_ptr, fuzz_str_3);

    // Example: Write PNG image to memory
    png_image image;
    memset(&image, 0, sizeof(png_image));
    image.version = PNG_IMAGE_VERSION;
    image.width = png_get_image_width(png_ptr, info_ptr);
    image.height = png_get_image_height(png_ptr, info_ptr);
    image.format = PNG_FORMAT_RGBA;
    image.flags = 0;
    png_image_write_to_memory(&image, NULL, NULL, fuzz_int32_t_4, NULL, fuzz_int32_t_5, NULL);

    // Example: Get progressive ptr
    png_voidp progressive_ptr = png_get_progressive_ptr(png_ptr);

    // Example: Get max chunk malloc size
    png_alloc_size_t malloc_max = png_get_chunk_malloc_max(png_ptr);

    // Example: Generate general warning
    png_warning(png_ptr, "This is a general warning");

    // Example: Set gamma value
    png_set_gamma(png_ptr, 2.2, 0.45455);

    // Clean up and release resources
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;

    assert_file_name_closed("output_file.png");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}