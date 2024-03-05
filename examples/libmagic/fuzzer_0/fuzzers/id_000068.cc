#include "FDSan.h"
#include <vpx/vp8dx.h>
#include <vpx/vp8cx.h>
#include <vpx/vpx_decoder.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 2635
//<Prompt> ["vpx_codec_vp9_dx","vpx_codec_dec_init_ver","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_multi_ver","vpx_codec_vp9_cx","vpx_codec_dec_init_ver","vpx_codec_decode","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_decode","vpx_codec_get_frame","vpx_img_free","vpx_codec_destroy"]
/*<Combination>: [vpx_codec_iface_t *vpx_codec_vp9_dx(),
    vpx_codec_err_t vpx_codec_dec_init_ver(vpx_codec_ctx_t * ctx, vpx_codec_iface_t * iface, const vpx_codec_dec_cfg_t * cfg, vpx_codec_flags_t flags, int ver),
    vpx_codec_iface_t *vpx_codec_vp9_cx(),
    vpx_codec_err_t vpx_codec_enc_config_default(vpx_codec_iface_t * iface, vpx_codec_enc_cfg_t * cfg, unsigned int usage),
    vpx_codec_err_t vpx_codec_enc_init_multi_ver(vpx_codec_ctx_t * ctx, vpx_codec_iface_t * iface, vpx_codec_enc_cfg_t * cfg, int num_enc, vpx_codec_flags_t flags, vpx_rational_t * dsf, int ver),
    vpx_codec_iface_t *vpx_codec_vp9_cx(),
    vpx_codec_err_t vpx_codec_dec_init_ver(vpx_codec_ctx_t * ctx, vpx_codec_iface_t * iface, const vpx_codec_dec_cfg_t * cfg, vpx_codec_flags_t flags, int ver),
    vpx_codec_err_t vpx_codec_decode(vpx_codec_ctx_t * ctx, const uint8_t * data, unsigned int data_sz, void * user_priv, long deadline),
    vpx_codec_err_t vpx_codec_encode(vpx_codec_ctx_t * ctx, const vpx_image_t * img, vpx_codec_pts_t pts, unsigned long duration, vpx_enc_frame_flags_t flags, unsigned long deadline),
    const vpx_codec_cx_pkt_t *vpx_codec_get_cx_data(vpx_codec_ctx_t * ctx, vpx_codec_iter_t * iter),
    vpx_codec_err_t vpx_codec_decode(vpx_codec_ctx_t * ctx, const uint8_t * data, unsigned int data_sz, void * user_priv, long deadline),
    vpx_image_t *vpx_codec_get_frame(vpx_codec_ctx_t * ctx, vpx_codec_iter_t * iter),
    void vpx_img_free(vpx_image_t * img),
    vpx_codec_err_t vpx_codec_destroy(vpx_codec_ctx_t * ctx)
*/
//<score> 11, nr_unique_branch: 0
//<Quality> {"density":11,"unique_branches":{},"library_calls":["vpx_codec_vp9_dx","vpx_codec_dec_init_ver","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_multi_ver","vpx_codec_decode","vpx_codec_get_frame","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_destroy","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp9_dx","vpx_codec_dec_init_ver","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_multi_ver","vpx_codec_decode","vpx_codec_get_frame","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_destroy","vpx_codec_destroy"],"visited":0}
/**/

extern "C" int LLVMFuzzerTestOneInput_68(const uint8_t *data, size_t size) {
	if(size<0) return 0;

    // Create a decoder configuration
    vpx_codec_iface_t *decoder_iface = vpx_codec_vp9_dx();
    vpx_codec_dec_cfg_t decoder_cfg = {0};
    decoder_cfg.w = 640;
    decoder_cfg.h = 480;
    vpx_codec_ctx_t decoder_ctx;
    vpx_codec_dec_init_ver(&decoder_ctx, decoder_iface, &decoder_cfg, 0, VPX_DECODER_ABI_VERSION);

    // Create an encoder configuration
    vpx_codec_enc_cfg_t encoder_cfg;
    vpx_codec_iface_t *encoder_iface = vpx_codec_vp9_cx();
    vpx_codec_enc_config_default(encoder_iface, &encoder_cfg, 0);
    encoder_cfg.g_w = 640;
    encoder_cfg.g_h = 480;
    vpx_codec_ctx_t encoder_ctx;
    vpx_codec_enc_init_multi_ver(&encoder_ctx, encoder_iface, &encoder_cfg, 1, 0, nullptr, VPX_ENCODER_ABI_VERSION);

    // Decode the input data
    vpx_codec_decode(&decoder_ctx, data, size, nullptr, 0);

    // Get the decoded frame
    vpx_codec_iter_t decoder_iter = nullptr;
    const vpx_image_t *decoded_frame = nullptr;
    while ((decoded_frame = vpx_codec_get_frame(&decoder_ctx, &decoder_iter)) != nullptr) {
        // Encode the frame
        vpx_codec_encode(&encoder_ctx, decoded_frame, 0, 0, 0, 0);
    }

    // Get the encoded data
    const vpx_codec_cx_pkt_t *encoded_data = nullptr;
    vpx_codec_iter_t encoder_iter = nullptr;
    while ((encoded_data = vpx_codec_get_cx_data(&encoder_ctx, &encoder_iter)) != nullptr) {
        // Use the encoded data
    }

    // Free the decoder and encoder contexts
    vpx_codec_destroy(&decoder_ctx);
    vpx_codec_destroy(&encoder_ctx);

    return 0;
}