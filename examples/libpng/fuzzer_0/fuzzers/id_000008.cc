#include "FDSan.h"
#include <png.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 412
//<Prompt> ["png_set_compression_method","png_set_user_transform_info","png_longjmp","png_info_init_3","png_get_io_ptr","png_set_compression_strategy","png_get_iCCP","png_chunk_error"]
/*<Combination>: [void png_set_compression_method(png_structrp png_ptr, int method),
    void png_set_user_transform_info(png_structrp png_ptr, png_voidp user_transform_ptr, int user_transform_depth, int user_transform_channels),
    void png_longjmp(png_const_structrp png_ptr, int val),
    void png_info_init_3(png_infopp info_ptr, size_t png_info_struct_size),
    png_voidp png_get_io_ptr(png_const_structrp png_ptr),
    void png_set_compression_strategy(png_structrp png_ptr, int strategy),
    png_uint_32 png_get_iCCP(png_const_structrp png_ptr, png_inforp info_ptr, png_charpp name, int * compression_type, png_bytepp profile, png_uint_32 * proflen),
    void png_chunk_error(png_const_structrp png_ptr, png_const_charp error_message)
*/
//<score> 7, nr_unique_branch: 0
//<Quality> {"density":7,"unique_branches":{},"library_calls":["png_create_read_struct","png_create_info_struct","png_destroy_read_struct","png_set_longjmp_fn","png_destroy_read_struct","png_set_read_fn","png_get_io_ptr","png_get_io_ptr","png_read_png","png_destroy_read_struct"],"critical_calls":["png_create_read_struct","png_create_info_struct","png_set_longjmp_fn","png_set_read_fn","png_get_io_ptr","png_get_io_ptr","png_read_png","png_destroy_read_struct"],"visited":0}
/*Here is the C++ program that achieves the event using the libpng APIs:

*/


extern "C" int LLVMFuzzerTestOneInput_8(const uint8_t *data, size_t size) {
	if(size<0) return 0;

	FUZZ_FILE_INIT();
    FILE *in_file = fmemopen((void *)data, size, "rb");
	FUZZ_FILE_PUSH(in_file)
    if (in_file == NULL) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    png_struct *png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (png_ptr == NULL) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    png_info *info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == NULL) {
        assert_file_closed(&in_file);;
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        assert_file_closed(&in_file);
	return 0;
    }
    
    if (setjmp(png_jmpbuf(png_ptr))) {
	FUZZ_FILE_CLOSE();
        assert_file_closed(&in_file);;
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        assert_file_closed(&in_file);
	return 0;
    }
    
    png_set_read_fn(png_ptr, (png_voidp)in_file, [](png_structrp png_ptr, png_bytep data, png_size_t length) {
        FILE *file = (FILE *)png_get_io_ptr(png_ptr);
        fread(data, 1, length, file);
    });
    
    png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, NULL);
    
    assert_file_closed(&in_file);;
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    
    assert_file_closed(&in_file);
	return 0;
}