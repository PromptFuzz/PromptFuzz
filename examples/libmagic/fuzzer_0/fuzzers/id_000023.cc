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
//<ID> 1131
//<Prompt> ["vpx_codec_vp8_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_err_to_string","vpx_codec_vp8_dx","vpx_codec_set_frame_buffer_functions","vpx_codec_error","vpx_codec_dec_init_ver","vpx_codec_register_put_frame_cb","vpx_codec_encode","vpx_codec_get_preview_frame","vpx_codec_get_frame","vpx_codec_destroy","vpx_img_free"]
/*<Combination>: [vpx_codec_iface_t *vpx_codec_vp8_cx(),
    vpx_codec_err_t vpx_codec_enc_config_default(vpx_codec_iface_t * iface, vpx_codec_enc_cfg_t * cfg, unsigned int usage),
    vpx_codec_err_t vpx_codec_enc_init_ver(vpx_codec_ctx_t * ctx, vpx_codec_iface_t * iface, const vpx_codec_enc_cfg_t * cfg, vpx_codec_flags_t flags, int ver),
    const char *vpx_codec_err_to_string(vpx_codec_err_t err),
    vpx_codec_iface_t *vpx_codec_vp8_dx(),
    vpx_codec_err_t vpx_codec_set_frame_buffer_functions(vpx_codec_ctx_t * ctx, vpx_get_frame_buffer_cb_fn_t cb_get, vpx_release_frame_buffer_cb_fn_t cb_release, void * cb_priv),
    const char *vpx_codec_error(const vpx_codec_ctx_t * ctx),
    vpx_codec_err_t vpx_codec_dec_init_ver(vpx_codec_ctx_t * ctx, vpx_codec_iface_t * iface, const vpx_codec_dec_cfg_t * cfg, vpx_codec_flags_t flags, int ver),
    vpx_codec_err_t vpx_codec_register_put_frame_cb(vpx_codec_ctx_t * ctx, vpx_codec_put_frame_cb_fn_t cb, void * user_priv),
    vpx_codec_err_t vpx_codec_encode(vpx_codec_ctx_t * ctx, const vpx_image_t * img, vpx_codec_pts_t pts, unsigned long duration, vpx_enc_frame_flags_t flags, unsigned long deadline),
    const vpx_image_t *vpx_codec_get_preview_frame(vpx_codec_ctx_t * ctx),
    vpx_image_t *vpx_codec_get_frame(vpx_codec_ctx_t * ctx, vpx_codec_iter_t * iter),
    vpx_codec_err_t vpx_codec_destroy(vpx_codec_ctx_t * ctx),
    void vpx_img_free(vpx_image_t * img)
*/
//<score> 6, nr_unique_branch: 1
//<Quality> {"density":9,"unique_branches":{"vp8e_get_preview":[[1139,7,1139,60,0,0,4,0]]},"library_calls":["vpx_codec_vp8_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_vp8_dx","vpx_codec_dec_init_ver","vpx_codec_set_frame_buffer_functions","vpx_codec_set_frame_buffer_functions","vpx_img_alloc","vpx_codec_encode","vpx_codec_get_preview_frame","vpx_img_alloc","vpx_codec_decode","vpx_codec_get_frame","vpx_img_free","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp8_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_vp8_dx","vpx_codec_dec_init_ver","vpx_codec_set_frame_buffer_functions","vpx_codec_set_frame_buffer_functions","vpx_img_alloc","vpx_codec_encode","vpx_codec_get_preview_frame","vpx_img_alloc","vpx_codec_decode","vpx_codec_get_frame","vpx_img_free","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy"],"visited":2}
/**/


extern "C" int LLVMFuzzerTestOneInput_23(const uint8_t *data, size_t size) {
	if(size<0) return 0;

    vpx_codec_ctx_t encoder;
    vpx_codec_enc_cfg_t encoder_config;
    vpx_codec_iface_t *encoder_iface = vpx_codec_vp8_cx();

    // Initialize encoder configuration with default values
    vpx_codec_enc_config_default(encoder_iface, &encoder_config, 0);

    // Set up encoder with the configuration
    vpx_codec_enc_init_ver(&encoder, encoder_iface, &encoder_config, 0, VPX_ENCODER_ABI_VERSION);

    vpx_codec_ctx_t decoder;
    vpx_codec_dec_cfg_t decoder_config;
    vpx_codec_iface_t *decoder_iface = vpx_codec_vp8_dx();

    // Initialize decoder configuration with default values
    vpx_codec_dec_init_ver(&decoder, decoder_iface, &decoder_config, 0, VPX_DECODER_ABI_VERSION);

    // Set frame buffer functions for encoder and decoder
    vpx_codec_set_frame_buffer_functions(&encoder, nullptr, nullptr, nullptr);
    vpx_codec_set_frame_buffer_functions(&decoder, nullptr, nullptr, nullptr);

    // Create a vpx_image_t object to hold the input image
    vpx_image_t input_image;
    vpx_img_alloc(&input_image, VPX_IMG_FMT_I420, encoder_config.g_w, encoder_config.g_h, 32);

    // Encode the input image
    vpx_codec_encode(&encoder, &input_image, 0, 1, 0, 0);

    // Get the preview frame from the encoder
    const vpx_image_t *preview_frame = vpx_codec_get_preview_frame(&encoder);

    // Create a vpx_image_t object to hold the output image
    vpx_image_t output_image;
    vpx_img_alloc(&output_image, VPX_IMG_FMT_I420, decoder_config.w, decoder_config.h, 32);

    // Decode the output image
    vpx_codec_decode(&decoder, data, size, nullptr, -1);

    // Get the decoded frame from the decoder
    const vpx_image_t *decoded_frame = vpx_codec_get_frame(&decoder, nullptr);

    // Free the allocated image buffers
    vpx_img_free(&input_image);
    vpx_img_free(&output_image);

    // Destroy the encoder and decoder contexts
    vpx_codec_destroy(&encoder);
    vpx_codec_destroy(&decoder);

    return 0;
}