#include "FDSan.h"
#include <png.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 1389
//<Prompt> ["png_image_write_to_memory","png_get_eXIf","png_handle_as_unknown","png_set_read_user_chunk_fn","png_set_keep_unknown_chunks","png_get_rows"]
/*<Combination>: [int png_image_write_to_memory(png_imagep image, void * memory, png_alloc_size_t *__restrict memory_bytes, int convert_to_8_bit, const void * buffer, png_int_32 row_stride, const void * colormap),
    png_uint_32 png_get_eXIf(png_const_structrp png_ptr, png_inforp info_ptr, png_bytep * exif),
    int png_handle_as_unknown(png_const_structrp png_ptr, png_const_bytep chunk_name),
    void png_set_read_user_chunk_fn(png_structrp png_ptr, png_voidp user_chunk_ptr, png_user_chunk_ptr read_user_chunk_fn),
    void png_set_keep_unknown_chunks(png_structrp png_ptr, int keep, png_const_bytep chunk_list, int num_chunks),
    png_bytepp png_get_rows(png_const_structrp png_ptr, png_const_inforp info_ptr)
*/
//<score> 6, nr_unique_branch: 2
//<Quality> {"density":3,"unique_branches":{"png_image_write_to_memory":[[2233,11,2233,31,0,0,4,0],[2233,35,2233,49,0,0,4,1]]},"library_calls":["png_image_begin_read_from_stdio","png_image_write_to_memory","png_image_free"],"critical_calls":["png_image_begin_read_from_stdio","png_image_write_to_memory","png_image_free"],"visited":0}
/**/

extern "C" int LLVMFuzzerTestOneInput_19(const uint8_t *data, size_t size) {
	if(size<0) return 0;

    // Create a png_image object
    png_image image;
    memset(&image, 0, sizeof(png_image));
    image.version = PNG_IMAGE_VERSION;

    // Set up output memory buffer
    void *memory = nullptr;
    png_alloc_size_t memory_bytes = 0;

    // Set up input file
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Read the PNG image from input file
    if (png_image_begin_read_from_stdio(&image, in_file) != 0) {
        // Set up output file
        FILE *out_file = fopen("output_file", "wb");
        if (!out_file) {
            assert_file_closed(&in_file);;
            assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
        }

        // Convert the image to 8-bit format
        int convert_to_8_bit = 1;

        // Set up colormap
        const void *colormap = nullptr;

        // Get the row stride of the image
        png_int_32 row_stride = PNG_IMAGE_ROW_STRIDE(image);

        // Write the PNG image to memory
        if (png_image_write_to_memory(&image, memory, &memory_bytes, convert_to_8_bit, nullptr, row_stride, colormap) != 0) {
            // Write the memory buffer to output file
            fwrite(memory, 1, memory_bytes, out_file);
        }

        // Clean up output file
        assert_file_closed(&out_file);;
    }

    // Clean up input file
    assert_file_closed(&in_file);;

    // Clean up memory buffer
    png_image_free(&image);

    assert_file_closed(&in_file);
	return 0;
}