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
//<ID> 592
//<Prompt> ["vpx_codec_vp8_cx","vpx_codec_vp8_dx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_vp8_cx","vpx_codec_enc_config_set","vpx_codec_register_put_frame_cb","vpx_codec_encode","vpx_codec_decode","vpx_codec_get_frame","vpx_img_free","vpx_codec_destroy"]
/*<Combination>: [vpx_codec_iface_t *vpx_codec_vp8_cx(),
    vpx_codec_iface_t *vpx_codec_vp8_dx(),
    vpx_codec_err_t vpx_codec_enc_config_default(vpx_codec_iface_t * iface, vpx_codec_enc_cfg_t * cfg, unsigned int usage),
    vpx_codec_err_t vpx_codec_enc_init_ver(vpx_codec_ctx_t * ctx, vpx_codec_iface_t * iface, const vpx_codec_enc_cfg_t * cfg, vpx_codec_flags_t flags, int ver),
    vpx_codec_iface_t *vpx_codec_vp8_cx(),
    vpx_codec_err_t vpx_codec_enc_config_set(vpx_codec_ctx_t * ctx, const vpx_codec_enc_cfg_t * cfg),
    vpx_codec_err_t vpx_codec_register_put_frame_cb(vpx_codec_ctx_t * ctx, vpx_codec_put_frame_cb_fn_t cb, void * user_priv),
    vpx_codec_err_t vpx_codec_encode(vpx_codec_ctx_t * ctx, const vpx_image_t * img, vpx_codec_pts_t pts, unsigned long duration, vpx_enc_frame_flags_t flags, unsigned long deadline),
    vpx_codec_err_t vpx_codec_decode(vpx_codec_ctx_t * ctx, const uint8_t * data, unsigned int data_sz, void * user_priv, long deadline),
    vpx_image_t *vpx_codec_get_frame(vpx_codec_ctx_t * ctx, vpx_codec_iter_t * iter),
    void vpx_img_free(vpx_image_t * img),
    vpx_codec_err_t vpx_codec_destroy(vpx_codec_ctx_t * ctx)
*/
//<score> 6.6666665, nr_unique_branch: 5
//<Quality> {"density":10,"unique_branches":{"vp8e_set_config":[[462,7,462,31,0,0,4,1],[462,35,462,59,0,0,4,1],[475,7,475,56,0,0,4,1],[479,7,479,26,0,0,4,1],[495,7,495,45,0,0,4,1]]},"library_calls":["vpx_codec_enc_config_default","vpx_codec_vp8_cx","vpx_codec_enc_init_ver","vpx_codec_vp8_cx","vpx_codec_enc_config_set","vpx_codec_dec_init_ver","vpx_codec_vp8_dx","vpx_codec_register_put_frame_cb","vpx_img_alloc","vpx_codec_encode","vpx_codec_decode","vpx_codec_get_frame","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy"],"critical_calls":["vpx_codec_enc_config_default","vpx_codec_vp8_cx","vpx_codec_enc_init_ver","vpx_codec_vp8_cx","vpx_codec_enc_config_set","vpx_codec_dec_init_ver","vpx_codec_vp8_dx","vpx_codec_register_put_frame_cb","vpx_img_alloc","vpx_codec_encode","vpx_codec_decode","vpx_codec_get_frame","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy"],"visited":2}
/**/


// Include the libvpx headers

// Implement the LLVMFuzzerTestOneInput_12 function
extern "C" int LLVMFuzzerTestOneInput_12(const uint8_t* f_data, size_t f_size) {
	if(f_size<60) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_1, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_2, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_3, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_4, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_5, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_6, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_7, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_8, fdp);
	//fuzzer shim end}




    // Create a vpx_codec_enc_cfg_t structure for encoder configuration
    vpx_codec_enc_cfg_t enc_cfg;
    vpx_codec_enc_config_default(vpx_codec_vp8_cx(), &enc_cfg, fuzz_uint32_t_1);

    // Create an encoder context
    vpx_codec_ctx_t enc_ctx;
    vpx_codec_enc_init_ver(&enc_ctx, vpx_codec_vp8_cx(), &enc_cfg, fuzz_int64_t_2, VPX_ENCODER_ABI_VERSION);

    // Set encoder configuration
    vpx_codec_enc_config_set(&enc_ctx, &enc_cfg);

    // Create a decoder context
    vpx_codec_ctx_t dec_ctx;
    vpx_codec_dec_cfg_t dec_cfg;
    dec_cfg.w = enc_cfg.g_w;
    dec_cfg.h = enc_cfg.g_h;
    vpx_codec_dec_init_ver(&dec_ctx, vpx_codec_vp8_dx(), &dec_cfg, fuzz_int64_t_3, VPX_DECODER_ABI_VERSION);

    // Register a put frame callback function for the encoder
    vpx_codec_register_put_frame_cb(&enc_ctx, NULL, NULL);

    // Create an input image
    vpx_image_t img;
    vpx_img_alloc(&img, VPX_IMG_FMT_I420, enc_cfg.g_w, enc_cfg.g_h, 32);

    // Encode the input image
    vpx_codec_encode(&enc_ctx, &img, fuzz_int64_t_4, fuzz_uint64_t_5, fuzz_int64_t_6, fuzz_uint64_t_7);

    // Decode the encoded data
    vpx_codec_decode(&dec_ctx, data, size, NULL, fuzz_int64_t_8);

    // Get the decoded frame
    vpx_codec_iter_t iter = NULL;
    const vpx_image_t *dec_img = vpx_codec_get_frame(&dec_ctx, &iter);

    // Free the image memory
    vpx_img_free(&img);

    // Destroy the encoder and decoder contexts
    vpx_codec_destroy(&enc_ctx);
    vpx_codec_destroy(&dec_ctx);

    return 0;
}