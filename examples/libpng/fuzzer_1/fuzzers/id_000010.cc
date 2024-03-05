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
//<ID> 519
//<Prompt> ["png_set_PLTE","png_set_bKGD","png_image_write_to_stdio","png_read_info","png_set_swap_alpha","png_convert_from_struct_tm","png_get_unknown_chunks"]
/*<Combination>: [void png_set_PLTE(png_structrp png_ptr, png_inforp info_ptr, png_const_colorp palette, int num_palette),
    void png_set_bKGD(png_const_structrp png_ptr, png_inforp info_ptr, png_const_color_16p background),
    int png_image_write_to_stdio(png_imagep image, FILE * file, int convert_to_8_bit, const void * buffer, png_int_32 row_stride, const void * colormap),
    void png_read_info(png_structrp png_ptr, png_inforp info_ptr),
    void png_set_swap_alpha(png_structrp png_ptr),
    void png_convert_from_struct_tm(png_timep ptime, const struct tm * ttime),
    int png_get_unknown_chunks(png_const_structrp png_ptr, png_inforp info_ptr, png_unknown_chunkpp entries)
*/
//<score> 48, nr_unique_branch: 6
//<Quality> {"density":12,"unique_branches":{"png_set_PLTE":[[575,27,575,65,0,0,4,0],[577,11,577,57,0,0,4,0]],"png_image_write_to_stdio":[[2297,8,2297,21,0,0,4,0],[2297,25,2297,60,0,0,4,1],[2333,13,2333,26,0,0,4,0]],"png_set_swap_alpha":[[227,8,227,23,0,0,4,1]]},"library_calls":["png_create_read_struct","png_create_info_struct","png_destroy_read_struct","png_set_longjmp_fn","png_destroy_read_struct","png_init_io","png_read_info","png_set_PLTE","png_set_bKGD","png_convert_from_struct_tm","png_set_swap_alpha","png_get_unknown_chunks","png_set_longjmp_fn","png_destroy_read_struct","png_image_write_to_stdio","png_destroy_read_struct"],"critical_calls":["png_create_read_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_read_info","png_set_PLTE","png_set_bKGD","png_convert_from_struct_tm","png_set_swap_alpha","png_get_unknown_chunks","png_set_longjmp_fn","png_image_write_to_stdio","png_destroy_read_struct"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_10(const uint8_t* f_data, size_t f_size) {
	if(f_size<13) return 0;

	FUZZ_FILE_INIT();
	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
	//fuzzer shim end}




    png_image image;
    memset(&image, 0, sizeof(image));

    // Create input file
    FILE *in_file = fmemopen((void *)data, size, "rb");
	FUZZ_FILE_PUSH(in_file)
    if (in_file == NULL) {
        assert_file_closed(&in_file);
	return 0;
    }
	FUZZ_FILENAME_PUSH("output_file")

    // Create output file
    FILE *out_file = fopen("output_file", "wb");
	FUZZ_FILE_PUSH(out_file)
    if (out_file == NULL) {
        assert_file_closed(&in_file);;
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Read PNG info
    png_structp png_ptr = png_create_read_struct(fuzz_str_1, NULL, NULL, NULL);
    if (png_ptr == NULL) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == NULL) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    if (setjmp(png_jmpbuf(png_ptr))) {
	FUZZ_FILE_CLOSE();
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    png_init_io(png_ptr, in_file);
    png_read_info(png_ptr, info_ptr);

    // Set PLTE (palette)
    png_color palette[256];
    int num_palette = 256;
    // Fill palette with some values...

    png_set_PLTE(png_ptr, info_ptr, palette, num_palette);

    // Set bKGD (background color)
    png_color_16 background_color;
    // Set background_color values...

    png_set_bKGD(png_ptr, info_ptr, &background_color);

    // Convert time to png_time structure
    struct tm ttime;
    // Set ttime values...

    png_time ptime;
    png_convert_from_struct_tm(&ptime, &ttime);

    // Set swap alpha
    png_set_swap_alpha(png_ptr);

    // Get unknown chunks
    png_unknown_chunkpp entries;
    int num_unknowns = 0;

    png_get_unknown_chunks(png_ptr, info_ptr, entries);

    // Write PNG image to output file
    if (setjmp(png_jmpbuf(png_ptr))) {
	FUZZ_FILE_CLOSE();
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    png_image_write_to_stdio(&image, out_file, 0, NULL, 0, NULL);

    // Cleanup
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

    assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}