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
//<ID> 2609
//<Prompt> ["vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_multi_ver","vpx_codec_register_put_frame_cb","vpx_img_alloc","vpx_codec_encode","vpx_codec_get_global_headers","vpx_codec_register_put_frame_cb","vpx_codec_encode","vpx_codec_decode","vpx_codec_get_stream_info","vpx_codec_decode","vpx_img_free","vpx_codec_destroy"]
/*<Combination>: [vpx_codec_iface_t *vpx_codec_vp9_cx(),
    vpx_codec_err_t vpx_codec_enc_config_default(vpx_codec_iface_t * iface, vpx_codec_enc_cfg_t * cfg, unsigned int usage),
    vpx_codec_err_t vpx_codec_enc_init_multi_ver(vpx_codec_ctx_t * ctx, vpx_codec_iface_t * iface, vpx_codec_enc_cfg_t * cfg, int num_enc, vpx_codec_flags_t flags, vpx_rational_t * dsf, int ver),
    vpx_codec_err_t vpx_codec_register_put_frame_cb(vpx_codec_ctx_t * ctx, vpx_codec_put_frame_cb_fn_t cb, void * user_priv),
    vpx_image_t *vpx_img_alloc(vpx_image_t * img, vpx_img_fmt_t fmt, unsigned int d_w, unsigned int d_h, unsigned int align),
    vpx_codec_err_t vpx_codec_encode(vpx_codec_ctx_t * ctx, const vpx_image_t * img, vpx_codec_pts_t pts, unsigned long duration, vpx_enc_frame_flags_t flags, unsigned long deadline),
    vpx_fixed_buf_t *vpx_codec_get_global_headers(vpx_codec_ctx_t * ctx),
    vpx_codec_err_t vpx_codec_register_put_frame_cb(vpx_codec_ctx_t * ctx, vpx_codec_put_frame_cb_fn_t cb, void * user_priv),
    vpx_codec_err_t vpx_codec_encode(vpx_codec_ctx_t * ctx, const vpx_image_t * img, vpx_codec_pts_t pts, unsigned long duration, vpx_enc_frame_flags_t flags, unsigned long deadline),
    vpx_codec_err_t vpx_codec_decode(vpx_codec_ctx_t * ctx, const uint8_t * data, unsigned int data_sz, void * user_priv, long deadline),
    vpx_codec_err_t vpx_codec_get_stream_info(vpx_codec_ctx_t * ctx, vpx_codec_stream_info_t * si),
    vpx_codec_err_t vpx_codec_decode(vpx_codec_ctx_t * ctx, const uint8_t * data, unsigned int data_sz, void * user_priv, long deadline),
    void vpx_img_free(vpx_image_t * img),
    vpx_codec_err_t vpx_codec_destroy(vpx_codec_ctx_t * ctx)
*/
//<score> 9, nr_unique_branch: 0
//<Quality> {"density":9,"unique_branches":{},"library_calls":["vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_multi_ver","vpx_codec_register_put_frame_cb","vpx_img_alloc","vpx_codec_encode","vpx_codec_get_global_headers","vpx_codec_vp9_dx","vpx_codec_dec_init_ver","vpx_codec_decode","vpx_codec_get_stream_info","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_multi_ver","vpx_codec_register_put_frame_cb","vpx_img_alloc","vpx_codec_encode","vpx_codec_get_global_headers","vpx_codec_vp9_dx","vpx_codec_dec_init_ver","vpx_codec_decode","vpx_codec_get_stream_info","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy"],"visited":0}
/**/

extern "C" int LLVMFuzzerTestOneInput_66(const uint8_t *data, size_t size) {
	if(size<0) return 0;

    // Create a VP9 encoder
    vpx_codec_ctx_t enc_ctx;
    vpx_codec_iface_t *iface = vpx_codec_vp9_cx();
    vpx_codec_enc_cfg_t enc_cfg;
    vpx_codec_enc_config_default(iface, &enc_cfg, 0);
    vpx_codec_enc_init_multi_ver(&enc_ctx, iface, &enc_cfg, 1, 0, NULL, VPX_CODEC_USE_PSNR);
    
    // Register a frame callback
    vpx_codec_register_put_frame_cb(&enc_ctx, [](void *user_priv, const vpx_image_t *img) -> void {
        // Handle the frame callback
    }, NULL);
    
    // Allocate an image
    vpx_image_t img;
    vpx_img_alloc(&img, VPX_IMG_FMT_I420, enc_cfg.g_w, enc_cfg.g_h, 32);
    
    // Encode the image
    vpx_codec_encode(&enc_ctx, &img, 0, 1, 0, VPX_DL_GOOD_QUALITY);
    
    // Get the global headers
    vpx_fixed_buf_t *global_headers = vpx_codec_get_global_headers(&enc_ctx);
    
    // Create a VP9 decoder
    vpx_codec_ctx_t dec_ctx;
    vpx_codec_iface_t *dec_iface = vpx_codec_vp9_dx();
    vpx_codec_dec_cfg_t dec_cfg;
    vpx_codec_dec_init_ver(&dec_ctx, dec_iface, &dec_cfg, 0, VPX_DECODER_ABI_VERSION);
    
    // Decode the data
    vpx_codec_decode(&dec_ctx, data, size, NULL, -1);
    
    // Get the stream info
    vpx_codec_stream_info_t si;
    vpx_codec_get_stream_info(&dec_ctx, &si);
    
    // Cleanup
    vpx_img_free(&img);
    vpx_codec_destroy(&enc_ctx);
    vpx_codec_destroy(&dec_ctx);
    
    return 0;
}