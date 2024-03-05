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
//<ID> 3412
//<Prompt> ["vpx_codec_enc_init_multi_ver","vpx_read_tpl_gop_stats","vpx_codec_get_stream_info","vpx_write_tpl_gop_stats","vpx_img_wrap","vpx_codec_get_frame","vpx_codec_control_","vpx_codec_enc_config_set","vpx_codec_get_cx_data"]
/*<Combination>: [vpx_codec_err_t vpx_codec_enc_init_multi_ver(vpx_codec_ctx_t * ctx, vpx_codec_iface_t * iface, vpx_codec_enc_cfg_t * cfg, int num_enc, vpx_codec_flags_t flags, vpx_rational_t * dsf, int ver),
    vpx_codec_err_t vpx_read_tpl_gop_stats(FILE * tpl_file, VpxTplGopStats * tpl_gop_stats),
    vpx_codec_err_t vpx_codec_get_stream_info(vpx_codec_ctx_t * ctx, vpx_codec_stream_info_t * si),
    vpx_codec_err_t vpx_write_tpl_gop_stats(FILE * tpl_file, const VpxTplGopStats * tpl_gop_stats),
    vpx_image_t *vpx_img_wrap(vpx_image_t * img, vpx_img_fmt_t fmt, unsigned int d_w, unsigned int d_h, unsigned int stride_align, unsigned char * img_data),
    vpx_image_t *vpx_codec_get_frame(vpx_codec_ctx_t * ctx, vpx_codec_iter_t * iter),
    vpx_codec_err_t vpx_codec_control_(vpx_codec_ctx_t * ctx, int ctrl_id),
    vpx_codec_err_t vpx_codec_enc_config_set(vpx_codec_ctx_t * ctx, const vpx_codec_enc_cfg_t * cfg),
    const vpx_codec_cx_pkt_t *vpx_codec_get_cx_data(vpx_codec_ctx_t * ctx, vpx_codec_iter_t * iter)
*/
//<score> 10, nr_unique_branch: 0
//<Quality> {"density":10,"unique_branches":{},"library_calls":["vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_read_tpl_gop_stats","vpx_codec_enc_config_set","vpx_codec_get_stream_info","vpx_write_tpl_gop_stats","vpx_img_wrap","vpx_codec_get_cx_data","vpx_codec_control_","vpx_codec_destroy","vpx_img_free","vpx_free_tpl_gop_stats"],"critical_calls":["vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_read_tpl_gop_stats","vpx_codec_enc_config_set","vpx_codec_get_stream_info","vpx_write_tpl_gop_stats","vpx_img_wrap","vpx_codec_get_cx_data","vpx_codec_control_","vpx_codec_destroy","vpx_img_free","vpx_free_tpl_gop_stats"],"visited":0}
/**/

extern "C" int LLVMFuzzerTestOneInput_97(const uint8_t *data, size_t size) {
	if(size<0) return 0;

  // Create input file
  FILE *in_file = fmemopen((void *)data, size, "rb");
  if (in_file == NULL) {
    assert_file_closed(&in_file);
	return 0;
  }

  // Create output file
  FILE *out_file = fopen("output_file", "wb");
  if (out_file == NULL) {
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
  }

  // Initialize encoder
  vpx_codec_enc_cfg_t cfg;
  vpx_codec_iface_t *iface = vpx_codec_vp9_cx();
  vpx_codec_enc_config_default(iface, &cfg, 0);
  vpx_codec_ctx_t encoder;
  vpx_codec_enc_init_ver(&encoder, iface, &cfg, 0, VPX_ENCODER_ABI_VERSION);

  // Read TPL GOP stats
  VpxTplGopStats tpl_gop_stats;
  vpx_read_tpl_gop_stats(in_file, &tpl_gop_stats);

  // Set encoder config
  vpx_codec_enc_config_set(&encoder, &cfg);

  // Get stream info
  vpx_codec_stream_info_t si;
  vpx_codec_get_stream_info(&encoder, &si);

  // Write TPL GOP stats
  vpx_write_tpl_gop_stats(out_file, &tpl_gop_stats);

  // Wrap image data
  vpx_image_t img;
  vpx_img_wrap(&img, VPX_IMG_FMT_I420, si.w, si.h, 16, NULL);

  // Get the frame from the encoder
  vpx_codec_iter_t iter = NULL;
  const vpx_codec_cx_pkt_t *pkt = vpx_codec_get_cx_data(&encoder, &iter);

  // Control the encoder
  vpx_codec_control_(&encoder, VP8E_SET_CPUUSED, 4);

  // Release resources
  vpx_codec_destroy(&encoder);
  vpx_img_free(&img);
  vpx_free_tpl_gop_stats(&tpl_gop_stats);
  assert_file_closed(&in_file);;
  assert_file_closed(&out_file);;

  assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}