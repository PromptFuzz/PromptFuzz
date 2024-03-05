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
//<ID> 1975
//<Prompt> ["vpx_codec_dec_init_ver","vpx_codec_vp9_dx","vpx_codec_enc_init_ver","vpx_codec_enc_config_set","vpx_codec_peek_stream_info","vpx_read_tpl_gop_stats","vpx_codec_register_put_frame_cb","vpx_codec_enc_config_default","vpx_codec_enc_init_multi_ver","vpx_img_wrap","vpx_img_set_rect","vpx_codec_decode","vpx_codec_get_preview_frame","vpx_img_free","vpx_codec_destroy"]
/*<Combination>: [vpx_codec_err_t vpx_codec_dec_init_ver(vpx_codec_ctx_t * ctx, vpx_codec_iface_t * iface, const vpx_codec_dec_cfg_t * cfg, vpx_codec_flags_t flags, int ver),
    vpx_codec_iface_t *vpx_codec_vp9_dx(),
    vpx_codec_err_t vpx_codec_enc_init_ver(vpx_codec_ctx_t * ctx, vpx_codec_iface_t * iface, const vpx_codec_enc_cfg_t * cfg, vpx_codec_flags_t flags, int ver),
    vpx_codec_err_t vpx_codec_enc_config_set(vpx_codec_ctx_t * ctx, const vpx_codec_enc_cfg_t * cfg),
    vpx_codec_err_t vpx_codec_peek_stream_info(vpx_codec_iface_t * iface, const uint8_t * data, unsigned int data_sz, vpx_codec_stream_info_t * si),
    vpx_codec_err_t vpx_read_tpl_gop_stats(FILE * tpl_file, VpxTplGopStats * tpl_gop_stats),
    vpx_codec_err_t vpx_codec_register_put_frame_cb(vpx_codec_ctx_t * ctx, vpx_codec_put_frame_cb_fn_t cb, void * user_priv),
    vpx_codec_err_t vpx_codec_enc_config_default(vpx_codec_iface_t * iface, vpx_codec_enc_cfg_t * cfg, unsigned int usage),
    vpx_codec_err_t vpx_codec_enc_init_multi_ver(vpx_codec_ctx_t * ctx, vpx_codec_iface_t * iface, vpx_codec_enc_cfg_t * cfg, int num_enc, vpx_codec_flags_t flags, vpx_rational_t * dsf, int ver),
    vpx_image_t *vpx_img_wrap(vpx_image_t * img, vpx_img_fmt_t fmt, unsigned int d_w, unsigned int d_h, unsigned int stride_align, unsigned char * img_data),
    int vpx_img_set_rect(vpx_image_t * img, unsigned int x, unsigned int y, unsigned int w, unsigned int h),
    vpx_codec_err_t vpx_codec_decode(vpx_codec_ctx_t * ctx, const uint8_t * data, unsigned int data_sz, void * user_priv, long deadline),
    const vpx_image_t *vpx_codec_get_preview_frame(vpx_codec_ctx_t * ctx),
    void vpx_img_free(vpx_image_t * img),
    vpx_codec_err_t vpx_codec_destroy(vpx_codec_ctx_t * ctx)
*/
//<score> 12, nr_unique_branch: 2
//<Quality> {"density":12,"unique_branches":{"vpx_codec_dec_init_ver":[[32,20,32,26,0,0,4,0]],"vpx_codec_enc_init_ver":[[37,20,37,26,0,0,4,0]]},"library_calls":["vpx_codec_dec_init_ver","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_register_put_frame_cb","vpx_read_tpl_gop_stats","vpx_img_wrap","vpx_img_set_rect","vpx_codec_decode","vpx_codec_get_preview_frame","vpx_codec_encode","vpx_codec_destroy","vpx_codec_destroy","vpx_img_free"],"critical_calls":["vpx_codec_dec_init_ver","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_register_put_frame_cb","vpx_read_tpl_gop_stats","vpx_img_wrap","vpx_img_set_rect","vpx_codec_decode","vpx_codec_get_preview_frame","vpx_codec_encode","vpx_codec_destroy","vpx_codec_destroy","vpx_img_free"],"visited":2}
/**/


extern "C" int LLVMFuzzerTestOneInput_41(const uint8_t *data, size_t size) {
	if(size<0) return 0;

  // Create a decoder
  vpx_codec_ctx_t decoder;
  vpx_codec_dec_cfg_t dec_cfg;
  vpx_codec_iface_t *decoder_iface;

  // Initialize the decoder configuration
  dec_cfg.threads = 1;
  dec_cfg.w = 640;
  dec_cfg.h = 480;

  // Initialize the decoder
  vpx_codec_dec_init_ver(&decoder, decoder_iface, &dec_cfg, 0, VPX_DECODER_ABI_VERSION);

  // Create an encoder
  vpx_codec_ctx_t encoder;
  vpx_codec_enc_cfg_t enc_cfg;
  vpx_codec_iface_t *encoder_iface;

  // Initialize the encoder configuration
  vpx_codec_enc_config_default(encoder_iface, &enc_cfg, 0);
  enc_cfg.g_w = 640;
  enc_cfg.g_h = 480;
  enc_cfg.rc_target_bitrate = 1000;

  // Initialize the encoder
  vpx_codec_enc_init_ver(&encoder, encoder_iface, &enc_cfg, 0, VPX_ENCODER_ABI_VERSION);

  // Register put frame callback for the encoder
  vpx_codec_register_put_frame_cb(&encoder, nullptr, nullptr);

  // Read GOP stats from a file
  FILE *gop_file = fopen("gop_stats.txt", "r");
  VpxTplGopStats tpl_gop_stats;
  vpx_read_tpl_gop_stats(gop_file, &tpl_gop_stats);
  assert_file_closed(&gop_file);;

  // Wrap input data as a vpx_image_t
  vpx_image_t input_image;
  vpx_img_wrap(&input_image, VPX_IMG_FMT_I420, 640, 480, 32, const_cast<uint8_t *>(data));

  // Set rect for the input image
  vpx_img_set_rect(&input_image, 0, 0, 640, 480);

  // Decode the input data
  vpx_codec_decode(&decoder, data, size, nullptr, -1);

  // Get the preview frame from the decoder
  const vpx_image_t *preview_frame = vpx_codec_get_preview_frame(&decoder);

  // Encode the preview frame with the encoder
  vpx_codec_encode(&encoder, preview_frame, 0, 1, 0, 0);

  // Destroy the decoder and encoder
  vpx_codec_destroy(&decoder);
  vpx_codec_destroy(&encoder);

  // Free the image memory
  vpx_img_free(&input_image);

  assert_file_closed(&gop_file);
	return 0;
}