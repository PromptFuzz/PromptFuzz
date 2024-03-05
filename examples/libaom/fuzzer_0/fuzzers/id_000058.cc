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
//<ID> 2206
//<Prompt> ["aom_codec_set_frame_buffer_functions","aom_codec_peek_stream_info","aom_uleb_size_in_bytes","aom_img_add_metadata","aom_codec_enc_init_ver"]
/*<Combination>: [aom_codec_err_t aom_codec_set_frame_buffer_functions(aom_codec_ctx_t * ctx, aom_get_frame_buffer_cb_fn_t cb_get, aom_release_frame_buffer_cb_fn_t cb_release, void * cb_priv),
    aom_codec_err_t aom_codec_peek_stream_info(aom_codec_iface_t * iface, const uint8_t * data, size_t data_sz, aom_codec_stream_info_t * si),
    size_t aom_uleb_size_in_bytes(uint64_t value),
    int aom_img_add_metadata(aom_image_t * img, uint32_t type, const uint8_t * data, size_t sz, aom_metadata_insert_flags_t insert_flag),
    aom_codec_err_t aom_codec_enc_init_ver(aom_codec_ctx_t * ctx, aom_codec_iface_t * iface, const aom_codec_enc_cfg_t * cfg, aom_codec_flags_t flags, int ver)
*/
//<score> 2.6666667, nr_unique_branch: 3, p_cov: 1
//<Quality> {"diversity":12,"density":8,"covered_api":["aom_codec_set_frame_buffer_functions","aom_codec_peek_stream_info","aom_uleb_size_in_bytes","aom_img_add_metadata","aom_codec_enc_init_ver"],"uncovered_api":[],"unique_branches":{"aom_memalign":[[57,7,57,52,0]],"img_alloc_helper":[[146,7,146,21,0]],"check_size_argument_overflow":[[33,7,33,64,0]]},"library_calls":["aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_set_frame_buffer_functions","aom_codec_peek_stream_info","aom_uleb_size_in_bytes","aom_img_alloc","aom_img_add_metadata","aom_codec_enc_init_ver","aom_codec_av1_cx","aom_codec_destroy","aom_img_free","aom_codec_destroy"]}
/**/

extern "C" int LLVMFuzzerTestOneInput_58(const uint8_t *data, size_t size) {
	if(size<=0) return 0;

    // Step 1: Create and initialize the codec context
    aom_codec_ctx_t codec;
    aom_codec_iface_t *codec_iface = aom_codec_av1_dx();
    aom_codec_dec_cfg_t codec_cfg;
    aom_codec_dec_init_ver(&codec, codec_iface, &codec_cfg, 0, AOM_DECODER_ABI_VERSION);

    // Step 2: Set the frame buffer functions
    aom_codec_set_frame_buffer_functions(
        &codec,
        [](void *cb_priv, size_t size, aom_codec_frame_buffer_t *fb) -> int {
            // Implement your own frame buffer allocation logic here
            // Set the required fields of the frame buffer

            return 0;  // Return 0 for success
        },
        [](void *cb_priv, aom_codec_frame_buffer_t *fb) -> int {
            // Implement your own frame buffer release logic here

            return 0;  // Return 0 for success
        },
        nullptr);

    // Step 3: Create a stream info object
    aom_codec_stream_info_t stream_info;
    aom_codec_peek_stream_info(codec_iface, data, size, &stream_info);

    // Step 4: Get the size of a ULEB number
    uint64_t value = 42;
    size_t leb_size = aom_uleb_size_in_bytes(value);

    // Step 5: Create an image object and add metadata
    aom_image_t image;
    aom_img_alloc(&image, AOM_IMG_FMT_I420, stream_info.w, stream_info.h, 16);

    uint32_t type = 1;
    const uint8_t metadata[] = {0x01, 0x02, 0x03};
    size_t metadata_size = sizeof(metadata);
    aom_img_add_metadata(&image, type, metadata, metadata_size, AOM_MIF_ANY_FRAME);

    // Step 6: Initialize the encoder
    aom_codec_ctx_t enc_codec;
    aom_codec_enc_cfg_t enc_cfg;
    aom_codec_enc_init_ver(&enc_codec, aom_codec_av1_cx(), &enc_cfg, 0, AOM_ENCODER_ABI_VERSION);

    // Step 7: Cleanup and release resources
    aom_codec_destroy(&enc_codec);
    aom_img_free(&image);
    aom_codec_destroy(&codec);

    return 0;
}