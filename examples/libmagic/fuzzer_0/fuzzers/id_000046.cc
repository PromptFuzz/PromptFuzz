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
//<ID> 2093
//<Prompt> ["vpx_codec_vp8_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_img_alloc","vpx_codec_encode","vpx_codec_get_stream_info","vpx_write_tpl_gop_stats","vpx_codec_vp8_dx","vpx_codec_dec_init_ver","vpx_codec_set_frame_buffer_functions","vpx_codec_get_preview_frame","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_destroy"]
/*<Combination>: [vpx_codec_iface_t *vpx_codec_vp8_cx(),
    vpx_codec_err_t vpx_codec_enc_config_default(vpx_codec_iface_t * iface, vpx_codec_enc_cfg_t * cfg, unsigned int usage),
    vpx_codec_err_t vpx_codec_enc_init_ver(vpx_codec_ctx_t * ctx, vpx_codec_iface_t * iface, const vpx_codec_enc_cfg_t * cfg, vpx_codec_flags_t flags, int ver),
    vpx_image_t *vpx_img_alloc(vpx_image_t * img, vpx_img_fmt_t fmt, unsigned int d_w, unsigned int d_h, unsigned int align),
    vpx_codec_err_t vpx_codec_encode(vpx_codec_ctx_t * ctx, const vpx_image_t * img, vpx_codec_pts_t pts, unsigned long duration, vpx_enc_frame_flags_t flags, unsigned long deadline),
    vpx_codec_err_t vpx_codec_get_stream_info(vpx_codec_ctx_t * ctx, vpx_codec_stream_info_t * si),
    vpx_codec_err_t vpx_write_tpl_gop_stats(FILE * tpl_file, const VpxTplGopStats * tpl_gop_stats),
    vpx_codec_iface_t *vpx_codec_vp8_dx(),
    vpx_codec_err_t vpx_codec_dec_init_ver(vpx_codec_ctx_t * ctx, vpx_codec_iface_t * iface, const vpx_codec_dec_cfg_t * cfg, vpx_codec_flags_t flags, int ver),
    vpx_codec_err_t vpx_codec_set_frame_buffer_functions(vpx_codec_ctx_t * ctx, vpx_get_frame_buffer_cb_fn_t cb_get, vpx_release_frame_buffer_cb_fn_t cb_release, void * cb_priv),
    const vpx_image_t *vpx_codec_get_preview_frame(vpx_codec_ctx_t * ctx),
    vpx_codec_err_t vpx_codec_encode(vpx_codec_ctx_t * ctx, const vpx_image_t * img, vpx_codec_pts_t pts, unsigned long duration, vpx_enc_frame_flags_t flags, unsigned long deadline),
    const vpx_codec_cx_pkt_t *vpx_codec_get_cx_data(vpx_codec_ctx_t * ctx, vpx_codec_iter_t * iter),
    vpx_codec_err_t vpx_codec_destroy(vpx_codec_ctx_t * ctx)
*/
//<score> 8.75, nr_unique_branch: 15
//<Quality> {"density":10,"unique_branches":{"vp8_convert_rfct_to_prob":[[394,7,394,72,0,0,4,0]],"vp8_loop_filter_frame":[[304,13,304,25,0,0,4,1]],"vp8_estimate_entropy_savings":[[810,9,810,62,0,0,4,0]],"vp8_loop_filter_frame_yonly":[[425,11,425,23,0,0,4,1]],"calc_pframe_target_size":[[594,11,594,37,0,0,4,1],[621,11,621,39,0,0,4,1],[691,11,691,63,0,0,4,1],[692,11,692,66,0,0,4,1],[787,13,787,62,0,0,4,1],[792,20,792,73,0,0,4,0],[797,13,797,52,0,0,4,0],[810,13,810,30,0,0,4,0],[810,34,810,54,0,0,4,1],[22,23,22,34,2,0,4,0]],"pack_inter_mode_mvs":[[430,9,430,29,0,0,4,0]]},"library_calls":["vpx_codec_vp8_cx","vpx_codec_vp8_dx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_img_alloc","vpx_codec_encode","vpx_codec_get_stream_info","vpx_write_tpl_gop_stats","vpx_codec_dec_init_ver","vpx_codec_set_frame_buffer_functions","vpx_codec_get_preview_frame","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_destroy","vpx_codec_destroy","vpx_img_free"],"critical_calls":["vpx_codec_vp8_cx","vpx_codec_vp8_dx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_img_alloc","vpx_codec_encode","vpx_codec_get_stream_info","vpx_write_tpl_gop_stats","vpx_codec_dec_init_ver","vpx_codec_set_frame_buffer_functions","vpx_codec_get_preview_frame","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_destroy","vpx_codec_destroy","vpx_img_free"],"visited":7}
/**/

extern "C" int LLVMFuzzerTestOneInput_46(const uint8_t *data, size_t size) {
	if(size<0) return 0;

    // Step 1: Create libvpx encoder and decoder
    vpx_codec_iface_t *encoder_iface = vpx_codec_vp8_cx();
    vpx_codec_iface_t *decoder_iface = vpx_codec_vp8_dx();

    // Step 2: Create encoder configuration
    vpx_codec_enc_cfg_t encoder_cfg;
    vpx_codec_enc_config_default(encoder_iface, &encoder_cfg, 0);

    // Step 3: Initialize encoder
    vpx_codec_ctx_t encoder_ctx;
    vpx_codec_enc_init_ver(&encoder_ctx, encoder_iface, &encoder_cfg, 0, VPX_ENCODER_ABI_VERSION);

    // Step 4: Allocate input image
    vpx_image_t input_image;
    vpx_img_alloc(&input_image, VPX_IMG_FMT_I420, encoder_cfg.g_w, encoder_cfg.g_h, 1);

    // Step 5: Encode the input image
    vpx_codec_pts_t pts = 0;
    unsigned long duration = 1;
    vpx_enc_frame_flags_t flags = 0;
    unsigned long deadline = 0;
    vpx_codec_encode(&encoder_ctx, &input_image, pts, duration, flags, deadline);

    // Step 6: Get stream info
    vpx_codec_stream_info_t stream_info;
    vpx_codec_get_stream_info(&encoder_ctx, &stream_info);

    // Step 7: Write GOP stats
    FILE *tpl_file = fopen("gop_stats", "wb");
    VpxTplGopStats tpl_gop_stats;
    vpx_write_tpl_gop_stats(tpl_file, &tpl_gop_stats);
    assert_file_closed(&tpl_file);;

    // Step 8: Initialize decoder
    vpx_codec_ctx_t decoder_ctx;
    vpx_codec_dec_cfg_t decoder_cfg;
    decoder_cfg.threads = 1;
    decoder_cfg.w = encoder_cfg.g_w;
    decoder_cfg.h = encoder_cfg.g_h;
    vpx_codec_dec_init_ver(&decoder_ctx, decoder_iface, &decoder_cfg, 0, VPX_DECODER_ABI_VERSION);

    // Step 9: Set frame buffer functions
    vpx_codec_set_frame_buffer_functions(&decoder_ctx, nullptr, nullptr, nullptr);

    // Step 10: Get preview frame
    const vpx_image_t *preview_frame = vpx_codec_get_preview_frame(&decoder_ctx);

    // Step 11: Encode again
    vpx_codec_encode(&encoder_ctx, &input_image, pts, duration, flags, deadline);

    // Step 12: Get compressed data
    const vpx_codec_cx_pkt_t *cx_data = nullptr;
    vpx_codec_iter_t iter = nullptr;
    while ((cx_data = vpx_codec_get_cx_data(&encoder_ctx, &iter)) != nullptr) {
        // Process compressed data (cx_data->data.frame.buf, cx_data->data.frame.sz)
    }

    // Step 13: Destroy encoder and decoder contexts
    vpx_codec_destroy(&encoder_ctx);
    vpx_codec_destroy(&decoder_ctx);

    // Step 14: Free allocated resources
    vpx_img_free(&input_image);

    assert_file_closed(&tpl_file);
	return 0;
}