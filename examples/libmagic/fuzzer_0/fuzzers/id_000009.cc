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
//<ID> 462
//<Prompt> ["vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_register_put_frame_cb","vpx_img_alloc","vpx_codec_encode","vpx_codec_get_global_headers","vpx_codec_vp9_dx","vpx_codec_dec_init_ver","vpx_codec_register_put_slice_cb","vpx_codec_encode","vpx_codec_set_frame_buffer_functions","vpx_codec_destroy"]
/*<Combination>: [vpx_codec_iface_t *vpx_codec_vp9_cx(),
    vpx_codec_err_t vpx_codec_enc_config_default(vpx_codec_iface_t * iface, vpx_codec_enc_cfg_t * cfg, unsigned int usage),
    vpx_codec_err_t vpx_codec_enc_init_ver(vpx_codec_ctx_t * ctx, vpx_codec_iface_t * iface, const vpx_codec_enc_cfg_t * cfg, vpx_codec_flags_t flags, int ver),
    vpx_codec_err_t vpx_codec_register_put_frame_cb(vpx_codec_ctx_t * ctx, vpx_codec_put_frame_cb_fn_t cb, void * user_priv),
    vpx_image_t *vpx_img_alloc(vpx_image_t * img, vpx_img_fmt_t fmt, unsigned int d_w, unsigned int d_h, unsigned int align),
    vpx_codec_err_t vpx_codec_encode(vpx_codec_ctx_t * ctx, const vpx_image_t * img, vpx_codec_pts_t pts, unsigned long duration, vpx_enc_frame_flags_t flags, unsigned long deadline),
    vpx_fixed_buf_t *vpx_codec_get_global_headers(vpx_codec_ctx_t * ctx),
    vpx_codec_iface_t *vpx_codec_vp9_dx(),
    vpx_codec_err_t vpx_codec_dec_init_ver(vpx_codec_ctx_t * ctx, vpx_codec_iface_t * iface, const vpx_codec_dec_cfg_t * cfg, vpx_codec_flags_t flags, int ver),
    vpx_codec_err_t vpx_codec_register_put_slice_cb(vpx_codec_ctx_t * ctx, vpx_codec_put_slice_cb_fn_t cb, void * user_priv),
    vpx_codec_err_t vpx_codec_encode(vpx_codec_ctx_t * ctx, const vpx_image_t * img, vpx_codec_pts_t pts, unsigned long duration, vpx_enc_frame_flags_t flags, unsigned long deadline),
    vpx_codec_err_t vpx_codec_set_frame_buffer_functions(vpx_codec_ctx_t * ctx, vpx_get_frame_buffer_cb_fn_t cb_get, vpx_release_frame_buffer_cb_fn_t cb_release, void * cb_priv),
    vpx_codec_err_t vpx_codec_destroy(vpx_codec_ctx_t * ctx)
*/
//<score> 9, nr_unique_branch: 1
//<Quality> {"density":9,"unique_branches":{"vp9_set_row_mt":[[7052,37,7052,57,0,0,4,1]]},"library_calls":["vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_register_put_frame_cb","vpx_img_alloc","vpx_codec_encode","vpx_codec_get_global_headers","vpx_codec_vp9_dx","vpx_codec_dec_init_ver","vpx_codec_register_put_slice_cb","vpx_codec_set_frame_buffer_functions","vpx_codec_decode","vpx_codec_destroy","vpx_codec_destroy","vpx_img_free"],"critical_calls":["vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_register_put_frame_cb","vpx_img_alloc","vpx_codec_encode","vpx_codec_get_global_headers","vpx_codec_vp9_dx","vpx_codec_dec_init_ver","vpx_codec_register_put_slice_cb","vpx_codec_set_frame_buffer_functions","vpx_codec_decode","vpx_codec_destroy","vpx_codec_destroy","vpx_img_free"],"visited":1}
/*Here is the C++ program that achieves the event using the libvpx APIs:

*/



extern "C" int LLVMFuzzerTestOneInput_9(const uint8_t *data, size_t size) {
	if(size<0) return 0;


    FILE *in_file = fmemopen((void *)data, size, "rb");
    FILE *out_file = fopen("output_file", "wb");

    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);

    // Initialize encoder
    vpx_codec_ctx_t enc_ctx;
    vpx_codec_enc_cfg_t enc_cfg;
    vpx_codec_iface_t *enc_iface = vpx_codec_vp9_cx();
    vpx_codec_enc_config_default(enc_iface, &enc_cfg, 0);
    vpx_codec_enc_init_ver(&enc_ctx, enc_iface, &enc_cfg, 0, VPX_ENCODER_ABI_VERSION);

    // Register put frame callback
    vpx_codec_register_put_frame_cb(&enc_ctx, NULL, NULL);

    // Initialize image
    vpx_image_t raw_img;
    vpx_img_alloc(&raw_img, VPX_IMG_FMT_I420, enc_cfg.g_w, enc_cfg.g_h, 32);

    // Encode the image
    vpx_codec_encode(&enc_ctx, &raw_img, 0, 1, 0, VPX_DL_REALTIME);

    // Get global headers
    vpx_fixed_buf_t *global_headers = vpx_codec_get_global_headers(&enc_ctx);

    // Initialize decoder
    vpx_codec_ctx_t dec_ctx;
    vpx_codec_dec_cfg_t dec_cfg;
    vpx_codec_iface_t *dec_iface = vpx_codec_vp9_dx();
    vpx_codec_dec_init_ver(&dec_ctx, dec_iface, &dec_cfg, 0, VPX_DECODER_ABI_VERSION);

    // Register put slice callback
    vpx_codec_register_put_slice_cb(&dec_ctx, NULL, NULL);

    // Set frame buffer functions
    vpx_codec_set_frame_buffer_functions(&dec_ctx, NULL, NULL, NULL);

    // Decode the encoded data
    vpx_codec_decode(&dec_ctx, data, size, NULL, -1);

    // Destroy the codec context
    vpx_codec_destroy(&enc_ctx);
    vpx_codec_destroy(&dec_ctx);

    // Free the image
    vpx_img_free(&raw_img);

    // Close the files
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}