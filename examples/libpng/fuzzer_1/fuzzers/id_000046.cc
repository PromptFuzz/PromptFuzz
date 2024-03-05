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
//<ID> 2196
//<Prompt> ["png_create_write_struct","png_get_pHYs","png_save_uint_16","png_set_expand","png_get_gAMA_fixed"]
/*<Combination>: [png_structp png_create_write_struct(png_const_charp user_png_ver, png_voidp error_ptr, png_error_ptr error_fn, png_error_ptr warn_fn),
    png_uint_32 png_get_pHYs(png_const_structrp png_ptr, png_const_inforp info_ptr, png_uint_32 * res_x, png_uint_32 * res_y, int * unit_type),
    void png_save_uint_16(png_bytep buf, unsigned int i),
    void png_set_expand(png_structrp png_ptr),
    png_uint_32 png_get_gAMA_fixed(png_const_structrp png_ptr, png_const_inforp info_ptr, png_fixed_point * int_file_gamma)
*/
//<score> 10, nr_unique_branch: 1
//<Quality> {"density":5,"unique_branches":{"png_get_gAMA_fixed":[[694,27,694,43,0,0,4,1]]},"library_calls":["png_create_write_struct","png_set_pHYs","png_save_uint_16","png_set_expand","png_get_gAMA_fixed","png_destroy_write_struct"],"critical_calls":["png_create_write_struct","png_set_pHYs","png_save_uint_16","png_set_expand","png_get_gAMA_fixed","png_destroy_write_struct"],"visited":0}
/*1. The event that can be achieved using the libpng APIs mentioned is to create a PNG write structure, set the physical pixel dimensions, expand the image to 8-bit depth, and retrieve the fixed point gamma value.

2. Here is the updated LLVMFuzzerTestOneInput_46 function that achieves the desired event:

*/


extern "C" int LLVMFuzzerTestOneInput_46(const uint8_t* f_data, size_t f_size) {
	if(f_size<29) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_5, fdp);
	//fuzzer shim end}




    png_structp png_ptr = NULL;
    png_bytep row = NULL;
    FILE *in_file = NULL;
    FILE *out_file = NULL;
    int result = 1;

    // Step 4: Create a FILE pointer for reading the input data
    in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        return 0;
    }

    // Step 4: Create a FILE pointer for writing output data
    out_file = fopen("output_file.png", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_name_closed("output_file.png");
	return 0;
    }

    // Step 6: Get the file descriptor for reading or writing
    int in_file_fd = fuzz_fileno(in_file);
    int out_file_fd = fuzz_fileno(out_file);

    // Step 1: Create a PNG write structure
    png_ptr = png_create_write_struct(fuzz_str_1, NULL, NULL, NULL);
    if (!png_ptr) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_name_closed("output_file.png");
	assert_fd_closed(out_file_fd);
	assert_fd_closed(in_file_fd);
	return 0;
    }

    // Step 2: Set the physical pixel dimensions
    png_uint_32 res_x = 300;
    png_uint_32 res_y = 300;
    int unit_type = PNG_RESOLUTION_METER;
    png_set_pHYs(png_ptr, NULL, fuzz_uint32_t_2, fuzz_uint32_t_3, fuzz_int32_t_4);

    // Step 3: Save an unsigned int as a 16-bit value
    png_byte buf[2];
    unsigned int i = 65535;
    png_save_uint_16(buf, fuzz_uint32_t_5);

    // Step 3: Set the image expand flag
    png_set_expand(png_ptr);

    // Step 5: Get the fixed point gamma value
    png_fixed_point gamma;
    png_get_gAMA_fixed(png_ptr, NULL, &gamma);

    // Cleanup
    png_destroy_write_struct(&png_ptr, NULL);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;

    assert_file_name_closed("output_file.png");
	assert_fd_closed(out_file_fd);
	assert_fd_closed(in_file_fd);
	return 0;
}