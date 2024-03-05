#include "FDSan.h"
#include <png.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 855
//<Prompt> ["png_access_version_number","png_get_oFFs","png_malloc","png_read_update_info","png_create_info_struct","png_write_chunk_end","png_set_filter_heuristics"]
/*<Combination>: [png_uint_32 png_access_version_number(),
    png_uint_32 png_get_oFFs(png_const_structrp png_ptr, png_const_inforp info_ptr, png_int_32 * offset_x, png_int_32 * offset_y, int * unit_type),
    png_voidp png_malloc(png_const_structrp png_ptr, png_alloc_size_t size),
    void png_read_update_info(png_structrp png_ptr, png_inforp info_ptr),
    png_infop png_create_info_struct(png_const_structrp png_ptr),
    void png_write_chunk_end(png_structrp png_ptr),
    void png_set_filter_heuristics(png_structrp png_ptr, int heuristic_method, int num_weights, png_const_doublep filter_weights, png_const_doublep filter_costs)
*/
//<score> 24, nr_unique_branch: 3
//<Quality> {"density":12,"unique_branches":{"png_get_oFFs":[[886,8,886,23,0,0,4,0],[886,27,886,43,0,0,4,0],[887,8,887,46,0,0,4,1]]},"library_calls":["png_create_read_struct","png_create_info_struct","png_destroy_read_struct","png_destroy_read_struct","png_init_io","png_read_update_info","png_get_oFFs","png_malloc","png_destroy_read_struct","png_set_filter_heuristics","png_free","png_destroy_read_struct"],"critical_calls":["png_create_read_struct","png_create_info_struct","png_init_io","png_read_update_info","png_get_oFFs","png_malloc","png_set_filter_heuristics","png_free","png_destroy_read_struct"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_14(const uint8_t *data, size_t size) {
	if(size<0) return 0;

    // Create a PNG structure
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        return 0;
    }

    // Create an info structure
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return 0;
    }

    // Create a FILE pointer from the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        assert_file_closed(&in_file);
	return 0;
    }

    // Set the read function for libpng to use the FILE pointer
    png_init_io(png_ptr, in_file);

    // Read the PNG file
    png_read_update_info(png_ptr, info_ptr);

    // Get the offset and unit type
    png_int_32 offset_x, offset_y;
    int unit_type;
    png_get_oFFs(png_ptr, info_ptr, &offset_x, &offset_y, &unit_type);

    // Allocate memory using libpng's memory allocation function
    png_voidp ptr = png_malloc(png_ptr, size);
    if (!ptr) {
        assert_file_closed(&in_file);;
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        assert_file_closed(&in_file);
	return 0;
    }

    // Set the filter heuristics
    double filter_weights[] = {0.25, 0.25, 0.25, 0.25};
    double filter_costs[] = {1.0, 1.0, 1.0, 1.0};
    png_set_filter_heuristics(png_ptr, 0, 4, filter_weights, filter_costs);

    // Call other libpng APIs as needed to achieve the desired event

    // Release resources
    png_free(png_ptr, ptr);
    assert_file_closed(&in_file);;
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

    assert_file_closed(&in_file);
	return 0;
}