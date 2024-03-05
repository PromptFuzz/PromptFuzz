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
//<ID> 3802
//<Prompt> ["png_set_invalid","png_read_rows","png_set_filter_heuristics","png_set_gamma","png_set_rows","png_create_read_struct","png_get_compression_type","png_image_write_to_memory"]
/*<Combination>: [void png_set_invalid(png_const_structrp png_ptr, png_inforp info_ptr, int mask),
    void png_read_rows(png_structrp png_ptr, png_bytepp row, png_bytepp display_row, png_uint_32 num_rows),
    void png_set_filter_heuristics(png_structrp png_ptr, int heuristic_method, int num_weights, png_const_doublep filter_weights, png_const_doublep filter_costs),
    void png_set_gamma(png_structrp png_ptr, double screen_gamma, double override_file_gamma),
    void png_set_rows(png_const_structrp png_ptr, png_inforp info_ptr, png_bytepp row_pointers),
    png_structp png_create_read_struct(png_const_charp user_png_ver, png_voidp error_ptr, png_error_ptr error_fn, png_error_ptr warn_fn),
    png_byte png_get_compression_type(png_const_structrp png_ptr, png_const_inforp info_ptr),
    int png_image_write_to_memory(png_imagep image, void * memory, png_alloc_size_t *__restrict memory_bytes, int convert_to_8_bit, const void * buffer, png_int_32 row_stride, const void * colormap)
*/
//<score> 12, nr_unique_branch: 0
//<Quality> {"density":12,"unique_branches":{},"library_calls":["png_create_read_struct","png_create_info_struct","png_destroy_read_struct","png_set_longjmp_fn","png_destroy_read_struct","png_init_io","png_read_png","png_set_invalid","png_get_rows","png_get_image_height","png_get_image_width","png_get_image_height","png_image_write_to_memory","png_destroy_read_struct"],"critical_calls":["png_create_read_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_read_png","png_set_invalid","png_get_rows","png_get_image_height","png_get_image_width","png_get_image_height","png_image_write_to_memory","png_destroy_read_struct"],"visited":0}
/**/



extern "C" int LLVMFuzzerTestOneInput_79(const uint8_t* f_data, size_t f_size) {
	if(f_size<29) return 0;

	FUZZ_FILE_INIT();
	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
	//fuzzer shim end}




    FILE *in_file = fmemopen((void *)data, size, "rb");
	FUZZ_FILE_PUSH(in_file)
    if (in_file == NULL) {
        assert_file_closed(&in_file);
	return 0;
    }

    png_structp png_ptr = png_create_read_struct(fuzz_str_1, NULL, NULL, NULL);
    if (png_ptr == NULL) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == NULL) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    if (setjmp(png_jmpbuf(png_ptr))) {
	FUZZ_FILE_CLOSE();
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    png_init_io(png_ptr, in_file);
    png_read_png(png_ptr, info_ptr, fuzz_int32_t_2, NULL);

    // Perform the desired event using libpng APIs
    png_set_invalid(png_ptr, info_ptr, fuzz_int32_t_3);
    png_bytepp row_pointers = png_get_rows(png_ptr, info_ptr);

    // Process the rows
    png_uint_32 num_rows = png_get_image_height(png_ptr, info_ptr);
    for (png_uint_32 i = 0; i < num_rows; i++) {
        png_bytep row = row_pointers[i];
        // Process each row
    }

    // Write the modified image to memory
    png_image image;
    memset(&image, 0, sizeof(image));
    image.width = png_get_image_width(png_ptr, info_ptr);
    image.height = png_get_image_height(png_ptr, info_ptr);
    image.format = PNG_FORMAT_RGB;
    png_bytep memory = new png_byte[size];
    memcpy(memory, data, size);
    png_alloc_size_t memory_bytes;
    png_image_write_to_memory(&image, memory, &memory_bytes, fuzz_int32_t_4, memory, fuzz_int32_t_5, NULL);

    // Clean up
    delete[] memory;
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    assert_file_closed(&in_file);;

    assert_file_closed(&in_file);
	return 0;
}