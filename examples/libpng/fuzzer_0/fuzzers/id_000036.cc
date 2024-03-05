#include "FDSan.h"
#include <png.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 1959
//<Prompt> ["png_get_pHYs","png_set_keep_unknown_chunks","png_set_compression_mem_level","png_get_image_height","png_info_init_3","png_get_y_offset_inches_fixed","png_chunk_error","png_get_y_offset_microns","png_start_read_image"]
/*<Combination>: [png_uint_32 png_get_pHYs(png_const_structrp png_ptr, png_const_inforp info_ptr, png_uint_32 * res_x, png_uint_32 * res_y, int * unit_type),
    void png_set_keep_unknown_chunks(png_structrp png_ptr, int keep, png_const_bytep chunk_list, int num_chunks),
    void png_set_compression_mem_level(png_structrp png_ptr, int mem_level),
    png_uint_32 png_get_image_height(png_const_structrp png_ptr, png_const_inforp info_ptr),
    void png_info_init_3(png_infopp info_ptr, size_t png_info_struct_size),
    png_fixed_point png_get_y_offset_inches_fixed(png_const_structrp png_ptr, png_const_inforp info_ptr),
    void png_chunk_error(png_const_structrp png_ptr, png_const_charp error_message),
    png_int_32 png_get_y_offset_microns(png_const_structrp png_ptr, png_const_inforp info_ptr),
    void png_start_read_image(png_structrp png_ptr)
*/
//<score> 2, nr_unique_branch: 5
//<Quality> {"density":1,"unique_branches":{"png_get_y_offset_microns":[[270,8,270,23,0,0,4,0],[270,27,270,43,0,0,4,0],[271,8,271,46,0,0,4,0],[271,8,271,46,0,0,4,1],[275,11,275,62,0,0,4,0]]},"library_calls":[],"critical_calls":[],"visited":0}
/**/

// Event: Read the PNG file and extract information about the image size, resolution, and offset.
void  id_000036_read_png_info(FILE *pngfile) {
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        std::cerr << "Error: Failed to create png read struct" << std::endl;
        return;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        std::cerr << "Error: Failed to create png info struct" << std::endl;
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return;
    }

    // Set the error handling function
    if (setjmp(png_jmpbuf(png_ptr))) {
        std::cerr << "Error: An error occurred while reading the png file" << std::endl;
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return;
    }

    // Set up the input/output functions
    png_init_io(png_ptr, pngfile);

    // Read the header information
    png_read_info(png_ptr, info_ptr);

    // Get the image height
    png_uint_32 height = png_get_image_height(png_ptr, info_ptr);
    std::cout << "Image height: " << height << std::endl;

    // Get the resolution
    png_uint_32 res_x, res_y;
    int unit_type;
    png_get_pHYs(png_ptr, info_ptr, &res_x, &res_y, &unit_type);
    std::cout << "Resolution: " << res_x << "x" << res_y << " pixels per " << unit_type << std::endl;

    // Get the offset in inches and microns
    png_fixed_point offset_inches_fixed = png_get_y_offset_inches_fixed(png_ptr, info_ptr);
    png_int_32 offset_microns = png_get_y_offset_microns(png_ptr, info_ptr);
    std::cout << "Offset (inches_fixed): " << offset_inches_fixed << std::endl;
    std::cout << "Offset (microns): " << offset_microns << std::endl;

    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
}

extern "C" int LLVMFuzzerTestOneInput_36(const uint8_t *data, size_t size) {
	if(size<0) return 0;

    // Open the input file
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        std::cerr << "Error: Failed to open input file" << std::endl;
        assert_file_closed(&in_file);
	return 0;
    }

    // Call the event function
     id_000036_read_png_info(in_file);

    // Close the input file
    assert_file_closed(&in_file);;

    assert_file_closed(&in_file);
	return 0;
}