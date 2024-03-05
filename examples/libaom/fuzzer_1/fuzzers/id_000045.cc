#include "FDSan.h"
#include "FuzzedDataProvider.h"
#include <aom/aom_decoder.h>
#include <aom/aomcx.h>
#include <aom/aomdx.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 1729
//<Prompt> ["aom_codec_err_to_string","aom_codec_get_caps","aom_codec_destroy","aom_codec_iface_name","aom_img_alloc_with_border","aom_img_add_metadata","aom_img_get_metadata","aom_codec_error_detail","aom_img_remove_metadata","aom_codec_error","aom_img_metadata_alloc"]
/*<Combination>: [const char *aom_codec_err_to_string(aom_codec_err_t err),
    aom_codec_caps_t aom_codec_get_caps(aom_codec_iface_t * iface),
    aom_codec_err_t aom_codec_destroy(aom_codec_ctx_t * ctx),
    const char *aom_codec_iface_name(aom_codec_iface_t * iface),
    aom_image_t *aom_img_alloc_with_border(aom_image_t * img, aom_img_fmt_t fmt, unsigned int d_w, unsigned int d_h, unsigned int align, unsigned int size_align, unsigned int border),
    int aom_img_add_metadata(aom_image_t * img, uint32_t type, const uint8_t * data, size_t sz, aom_metadata_insert_flags_t insert_flag),
    const aom_metadata_t *aom_img_get_metadata(const aom_image_t * img, size_t index),
    const char *aom_codec_error_detail(const aom_codec_ctx_t * ctx),
    void aom_img_remove_metadata(aom_image_t * img),
    const char *aom_codec_error(const aom_codec_ctx_t * ctx),
    aom_metadata_t *aom_img_metadata_alloc(uint32_t type, const uint8_t * data, size_t sz, aom_metadata_insert_flags_t insert_flag)
*/
//<score> 1.590909, nr_unique_branch: 2, p_cov: 0.90909094
//<Quality> {"diversity":12,"density":7,"covered_api":["aom_codec_get_caps","aom_codec_destroy","aom_codec_iface_name","aom_img_alloc_with_border","aom_img_add_metadata","aom_img_get_metadata","aom_codec_error_detail","aom_img_remove_metadata","aom_codec_error","aom_img_metadata_alloc"],"uncovered_api":["aom_codec_err_to_string"],"unique_branches":{"aom_codec_get_caps":[[84,10,84,15,1]],"aom_img_get_metadata":[[392,16,392,33,1]]},"library_calls":["aom_codec_error_detail","aom_codec_get_caps","aom_codec_destroy","aom_codec_iface_name","aom_img_alloc_with_border","aom_img_add_metadata","aom_img_get_metadata","aom_img_remove_metadata","aom_codec_error","aom_img_metadata_alloc","aom_img_free","aom_img_metadata_free"]}
/**/


extern "C" int LLVMFuzzerTestOneInput_45(const uint8_t* f_data, size_t f_size) {
	if(f_size<=0) return 0;

	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(f_data, sizeof(uint8_t), f_size, input_file_ptr);
	fclose(input_file_ptr);

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Step 4: Create a FILE pointer for reading input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file == NULL) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Step 5: Get file descriptor for input file
    int in_fd = fuzz_fileno(in_file);

    // Step 6: Set the input file name
    const char *input_file = "input_file";
    
    // Step 7: Create a FILE pointer for writing output data
    FILE *out_file = fopen("output_file", "wb");
    if (out_file == NULL) {
        assert_file_closed(&in_file);;
        assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Step 8: Get file descriptor for output file
    int out_fd = fuzz_fileno(out_file);
    
    // Step 1: Event - Get the error detail of a codec context
    aom_codec_ctx_t codec_ctx;
    const char *error_detail = aom_codec_error_detail(&codec_ctx);
    
    // Step 2: Event - Get the capabilities of a codec interface
    aom_codec_iface_t *iface = NULL;
    aom_codec_caps_t caps = aom_codec_get_caps(iface);
    
    // Step 3: Event - Destroy a codec context
    aom_codec_destroy(&codec_ctx);
    
    // Step 4: Event - Get the name of a codec interface
    const char *iface_name = aom_codec_iface_name(iface);
    
    // Step 5: Event - Allocate an image with a border
    aom_image_t img;
    aom_img_fmt_t fmt = AOM_IMG_FMT_I420;
    unsigned int d_w = 1920;
    unsigned int d_h = 1080;
    unsigned int align = 16;
    unsigned int size_align = 32;
    unsigned int border = 8;
    aom_img_alloc_with_border(&img, fmt, d_w, d_h, align, size_align, border);
    
    // Step 6: Event - Add metadata to an image
    uint32_t type = 0;
    const uint8_t *metadata_data = NULL;
    size_t metadata_sz = 0;
    aom_metadata_insert_flags_t insert_flag = AOM_MIF_NON_KEY_FRAME;
    aom_img_add_metadata(&img, type, metadata_data, metadata_sz, insert_flag);
    
    // Step 7: Event - Get metadata from an image
    size_t index = 0;
    const aom_metadata_t *metadata = aom_img_get_metadata(&img, index);
    
    // Step 8: Event - Remove metadata from an image
    aom_img_remove_metadata(&img);
    
    // Step 9: Event - Get the error string of a codec context
    const char *error_string = aom_codec_error(&codec_ctx);
    
    // Step 10: Event - Allocate metadata
    aom_metadata_t *metadata_alloc = aom_img_metadata_alloc(type, metadata_data, metadata_sz, insert_flag);
    
    // Step 11: Release allocated resources
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    aom_img_free(&img);
    aom_img_metadata_free(metadata_alloc);
    
    assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}