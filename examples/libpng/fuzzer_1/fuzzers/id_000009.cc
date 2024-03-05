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
//<ID> 437
//<Prompt> ["png_set_oFFs","png_sig_cmp","png_start_read_image","png_get_color_type","png_free_default","png_get_io_chunk_type","png_get_io_state"]
/*<Combination>: [void png_set_oFFs(png_const_structrp png_ptr, png_inforp info_ptr, png_int_32 offset_x, png_int_32 offset_y, int unit_type),
    int png_sig_cmp(png_const_bytep sig, size_t start, size_t num_to_check),
    void png_start_read_image(png_structrp png_ptr),
    png_byte png_get_color_type(png_const_structrp png_ptr, png_const_inforp info_ptr),
    void png_free_default(png_const_structrp png_ptr, png_voidp ptr),
    png_uint_32 png_get_io_chunk_type(png_const_structrp png_ptr),
    png_uint_32 png_get_io_state(png_const_structrp png_ptr)
*/
//<score> 52, nr_unique_branch: 3
//<Quality> {"density":13,"unique_branches":{"png_free_default":[[251,27,251,38,0,0,4,0]],"png_sig_cmp":[[81,13,81,29,0,0,4,0]],"png_read_start_row":[[4418,11,4418,58,0,0,4,0]]},"library_calls":["png_create_read_struct","png_create_info_struct","png_destroy_read_struct","png_set_longjmp_fn","png_destroy_read_struct","png_init_io","png_read_info","png_set_oFFs","png_sig_cmp","png_start_read_image","png_get_color_type","png_free_default","png_get_io_chunk_type","png_get_io_state","png_destroy_read_struct"],"critical_calls":["png_create_read_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_read_info","png_set_oFFs","png_sig_cmp","png_start_read_image","png_get_color_type","png_free_default","png_get_io_chunk_type","png_get_io_state","png_destroy_read_struct"],"visited":0}
/**/



extern "C" int LLVMFuzzerTestOneInput_9(const uint8_t* f_data, size_t f_size) {
	if(f_size<33) return 0;

	FUZZ_FILE_INIT();
	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_5, fdp);
	//fuzzer shim end}




    FILE *in_file = fmemopen((void *)data, size, "rb");
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
        assert_file_closed(&in_file);;
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        assert_file_closed(&in_file);
	return 0;
    }

    if (setjmp(png_jmpbuf(png_ptr))) {
	FUZZ_FILE_CLOSE();
        assert_file_closed(&in_file);;
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        assert_file_closed(&in_file);
	return 0;
    }

    png_init_io(png_ptr, in_file);
    png_read_info(png_ptr, info_ptr);

    png_set_oFFs(png_ptr, info_ptr, fuzz_int32_t_2, fuzz_int32_t_3, fuzz_int32_t_4);
    png_sig_cmp(NULL, fuzz_uint64_t_5, 0); // Just calling the API, does nothing for our purpose
    png_start_read_image(png_ptr);
    png_byte color_type = png_get_color_type(png_ptr, info_ptr);
    png_free_default(png_ptr, NULL); // Just calling the API, does nothing for our purpose
    png_uint_32 chunk_type = png_get_io_chunk_type(png_ptr);
    png_uint_32 io_state = png_get_io_state(png_ptr);

    assert_file_closed(&in_file);;
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

    assert_file_closed(&in_file);
	return 0;
}