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
//<ID> 125
//<Prompt> ["png_set_chunk_malloc_max","png_get_image_height","png_image_finish_read","png_write_chunk_start","png_get_pixel_aspect_ratio","png_save_uint_32","png_convert_from_struct_tm","png_get_uint_31","png_free"]
/*<Combination>: [void png_set_chunk_malloc_max(png_structrp png_ptr, png_alloc_size_t user_chunk_cache_max),
    png_uint_32 png_get_image_height(png_const_structrp png_ptr, png_const_inforp info_ptr),
    int png_image_finish_read(png_imagep image, png_const_colorp background, void * buffer, png_int_32 row_stride, void * colormap),
    void png_write_chunk_start(png_structrp png_ptr, png_const_bytep chunk_name, png_uint_32 length),
    float png_get_pixel_aspect_ratio(png_const_structrp png_ptr, png_const_inforp info_ptr),
    void png_save_uint_32(png_bytep buf, png_uint_32 i),
    void png_convert_from_struct_tm(png_timep ptime, const struct tm * ttime),
    png_uint_32 png_get_uint_31(png_const_structrp png_ptr, png_const_bytep buf),
    void png_free(png_const_structrp png_ptr, png_voidp ptr)
*/
//<score> 27, nr_unique_branch: 2
//<Quality> {"density":9,"unique_branches":{"png_check_chunk_length":[[3160,8,3160,46,0,0,4,1]],"png_set_chunk_malloc_max":[[1654,8,1654,23,0,0,4,0]]},"library_calls":["png_create_read_struct","png_create_info_struct","png_destroy_read_struct","png_set_longjmp_fn","png_destroy_read_struct","png_set_chunk_malloc_max","png_init_io","png_read_info","png_get_image_height","png_destroy_read_struct"],"critical_calls":["png_create_read_struct","png_create_info_struct","png_set_longjmp_fn","png_set_chunk_malloc_max","png_init_io","png_read_info","png_get_image_height","png_destroy_read_struct"],"visited":0}
/*Here is the C++ program that completes the steps mentioned above:
*/


extern "C" int LLVMFuzzerTestOneInput_1(const uint8_t* f_data, size_t f_size) {
	if(f_size<13) return 0;

	FUZZ_FILE_INIT();
	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
	//fuzzer shim end}




    // Step 4: Create the input file stream
    FILE *in_file = fmemopen((void *)data, size, "rb");
	FUZZ_FILE_PUSH(in_file)
    if (in_file == NULL) {
        assert_file_closed(&in_file);
	return 0;
    }
	FUZZ_FILENAME_PUSH("output_file")

    // Step 6: Define the output file name
    const char *output_file = "output_file";

    // Step 2: Initialize libpng read structures
    png_structp png_ptr = png_create_read_struct(fuzz_str_1, NULL, NULL, NULL);
    if (png_ptr == NULL) {
        assert_file_closed(&in_file);;
        assert_file_name_closed("output_file");
	assert_file_closed(&in_file);
	return 0;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == NULL) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        assert_file_closed(&in_file);;
        assert_file_name_closed("output_file");
	assert_file_closed(&in_file);
	return 0;
    }

    if (setjmp(png_jmpbuf(png_ptr))) {
	FUZZ_FILE_CLOSE();
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        assert_file_closed(&in_file);;
        assert_file_name_closed("output_file");
	assert_file_closed(&in_file);
	return 0;
    }

    // Step 1: Use png_set_chunk_malloc_max function
    png_set_chunk_malloc_max(png_ptr, UINT_MAX);

    // Step 3: Read PNG information from input file stream
    png_init_io(png_ptr, in_file);
    png_read_info(png_ptr, info_ptr);

    // Step 5: Use the png_get_image_height function
    png_uint_32 image_height = png_get_image_height(png_ptr, info_ptr);

    // Step 7: Release resources
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    assert_file_closed(&in_file);;

    assert_file_name_closed("output_file");
	assert_file_closed(&in_file);
	return 0;
}