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
//<ID> 513
//<Prompt> ["vpx_codec_enc_config_set","vpx_img_wrap","vpx_codec_register_put_slice_cb","vpx_codec_control_","vpx_codec_get_preview_frame","vpx_codec_error","vpx_free_tpl_gop_stats","vpx_codec_get_cx_data","vpx_img_free"]
/*<Combination>: [vpx_codec_err_t vpx_codec_enc_config_set(vpx_codec_ctx_t * ctx, const vpx_codec_enc_cfg_t * cfg),
    vpx_image_t *vpx_img_wrap(vpx_image_t * img, vpx_img_fmt_t fmt, unsigned int d_w, unsigned int d_h, unsigned int stride_align, unsigned char * img_data),
    vpx_codec_err_t vpx_codec_register_put_slice_cb(vpx_codec_ctx_t * ctx, vpx_codec_put_slice_cb_fn_t cb, void * user_priv),
    vpx_codec_err_t vpx_codec_control_(vpx_codec_ctx_t * ctx, int ctrl_id),
    const vpx_image_t *vpx_codec_get_preview_frame(vpx_codec_ctx_t * ctx),
    const char *vpx_codec_error(const vpx_codec_ctx_t * ctx),
    void vpx_free_tpl_gop_stats(VpxTplGopStats * tpl_gop_stats),
    const vpx_codec_cx_pkt_t *vpx_codec_get_cx_data(vpx_codec_ctx_t * ctx, vpx_codec_iter_t * iter),
    void vpx_img_free(vpx_image_t * img)
*/
//<score> 10, nr_unique_branch: 1
//<Quality> {"density":10,"unique_branches":{"vpx_codec_err_to_string":[[36,5,36,25,0,0,4,0]]},"library_calls":["vpx_codec_vp8_cx","vpx_codec_enc_config_default","vpx_codec_enc_config_set","vpx_img_wrap","vpx_codec_vp8_dx","vpx_codec_dec_init_ver","vpx_codec_register_put_slice_cb","vpx_codec_encode","vpx_codec_get_preview_frame","vpx_codec_error","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp8_cx","vpx_codec_enc_config_default","vpx_codec_enc_config_set","vpx_img_wrap","vpx_codec_vp8_dx","vpx_codec_dec_init_ver","vpx_codec_register_put_slice_cb","vpx_codec_encode","vpx_codec_get_preview_frame","vpx_codec_error","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy"],"visited":1}
/*Here is the C++ program that achieves the event using the mentioned libvpx APIs:

*/



extern "C" int LLVMFuzzerTestOneInput_10(const uint8_t* data, size_t size) {
	if(size<0) return 0;

  // Create a vpx_codec_ctx for encoding
  vpx_codec_ctx_t enc_ctx;
  vpx_codec_enc_cfg_t enc_cfg;
  vpx_codec_iface_t* enc_iface = vpx_codec_vp8_cx();
  vpx_codec_enc_config_default(enc_iface, &enc_cfg, 0);
  vpx_codec_enc_config_set(&enc_ctx, &enc_cfg);

  // Create a vpx_image to hold the input data
  vpx_image_t img;
  vpx_img_wrap(&img, VPX_IMG_FMT_I420, enc_cfg.g_w, enc_cfg.g_h, 1,
               const_cast<uint8_t*>(data));

  // Create a vpx_codec_ctx for decoding
  vpx_codec_ctx_t dec_ctx;
  vpx_codec_dec_cfg_t dec_cfg;
  vpx_codec_iface_t* dec_iface = vpx_codec_vp8_dx();
  vpx_codec_dec_init_ver(&dec_ctx, dec_iface, &dec_cfg, 0, VPX_DECODER_ABI_VERSION);

  // Register a vpx_codec_put_slice_cb callback
  vpx_codec_register_put_slice_cb(&enc_ctx, nullptr, nullptr);

  // Perform encoding
  vpx_codec_encode(&enc_ctx, &img, 0, 1, 0, VPX_DL_GOOD_QUALITY);

  // Get the preview frame from the encoding context
  const vpx_image_t* preview_frame = vpx_codec_get_preview_frame(&enc_ctx);

  // Get the error message from the encoding context
  const char* error_msg = vpx_codec_error(&enc_ctx);

  // Release memory allocated by the vpx_image
  vpx_img_free(&img);

  // Release the encoding and decoding contexts
  vpx_codec_destroy(&enc_ctx);
  vpx_codec_destroy(&dec_ctx);

  return 0;
}