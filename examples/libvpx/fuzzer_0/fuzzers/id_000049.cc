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
//<ID> 2142
//<Prompt> ["vpx_codec_dec_init_ver","vpx_codec_vp9_dx","vpx_codec_get_global_headers","vpx_codec_register_put_slice_cb","vpx_write_tpl_gop_stats","vpx_codec_control_","vpx_codec_enc_init_multi_ver","vpx_codec_vp9_cx","vpx_img_wrap","vpx_img_set_rect","vpx_codec_peek_stream_info","vpx_img_alloc","vpx_codec_register_put_frame_cb","vpx_codec_decode","vpx_codec_err_to_string","vpx_img_free","vpx_codec_destroy"]
/*<Combination>: [vpx_codec_err_t vpx_codec_dec_init_ver(vpx_codec_ctx_t * ctx, vpx_codec_iface_t * iface, const vpx_codec_dec_cfg_t * cfg, vpx_codec_flags_t flags, int ver),
    vpx_codec_iface_t *vpx_codec_vp9_dx(),
    vpx_fixed_buf_t *vpx_codec_get_global_headers(vpx_codec_ctx_t * ctx),
    vpx_codec_err_t vpx_codec_register_put_slice_cb(vpx_codec_ctx_t * ctx, vpx_codec_put_slice_cb_fn_t cb, void * user_priv),
    vpx_codec_err_t vpx_write_tpl_gop_stats(FILE * tpl_file, const VpxTplGopStats * tpl_gop_stats),
    vpx_codec_err_t vpx_codec_control_(vpx_codec_ctx_t * ctx, int ctrl_id),
    vpx_codec_err_t vpx_codec_enc_init_multi_ver(vpx_codec_ctx_t * ctx, vpx_codec_iface_t * iface, vpx_codec_enc_cfg_t * cfg, int num_enc, vpx_codec_flags_t flags, vpx_rational_t * dsf, int ver),
    vpx_codec_iface_t *vpx_codec_vp9_cx(),
    vpx_image_t *vpx_img_wrap(vpx_image_t * img, vpx_img_fmt_t fmt, unsigned int d_w, unsigned int d_h, unsigned int stride_align, unsigned char * img_data),
    int vpx_img_set_rect(vpx_image_t * img, unsigned int x, unsigned int y, unsigned int w, unsigned int h),
    vpx_codec_err_t vpx_codec_peek_stream_info(vpx_codec_iface_t * iface, const uint8_t * data, unsigned int data_sz, vpx_codec_stream_info_t * si),
    vpx_image_t *vpx_img_alloc(vpx_image_t * img, vpx_img_fmt_t fmt, unsigned int d_w, unsigned int d_h, unsigned int align),
    vpx_codec_err_t vpx_codec_register_put_frame_cb(vpx_codec_ctx_t * ctx, vpx_codec_put_frame_cb_fn_t cb, void * user_priv),
    vpx_codec_err_t vpx_codec_decode(vpx_codec_ctx_t * ctx, const uint8_t * data, unsigned int data_sz, void * user_priv, long deadline),
    const char *vpx_codec_err_to_string(vpx_codec_err_t err),
    void vpx_img_free(vpx_image_t * img),
    vpx_codec_err_t vpx_codec_destroy(vpx_codec_ctx_t * ctx)
*/
//<score> 9, nr_unique_branch: 0
//<Quality> {"density":9,"unique_branches":{},"library_calls":["vpx_codec_dec_init_ver","vpx_codec_vp9_dx","vpx_codec_get_global_headers","vpx_codec_register_put_slice_cb","vpx_write_tpl_gop_stats","vpx_codec_control_","vpx_codec_enc_init_multi_ver","vpx_codec_vp9_cx","vpx_img_wrap","vpx_img_set_rect","vpx_codec_peek_stream_info","vpx_codec_vp9_dx","vpx_img_alloc","vpx_codec_register_put_frame_cb","vpx_codec_decode","vpx_codec_err_to_string","vpx_img_free","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy"],"critical_calls":["vpx_codec_dec_init_ver","vpx_codec_vp9_dx","vpx_codec_get_global_headers","vpx_codec_register_put_slice_cb","vpx_write_tpl_gop_stats","vpx_codec_control_","vpx_codec_enc_init_multi_ver","vpx_codec_vp9_cx","vpx_img_wrap","vpx_img_set_rect","vpx_codec_peek_stream_info","vpx_codec_vp9_dx","vpx_img_alloc","vpx_codec_register_put_frame_cb","vpx_codec_decode","vpx_codec_err_to_string","vpx_img_free","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_49(const uint8_t *data, size_t size) {
	if(size<0) return 0;

    // Create input file stream
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file == NULL) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Create output file stream
    FILE *out_file = fopen("output_file", "wb");
    if (out_file == NULL) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Initialize libvpx decoder
    vpx_codec_ctx_t decoder;
    vpx_codec_dec_cfg_t dec_cfg;
    vpx_codec_dec_init_ver(&decoder, vpx_codec_vp9_dx(), &dec_cfg, 0, VPX_DECODER_ABI_VERSION);

    // Get global headers
    vpx_fixed_buf_t *global_headers = vpx_codec_get_global_headers(&decoder);

    // Register put slice callback
    vpx_codec_register_put_slice_cb(&decoder, NULL, NULL);

    // Write TPL GOP stats
    FILE *tpl_file = fopen("tpl_file", "wb");
    VpxTplGopStats tpl_gop_stats;
    vpx_write_tpl_gop_stats(tpl_file, &tpl_gop_stats);
    assert_file_closed(&tpl_file);;

    // Control codec
    vpx_codec_control_(&decoder, 0);

    // Initialize libvpx encoder
    vpx_codec_ctx_t encoder;
    vpx_codec_enc_cfg_t enc_cfg;
    vpx_rational_t dsf;
    vpx_codec_enc_init_multi_ver(&encoder, vpx_codec_vp9_cx(), &enc_cfg, 1, 0, &dsf, VPX_ENCODER_ABI_VERSION);

    // Wrap image
    vpx_image_t image;
    vpx_img_wrap(&image, VPX_IMG_FMT_I420, 1280, 720, 1, NULL);

    // Set image rect
    vpx_img_set_rect(&image, 0, 0, 1280, 720);

    // Peek stream info
    vpx_codec_stream_info_t si;
    vpx_codec_peek_stream_info(vpx_codec_vp9_dx(), data, size, &si);

    // Allocate image
    vpx_image_t image_alloc;
    vpx_img_alloc(&image_alloc, VPX_IMG_FMT_I420, 1280, 720, 1);

    // Register put frame callback
    vpx_codec_register_put_frame_cb(&encoder, NULL, NULL);

    // Decode
    vpx_codec_decode(&decoder, data, size, NULL, 0);

    // Get error string
    const char *error_str = vpx_codec_err_to_string(VPX_CODEC_OK);

    // Free images
    vpx_img_free(&image);
    vpx_img_free(&image_alloc);

    // Destroy codecs
    vpx_codec_destroy(&decoder);
    vpx_codec_destroy(&encoder);

    // Close files
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;

    assert_file_closed(&tpl_file);
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}