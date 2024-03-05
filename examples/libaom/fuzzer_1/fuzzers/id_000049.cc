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
//<ID> 1965
//<Prompt> ["aom_codec_iface_name","aom_img_add_metadata","aom_uleb_encode","aom_img_wrap","aom_codec_error_detail","aom_codec_get_preview_frame","aom_codec_set_frame_buffer_functions","aom_img_metadata_alloc"]
/*<Combination>: [const char *aom_codec_iface_name(aom_codec_iface_t * iface),
    int aom_img_add_metadata(aom_image_t * img, uint32_t type, const uint8_t * data, size_t sz, aom_metadata_insert_flags_t insert_flag),
    int aom_uleb_encode(uint64_t value, size_t available, uint8_t * coded_value, size_t * coded_size),
    aom_image_t *aom_img_wrap(aom_image_t * img, aom_img_fmt_t fmt, unsigned int d_w, unsigned int d_h, unsigned int align, unsigned char * img_data),
    const char *aom_codec_error_detail(const aom_codec_ctx_t * ctx),
    const aom_image_t *aom_codec_get_preview_frame(aom_codec_ctx_t * ctx),
    aom_codec_err_t aom_codec_set_frame_buffer_functions(aom_codec_ctx_t * ctx, aom_get_frame_buffer_cb_fn_t cb_get, aom_release_frame_buffer_cb_fn_t cb_release, void * cb_priv),
    aom_metadata_t *aom_img_metadata_alloc(uint32_t type, const uint8_t * data, size_t sz, aom_metadata_insert_flags_t insert_flag)
*/
//<score> 1, nr_unique_branch: 1, p_cov: 0.625
//<Quality> {"diversity":10,"density":8,"covered_api":["aom_img_wrap","aom_codec_error_detail","aom_codec_get_preview_frame","aom_codec_set_frame_buffer_functions","aom_img_metadata_alloc"],"uncovered_api":["aom_codec_iface_name","aom_img_add_metadata","aom_uleb_encode"],"unique_branches":{"img_alloc_helper":[[51,7,51,20,0]]},"library_calls":["aom_codec_av1_dx","aom_codec_dec_init_ver","aom_img_wrap","aom_codec_set_frame_buffer_functions","aom_img_metadata_alloc","aom_codec_decode","aom_codec_get_preview_frame","aom_codec_error_detail","aom_codec_destroy","aom_img_metadata_free"]}
/*Here is the modified code that incorporates the libaom APIs and follows the provided instructions:

*/


extern "C" int LLVMFuzzerTestOneInput_49(const uint8_t* f_data, size_t f_size) {
	if(f_size<=16) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int64_t, fuzz_int64_t_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
	//fuzzer shim end}




    // Step 1: Declare variables
    aom_codec_iface_t *iface;
    aom_codec_ctx_t *ctx;
    aom_image_t *img;
    aom_metadata_t *metadata;
    aom_get_frame_buffer_cb_fn_t get_frame_buffer_cb;
    aom_release_frame_buffer_cb_fn_t release_frame_buffer_cb;
    uint32_t metadata_type;
    uint8_t *metadata_data;
    size_t metadata_size;
    aom_metadata_insert_flags_t metadata_flags;

    // Step 2: Initialize the libaom codec and context
    iface = aom_codec_av1_dx();
    ctx = new aom_codec_ctx_t;
    aom_codec_dec_cfg_t dec_cfg;
    aom_codec_dec_init_ver(ctx, iface, &dec_cfg, fuzz_int64_t_1, AOM_DECODER_ABI_VERSION);

    // Step 3: Create an image and wrap it with the input data
    img = new aom_image_t;
    aom_img_wrap(img, AOM_IMG_FMT_YV12, fuzz_uint32_t_2, fuzz_uint32_t_3, 0, const_cast<uint8_t *>(data));

    // Step 4: Set frame buffer functions
    aom_codec_set_frame_buffer_functions(ctx, get_frame_buffer_cb, release_frame_buffer_cb, nullptr);

    // Step 5: Add metadata to the image
    metadata_type = 0;  // Set the desired metadata type
    metadata_data = nullptr;  // Set the metadata data
    metadata_size = 0;  // Set the metadata size
    metadata_flags = AOM_MIF_ANY_FRAME;  // Set the metadata insert flag
    aom_img_metadata_alloc(metadata_type, metadata_data, metadata_size, metadata_flags);

    // Step 6: Decode the image
    aom_codec_decode(ctx, data, size, nullptr);

    // Step 7: Get the preview frame
    const aom_image_t *preview_frame = aom_codec_get_preview_frame(ctx);

    // Step 8: Print codec error details
    const char *error_details = aom_codec_error_detail(ctx);
    printf("Codec Error Details: %s\n", error_details);

    // Step 9: Clean up resources
    aom_codec_destroy(ctx);
    delete ctx;
    delete img;
    aom_img_metadata_free(metadata);

    return 0;
}