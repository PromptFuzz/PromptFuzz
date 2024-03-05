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
//<ID> 2019
//<Prompt> ["vpx_img_wrap","vpx_codec_register_put_slice_cb","vpx_codec_enc_init_ver","vpx_codec_vp8_cx","vpx_codec_encode","vpx_codec_dec_init_ver","vpx_codec_vp8_dx","vpx_codec_get_global_headers","vpx_codec_decode","vpx_write_tpl_gop_stats","vpx_read_tpl_gop_stats","vpx_codec_error","vpx_codec_set_cx_data_buf","vpx_free_tpl_gop_stats","vpx_codec_destroy","vpx_img_free"]
/*<Combination>: [vpx_image_t *vpx_img_wrap(vpx_image_t * img, vpx_img_fmt_t fmt, unsigned int d_w, unsigned int d_h, unsigned int stride_align, unsigned char * img_data),
    vpx_codec_err_t vpx_codec_register_put_slice_cb(vpx_codec_ctx_t * ctx, vpx_codec_put_slice_cb_fn_t cb, void * user_priv),
    vpx_codec_err_t vpx_codec_enc_init_ver(vpx_codec_ctx_t * ctx, vpx_codec_iface_t * iface, const vpx_codec_enc_cfg_t * cfg, vpx_codec_flags_t flags, int ver),
    vpx_codec_iface_t *vpx_codec_vp8_cx(),
    vpx_codec_err_t vpx_codec_encode(vpx_codec_ctx_t * ctx, const vpx_image_t * img, vpx_codec_pts_t pts, unsigned long duration, vpx_enc_frame_flags_t flags, unsigned long deadline),
    vpx_codec_err_t vpx_codec_dec_init_ver(vpx_codec_ctx_t * ctx, vpx_codec_iface_t * iface, const vpx_codec_dec_cfg_t * cfg, vpx_codec_flags_t flags, int ver),
    vpx_codec_iface_t *vpx_codec_vp8_dx(),
    vpx_fixed_buf_t *vpx_codec_get_global_headers(vpx_codec_ctx_t * ctx),
    vpx_codec_err_t vpx_codec_decode(vpx_codec_ctx_t * ctx, const uint8_t * data, unsigned int data_sz, void * user_priv, long deadline),
    vpx_codec_err_t vpx_write_tpl_gop_stats(FILE * tpl_file, const VpxTplGopStats * tpl_gop_stats),
    vpx_codec_err_t vpx_read_tpl_gop_stats(FILE * tpl_file, VpxTplGopStats * tpl_gop_stats),
    const char *vpx_codec_error(const vpx_codec_ctx_t * ctx),
    vpx_codec_err_t vpx_codec_set_cx_data_buf(vpx_codec_ctx_t * ctx, const vpx_fixed_buf_t * buf, unsigned int pad_before, unsigned int pad_after),
    void vpx_free_tpl_gop_stats(VpxTplGopStats * tpl_gop_stats),
    vpx_codec_err_t vpx_codec_destroy(vpx_codec_ctx_t * ctx),
    void vpx_img_free(vpx_image_t * img)
*/
//<score> 10, nr_unique_branch: 1
//<Quality> {"density":5,"unique_branches":{"validate_config":[[118,33,118,49,3,0,4,1]]},"library_calls":["vpx_codec_vp8_cx","vpx_codec_vp8_dx","vpx_codec_enc_init_ver","vpx_codec_dec_init_ver","vpx_codec_encode","vpx_codec_decode","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp8_cx","vpx_codec_vp8_dx","vpx_codec_enc_init_ver","vpx_codec_dec_init_ver","vpx_codec_encode","vpx_codec_decode","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy"],"visited":0}
/**/



extern "C" int LLVMFuzzerTestOneInput_45(const uint8_t *data, size_t size) {
	if(size<0) return 0;

    // Step 4: Create input and output file streams
    FILE *in_file = fmemopen((void *)data, size, "rb");
    FILE *out_file = fopen("output_file", "wb");

    // Step 5: Get file descriptors
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);
    
    // Step 1: Initialize variables and structures
    vpx_codec_ctx_t encoder;
    vpx_codec_ctx_t decoder;
    vpx_codec_enc_cfg_t enc_config;
    vpx_codec_dec_cfg_t dec_config;
    vpx_codec_iface_t *enc_iface = vpx_codec_vp8_cx();
    vpx_codec_iface_t *dec_iface = vpx_codec_vp8_dx();
    vpx_image_t raw_frame;
    vpx_fixed_buf_t global_headers;

    // Step 2: Initialize encoder
    enc_config.g_w = 640;
    enc_config.g_h = 480;
    vpx_codec_enc_init_ver(&encoder, enc_iface, &enc_config, 0, VPX_ENCODER_ABI_VERSION);
    
    // Step 2: Initialize decoder
    dec_config.w = 640;
    dec_config.h = 480;
    vpx_codec_dec_init_ver(&decoder, dec_iface, &dec_config, 0, VPX_DECODER_ABI_VERSION);
    
    // Step 3: Encode and decode frames
    vpx_codec_encode(&encoder, &raw_frame, 0, 0, 0, 0);
    vpx_codec_decode(&decoder, data, size, NULL, -1);

    // Step 6: Release resources
    vpx_img_free(&raw_frame);
    vpx_codec_destroy(&encoder);
    vpx_codec_destroy(&decoder);
    
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}