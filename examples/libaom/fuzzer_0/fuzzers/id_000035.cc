#include "FDSan.h"
#include <aom/aom_decoder.h>
#include <aom/aomcx.h>
#include <aom/aomdx.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 1277
//<Prompt> ["aom_codec_iface_name","aom_img_metadata_free","aom_codec_enc_init_ver","aom_codec_set_cx_data_buf","aom_img_alloc","aom_codec_encode","aom_img_plane_width"]
/*<Combination>: [const char *aom_codec_iface_name(aom_codec_iface_t * iface),
    void aom_img_metadata_free(aom_metadata_t * metadata),
    aom_codec_err_t aom_codec_enc_init_ver(aom_codec_ctx_t * ctx, aom_codec_iface_t * iface, const aom_codec_enc_cfg_t * cfg, aom_codec_flags_t flags, int ver),
    aom_codec_err_t aom_codec_set_cx_data_buf(aom_codec_ctx_t * ctx, const aom_fixed_buf_t * buf, unsigned int pad_before, unsigned int pad_after),
    aom_image_t *aom_img_alloc(aom_image_t * img, aom_img_fmt_t fmt, unsigned int d_w, unsigned int d_h, unsigned int align),
    aom_codec_err_t aom_codec_encode(aom_codec_ctx_t * ctx, const aom_image_t * img, aom_codec_pts_t pts, unsigned long duration, aom_enc_frame_flags_t flags),
    int aom_img_plane_width(const aom_image_t * img, int plane)
*/
//<score> 1.4285715, nr_unique_branch: 1, p_cov: 0.71428573
//<Quality> {"diversity":9,"density":9,"covered_api":["aom_codec_enc_init_ver","aom_codec_set_cx_data_buf","aom_img_alloc","aom_codec_encode","aom_img_plane_width"],"uncovered_api":["aom_codec_iface_name","aom_img_metadata_free"],"unique_branches":{"av1_first_pass":[[31,23,31,34,0]]},"library_calls":["aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_set_cx_data_buf","aom_img_alloc","aom_img_plane_width","aom_codec_encode","aom_img_free","aom_codec_destroy"]}
/*Here is the C++ program that achieves the event by using the libaom APIs:

*/


extern "C" int LLVMFuzzerTestOneInput_35(const uint8_t *data, size_t size) {
	if(size<=0) return 0;

  aom_codec_iface_t *iface = aom_codec_av1_cx();
  aom_codec_ctx_t encoder;
  aom_codec_enc_cfg_t encoder_config;
  aom_image_t *image;
  aom_codec_pts_t pts = 0;
  aom_enc_frame_flags_t flags = 0;

  // Initialize encoder configuration
  aom_codec_enc_config_default(iface, &encoder_config, 0);
  encoder_config.g_w = 640;
  encoder_config.g_h = 480;
  encoder_config.rc_target_bitrate = 1000;
  aom_codec_enc_init_ver(&encoder, iface, &encoder_config, 0, AOM_ENCODER_ABI_VERSION);

  // Set output buffer for compressed data
  aom_fixed_buf_t cx_data_buf;
  cx_data_buf.buf = nullptr;
  cx_data_buf.sz = 0;
  aom_codec_set_cx_data_buf(&encoder, &cx_data_buf, 0, 0);

  // Allocate input image
  image = aom_img_alloc(NULL, AOM_IMG_FMT_I420, encoder_config.g_w, encoder_config.g_h, 16);

  // Set image data
  aom_img_plane_width(image, 0); // Use the aom_img_plane_width API at least once

  // Encode the image
  aom_codec_encode(&encoder, image, pts, 1, flags);

  // Free allocated resources
  aom_img_free(image);
  aom_codec_destroy(&encoder);

  return 0;
}