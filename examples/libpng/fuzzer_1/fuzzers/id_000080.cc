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
//<ID> 3827
//<Prompt> ["png_write_chunk","png_destroy_info_struct","png_set_read_user_transform_fn","png_get_tRNS","png_get_image_height","png_get_user_width_max","png_image_begin_read_from_memory","png_set_tRNS_to_alpha","png_chunk_benign_error"]
/*<Combination>: [void png_write_chunk(png_structrp png_ptr, png_const_bytep chunk_name, png_const_bytep data, size_t length),
    void png_destroy_info_struct(png_const_structrp png_ptr, png_infopp info_ptr_ptr),
    void png_set_read_user_transform_fn(png_structrp png_ptr, png_user_transform_ptr read_user_transform_fn),
    png_uint_32 png_get_tRNS(png_const_structrp png_ptr, png_inforp info_ptr, png_bytep * trans_alpha, int * num_trans, png_color_16p * trans_color),
    png_uint_32 png_get_image_height(png_const_structrp png_ptr, png_const_inforp info_ptr),
    png_uint_32 png_get_user_width_max(png_const_structrp png_ptr),
    int png_image_begin_read_from_memory(png_imagep image, png_const_voidp memory, size_t size),
    void png_set_tRNS_to_alpha(png_structrp png_ptr),
    void png_chunk_benign_error(png_const_structrp png_ptr, png_const_charp warning_message)
*/
//<score> 48, nr_unique_branch: 11
//<Quality> {"density":12,"unique_branches":{"png_set_tRNS_to_alpha":[[930,8,930,37,0,0,4,1]],"png_image_begin_read_from_memory":[[1573,25,1573,60,0,0,4,1],[1597,13,1597,26,0,0,4,0]],"png_get_tRNS":[[1109,8,1109,46,0,0,4,0],[1113,11,1113,57,0,0,4,0],[1113,11,1113,57,0,0,4,1],[1115,14,1115,33,0,0,4,0],[1121,14,1121,33,0,0,4,0],[1127,14,1127,33,0,0,4,0],[1133,14,1133,33,0,0,4,0],[1137,11,1137,28,0,0,4,0]]},"library_calls":["png_create_read_struct","png_create_info_struct","png_destroy_read_struct","png_set_longjmp_fn","png_destroy_read_struct","png_init_io","png_read_info","png_get_tRNS","png_get_image_height","png_get_user_width_max","png_image_begin_read_from_memory","png_set_tRNS_to_alpha","png_chunk_benign_error","png_destroy_read_struct"],"critical_calls":["png_create_read_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_read_info","png_get_tRNS","png_get_image_height","png_get_user_width_max","png_image_begin_read_from_memory","png_set_tRNS_to_alpha","png_chunk_benign_error","png_destroy_read_struct"],"visited":0}
/**/

extern "C" int LLVMFuzzerTestOneInput_80(const uint8_t* f_data, size_t f_size) {
	if(f_size<36) return 0;

	FUZZ_FILE_INIT();
	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
	//fuzzer shim end}




    FILE *in_file = fmemopen((void*)data, size, "rb");
	FUZZ_FILE_PUSH(in_file)
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    png_structp png_ptr = png_create_read_struct(fuzz_str_1, NULL, NULL, NULL);
    if (!png_ptr) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
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
    png_read_info(png_ptr, info_ptr);

    // Use the libpng APIs here
    png_bytep trans_alpha;
    int num_trans;
    png_color_16p trans_color;
    png_get_tRNS(png_ptr, info_ptr, &trans_alpha, &num_trans, &trans_color);

    png_uint_32 image_height = png_get_image_height(png_ptr, info_ptr);
    png_uint_32 user_width_max = png_get_user_width_max(png_ptr);

    png_image image;
    memset(&image, 0, sizeof(image));
    png_image_begin_read_from_memory(&image, data, size);

    png_set_tRNS_to_alpha(png_ptr);

    png_chunk_benign_error(png_ptr, fuzz_str_2);

    // Clean up
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    assert_file_closed(&in_file);;

    assert_file_closed(&in_file);
	return 0;
}