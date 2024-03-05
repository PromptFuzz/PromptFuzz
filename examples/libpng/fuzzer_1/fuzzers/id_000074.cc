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
//<ID> 3569
//<Prompt> ["png_get_pixels_per_inch","png_get_cHRM","png_set_cHRM_XYZ","png_access_version_number","png_get_color_type","png_get_gAMA_fixed"]
/*<Combination>: [png_uint_32 png_get_pixels_per_inch(png_const_structrp png_ptr, png_const_inforp info_ptr),
    png_uint_32 png_get_cHRM(png_const_structrp png_ptr, png_const_inforp info_ptr, double * white_x, double * white_y, double * red_x, double * red_y, double * green_x, double * green_y, double * blue_x, double * blue_y),
    void png_set_cHRM_XYZ(png_const_structrp png_ptr, png_inforp info_ptr, double red_X, double red_Y, double red_Z, double green_X, double green_Y, double green_Z, double blue_X, double blue_Y, double blue_Z),
    png_uint_32 png_access_version_number(),
    png_byte png_get_color_type(png_const_structrp png_ptr, png_const_inforp info_ptr),
    png_uint_32 png_get_gAMA_fixed(png_const_structrp png_ptr, png_const_inforp info_ptr, png_fixed_point * int_file_gamma)
*/
//<score> 22, nr_unique_branch: 1
//<Quality> {"density":11,"unique_branches":{"png_colorspace_endpoints_match":[[758,35,758,60,1,0,4,0]]},"library_calls":["png_create_read_struct","png_create_info_struct","png_set_longjmp_fn","png_destroy_read_struct","png_init_io","png_read_info","png_get_pixels_per_inch","png_get_cHRM","png_set_cHRM_XYZ","png_access_version_number","png_get_color_type","png_get_gAMA_fixed","png_destroy_read_struct"],"critical_calls":["png_create_read_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_read_info","png_get_pixels_per_inch","png_get_cHRM","png_set_cHRM_XYZ","png_access_version_number","png_get_color_type","png_get_gAMA_fixed","png_destroy_read_struct"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_74(const uint8_t* f_data, size_t f_size)
{
	if(f_size<13) return 0;

	FUZZ_FILE_INIT();
	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
	//fuzzer shim end}




    FILE *in_file = fmemopen((void *)data, size, "rb");
	FUZZ_FILE_PUSH(in_file)
    
    if (!in_file)
    {
        assert_file_closed(&in_file);
	return 0;
    }
    
    png_structp png_ptr = png_create_read_struct(fuzz_str_1, NULL, NULL, NULL);
    png_infop info_ptr = png_create_info_struct(png_ptr);
    
    if (!png_ptr || !info_ptr)
    {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    if (setjmp(png_jmpbuf(png_ptr)))
    {
	FUZZ_FILE_CLOSE();
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    png_init_io(png_ptr, in_file);
    png_read_info(png_ptr, info_ptr);

    // Use libpng APIs to achieve the desired event

    png_uint_32 pixels_per_inch = png_get_pixels_per_inch(png_ptr, info_ptr);

    double white_x, white_y, red_x, red_y, green_x, green_y, blue_x, blue_y;
    png_uint_32 cHRM_result = png_get_cHRM(png_ptr, info_ptr, &white_x, &white_y, &red_x, &red_y, &green_x, &green_y, &blue_x, &blue_y);

    double red_X = 0.6400, red_Y = 0.3300, red_Z = 0.0300;
    double green_X = 0.3000, green_Y = 0.6000, green_Z = 0.1000;
    double blue_X = 0.1500, blue_Y = 0.0600, blue_Z = 0.7900;
    png_set_cHRM_XYZ(png_ptr, info_ptr, red_X, red_Y, red_Z, green_X, green_Y, green_Z, blue_X, blue_Y, blue_Z);
    
    png_uint_32 version = png_access_version_number();

    png_byte color_type = png_get_color_type(png_ptr, info_ptr);

    png_fixed_point int_file_gamma;
    png_uint_32 gAMA_result = png_get_gAMA_fixed(png_ptr, info_ptr, &int_file_gamma);

    // Clean up resources
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    assert_file_closed(&in_file);;

    assert_file_closed(&in_file);
	return 0;
}