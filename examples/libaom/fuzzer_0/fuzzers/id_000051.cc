#include "FDSan.h"
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
//<ID> 2012
//<Prompt> ["aom_img_add_metadata","aom_uleb_encode","aom_codec_error_detail","aom_codec_get_preview_frame","aom_uleb_encode_fixed_size","aom_img_metadata_alloc","aom_codec_av1_cx"]
/*<Combination>: [int aom_img_add_metadata(aom_image_t * img, uint32_t type, const uint8_t * data, size_t sz, aom_metadata_insert_flags_t insert_flag),
    int aom_uleb_encode(uint64_t value, size_t available, uint8_t * coded_value, size_t * coded_size),
    const char *aom_codec_error_detail(const aom_codec_ctx_t * ctx),
    const aom_image_t *aom_codec_get_preview_frame(aom_codec_ctx_t * ctx),
    int aom_uleb_encode_fixed_size(uint64_t value, size_t available, size_t pad_to_size, uint8_t * coded_value, size_t * coded_size),
    aom_metadata_t *aom_img_metadata_alloc(uint32_t type, const uint8_t * data, size_t sz, aom_metadata_insert_flags_t insert_flag),
    aom_codec_iface_t *aom_codec_av1_cx()
*/
//<score> 0, nr_unique_branch: 1, p_cov: 0
//<Quality> {"diversity":5,"density":5,"covered_api":[],"uncovered_api":["aom_img_add_metadata","aom_uleb_encode","aom_codec_error_detail","aom_codec_get_preview_frame","aom_uleb_encode_fixed_size","aom_img_metadata_alloc","aom_codec_av1_cx"],"unique_branches":{"read_metadata":[[777,7,777,25,0]]},"library_calls":["aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_decode","aom_codec_get_frame","aom_codec_destroy"]}
/*Here is the C++ implementation of the fuzz driver using the libaom library APIs:

*/


extern "C" int LLVMFuzzerTestOneInput_51(const uint8_t* data, size_t size) {
	if(size<=0) return 0;

	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);

    // Step 1: Create aom_codec_ctx_t variable
    aom_codec_ctx_t ctx;
    
    // Step 2: Initialize the decoder with AV1 codec
    aom_codec_iface_t* iface = aom_codec_av1_dx();
    aom_codec_dec_cfg_t dec_cfg;
    aom_codec_dec_init_ver(&ctx, iface, &dec_cfg, 0, AOM_DECODER_ABI_VERSION);
    
    // Step 3: Create aom_image_t variable for storing decoded frame
    aom_image_t* img = NULL;
    
    // Step 4: Create FILE pointers for reading input data and writing output data
    FILE* in_file = fmemopen((void*)data, size, "rb");
    FILE* out_file = fopen("output_file", "wb");
    
    // Step 5: Get file descriptors for reading and writing
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);
    
    // Step 6: Get file names as strings
    const char* in_file_name = "input_file";
    const char* out_file_name = "output_file";
    
    // Step 7: Implement your logic to process the input data using libaom APIs
    // ...
    // Example: Decode the input data and write the decoded frame to output file
    uint8_t buf[4096];
    size_t bytes_read;
    
    while ((bytes_read = fread(buf, 1, sizeof(buf), in_file)) > 0) {
        aom_codec_decode(&ctx, buf, bytes_read, NULL);
        
        const aom_image_t* frame = aom_codec_get_frame(&ctx, NULL);
        if (frame) {
            // Process the decoded frame (e.g., add metadata, encode it, etc.)
            // ...
            
            // Write the frame to output file
            fwrite(frame->img_data, frame->sz, 1, out_file);
        }
    }
    
    // Step 8: Release allocated resources
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    aom_codec_destroy(&ctx);
    
    assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}