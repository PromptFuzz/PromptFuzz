#include "FDSan.h"
#include <png.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 1520
//<Prompt> ["png_get_chunk_malloc_max","png_get_eXIf","png_malloc","png_set_longjmp_fn","png_get_pHYs_dpi","png_save_uint_32","png_set_gamma_fixed","png_set_chunk_malloc_max"]
/*<Combination>: [png_alloc_size_t png_get_chunk_malloc_max(png_const_structrp png_ptr),
    png_uint_32 png_get_eXIf(png_const_structrp png_ptr, png_inforp info_ptr, png_bytep * exif),
    png_voidp png_malloc(png_const_structrp png_ptr, png_alloc_size_t size),
    jmp_buf *png_set_longjmp_fn(png_structrp png_ptr, png_longjmp_ptr longjmp_fn, size_t jmp_buf_size),
    png_uint_32 png_get_pHYs_dpi(png_const_structrp png_ptr, png_const_inforp info_ptr, png_uint_32 * res_x, png_uint_32 * res_y, int * unit_type),
    void png_save_uint_32(png_bytep buf, png_uint_32 i),
    void png_set_gamma_fixed(png_structrp png_ptr, png_fixed_point screen_gamma, png_fixed_point override_file_gamma),
    void png_set_chunk_malloc_max(png_structrp png_ptr, png_alloc_size_t user_chunk_cache_max)
*/
//<score> 8, nr_unique_branch: 3
//<Quality> {"density":2,"unique_branches":{"png_set_chunk_malloc_max":[[1654,8,1654,23,0,0,4,1]],"png_set_gamma_fixed":[[823,8,823,37,0,0,4,0]],"png_get_pHYs_dpi":[[437,8,437,23,0,0,4,1]]},"library_calls":["png_get_chunk_malloc_max","png_get_eXIf","png_malloc","png_set_longjmp_fn","png_get_pHYs_dpi","png_save_uint_32","png_set_gamma_fixed","png_set_chunk_malloc_max"],"critical_calls":["png_get_chunk_malloc_max","png_get_eXIf","png_malloc","png_set_longjmp_fn","png_get_pHYs_dpi","png_save_uint_32","png_set_gamma_fixed","png_set_chunk_malloc_max"],"visited":0}
/*Here is the C++ program that follows the instructions provided:

*/


extern "C" int LLVMFuzzerTestOneInput_24(const uint8_t *data, size_t size) {
	if(size<0) return 0;

	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);

    // Step 1: Declare variables for libpng APIs
    png_alloc_size_t chunkMallocMax;
    png_uint_32 exif;
    png_voidp ptr;
    jmp_buf jmpbuf;
    png_uint_32 resX, resY;
    int unitType;
    png_byte buf[4];
    png_fixed_point screenGamma, overrideFileGamma;
    
    // Step 2: Call libpng APIs
    chunkMallocMax = png_get_chunk_malloc_max(nullptr);
    exif = png_get_eXIf(nullptr, nullptr, nullptr);
    ptr = png_malloc(nullptr, size);
    png_set_longjmp_fn(nullptr, nullptr, sizeof(jmp_buf));
    png_get_pHYs_dpi(nullptr, nullptr, &resX, &resY, &unitType);
    png_save_uint_32(buf, 0);
    png_set_gamma_fixed(nullptr, 0, 0);
    png_set_chunk_malloc_max(nullptr, size);
    
    // Step 3: Consume the input data and size using libpng APIs
    FILE *in_file = fmemopen((void *)data, size, "rb");
    int fileno_in = fuzz_fileno(in_file);
    const char *input_file = "input_file";
    
    // Step 4: Use the file input/output variables and file descriptor
    FILE *out_file = fopen("output_file", "wb");
    int fileno_out = fuzz_fileno(out_file);
    const char *output_file = "output_file";
    
    // Step 5: Release allocated resources
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Step 6: Print the variables
    std::cout << "chunkMallocMax: " << chunkMallocMax << std::endl;
    std::cout << "exif: " << exif << std::endl;
    std::cout << "ptr: " << ptr << std::endl;
    std::cout << "fileno_in: " << fileno_in << std::endl;
    std::cout << "input_file: " << input_file << std::endl;
    std::cout << "fileno_out: " << fileno_out << std::endl;
    std::cout << "output_file: " << output_file << std::endl;
    
    assert_file_name_closed("output_file");
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fileno_out);
	assert_fd_closed(fileno_in);
	return 0;
}