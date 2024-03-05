#include "FDSan.h"
#include <png.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 5246
//<Prompt> ["png_write_flush","png_set_filter","png_set_compression_buffer_size","png_get_uint_31","png_set_sCAL_fixed","png_set_strip_alpha","png_get_user_width_max","png_chunk_error"]
/*<Combination>: [void png_write_flush(png_structrp png_ptr),
    void png_set_filter(png_structrp png_ptr, int method, int filters),
    void png_set_compression_buffer_size(png_structrp png_ptr, size_t size),
    png_uint_32 png_get_uint_31(png_const_structrp png_ptr, png_const_bytep buf),
    void png_set_sCAL_fixed(png_const_structrp png_ptr, png_inforp info_ptr, int unit, png_fixed_point width, png_fixed_point height),
    void png_set_strip_alpha(png_structrp png_ptr),
    png_uint_32 png_get_user_width_max(png_const_structrp png_ptr),
    void png_chunk_error(png_const_structrp png_ptr, png_const_charp error_message)
*/
//<score> 735, nr_unique_branch: 279
//<Quality> {"density":21,"unique_branches":{"png_write_row":[[724,36,724,54,0,0,4,1],[773,8,773,32,0,0,4,0],[774,8,774,55,0,0,4,0],[778,10,778,16,0,0,4,0],[778,10,778,16,0,0,4,1],[779,17,779,50,0,0,4,0],[779,17,779,50,0,0,4,1],[786,10,786,16,0,0,4,0],[786,10,786,16,0,0,4,1],[787,17,787,50,0,0,4,0],[787,17,787,50,0,0,4,1],[787,54,787,72,0,0,4,1],[794,10,794,16,0,0,4,0],[794,10,794,16,0,0,4,1],[795,17,795,50,0,0,4,0],[795,17,795,50,0,0,4,1],[802,10,802,16,0,0,4,0],[802,10,802,16,0,0,4,1],[803,17,803,50,0,0,4,0],[803,17,803,50,0,0,4,1],[803,54,803,72,0,0,4,1],[810,10,810,16,0,0,4,0],[810,10,810,16,0,0,4,1],[811,17,811,50,0,0,4,0],[811,17,811,50,0,0,4,1],[818,10,818,16,0,0,4,0],[818,10,818,16,0,0,4,1],[819,17,819,50,0,0,4,0],[819,17,819,50,0,0,4,1],[819,54,819,72,0,0,4,1],[826,10,826,16,0,0,4,0],[826,10,826,16,0,0,4,1],[827,17,827,50,0,0,4,0],[827,17,827,50,0,0,4,1],[834,10,834,17,0,0,4,1],[860,8,860,27,0,0,4,0],[860,31,860,48,0,0,4,0],[860,31,860,48,0,0,4,1],[861,8,861,55,0,0,4,0],[865,11,865,30,0,0,4,1],[907,8,907,53,0,0,4,0],[908,8,908,37,0,0,4,0],[730,6,730,23,4,0,4,1]],"png_write_info":[[205,8,205,46,0,0,4,0],[213,8,213,45,0,0,4,0],[217,11,217,61,0,0,4,1],[236,8,236,46,0,0,4,0],[241,8,241,46,0,0,4,0],[254,8,254,46,0,0,4,0],[260,8,260,46,0,0,4,0],[267,8,267,46,0,0,4,0],[273,8,273,46,0,0,4,0],[279,8,279,46,0,0,4,0],[294,16,294,38,0,0,4,0],[299,11,299,44,0,0,4,0],[299,11,299,44,0,0,4,1],[310,14,310,72,0,0,4,1],[320,16,320,74,0,0,4,0],[320,16,320,74,0,0,4,1],[333,16,333,74,0,0,4,0]],"png_write_oFFs":[[1731,8,1731,36,0,0,4,1]],"png_compress_IDAT":[[941,11,941,40,0,0,4,1]],"png_write_end":[[384,11,384,49,0,0,4,0],[385,11,385,48,0,0,4,1],[391,19,391,41,0,0,4,0],[396,14,396,47,0,0,4,1],[416,19,416,77,0,0,4,1],[429,19,429,77,0,0,4,1],[445,11,445,49,0,0,4,0],[446,11,446,48,0,0,4,1]],"png_write_eXIf":[[1486,16,1486,28,0,0,4,0],[1486,16,1486,28,0,0,4,1]],"png_flush":[[73,8,73,40,0,0,4,0]],"png_write_sCAL_s":[[1821,8,1821,22,0,0,4,1]],"png_write_PLTE":[[851,25,851,72,0,0,4,0],[856,8,856,72,0,0,4,0],[858,8,858,20,0,0,4,1],[858,25,858,53,0,0,4,1],[872,8,872,57,0,0,4,1],[886,35,886,46,0,0,4,0],[886,35,886,46,0,0,4,1]],"png_write_png":[[1360,8,1360,23,0,0,4,1],[1360,27,1360,43,0,0,4,1],[1363,8,1363,46,0,0,4,1],[1375,8,1375,53,0,0,4,1],[1385,8,1385,47,0,0,4,1],[1394,8,1394,49,0,0,4,1],[1402,8,1402,52,0,0,4,1],[1413,8,1414,48,0,0,4,1],[1435,8,1435,45,0,0,4,1],[1443,8,1443,53,0,0,4,1],[1451,8,1451,50,0,0,4,1],[1459,8,1459,54,0,0,4,1]],"png_write_bKGD":[[1420,8,1420,44,0,0,4,0],[1420,8,1420,44,0,0,4,1],[1424,12,1424,37,0,0,4,0],[1427,10,1427,45,0,0,4,1],[1437,13,1437,53,0,0,4,0],[1437,13,1437,53,0,0,4,1],[1443,11,1443,34,0,0,4,0],[1443,11,1443,34,0,0,4,1],[1443,38,1443,69,0,0,4,1],[1460,11,1460,50,0,0,4,1]],"png_write_finish_row":[[2034,8,2034,32,0,0,4,0],[2037,11,2037,58,0,0,4,0],[2070,11,2070,28,0,0,4,0],[2070,11,2070,28,0,0,4,1],[2072,14,2072,39,0,0,4,0],[2072,14,2072,39,0,0,4,1],[730,6,730,23,4,0,4,0]],"png_do_write_interlace":[[2109,8,2109,16,0,0,4,0],[2114,10,2114,16,0,0,4,0],[2114,10,2114,16,0,0,4,1],[2128,44,2128,57,0,0,4,0],[2128,44,2128,57,0,0,4,1],[2135,20,2135,30,0,0,4,0],[2135,20,2135,30,0,0,4,1],[2146,17,2146,27,0,0,4,0],[2146,17,2146,27,0,0,4,1],[2152,10,2152,16,0,0,4,0],[2152,10,2152,16,0,0,4,1],[2166,44,2166,57,0,0,4,0],[2166,44,2166,57,0,0,4,1],[2173,20,2173,30,0,0,4,0],[2173,20,2173,30,0,0,4,1],[2183,17,2183,27,0,0,4,1],[2189,10,2189,16,0,0,4,0],[2189,10,2189,16,0,0,4,1],[2202,44,2202,57,0,0,4,0],[2202,44,2202,57,0,0,4,1],[2209,20,2209,30,0,0,4,0],[2209,20,2209,30,0,0,4,1],[2219,17,2219,27,0,0,4,1],[2225,10,2225,17,0,0,4,0],[2225,10,2225,17,0,0,4,1],[2240,44,2240,57,0,0,4,0],[2240,44,2240,57,0,0,4,1],[2247,20,2247,28,0,0,4,0],[2247,20,2247,28,0,0,4,1],[730,6,730,23,2,0,4,0],[730,6,730,23,2,0,4,1]],"png_text_compress":[[505,8,505,19,0,0,4,1],[531,14,531,34,0,0,4,0],[538,14,538,45,0,0,4,1],[581,14,581,27,0,0,4,1],[589,14,589,25,0,0,4,1],[602,11,602,53,0,0,4,1],[617,11,617,30,0,0,4,0],[617,34,617,48,0,0,4,0]],"png_write_info_before_PLTE":[[170,14,170,52,0,0,4,0]],"png_write_pCAL":[[1757,8,1757,33,0,0,4,1],[1762,8,1762,24,0,0,4,1],[1768,33,1768,45,0,0,4,1],[1778,16,1778,27,0,0,4,0],[1778,16,1778,27,0,0,4,1],[1780,44,1780,60,0,0,4,0],[1780,44,1780,60,0,0,4,1],[1796,16,1796,27,0,0,4,0],[1796,16,1796,27,0,0,4,1]],"png_image_size":[[213,11,213,35,0,0,4,0],[223,34,223,41,0,0,4,0],[223,34,223,41,0,0,4,1],[227,17,227,23,0,0,4,0],[730,6,730,23,2,0,4,0],[730,6,730,23,2,0,4,1],[2459,35,2459,43,4,0,4,0],[2459,35,2459,43,4,0,4,1],[2459,35,2459,43,6,0,4,0],[2459,35,2459,43,6,0,4,1],[2458,35,2458,43,7,0,4,0],[2458,35,2458,43,7,0,4,1],[2458,35,2458,43,9,0,4,0],[2458,35,2458,43,9,0,4,1]],"png_write_start_row":[[1943,8,1944,32,0,0,4,1],[1977,8,1977,32,0,0,4,0],[1979,11,1979,58,0,0,4,1],[730,6,730,23,2,0,4,1]],"png_default_flush":[[83,8,83,23,0,0,4,1]],"png_write_pHYs":[[1847,8,1847,40,0,0,4,1]],"png_write_tIME":[[1869,8,1869,29,0,0,4,1],[1869,33,1869,53,0,0,4,1],[1870,8,1870,29,0,0,4,1],[1870,33,1870,53,0,0,4,1],[1871,8,1871,29,0,0,4,1],[1871,33,1871,54,0,0,4,1]],"png_write_compressed_data_out":[[646,11,646,29,0,0,4,0],[653,11,653,26,0,0,4,0],[662,8,662,22,0,0,4,1]],"png_write_flush":[[942,8,942,48,0,0,4,1]],"png_write_zTXt":[[1582,8,1582,48,0,0,4,1],[1588,8,1588,48,0,0,4,1],[1593,8,1593,20,0,0,4,1],[1602,8,1602,20,0,0,4,1],[1604,8,1604,68,0,0,4,1]],"png_write_image":[[638,31,638,50,0,0,4,0]],"png_write_find_filter":[[2609,10,2609,41,0,0,4,1],[2642,13,2642,49,0,0,4,1]],"png_write_iTXt":[[1636,8,1636,20,0,0,4,1],[1642,7,1642,37,0,0,4,1],[1643,7,1643,37,0,0,4,1],[1647,7,1647,37,0,0,4,1],[1648,7,1648,37,0,0,4,0],[1652,7,1652,14,0,0,4,1],[1669,8,1669,20,0,0,4,1],[1671,8,1671,24,0,0,4,1],[1673,8,1673,20,0,0,4,1],[1676,8,1676,45,0,0,4,1],[1681,8,1681,49,0,0,4,1],[1688,8,1688,24,0,0,4,0],[1690,11,1690,74,0,0,4,1],[1711,8,1711,24,0,0,4,0]],"png_do_write_transformations":[[543,8,543,56,0,0,4,1]],"png_write_IHDR":[[687,13,687,19,0,0,4,0],[688,13,688,19,0,0,4,0],[689,13,689,19,0,0,4,0],[713,7,713,34,0,0,4,0],[716,13,716,19,0,0,4,0],[716,13,716,19,0,0,4,1],[717,13,717,19,0,0,4,0],[717,13,717,19,0,0,4,1],[718,13,718,19,0,0,4,0],[718,13,718,19,0,0,4,1],[719,13,719,19,0,0,4,0],[719,13,719,19,0,0,4,1],[723,13,723,20,0,0,4,1],[784,8,784,44,0,0,4,0],[785,8,785,45,0,0,4,1],[826,11,826,56,0,0,4,0],[827,11,827,33,0,0,4,0],[730,6,730,23,19,0,4,1]],"png_write_tRNS":[[1355,8,1355,44,0,0,4,0],[1355,8,1355,44,0,0,4,1],[1357,11,1357,25,0,0,4,1],[1357,29,1357,66,0,0,4,1],[1369,13,1369,46,0,0,4,0],[1369,13,1369,46,0,0,4,1],[1372,11,1372,50,0,0,4,1],[1384,13,1384,45,0,0,4,0],[1391,11,1391,34,0,0,4,0],[1391,11,1391,34,0,0,4,1],[1391,38,1391,69,0,0,4,1]],"png_deflate_claim":[[334,11,334,28,0,0,4,1],[339,19,339,56,0,0,4,1],[388,11,388,63,0,0,4,0],[389,11,389,43,0,0,4,0],[395,14,395,51,0,0,4,1]],"png_write_sBIT":[[1271,8,1271,48,0,0,4,0],[1275,28,1275,62,0,0,4,0],[1275,28,1275,62,0,0,4,1],[1278,11,1278,25,0,0,4,1],[1278,29,1278,48,0,0,4,1],[1279,11,1279,27,0,0,4,1],[1279,31,1279,52,0,0,4,1],[1280,11,1280,26,0,0,4,1],[1280,30,1280,50,0,0,4,1],[1304,8,1304,48,0,0,4,0],[1304,8,1304,48,0,0,4,1],[1306,11,1306,27,0,0,4,1],[1306,31,1306,67,0,0,4,1]],"png_check_keyword":[[1728,8,1728,19,0,0,4,1],[1734,11,1734,15,0,0,4,0],[1734,11,1734,15,0,0,4,1],[1734,19,1734,31,0,0,4,0],[1738,12,1738,19,0,0,4,0],[1738,12,1738,19,0,0,4,1],[1738,23,1738,32,0,0,4,0],[1738,37,1738,65,0,0,4,1],[1743,16,1743,26,0,0,4,0],[1751,14,1751,22,0,0,4,1],[1759,8,1759,19,0,0,4,0],[1759,23,1759,33,0,0,4,1],[1769,8,1769,20,0,0,4,1],[1774,8,1774,17,0,0,4,1],[1777,13,1777,31,0,0,4,1]],"png_write_tEXt":[[1540,8,1540,20,0,0,4,1],[1543,8,1543,20,0,0,4,1],[1543,24,1543,37,0,0,4,1],[1549,8,1549,48,0,0,4,1],[1563,8,1563,21,0,0,4,0]]},"library_calls":["png_create_read_struct","png_create_info_struct","png_destroy_read_struct","png_set_longjmp_fn","png_destroy_read_struct","png_init_io","png_set_sig_bytes","png_read_png","png_get_image_width","png_get_image_height","png_create_write_struct","png_destroy_read_struct","png_create_info_struct","png_destroy_read_struct","png_destroy_write_struct","png_destroy_read_struct","png_destroy_write_struct","png_init_io","png_write_png","png_write_flush","png_destroy_read_struct","png_destroy_write_struct"],"critical_calls":["png_create_read_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_set_sig_bytes","png_read_png","png_get_image_width","png_get_image_height","png_create_write_struct","png_create_info_struct","png_init_io","png_write_png","png_write_flush","png_destroy_read_struct","png_destroy_write_struct"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_109(const uint8_t *data, size_t size) {
	if(size<0) return 0;

	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);

	FUZZ_FILE_INIT();
    // Step 4: Open the input file using fmemopen to create a FILE *
    FILE *in_file = fmemopen((void *)data, size, "rb");
	FUZZ_FILE_PUSH(in_file)
    if (in_file == NULL) {
        assert_file_closed(&in_file);
	return 0;
    }
	FUZZ_FILENAME_PUSH("input_file")

    // Step 6: Assign the input file name
    const char *input_file = "input_file";
	FUZZ_FILENAME_PUSH("output_file")

    // Step 7: Assign the output file name
    const char *output_file = "output_file";

    // Step 8: Create a PNG read struct
    png_structp png_read_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (png_read_ptr == NULL) {
        assert_file_closed(&in_file);;
        assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_closed(&in_file);
	return 0;
    }

    // Step 9: Create a PNG info struct
    png_infop png_info_ptr = png_create_info_struct(png_read_ptr);
    if (png_info_ptr == NULL) {
        assert_file_closed(&in_file);;
        png_destroy_read_struct(&png_read_ptr, NULL, NULL);
        assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_closed(&in_file);
	return 0;
    }

    // Step 10: Set up error handling
    if (setjmp(png_jmpbuf(png_read_ptr))) {
	FUZZ_FILE_CLOSE();
        // Error handling code
        assert_file_closed(&in_file);;
        png_destroy_read_struct(&png_read_ptr, &png_info_ptr, NULL);
        assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_closed(&in_file);
	return 0;
    }

    // Step 11: Set up the read function using the input file and file descriptor
    png_init_io(png_read_ptr, in_file);
    png_set_sig_bytes(png_read_ptr, 0);

    // Step 12: Read the PNG image
    png_read_png(png_read_ptr, png_info_ptr, PNG_TRANSFORM_IDENTITY, NULL);

    // Step 13: Get the image width and height
    int width = png_get_image_width(png_read_ptr, png_info_ptr);
    int height = png_get_image_height(png_read_ptr, png_info_ptr);

    // Step 14: Create a PNG write struct
    png_structp png_write_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (png_write_ptr == NULL) {
        assert_file_closed(&in_file);;
        png_destroy_read_struct(&png_read_ptr, &png_info_ptr, NULL);
        assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_closed(&in_file);
	return 0;
    }

    // Step 15: Create a PNG info struct for writing
    png_infop png_write_info_ptr = png_create_info_struct(png_write_ptr);
    if (png_write_info_ptr == NULL) {
        assert_file_closed(&in_file);;
        png_destroy_read_struct(&png_read_ptr, &png_info_ptr, NULL);
        png_destroy_write_struct(&png_write_ptr, NULL);
        assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_closed(&in_file);
	return 0;
    }

    // Step 16: Create the output file using fopen
    FILE *out_file = fopen(output_file, "wb");
	FUZZ_FILE_PUSH(out_file)
    if (out_file == NULL) {
        assert_file_closed(&in_file);;
        png_destroy_read_struct(&png_read_ptr, &png_info_ptr, NULL);
        png_destroy_write_struct(&png_write_ptr, &png_write_info_ptr);
        assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Step 17: Set up the write function using the output file and file descriptor
    png_init_io(png_write_ptr, out_file);

    // Step 18: Write the PNG image
    png_write_png(png_write_ptr, png_info_ptr, PNG_TRANSFORM_IDENTITY, NULL);
    
    // Step 19: Flush the write operation
    png_write_flush(png_write_ptr);

    // Step 20: Cleanup and release resources
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    png_destroy_read_struct(&png_read_ptr, &png_info_ptr, NULL);
    png_destroy_write_struct(&png_write_ptr, &png_write_info_ptr);

    assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}