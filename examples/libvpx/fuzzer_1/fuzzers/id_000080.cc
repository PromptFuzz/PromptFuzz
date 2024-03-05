#include "FDSan.h"
#include "FuzzedDataProvider.h"
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
//<ID> 2911
//<Prompt> ["vpx_codec_vp9_dx","vpx_codec_dec_init_ver","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_set_frame_buffer_functions","vpx_codec_register_put_frame_cb","vpx_img_set_rect","vpx_codec_register_put_slice_cb","vpx_codec_encode","vpx_codec_control_","vpx_codec_decode","vpx_codec_get_stream_info","vpx_codec_err_to_string","vpx_img_free","vpx_codec_destroy"]
/*<Combination>: [vpx_codec_iface_t *vpx_codec_vp9_dx(),
    vpx_codec_err_t vpx_codec_dec_init_ver(vpx_codec_ctx_t * ctx, vpx_codec_iface_t * iface, const vpx_codec_dec_cfg_t * cfg, vpx_codec_flags_t flags, int ver),
    vpx_codec_iface_t *vpx_codec_vp9_cx(),
    vpx_codec_err_t vpx_codec_enc_config_default(vpx_codec_iface_t * iface, vpx_codec_enc_cfg_t * cfg, unsigned int usage),
    vpx_codec_err_t vpx_codec_enc_init_ver(vpx_codec_ctx_t * ctx, vpx_codec_iface_t * iface, const vpx_codec_enc_cfg_t * cfg, vpx_codec_flags_t flags, int ver),
    vpx_codec_err_t vpx_codec_set_frame_buffer_functions(vpx_codec_ctx_t * ctx, vpx_get_frame_buffer_cb_fn_t cb_get, vpx_release_frame_buffer_cb_fn_t cb_release, void * cb_priv),
    vpx_codec_err_t vpx_codec_register_put_frame_cb(vpx_codec_ctx_t * ctx, vpx_codec_put_frame_cb_fn_t cb, void * user_priv),
    int vpx_img_set_rect(vpx_image_t * img, unsigned int x, unsigned int y, unsigned int w, unsigned int h),
    vpx_codec_err_t vpx_codec_register_put_slice_cb(vpx_codec_ctx_t * ctx, vpx_codec_put_slice_cb_fn_t cb, void * user_priv),
    vpx_codec_err_t vpx_codec_encode(vpx_codec_ctx_t * ctx, const vpx_image_t * img, vpx_codec_pts_t pts, unsigned long duration, vpx_enc_frame_flags_t flags, unsigned long deadline),
    vpx_codec_err_t vpx_codec_control_(vpx_codec_ctx_t * ctx, int ctrl_id),
    vpx_codec_err_t vpx_codec_decode(vpx_codec_ctx_t * ctx, const uint8_t * data, unsigned int data_sz, void * user_priv, long deadline),
    vpx_codec_err_t vpx_codec_get_stream_info(vpx_codec_ctx_t * ctx, vpx_codec_stream_info_t * si),
    const char *vpx_codec_err_to_string(vpx_codec_err_t err),
    void vpx_img_free(vpx_image_t * img),
    vpx_codec_err_t vpx_codec_destroy(vpx_codec_ctx_t * ctx)
*/
//<score> 11, nr_unique_branch: 0
//<Quality> {"density":11,"unique_branches":{},"library_calls":["vpx_codec_dec_init_ver","vpx_codec_vp9_dx","vpx_codec_enc_config_default","vpx_codec_vp9_cx","vpx_codec_enc_init_ver","vpx_codec_vp9_cx","vpx_img_alloc","vpx_img_set_rect","vpx_codec_register_put_frame_cb","vpx_codec_register_put_slice_cb","vpx_codec_encode","vpx_codec_decode","vpx_codec_get_stream_info","vpx_codec_err_to_string","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy"],"critical_calls":["vpx_codec_dec_init_ver","vpx_codec_vp9_dx","vpx_codec_enc_config_default","vpx_codec_vp9_cx","vpx_codec_enc_init_ver","vpx_codec_vp9_cx","vpx_img_alloc","vpx_img_set_rect","vpx_codec_register_put_frame_cb","vpx_codec_register_put_slice_cb","vpx_codec_encode","vpx_codec_decode","vpx_codec_get_stream_info","vpx_codec_err_to_string","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy"],"visited":0}
/**/

// LLVMFuzzerTestOneInput_80 function
extern "C" int LLVMFuzzerTestOneInput_80(const uint8_t* f_data, size_t f_size) {
	if(f_size<68) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int64_t, fuzz_int64_t_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_3, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_4, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_5, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_6, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_7, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_8, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_9, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_10, fdp);
	//fuzzer shim end}




    // Create a decoder context
    vpx_codec_ctx_t decoder_ctx;
    vpx_codec_dec_cfg_t decoder_cfg;

    // Initialize decoder configuration
    vpx_codec_dec_init_ver(&decoder_ctx, vpx_codec_vp9_dx(), &decoder_cfg, fuzz_int64_t_1, VPX_DECODER_ABI_VERSION);

    // Create an encoder context
    vpx_codec_ctx_t encoder_ctx;
    vpx_codec_enc_cfg_t encoder_cfg;

    // Set default encoder configuration
    vpx_codec_enc_config_default(vpx_codec_vp9_cx(), &encoder_cfg, fuzz_uint32_t_2);

    // Initialize encoder configuration
    vpx_codec_enc_init_ver(&encoder_ctx, vpx_codec_vp9_cx(), &encoder_cfg, fuzz_int64_t_3, VPX_ENCODER_ABI_VERSION);

    // Create an image
    vpx_image_t image;
    vpx_img_alloc(&image, VPX_IMG_FMT_I420, 1920, 1080, 1);

    // Set rectangle
    vpx_img_set_rect(&image, fuzz_uint32_t_4, fuzz_uint32_t_5, image.d_w, image.d_h);

    // Register put frame callback
    vpx_codec_register_put_frame_cb(&encoder_ctx, NULL, NULL);

    // Register put slice callback
    vpx_codec_register_put_slice_cb(&encoder_ctx, NULL, NULL);

    // Encode the image
    vpx_codec_encode(&encoder_ctx, &image, fuzz_int64_t_6, fuzz_uint64_t_7, fuzz_int64_t_8, fuzz_uint64_t_9);

    // Decode the data
    vpx_codec_decode(&decoder_ctx, data, size, NULL, fuzz_int64_t_10);

    // Get stream information
    vpx_codec_stream_info_t stream_info;
    vpx_codec_get_stream_info(&decoder_ctx, &stream_info);

    // Convert error to string
    const char* error_str = vpx_codec_err_to_string(decoder_ctx.err);

    // Free the image
    vpx_img_free(&image);

    // Destroy the decoder context
    vpx_codec_destroy(&decoder_ctx);

    // Destroy the encoder context
    vpx_codec_destroy(&encoder_ctx);

    return 0;
}