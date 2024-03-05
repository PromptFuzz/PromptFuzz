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
//<ID> 294
//<Prompt> ["png_set_strip_16","png_get_x_offset_microns","png_set_bKGD","png_get_rgb_to_gray_status","png_set_mem_fn","png_image_finish_read","png_write_chunk_data","png_set_expand_gray_1_2_4_to_8"]
/*<Combination>: [void png_set_strip_16(png_structrp png_ptr),
    png_int_32 png_get_x_offset_microns(png_const_structrp png_ptr, png_const_inforp info_ptr),
    void png_set_bKGD(png_const_structrp png_ptr, png_inforp info_ptr, png_const_color_16p background),
    png_byte png_get_rgb_to_gray_status(png_const_structrp png_ptr),
    void png_set_mem_fn(png_structrp png_ptr, png_voidp mem_ptr, png_malloc_ptr malloc_fn, png_free_ptr free_fn),
    int png_image_finish_read(png_imagep image, png_const_colorp background, void * buffer, png_int_32 row_stride, void * colormap),
    void png_write_chunk_data(png_structrp png_ptr, png_const_bytep data, size_t length),
    void png_set_expand_gray_1_2_4_to_8(png_structrp png_ptr)
*/
//<score> 42, nr_unique_branch: 8
//<Quality> {"density":14,"unique_branches":{"png_get_x_offset_microns":[[251,8,251,46,0,0,4,0],[255,11,255,62,0,0,4,0]],"png_image_finish_read":[[4108,25,4108,60,0,0,4,0],[4122,11,4122,47,0,0,4,0],[4127,14,4127,29,0,0,4,0],[4130,14,4130,28,0,0,4,1],[4140,14,4140,35,0,0,4,1],[2880,5,2880,37,6,0,4,1]]},"library_calls":["png_create_read_struct","png_create_info_struct","png_destroy_read_struct","png_set_longjmp_fn","png_destroy_read_struct","png_init_io","png_read_info","png_set_strip_16","png_get_x_offset_microns","png_set_bKGD","png_get_rgb_to_gray_status","png_set_mem_fn","png_image_finish_read","png_write_chunk_data","png_set_expand_gray_1_2_4_to_8","png_destroy_read_struct"],"critical_calls":["png_create_read_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_read_info","png_set_strip_16","png_get_x_offset_microns","png_set_bKGD","png_get_rgb_to_gray_status","png_set_mem_fn","png_image_finish_read","png_write_chunk_data","png_set_expand_gray_1_2_4_to_8","png_destroy_read_struct"],"visited":0}
/*Here is the implementation of the C++ program using the libpng library APIs:

*/


extern "C" int LLVMFuzzerTestOneInput_5(const uint8_t* f_data, size_t f_size) {
	if(f_size<25) return 0;

	FUZZ_FILE_INIT();
	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_3, fdp);
	//fuzzer shim end}




    FILE *in_file = fmemopen((void *)data, size, "rb");
	FUZZ_FILE_PUSH(in_file)
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    png_structp png_ptr = png_create_read_struct(fuzz_str_1, nullptr, nullptr, nullptr);
    if (!png_ptr) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    if (setjmp(png_jmpbuf(png_ptr))) {
	FUZZ_FILE_CLOSE();
        // Error handling
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    png_init_io(png_ptr, in_file);
    png_read_info(png_ptr, info_ptr);

    // Call the libpng APIs to achieve the desired event
    png_set_strip_16(png_ptr);
    png_get_x_offset_microns(png_ptr, info_ptr);
    png_color_16 background_color;
    png_set_bKGD(png_ptr, info_ptr, &background_color);
    png_get_rgb_to_gray_status(png_ptr);
    png_set_mem_fn(png_ptr, nullptr, nullptr, nullptr);
    png_image image;
    image.version = PNG_IMAGE_VERSION;
    png_image_finish_read(&image, nullptr, nullptr, fuzz_int32_t_2, nullptr);
    png_write_chunk_data(png_ptr, nullptr, fuzz_uint64_t_3);
    png_set_expand_gray_1_2_4_to_8(png_ptr);

    // Clean up resources
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    assert_file_closed(&in_file);;

    assert_file_closed(&in_file);
	return 0;
}