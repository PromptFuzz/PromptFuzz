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
//<ID> 2611
//<Prompt> ["vpx_codec_vp8_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_img_alloc","vpx_codec_get_frame","vpx_codec_get_preview_frame","vpx_codec_destroy","vpx_codec_vp8_dx","vpx_codec_dec_init_ver","vpx_codec_set_frame_buffer_functions","vpx_codec_decode","vpx_codec_get_frame","vpx_img_free","vpx_codec_destroy"]
/*<Combination>: [vpx_codec_iface_t *vpx_codec_vp8_cx(),
    vpx_codec_err_t vpx_codec_enc_config_default(vpx_codec_iface_t * iface, vpx_codec_enc_cfg_t * cfg, unsigned int usage),
    vpx_codec_err_t vpx_codec_enc_init_ver(vpx_codec_ctx_t * ctx, vpx_codec_iface_t * iface, const vpx_codec_enc_cfg_t * cfg, vpx_codec_flags_t flags, int ver),
    vpx_image_t *vpx_img_alloc(vpx_image_t * img, vpx_img_fmt_t fmt, unsigned int d_w, unsigned int d_h, unsigned int align),
    vpx_image_t *vpx_codec_get_frame(vpx_codec_ctx_t * ctx, vpx_codec_iter_t * iter),
    const vpx_image_t *vpx_codec_get_preview_frame(vpx_codec_ctx_t * ctx),
    vpx_codec_err_t vpx_codec_destroy(vpx_codec_ctx_t * ctx),
    vpx_codec_iface_t *vpx_codec_vp8_dx(),
    vpx_codec_err_t vpx_codec_dec_init_ver(vpx_codec_ctx_t * ctx, vpx_codec_iface_t * iface, const vpx_codec_dec_cfg_t * cfg, vpx_codec_flags_t flags, int ver),
    vpx_codec_err_t vpx_codec_set_frame_buffer_functions(vpx_codec_ctx_t * ctx, vpx_get_frame_buffer_cb_fn_t cb_get, vpx_release_frame_buffer_cb_fn_t cb_release, void * cb_priv),
    vpx_codec_err_t vpx_codec_decode(vpx_codec_ctx_t * ctx, const uint8_t * data, unsigned int data_sz, void * user_priv, long deadline),
    vpx_image_t *vpx_codec_get_frame(vpx_codec_ctx_t * ctx, vpx_codec_iter_t * iter),
    void vpx_img_free(vpx_image_t * img),
    vpx_codec_err_t vpx_codec_destroy(vpx_codec_ctx_t * ctx)
*/
//<score> 7, nr_unique_branch: 1
//<Quality> {"density":7,"unique_branches":{"vp8_new_framerate":[[1258,7,1258,21,0,0,4,0]]},"library_calls":["vpx_codec_vp8_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_img_alloc","vpx_codec_encode","vpx_codec_vp8_dx","vpx_codec_dec_init_ver","vpx_codec_set_frame_buffer_functions","vpx_codec_decode","vpx_codec_get_frame","vpx_codec_destroy","vpx_img_free","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp8_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_img_alloc","vpx_codec_encode","vpx_codec_vp8_dx","vpx_codec_dec_init_ver","vpx_codec_set_frame_buffer_functions","vpx_codec_decode","vpx_codec_get_frame","vpx_codec_destroy","vpx_img_free","vpx_codec_destroy"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_67(const uint8_t* f_data, size_t f_size) {
	if(f_size<60) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_1, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_2, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_3, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_4, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_5, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_6, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_7, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_8, fdp);
	//fuzzer shim end}




    // Step 1: Create a VPX encoder context
    vpx_codec_iface_t *encoder_iface = vpx_codec_vp8_cx();
    vpx_codec_enc_cfg_t encoder_cfg;
    vpx_codec_enc_config_default(encoder_iface, &encoder_cfg, fuzz_uint32_t_1);
    vpx_codec_ctx_t encoder_ctx;
    vpx_codec_enc_init_ver(&encoder_ctx, encoder_iface, &encoder_cfg, fuzz_int64_t_2, VPX_ENCODER_ABI_VERSION);

    // Step 2: Create a VPX image
    vpx_image_t image;
    vpx_img_alloc(&image, VPX_IMG_FMT_I420, encoder_cfg.g_w, encoder_cfg.g_h, 1);

    // Step 3: Encode the input data
    vpx_codec_encode(&encoder_ctx, &image, fuzz_int64_t_3, fuzz_uint64_t_4, fuzz_int64_t_5, fuzz_uint64_t_6);

    // Step 4: Create a VPX decoder context
    vpx_codec_iface_t *decoder_iface = vpx_codec_vp8_dx();
    vpx_codec_ctx_t decoder_ctx;
    vpx_codec_dec_init_ver(&decoder_ctx, decoder_iface, NULL, fuzz_int64_t_7, VPX_DECODER_ABI_VERSION);

    // Step 5: Set the frame buffer functions for the decoder
    vpx_codec_set_frame_buffer_functions(&decoder_ctx, NULL, NULL, NULL);

    // Step 6: Decode the encoded data
    vpx_codec_decode(&decoder_ctx, data, size, NULL, fuzz_int64_t_8);

    // Step 7: Get the decoded frame
    vpx_codec_iter_t iter = NULL;
    const vpx_image_t *decoded_frame = vpx_codec_get_frame(&decoder_ctx, &iter);

    // Step 8: Free allocated resources
    vpx_codec_destroy(&encoder_ctx);
    vpx_img_free(&image);
    vpx_codec_destroy(&decoder_ctx);

    return 0;
}