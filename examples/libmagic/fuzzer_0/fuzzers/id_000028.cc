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
//<ID> 1278
//<Prompt> ["vpx_codec_vp9_dx","vpx_codec_dec_init_ver","vpx_codec_peek_stream_info","vpx_write_tpl_gop_stats","vpx_codec_register_put_frame_cb","vpx_codec_control_","vpx_codec_enc_init_multi_ver","vpx_codec_error_detail","vpx_img_wrap","vpx_img_set_rect","vpx_codec_decode","vpx_codec_get_preview_frame","vpx_img_free","vpx_codec_destroy"]
/*<Combination>: [vpx_codec_iface_t *vpx_codec_vp9_dx(),
    vpx_codec_err_t vpx_codec_dec_init_ver(vpx_codec_ctx_t * ctx, vpx_codec_iface_t * iface, const vpx_codec_dec_cfg_t * cfg, vpx_codec_flags_t flags, int ver),
    vpx_codec_err_t vpx_codec_peek_stream_info(vpx_codec_iface_t * iface, const uint8_t * data, unsigned int data_sz, vpx_codec_stream_info_t * si),
    vpx_codec_err_t vpx_write_tpl_gop_stats(FILE * tpl_file, const VpxTplGopStats * tpl_gop_stats),
    vpx_codec_err_t vpx_codec_register_put_frame_cb(vpx_codec_ctx_t * ctx, vpx_codec_put_frame_cb_fn_t cb, void * user_priv),
    vpx_codec_err_t vpx_codec_control_(vpx_codec_ctx_t * ctx, int ctrl_id),
    vpx_codec_err_t vpx_codec_enc_init_multi_ver(vpx_codec_ctx_t * ctx, vpx_codec_iface_t * iface, vpx_codec_enc_cfg_t * cfg, int num_enc, vpx_codec_flags_t flags, vpx_rational_t * dsf, int ver),
    const char *vpx_codec_error_detail(const vpx_codec_ctx_t * ctx),
    vpx_image_t *vpx_img_wrap(vpx_image_t * img, vpx_img_fmt_t fmt, unsigned int d_w, unsigned int d_h, unsigned int stride_align, unsigned char * img_data),
    int vpx_img_set_rect(vpx_image_t * img, unsigned int x, unsigned int y, unsigned int w, unsigned int h),
    vpx_codec_err_t vpx_codec_decode(vpx_codec_ctx_t * ctx, const uint8_t * data, unsigned int data_sz, void * user_priv, long deadline),
    const vpx_image_t *vpx_codec_get_preview_frame(vpx_codec_ctx_t * ctx),
    void vpx_img_free(vpx_image_t * img),
    vpx_codec_err_t vpx_codec_destroy(vpx_codec_ctx_t * ctx)
*/
//<score> 8.25, nr_unique_branch: 5
//<Quality> {"density":11,"unique_branches":{"vpx_img_set_rect":[[162,9,162,41,0,0,4,0]],"img_alloc_helper":[[54,5,54,12,0,0,4,0],[66,5,66,12,0,0,4,0],[76,5,76,12,0,0,4,0],[83,7,83,33,0,0,4,1]]},"library_calls":["vpx_codec_vp9_dx","vpx_codec_dec_init_ver","vpx_codec_peek_stream_info","vpx_write_tpl_gop_stats","vpx_codec_register_put_frame_cb","vpx_codec_control_","vpx_img_wrap","vpx_img_set_rect","vpx_codec_decode","vpx_codec_get_preview_frame","vpx_img_free","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp9_dx","vpx_codec_dec_init_ver","vpx_codec_peek_stream_info","vpx_write_tpl_gop_stats","vpx_codec_register_put_frame_cb","vpx_codec_control_","vpx_img_wrap","vpx_img_set_rect","vpx_codec_decode","vpx_codec_get_preview_frame","vpx_img_free","vpx_codec_destroy"],"visited":3}
/*Here is a C++ program that achieves the event described using the libvpx APIs:

*/


extern "C" int LLVMFuzzerTestOneInput_28(const uint8_t *data, size_t size) {
	if(size<0) return 0;

    // Create input file
    FILE *in_file = fmemopen((void *)data, size, "rb");
    
    // Create output file
    FILE *out_file = fopen("output_file", "wb");
    
    // Initialize libvpx decoder
    vpx_codec_iface_t *decoder_iface = vpx_codec_vp9_dx();
    vpx_codec_ctx_t decoder_ctx;
    vpx_codec_dec_cfg_t decoder_cfg;
    memset(&decoder_cfg, 0, sizeof(decoder_cfg));
    decoder_cfg.threads = 1;
    decoder_cfg.w = 1920;
    decoder_cfg.h = 1080;
    vpx_codec_err_t decoder_init_res = vpx_codec_dec_init_ver(&decoder_ctx, decoder_iface, &decoder_cfg, 0, VPX_DECODER_ABI_VERSION);
    if (decoder_init_res != VPX_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Peek stream info
    vpx_codec_stream_info_t stream_info;
    vpx_codec_peek_stream_info(decoder_iface, (const uint8_t *)data, size, &stream_info);
    
    // Write GOP stats to file
    VpxTplGopStats tpl_gop_stats;
    vpx_write_tpl_gop_stats(out_file, &tpl_gop_stats);
    
    // Register put frame callback
    vpx_codec_register_put_frame_cb(&decoder_ctx, NULL, NULL);
    
    // Control codec
    vpx_codec_control_(&decoder_ctx, 0);
    
    // Wrap input image
    vpx_image_t img;
    vpx_img_wrap(&img, VPX_IMG_FMT_NONE, 1920, 1080, 32, (unsigned char *)data);
    
    // Set image rect
    vpx_img_set_rect(&img, 0, 0, 1920, 1080);
    
    // Decode image
    vpx_codec_decode(&decoder_ctx, data, size, NULL, 0);
    
    // Get preview frame
    const vpx_image_t *preview_frame = vpx_codec_get_preview_frame(&decoder_ctx);
    
    // Free image
    vpx_img_free(&img);
    
    // Destroy decoder
    vpx_codec_destroy(&decoder_ctx);
    
    // Close files
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}