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
//<ID> 734
//<Prompt> ["png_image_finish_read","png_get_hIST","png_set_invert_mono","png_set_error_fn","png_access_version_number","png_set_filter_heuristics_fixed","png_set_check_for_invalid_index","png_set_invalid"]
/*<Combination>: [int png_image_finish_read(png_imagep image, png_const_colorp background, void * buffer, png_int_32 row_stride, void * colormap),
    png_uint_32 png_get_hIST(png_const_structrp png_ptr, png_inforp info_ptr, png_uint_16p * hist),
    void png_set_invert_mono(png_structrp png_ptr),
    void png_set_error_fn(png_structrp png_ptr, png_voidp error_ptr, png_error_ptr error_fn, png_error_ptr warning_fn),
    png_uint_32 png_access_version_number(),
    void png_set_filter_heuristics_fixed(png_structrp png_ptr, int heuristic_method, int num_weights, png_const_fixed_point_p filter_weights, png_const_fixed_point_p filter_costs),
    void png_set_check_for_invalid_index(png_structrp png_ptr, int allowed),
    void png_set_invalid(png_const_structrp png_ptr, png_inforp info_ptr, int mask)
*/
//<score> 100, nr_unique_branch: 182
//<Quality> {"density":4,"unique_branches":{"png_build_16to8_table":[[4064,16,4064,23,0,0,4,0],[4064,16,4064,23,0,0,4,1],[4085,16,4085,23,0,0,4,0],[4085,16,4085,23,0,0,4,1],[4096,14,4096,26,0,0,4,0],[4096,14,4096,26,0,0,4,1],[4104,11,4104,28,0,0,4,0],[4104,11,4104,28,0,0,4,1]],"png_do_scale_16_to_8":[[2393,8,2393,33,0,0,4,0],[2399,14,2399,21,0,0,4,0],[2399,14,2399,21,0,0,4,1]],"png_do_read_transformations":[[4772,11,4772,57,0,0,4,0],[4868,8,4868,57,0,0,4,0],[4869,8,4869,53,0,0,4,0],[4917,8,4917,59,0,0,4,0],[4953,8,4953,57,0,0,4,0],[4954,8,4954,53,0,0,4,1],[4997,8,4997,52,0,0,4,0]],"png_build_gamma_table":[[4277,11,4277,78,0,0,4,0],[4283,14,4283,45,0,0,4,0],[4297,11,4297,78,0,0,4,0],[4299,11,4299,36,0,0,4,0]],"png_read_start_row":[[4457,11,4457,56,0,0,4,0],[4459,14,4459,37,0,0,4,0],[4459,14,4459,37,0,0,4,1],[4505,8,4505,54,0,0,4,0],[4507,11,4507,53,0,0,4,0],[4507,11,4507,53,0,0,4,1],[4509,14,4509,34,0,0,4,0],[4509,14,4509,34,0,0,4,1],[4516,16,4516,57,0,0,4,0],[4516,16,4516,57,0,0,4,1],[4517,10,4517,55,0,0,4,0],[4519,14,4519,35,0,0,4,0],[4519,14,4519,35,0,0,4,1],[4529,8,4529,57,0,0,4,0],[4533,12,4533,35,0,0,4,0],[4533,12,4533,35,0,0,4,1],[4534,11,4534,55,0,0,4,0],[4537,11,4537,57,0,0,4,0],[4537,11,4537,57,0,0,4,1],[4539,11,4539,59,0,0,4,0],[4541,14,4541,35,0,0,4,0],[4541,14,4541,35,0,0,4,1]],"png_image_write_init":[[1487,8,1487,23,0,0,4,0],[1491,11,1491,27,0,0,4,0],[1496,14,1496,29,0,0,4,0]],"png_do_expand_palette":[[4220,8,4220,54,0,0,4,0],[4222,11,4222,34,0,0,4,0],[4222,11,4222,34,0,0,4,1],[4226,13,4226,19,0,0,4,0],[4226,13,4226,19,0,0,4,1],[4231,28,4231,41,0,0,4,0],[4231,28,4231,41,0,0,4,1],[4233,23,4233,44,0,0,4,0],[4233,23,4233,44,0,0,4,1],[4239,23,4239,33,0,0,4,0],[4239,23,4239,33,0,0,4,1],[4253,13,4253,19,0,0,4,0],[4253,13,4253,19,0,0,4,1],[4258,28,4258,41,0,0,4,0],[4258,28,4258,41,0,0,4,1],[4262,23,4262,33,0,0,4,0],[4262,23,4262,33,0,0,4,1],[4276,13,4276,19,0,0,4,0],[4276,13,4276,19,0,0,4,1],[4281,28,4281,41,0,0,4,0],[4281,28,4281,41,0,0,4,1],[4285,23,4285,33,0,0,4,0],[4285,23,4285,33,0,0,4,1],[4299,13,4299,20,0,0,4,1],[4307,11,4307,35,0,0,4,0],[4310,17,4310,30,0,0,4,0],[4310,17,4310,30,0,0,4,1],[4330,23,4330,36,0,0,4,0],[4330,23,4330,36,0,0,4,1],[4332,23,4332,46,0,0,4,0],[4332,23,4332,46,0,0,4,1],[4360,23,4360,36,0,0,4,0],[4360,23,4360,36,0,0,4,1]],"png_set_add_alpha":[[209,8,209,23,0,0,4,1],[214,8,214,52,0,0,4,0]],"png_compress_IDAT":[[1000,17,1000,53,0,0,4,1],[1016,29,1016,48,0,0,4,1]],"png_read_transform_info":[[1947,11,1947,57,0,0,4,0],[1953,14,1953,36,0,0,4,0],[1953,14,1953,36,0,0,4,1],[1962,14,1962,38,0,0,4,1],[2006,14,2006,65,0,0,4,0],[2041,8,2041,57,0,0,4,0],[2102,8,2102,52,0,0,4,0],[2103,9,2103,51,0,0,4,0],[2108,11,2108,58,0,0,4,0]],"png_do_read_filler":[[2687,8,2687,51,0,0,4,1],[2764,13,2764,55,0,0,4,0],[2766,11,2766,35,0,0,4,0],[2768,14,2768,50,0,0,4,0],[2773,25,2773,38,0,0,4,0],[2773,25,2773,38,0,0,4,1]],"png_product2":[[3528,8,3528,24,0,0,4,0],[3528,28,3528,45,0,0,4,0]],"png_image_write_main":[[1955,17,1955,26,0,0,4,0],[1955,30,1955,63,0,0,4,1],[1956,16,1956,25,0,0,4,0],[1956,29,1956,61,0,0,4,0],[1957,22,1957,28,0,0,4,1],[1970,11,1970,47,0,0,4,0],[1975,14,1975,38,0,0,4,1],[1978,14,1978,37,0,0,4,1],[1984,14,1984,37,0,0,4,0],[1990,17,1990,59,0,0,4,1],[2003,8,2003,48,0,0,4,1],[2024,11,2024,22,0,0,4,1],[2025,12,2025,44,0,0,4,0],[2026,12,2026,44,0,0,4,0],[2035,8,2035,24,0,0,4,1],[2050,13,2050,69,0,0,4,0],[2067,8,2067,24,0,0,4,1],[2076,11,2076,46,0,0,4,1],[2085,11,2085,49,0,0,4,1],[2096,8,2096,21,0,0,4,1],[2100,8,2101,65,0,0,4,1],[2108,11,2108,22,0,0,4,1],[2111,11,2111,24,0,0,4,1],[2119,8,2119,49,0,0,4,1],[2136,9,2136,20,0,0,4,1],[2137,9,2137,22,0,0,4,0],[2137,26,2137,55,0,0,4,1],[2166,14,2166,19,0,0,4,0],[2166,14,2166,19,0,0,4,1],[2880,5,2880,37,39,0,4,1]],"png_image_finish_read":[[4127,14,4127,29,0,0,4,1]],"png_do_gamma":[[4005,17,4005,41,0,0,4,1],[4026,28,4026,41,0,0,4,0],[4026,28,4026,41,0,0,4,1]],"png_set_benign_errors":[[1672,8,1672,20,0,0,4,1]],"png_image_read_direct":[[3698,11,3698,48,0,0,4,0],[3701,14,3701,51,0,0,4,0],[3801,11,3801,49,0,0,4,0],[3803,14,3803,25,0,0,4,1],[3813,11,3813,48,0,0,4,0],[3819,14,3819,56,0,0,4,1],[3877,17,3877,28,0,0,4,1],[3884,17,3884,55,0,0,4,1],[4017,14,4017,61,0,0,4,0],[4018,13,4018,58,0,0,4,1]],"png_do_gray_to_rgb":[[2867,8,2867,32,0,0,4,0],[2868,8,2868,58,0,0,4,0],[2870,11,2870,54,0,0,4,0],[2870,11,2870,54,0,0,4,1],[2872,14,2872,38,0,0,4,0],[2872,14,2872,38,0,0,4,1],[2877,25,2877,38,0,0,4,0],[2877,25,2877,38,0,0,4,1],[2890,25,2890,38,0,0,4,0],[2890,25,2890,38,0,0,4,1],[2902,16,2902,65,0,0,4,0],[2904,14,2904,38,0,0,4,0],[2904,14,2904,38,0,0,4,1],[2909,25,2909,38,0,0,4,0],[2909,25,2909,38,0,0,4,1],[2923,25,2923,38,0,0,4,0],[2923,25,2923,38,0,0,4,1],[730,6,730,23,6,0,4,0]],"png_set_filler":[[124,8,124,23,0,0,4,1],[130,8,130,49,0,0,4,0],[196,8,196,38,0,0,4,0]],"png_image_free_function":[[4560,8,4560,24,0,0,4,0]],"png_init_read_transformations":[[1528,8,1528,73,0,0,4,0],[1529,8,1529,53,0,0,4,1],[1818,11,1818,56,0,0,4,0],[1821,14,1821,58,0,0,4,1],[1822,10,1822,59,0,0,4,0],[1833,22,1833,37,0,0,4,0],[1833,22,1833,37,0,0,4,1]],"png_set_scale_16":[[187,8,187,37,0,0,4,1]],"png_image_write_to_stdio":[[2297,25,2297,60,0,0,4,0],[2299,11,2299,23,0,0,4,0],[2299,27,2299,41,0,0,4,0],[2301,14,2301,46,0,0,4,0]],"png_gamma_16bit_correct":[[3914,8,3914,17,0,0,4,0],[3914,21,3914,34,0,0,4,0]]},"library_calls":["png_image_begin_read_from_stdio","png_image_finish_read","png_image_write_to_stdio","png_image_free"],"critical_calls":["png_image_begin_read_from_stdio","png_image_finish_read","png_image_write_to_stdio","png_image_free"],"visited":0}
/**/

extern "C" int LLVMFuzzerTestOneInput_13(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    FILE *in_file = fmemopen((void *)data, size, "rb");
    FILE *out_file = fopen("output_file", "wb");

    if (!in_file || !out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    png_image image;
    memset(&image, 0, sizeof(image));
    image.version = PNG_IMAGE_VERSION;

    // Read the image from the input file
    if (png_image_begin_read_from_stdio(&image, in_file) != 0) {
        png_bytep buffer;
        png_int_32 row_stride;

        image.format = PNG_FORMAT_RGBA;

        // Allocate memory for the image data
        size_t image_size = PNG_IMAGE_SIZE(image);
        buffer = (png_bytep)malloc(image_size);
        if (!buffer) {
            assert_file_closed(&in_file);;
            assert_file_closed(&out_file);;
            assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
        }
        row_stride = PNG_IMAGE_ROW_STRIDE(image);

        // Read the image data into the allocated buffer
        if (png_image_finish_read(&image, NULL, buffer, row_stride, NULL) != 0) {
            // Process the image data here

            // Write the image data to the output file
            if (png_image_write_to_stdio(&image, out_file, 0, buffer, row_stride, NULL) != 0) {
                assert_file_closed(&in_file);;
                assert_file_closed(&out_file);;
                free(buffer);
                assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
            }
        }

        // Free the allocated memory
        free(buffer);
    }

    // Clean up
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    png_image_free(&image);

    assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}