#include "FDSan.h"
#include <png.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 1867
//<Prompt> ["png_get_bit_depth","png_get_copyright","png_set_rows","png_get_current_pass_number","png_get_sCAL_fixed"]
/*<Combination>: [png_byte png_get_bit_depth(png_const_structrp png_ptr, png_const_inforp info_ptr),
    png_const_charp png_get_copyright(png_const_structrp png_ptr),
    void png_set_rows(png_const_structrp png_ptr, png_inforp info_ptr, png_bytepp row_pointers),
    png_byte png_get_current_pass_number(png_const_structrp ),
    png_uint_32 png_get_sCAL_fixed(png_const_structrp png_ptr, png_const_inforp info_ptr, int * unit, png_fixed_point * width, png_fixed_point * height)
*/
//<score> 48, nr_unique_branch: 6
//<Quality> {"density":12,"unique_branches":{"png_fixed":[[3320,8,3320,23,0,0,4,0]],"png_get_sCAL_fixed":[[936,8,936,46,0,0,4,0]],"png_fixed_error":[[580,8,580,20,0,0,4,0],[581,14,581,42,0,0,4,0],[581,46,581,60,0,0,4,0],[581,46,581,60,0,0,4,1]]},"library_calls":["png_create_read_struct","png_create_info_struct","png_destroy_read_struct","png_set_longjmp_fn","png_destroy_read_struct","png_init_io","png_read_info","png_get_bit_depth","png_get_copyright","png_get_rows","png_get_current_pass_number","png_get_sCAL_fixed","png_destroy_read_struct"],"critical_calls":["png_create_read_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_read_info","png_get_bit_depth","png_get_copyright","png_get_rows","png_get_current_pass_number","png_get_sCAL_fixed","png_destroy_read_struct"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_32(const uint8_t *data, size_t size) {
	if(size<0) return 0;

	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);

	FUZZ_FILE_INIT();
    // Step 4: Create input and output file
    FILE *in_file = fmemopen((void *)data, size, "rb");
	FUZZ_FILENAME_PUSH("output_file")
	FUZZ_FILE_PUSH(in_file)
    FILE *out_file = fopen("output_file", "wb");
	FUZZ_FILE_PUSH(out_file)

    if (!in_file || !out_file) {
        // Failed to create file
        if (in_file) assert_file_closed(&in_file);;
        if (out_file) assert_file_closed(&out_file);;
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Step 5: Get file descriptors
    int in_fd = fuzz_fileno(in_file);
	FUZZ_FD_PUSH(in_fd)
    int out_fd = fuzz_fileno(out_file);
	FUZZ_FILENAME_PUSH("input_file")
	FUZZ_FD_PUSH(out_fd)

    // Step 6: File names
    const char* in_file_name = "input_file";
	FUZZ_FILENAME_PUSH("output_file")
    const char* out_file_name = "output_file";

    // Step 1: Use libpng APIs to achieve the event
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }

    if (setjmp(png_jmpbuf(png_ptr))) {
	FUZZ_FILE_CLOSE();
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }

    png_init_io(png_ptr, in_file);
    png_read_info(png_ptr, info_ptr);

    png_byte bit_depth = png_get_bit_depth(png_ptr, info_ptr);
    png_const_charp copyright = png_get_copyright(png_ptr);
    png_bytepp row_pointers = png_get_rows(png_ptr, info_ptr);
    png_byte current_pass_number = png_get_current_pass_number(png_ptr);
    int unit;
    png_fixed_point width, height;
    png_get_sCAL_fixed(png_ptr, info_ptr, &unit, &width, &height);

    // Perform operations with the obtained data
    // ...

    // Step 7: Clean up
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;

    assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}