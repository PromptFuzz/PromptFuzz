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
//<ID> 1998
//<Prompt> ["vpx_codec_dec_init_ver","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_register_put_frame_cb","vpx_codec_get_stream_info","vpx_read_tpl_gop_stats","vpx_img_wrap","vpx_img_set_rect","vpx_img_alloc","vpx_codec_enc_init_multi_ver","vpx_codec_decode","vpx_codec_get_preview_frame","vpx_codec_encode","vpx_codec_destroy","vpx_img_free"]
/*<Combination>: [vpx_codec_err_t vpx_codec_dec_init_ver(vpx_codec_ctx_t * ctx, vpx_codec_iface_t * iface, const vpx_codec_dec_cfg_t * cfg, vpx_codec_flags_t flags, int ver),
    vpx_codec_err_t vpx_codec_enc_config_default(vpx_codec_iface_t * iface, vpx_codec_enc_cfg_t * cfg, unsigned int usage),
    vpx_codec_err_t vpx_codec_enc_init_ver(vpx_codec_ctx_t * ctx, vpx_codec_iface_t * iface, const vpx_codec_enc_cfg_t * cfg, vpx_codec_flags_t flags, int ver),
    vpx_codec_err_t vpx_codec_register_put_frame_cb(vpx_codec_ctx_t * ctx, vpx_codec_put_frame_cb_fn_t cb, void * user_priv),
    vpx_codec_err_t vpx_codec_get_stream_info(vpx_codec_ctx_t * ctx, vpx_codec_stream_info_t * si),
    vpx_codec_err_t vpx_read_tpl_gop_stats(FILE * tpl_file, VpxTplGopStats * tpl_gop_stats),
    vpx_image_t *vpx_img_wrap(vpx_image_t * img, vpx_img_fmt_t fmt, unsigned int d_w, unsigned int d_h, unsigned int stride_align, unsigned char * img_data),
    int vpx_img_set_rect(vpx_image_t * img, unsigned int x, unsigned int y, unsigned int w, unsigned int h),
    vpx_image_t *vpx_img_alloc(vpx_image_t * img, vpx_img_fmt_t fmt, unsigned int d_w, unsigned int d_h, unsigned int align),
    vpx_codec_err_t vpx_codec_enc_init_multi_ver(vpx_codec_ctx_t * ctx, vpx_codec_iface_t * iface, vpx_codec_enc_cfg_t * cfg, int num_enc, vpx_codec_flags_t flags, vpx_rational_t * dsf, int ver),
    vpx_codec_err_t vpx_codec_decode(vpx_codec_ctx_t * ctx, const uint8_t * data, unsigned int data_sz, void * user_priv, long deadline),
    const vpx_image_t *vpx_codec_get_preview_frame(vpx_codec_ctx_t * ctx),
    vpx_codec_err_t vpx_codec_encode(vpx_codec_ctx_t * ctx, const vpx_image_t * img, vpx_codec_pts_t pts, unsigned long duration, vpx_enc_frame_flags_t flags, unsigned long deadline),
    vpx_codec_err_t vpx_codec_destroy(vpx_codec_ctx_t * ctx),
    void vpx_img_free(vpx_image_t * img)
*/
//<score> 19, nr_unique_branch: 0
//<Quality> {"density":19,"unique_branches":{},"library_calls":["vpx_codec_enc_config_default","vpx_codec_vp9_cx","vpx_codec_enc_init_ver","vpx_codec_vp9_cx","vpx_codec_register_put_frame_cb","vpx_codec_peek_stream_info","vpx_codec_vp9_dx","vpx_codec_dec_init_ver","vpx_codec_vp9_dx","vpx_read_tpl_gop_stats","vpx_img_wrap","vpx_img_set_rect","vpx_img_alloc","vpx_codec_encode","vpx_codec_decode","vpx_codec_get_preview_frame","vpx_free_tpl_gop_stats","vpx_codec_destroy","vpx_codec_destroy","vpx_img_free","vpx_img_free"],"critical_calls":["vpx_codec_enc_config_default","vpx_codec_vp9_cx","vpx_codec_enc_init_ver","vpx_codec_vp9_cx","vpx_codec_register_put_frame_cb","vpx_codec_peek_stream_info","vpx_codec_vp9_dx","vpx_codec_dec_init_ver","vpx_codec_vp9_dx","vpx_read_tpl_gop_stats","vpx_img_wrap","vpx_img_set_rect","vpx_img_alloc","vpx_codec_encode","vpx_codec_decode","vpx_codec_get_preview_frame","vpx_free_tpl_gop_stats","vpx_codec_destroy","vpx_codec_destroy","vpx_img_free","vpx_img_free"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_43(const uint8_t *data, size_t size) {
	if(size<0) return 0;

    // Step 1: Create variables and allocate memory
    vpx_codec_ctx_t enc_ctx, dec_ctx;
    vpx_codec_enc_cfg_t enc_cfg;
    vpx_codec_dec_cfg_t dec_cfg;
    vpx_codec_stream_info_t si;
    VpxTplGopStats tpl_gop_stats;
    vpx_image_t img, dec_img;
    FILE *tpl_file = fmemopen((void *)data, size, "rb");
    
    // Step 2: Initialize encoder and decoder
    vpx_codec_enc_config_default(vpx_codec_vp9_cx(), &enc_cfg, 0);
    vpx_codec_enc_init_ver(&enc_ctx, vpx_codec_vp9_cx(), &enc_cfg, 0, VPX_ENCODER_ABI_VERSION);
    vpx_codec_register_put_frame_cb(&enc_ctx, NULL, NULL);
    
    vpx_codec_peek_stream_info(vpx_codec_vp9_dx(), data, size, &si);
    vpx_codec_dec_init_ver(&dec_ctx, vpx_codec_vp9_dx(), &dec_cfg, 0, VPX_DECODER_ABI_VERSION);
    
    // Step 3: Read GOP stats from template file
    vpx_read_tpl_gop_stats(tpl_file, &tpl_gop_stats);
    
    // Step 4: Wrap image with input data
    vpx_img_wrap(&img, VPX_IMG_FMT_I420, si.w, si.h, 1, (unsigned char *)data);
    
    // Step 5: Set rectangle on image
    vpx_img_set_rect(&img, 0, 0, si.w, si.h);
    
    // Step 6: Allocate memory for decoded image
    vpx_img_alloc(&dec_img, VPX_IMG_FMT_I420, si.w, si.h, 1);
    
    // Step 7: Encode image
    vpx_codec_encode(&enc_ctx, &img, 0, 1, 0, 0);
    
    // Step 8: Decode image
    vpx_codec_decode(&dec_ctx, data, size, NULL, 0);
    
    // Step 9: Get preview frame
    const vpx_image_t *preview_img = vpx_codec_get_preview_frame(&dec_ctx);
    
    // Step 10: Cleanup and release resources
    vpx_free_tpl_gop_stats(&tpl_gop_stats);
    
    vpx_codec_destroy(&enc_ctx);
    vpx_codec_destroy(&dec_ctx);
    
    vpx_img_free(&img);
    vpx_img_free(&dec_img);
    
    assert_file_closed(&tpl_file);;
    
    assert_file_closed(&tpl_file);
	return 0;
}