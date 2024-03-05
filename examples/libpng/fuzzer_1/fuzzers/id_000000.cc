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
//<ID> 58
//<Prompt> ["png_get_eXIf","png_get_bit_depth","png_set_text_compression_strategy","png_set_pHYs","png_get_PLTE","png_set_compression_buffer_size"]
/*<Combination>: [png_uint_32 png_get_eXIf(png_const_structrp png_ptr, png_inforp info_ptr, png_bytep * exif),
    png_byte png_get_bit_depth(png_const_structrp png_ptr, png_const_inforp info_ptr),
    void png_set_text_compression_strategy(png_structrp png_ptr, int strategy),
    void png_set_pHYs(png_const_structrp png_ptr, png_inforp info_ptr, png_uint_32 res_x, png_uint_32 res_y, int unit_type),
    png_uint_32 png_get_PLTE(png_const_structrp png_ptr, png_inforp info_ptr, png_colorp * palette, int * num_palette),
    void png_set_compression_buffer_size(png_structrp png_ptr, size_t size)
*/
//<score> 52, nr_unique_branch: 10
//<Quality> {"density":13,"unique_branches":{"png_set_text_compression_strategy":[[1284,8,1284,23,0,0,4,1]],"png_get_PLTE":[[1028,8,1028,23,0,0,4,0],[1028,27,1028,43,0,0,4,0],[1029,8,1029,46,0,0,4,0],[1029,8,1029,46,0,0,4,1],[1029,50,1029,65,0,0,4,0]],"png_set_compression_buffer_size":[[1559,8,1559,23,0,0,4,1],[1562,8,1562,17,0,0,4,1],[1562,21,1562,43,0,0,4,1],[1566,8,1566,49,0,0,4,0]]},"library_calls":["png_create_read_struct","png_create_info_struct","png_destroy_read_struct","png_set_longjmp_fn","png_destroy_read_struct","png_init_io","png_read_info","png_get_eXIf","png_get_bit_depth","png_set_text_compression_strategy","png_set_pHYs","png_get_PLTE","png_set_compression_buffer_size","png_destroy_read_struct"],"critical_calls":["png_create_read_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_read_info","png_get_eXIf","png_get_bit_depth","png_set_text_compression_strategy","png_set_pHYs","png_get_PLTE","png_set_compression_buffer_size","png_destroy_read_struct"],"visited":0}
/*Here is the implementation of the C++ program that utilizes the libpng library APIs to achieve the specified event:

*/


extern "C" int LLVMFuzzerTestOneInput_0(const uint8_t* f_data, size_t f_size) {
	if(f_size<25) return 0;

	FUZZ_FILE_INIT();
	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_3, fdp);
	//fuzzer shim end}




    // Step 4: Open the input file
    FILE *in_file = fmemopen((void *)data, size, "rb");
	FUZZ_FILE_PUSH(in_file)
    if (!in_file) {
        assert_file_closed(&in_file);
	return 1;
    }

    // Step 5: Get the file descriptor
    int in_fd = fuzz_fileno(in_file);
	FUZZ_FD_PUSH(in_fd)

    // Step 6: Set the output file name
    const char* output_file = "output.png";

    png_structp png_ptr = NULL;
    png_infop info_ptr = NULL;

    // Step 1: Read the PNG file and retrieve the necessary information
    png_ptr = png_create_read_struct(fuzz_str_1, NULL, NULL, NULL);
    if (!png_ptr) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 1;
    }

    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 1;
    }

    if (setjmp(png_jmpbuf(png_ptr))) {
	FUZZ_FILE_CLOSE();
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 1;
    }

    png_init_io(png_ptr, in_file);
    png_read_info(png_ptr, info_ptr);

    // Step 2: Use the retrieved information
    png_uint_32 exif_data_length;
    png_bytep exif_data;
    exif_data_length = png_get_eXIf(png_ptr, info_ptr, &exif_data);

    png_byte bit_depth = png_get_bit_depth(png_ptr, info_ptr);

    png_set_text_compression_strategy(png_ptr, fuzz_int32_t_2);

    png_uint_32 res_x, res_y;
    int unit_type;
    png_set_pHYs(png_ptr, info_ptr, res_x, res_y, unit_type);

    png_colorp palette;
    int num_palette = 0;
    png_uint_32 palette_length = png_get_PLTE(png_ptr, info_ptr, &palette, &num_palette);

    png_set_compression_buffer_size(png_ptr, fuzz_uint64_t_3);

    // Step 3: Use the acquired information or perform desired operations

    // Step 7: Release resources
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    assert_file_closed(&in_file);;

    assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
}