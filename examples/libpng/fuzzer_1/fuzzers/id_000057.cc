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
//<ID> 2777
//<Prompt> ["png_get_bKGD","png_set_read_fn","png_get_y_offset_microns","png_get_libpng_ver","png_set_bKGD","png_set_text_compression_mem_level"]
/*<Combination>: [png_uint_32 png_get_bKGD(png_const_structrp png_ptr, png_inforp info_ptr, png_color_16p * background),
    void png_set_read_fn(png_structrp png_ptr, png_voidp io_ptr, png_rw_ptr read_data_fn),
    png_int_32 png_get_y_offset_microns(png_const_structrp png_ptr, png_const_inforp info_ptr),
    png_const_charp png_get_libpng_ver(png_const_structrp png_ptr),
    void png_set_bKGD(png_const_structrp png_ptr, png_inforp info_ptr, png_const_color_16p background),
    void png_set_text_compression_mem_level(png_structrp png_ptr, int mem_level)
*/
//<score> 26, nr_unique_branch: 1
//<Quality> {"density":13,"unique_branches":{"png_set_bKGD":[[30,47,30,65,0,0,4,0]]},"library_calls":["png_create_read_struct","png_create_info_struct","png_destroy_read_struct","png_set_longjmp_fn","png_destroy_read_struct","png_set_read_fn","png_get_io_ptr","png_get_io_ptr","png_init_io","png_read_info","png_get_bKGD","png_get_y_offset_microns","png_get_libpng_ver","png_set_bKGD","png_set_text_compression_mem_level","png_destroy_read_struct"],"critical_calls":["png_create_read_struct","png_create_info_struct","png_set_longjmp_fn","png_set_read_fn","png_get_io_ptr","png_get_io_ptr","png_init_io","png_read_info","png_get_bKGD","png_get_y_offset_microns","png_get_libpng_ver","png_set_bKGD","png_set_text_compression_mem_level","png_destroy_read_struct"],"visited":0}
/*Here is the C++ program that implements the steps described above:

*/


extern "C" int LLVMFuzzerTestOneInput_57(const uint8_t* f_data, size_t f_size) {
	if(f_size<17) return 0;

	FUZZ_FILE_INIT();
	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
	//fuzzer shim end}




    FILE *input_file = fmemopen((void *)data, size, "rb");
	FUZZ_FILE_PUSH(input_file)
    if (!input_file) {
        assert_file_closed(&input_file);
	return 0;
    }

    png_structp png_ptr = png_create_read_struct(fuzz_str_1, NULL, NULL, NULL);
    if (!png_ptr) {
        assert_file_closed(&input_file);;
        assert_file_closed(&input_file);
	return 0;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        assert_file_closed(&input_file);;
        assert_file_closed(&input_file);
	return 0;
    }

    if (setjmp(png_jmpbuf(png_ptr))) {
	FUZZ_FILE_CLOSE();
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        assert_file_closed(&input_file);;
        assert_file_closed(&input_file);
	return 0;
    }

    png_set_read_fn(png_ptr, reinterpret_cast<png_voidp>(input_file), 
        [](png_structp png_ptr, png_bytep data, size_t length) {
            FILE *fp = reinterpret_cast<FILE*>(png_get_io_ptr(png_ptr));
            fread(data, 1, length, fp);
        });

    png_init_io(png_ptr, input_file);
    png_read_info(png_ptr, info_ptr);

    // Calling the required libpng APIs
    png_color_16p background;
    png_get_bKGD(png_ptr, info_ptr, &background);

    png_int_32 y_offset_microns = png_get_y_offset_microns(png_ptr, info_ptr);

    png_const_charp libpng_version = png_get_libpng_ver(png_ptr);

    png_set_bKGD(png_ptr, info_ptr, background);

    png_set_text_compression_mem_level(png_ptr, fuzz_int32_t_2);

    // Clean up
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    assert_file_closed(&input_file);;

    assert_file_closed(&input_file);
	return 0;
}