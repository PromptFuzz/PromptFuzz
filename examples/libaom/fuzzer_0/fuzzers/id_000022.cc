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
//<ID> 847
//<Prompt> ["aom_codec_decode","aom_codec_enc_init_ver","aom_codec_set_frame_buffer_functions","aom_codec_enc_config_default","aom_img_wrap","aom_codec_get_frame"]
/*<Combination>: [aom_codec_err_t aom_codec_decode(aom_codec_ctx_t * ctx, const uint8_t * data, size_t data_sz, void * user_priv),
    aom_codec_err_t aom_codec_enc_init_ver(aom_codec_ctx_t * ctx, aom_codec_iface_t * iface, const aom_codec_enc_cfg_t * cfg, aom_codec_flags_t flags, int ver),
    aom_codec_err_t aom_codec_set_frame_buffer_functions(aom_codec_ctx_t * ctx, aom_get_frame_buffer_cb_fn_t cb_get, aom_release_frame_buffer_cb_fn_t cb_release, void * cb_priv),
    aom_codec_err_t aom_codec_enc_config_default(aom_codec_iface_t * iface, aom_codec_enc_cfg_t * cfg, unsigned int usage),
    aom_image_t *aom_img_wrap(aom_image_t * img, aom_img_fmt_t fmt, unsigned int d_w, unsigned int d_h, unsigned int align, unsigned char * img_data),
    aom_image_t *aom_codec_get_frame(aom_codec_ctx_t * ctx, aom_codec_iter_t * iter)
*/
//<score> 3, nr_unique_branch: 2, p_cov: 1
//<Quality> {"diversity":14,"density":14,"covered_api":["aom_codec_decode","aom_codec_enc_init_ver","aom_codec_set_frame_buffer_functions","aom_codec_enc_config_default","aom_img_wrap","aom_codec_get_frame"],"uncovered_api":[],"unique_branches":{"decode_restoration_mode":[[1541,7,1541,21,1]],"aom_img_free":[[285,26,285,45,1]]},"library_calls":["aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_set_frame_buffer_functions","aom_img_wrap","aom_codec_decode","aom_codec_get_frame","aom_codec_encode","aom_codec_get_cx_data","aom_img_free","aom_codec_destroy","aom_codec_destroy"]}
/**/

extern "C" int LLVMFuzzerTestOneInput_22(const uint8_t *data, size_t size) {
	if(size<=0) return 0;

    // Step 1: Create a decoder context
    aom_codec_ctx_t decoder;
    aom_codec_iface_t *decoder_iface = aom_codec_av1_dx();
    aom_codec_dec_cfg_t decoder_cfg = {0};
    aom_codec_dec_init_ver(&decoder, decoder_iface, &decoder_cfg, 0, AOM_DECODER_ABI_VERSION);

    // Step 2: Create an encoder context
    aom_codec_ctx_t encoder;
    aom_codec_iface_t *encoder_iface = aom_codec_av1_cx();
    aom_codec_enc_cfg_t encoder_cfg;
    aom_codec_enc_config_default(encoder_iface, &encoder_cfg, 0);
    aom_codec_enc_init_ver(&encoder, encoder_iface, &encoder_cfg, 0, AOM_ENCODER_ABI_VERSION);

    // Step 3: Set frame buffer functions for the encoder
    aom_get_frame_buffer_cb_fn_t get_frame_buffer_cb = nullptr;
    aom_release_frame_buffer_cb_fn_t release_frame_buffer_cb = nullptr;
    void *cb_priv = nullptr;
    aom_codec_set_frame_buffer_functions(&encoder, get_frame_buffer_cb, release_frame_buffer_cb, cb_priv);

    // Step 4: Wrap the input data as an image
    unsigned int d_w = 0; // set the desired width of the image
    unsigned int d_h = 0; // set the desired height of the image
    unsigned int align = 1; // set the desired alignment of the image data
    aom_img_fmt_t fmt = AOM_IMG_FMT_I420; // set the desired pixel format
    aom_image_t *img = aom_img_wrap(nullptr, fmt, d_w, d_h, align, const_cast<uint8_t *>(data));

    // Step 5: Decode the input data
    aom_codec_decode(&decoder, data, size, nullptr);

    // Step 6: Get the decoded frame
    aom_codec_iter_t iter = nullptr;
    const aom_image_t *decoded_frame = aom_codec_get_frame(&decoder, &iter);

    // Step 7: Encode the decoded frame
    aom_codec_encode(&encoder, decoded_frame, 0, 0, 0);

    // Step 8: Get the encoded bitstream
    const aom_codec_cx_pkt_t *pkt = nullptr;
    iter = nullptr;
    while ((pkt = aom_codec_get_cx_data(&encoder, &iter)) != nullptr) {
        if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
            // Write the encoded bitstream to an output file
            FILE *out_file = fopen("output_file", "wb");
            fwrite(pkt->data.frame.buf, sizeof(uint8_t), pkt->data.frame.sz, out_file);
            assert_file_closed(&out_file);;
        }
    }

    // Step 9: Free the allocated resources
    aom_img_free(img);
    aom_codec_destroy(&encoder);
    aom_codec_destroy(&decoder);

    return 0;
}