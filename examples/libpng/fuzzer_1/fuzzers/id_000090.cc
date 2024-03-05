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
//<ID> 4250
//<Prompt> ["png_write_chunk_start","png_get_tIME","png_convert_to_rfc1123","png_get_y_pixels_per_inch","png_get_x_offset_pixels","png_get_gAMA"]
/*<Combination>: [void png_write_chunk_start(png_structrp png_ptr, png_const_bytep chunk_name, png_uint_32 length),
    png_uint_32 png_get_tIME(png_const_structrp png_ptr, png_inforp info_ptr, png_timep * mod_time),
    png_const_charp png_convert_to_rfc1123(png_structrp png_ptr, png_const_timep ptime),
    png_uint_32 png_get_y_pixels_per_inch(png_const_structrp png_ptr, png_const_inforp info_ptr),
    png_int_32 png_get_x_offset_pixels(png_const_structrp png_ptr, png_const_inforp info_ptr),
    png_uint_32 png_get_gAMA(png_const_structrp png_ptr, png_const_inforp info_ptr, double * file_gamma)
*/
//<score> 48, nr_unique_branch: 18
//<Quality> {"density":12,"unique_branches":{"png_format_number":[[145,11,145,22,0,0,4,1],[163,10,163,36,0,0,4,0],[168,10,168,34,0,0,4,0],[178,10,178,34,0,0,4,1]],"png_convert_to_rfc1123_buffer":[[747,8,747,25,0,0,4,1],[747,33,747,50,0,0,4,1],[748,8,748,25,0,0,4,1],[748,33,748,50,0,0,4,1],[749,8,749,25,0,0,4,1],[749,33,749,51,0,0,4,1],[750,8,750,26,0,0,4,1],[760,29,760,37,2,0,4,0],[760,29,760,37,4,0,4,0],[760,29,760,37,6,0,4,0],[760,29,760,37,8,0,4,0],[760,29,760,37,10,0,4,0]],"png_convert_to_rfc1123":[[793,8,793,23,0,0,4,0],[796,11,796,74,0,0,4,1]]},"library_calls":["png_create_read_struct","png_create_info_struct","png_destroy_read_struct","png_set_longjmp_fn","png_destroy_read_struct","png_init_io","png_read_png","png_get_tIME","png_convert_to_rfc1123","png_get_y_pixels_per_inch","png_get_x_offset_pixels","png_get_gAMA","png_destroy_read_struct"],"critical_calls":["png_create_read_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_read_png","png_get_tIME","png_convert_to_rfc1123","png_get_y_pixels_per_inch","png_get_x_offset_pixels","png_get_gAMA","png_destroy_read_struct"],"visited":0}
/*Here is the C++ program that uses the libpng library APIs to achieve the specified event:

*/


extern "C" int LLVMFuzzerTestOneInput_90(const uint8_t* f_data, size_t f_size) {
	if(f_size<17) return 0;

	FUZZ_FILE_INIT();
	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
	//fuzzer shim end}




    // Step 4: Create input file from the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
	FUZZ_FILE_PUSH(in_file)
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Step 5: Get file descriptor for reading
    int in_fd = fuzz_fileno(in_file);
	FUZZ_FD_PUSH(in_fd)

    // Initialize libpng structures
    png_structp png_ptr = png_create_read_struct(fuzz_str_1, nullptr, nullptr, nullptr);
    if (!png_ptr) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
    }

    // Step 2: Use libpng APIs to achieve the event
    // Read the PNG file
    if (setjmp(png_jmpbuf(png_ptr))) {
	FUZZ_FILE_CLOSE();
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
    }

    png_init_io(png_ptr, in_file);
    png_read_png(png_ptr, info_ptr, fuzz_int32_t_2, nullptr);

    // Get the tIME chunk
    png_timep mod_time;
    if (png_get_tIME(png_ptr, info_ptr, &mod_time)) {
        // Convert tIME to RFC1123 format
        png_const_charp time_str = png_convert_to_rfc1123(png_ptr, mod_time);
        if (time_str) {
            // Print the converted time string
            printf("Modified time: %s\n", time_str);
        }
    }

    // Get the Y pixels per inch
    png_uint_32 y_pixels_per_inch = png_get_y_pixels_per_inch(png_ptr, info_ptr);
    printf("Y pixels per inch: %u\n", y_pixels_per_inch);

    // Get the X offset in pixels
    png_int_32 x_offset_pixels = png_get_x_offset_pixels(png_ptr, info_ptr);
    printf("X offset in pixels: %d\n", x_offset_pixels);

    // Get the file gamma
    double file_gamma;
    if (png_get_gAMA(png_ptr, info_ptr, &file_gamma)) {
        printf("File gamma: %f\n", file_gamma);
    }

    // Step 7: Release allocated resources
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    assert_file_closed(&in_file);;

    assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
}