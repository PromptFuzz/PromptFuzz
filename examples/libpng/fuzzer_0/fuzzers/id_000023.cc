#include "FDSan.h"
#include <png.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 1507
//<Prompt> ["png_image_begin_read_from_file","png_get_eXIf_1","png_permit_mng_features","png_get_mem_ptr","png_benign_error","png_get_gAMA"]
/*<Combination>: [int png_image_begin_read_from_file(png_imagep image, const char * file_name),
    png_uint_32 png_get_eXIf_1(png_const_structrp png_ptr, png_const_inforp info_ptr, png_uint_32 * num_exif, png_bytep * exif),
    png_uint_32 png_permit_mng_features(png_structrp png_ptr, png_uint_32 mng_features_permitted),
    png_voidp png_get_mem_ptr(png_const_structrp png_ptr),
    void png_benign_error(png_const_structrp png_ptr, png_const_charp warning_message),
    png_uint_32 png_get_gAMA(png_const_structrp png_ptr, png_const_inforp info_ptr, double * file_gamma)
*/
//<score> 18, nr_unique_branch: 1
//<Quality> {"density":9,"unique_branches":{"png_permit_mng_features":[[1324,8,1324,23,0,0,4,1]]},"library_calls":["png_image_begin_read_from_file","png_create_read_struct","png_image_free","png_create_info_struct","png_destroy_read_struct","png_image_free","png_set_longjmp_fn","png_destroy_read_struct","png_image_free","png_init_io","png_permit_mng_features","png_get_eXIf_1","png_get_gAMA","png_destroy_read_struct","png_image_free"],"critical_calls":["png_image_begin_read_from_file","png_create_read_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_permit_mng_features","png_get_eXIf_1","png_get_gAMA","png_destroy_read_struct","png_image_free"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_23(const uint8_t *data, size_t size) {
	if(size<0) return 0;

	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);

    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file == nullptr) {
        assert_file_closed(&in_file);
	return 0;
    }

    png_image image;
    memset(&image, 0, sizeof(image));
    image.version = PNG_IMAGE_VERSION;
    if (png_image_begin_read_from_file(&image, "input_file") == 0) {
        assert_file_closed(&in_file);;
        assert_file_name_closed("input_file");
	assert_file_closed(&in_file);
	return 0;
    }

    png_bytep exif;
    png_uint_32 num_exif;
    png_uint_32 mng_features_permitted = 0;
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (png_ptr == nullptr) {
        png_image_free(&image);
        assert_file_closed(&in_file);;
        assert_file_name_closed("input_file");
	assert_file_closed(&in_file);
	return 0;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == nullptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        png_image_free(&image);
        assert_file_closed(&in_file);;
        assert_file_name_closed("input_file");
	assert_file_closed(&in_file);
	return 0;
    }

    if (setjmp(png_jmpbuf(png_ptr)) != 0) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        png_image_free(&image);
        assert_file_closed(&in_file);;
        assert_file_name_closed("input_file");
	assert_file_closed(&in_file);
	return 0;
    }

    png_init_io(png_ptr, in_file);
    png_permit_mng_features(png_ptr, mng_features_permitted);
    png_bytep *exif_data_ptr = &exif;
    if (png_get_eXIf_1(png_ptr, info_ptr, &num_exif, exif_data_ptr) != 0) {
        free(*exif_data_ptr);
    }

    double file_gamma;
    if (png_get_gAMA(png_ptr, info_ptr, &file_gamma) != 0) {
        // Perform operations with file_gamma
    }

    // Cleanup
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    png_image_free(&image);
    assert_file_closed(&in_file);;
    
    assert_file_name_closed("input_file");
	assert_file_closed(&in_file);
	return 0;
}