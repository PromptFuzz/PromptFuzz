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
//<ID> 587
//<Prompt> ["aom_codec_encode","aom_codec_control","aom_codec_build_config","aom_codec_set_cx_data_buf","aom_img_num_metadata"]
/*<Combination>: [aom_codec_err_t aom_codec_encode(aom_codec_ctx_t * ctx, const aom_image_t * img, aom_codec_pts_t pts, unsigned long duration, aom_enc_frame_flags_t flags),
    aom_codec_err_t aom_codec_control(aom_codec_ctx_t * ctx, int ctrl_id),
    const char *aom_codec_build_config(),
    aom_codec_err_t aom_codec_set_cx_data_buf(aom_codec_ctx_t * ctx, const aom_fixed_buf_t * buf, unsigned int pad_before, unsigned int pad_after),
    size_t aom_img_num_metadata(const aom_image_t * img)
*/
//<score> 2.1599998, nr_unique_branch: 2, p_cov: 0.8
//<Quality> {"diversity":10,"density":9,"covered_api":["aom_codec_encode","aom_codec_build_config","aom_codec_set_cx_data_buf","aom_img_num_metadata"],"uncovered_api":["aom_codec_control"],"unique_branches":{"validate_img":[[918,35,918,59,0]],"av1_select_sb_size":[[859,31,859,49,1]]},"library_calls":["aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_set_cx_data_buf","aom_img_alloc","aom_codec_encode","aom_codec_build_config","aom_img_num_metadata","aom_img_free","aom_codec_destroy"]}
/**/

extern "C" int LLVMFuzzerTestOneInput_15(const uint8_t *data, size_t size) {
	if(size<=0) return 0;

  aom_codec_ctx_t encoder;
  aom_codec_iface_t *encoder_iface = aom_codec_av1_cx();
  aom_codec_enc_cfg_t encoder_cfg;
  aom_fixed_buf_t encoder_buf;

  // Initialize encoder configuration
  aom_codec_enc_config_default(encoder_iface, &encoder_cfg, 0);
  encoder_cfg.g_w = encoder_cfg.g_h = 1280;
  encoder_cfg.rc_target_bitrate = 1000000;
  aom_codec_enc_init_ver(&encoder, encoder_iface, &encoder_cfg, 0, AOM_ENCODER_ABI_VERSION);

  // Set the output buffer for encoder
  uint8_t output_buffer[1024];
  encoder_buf.buf = output_buffer;
  encoder_buf.sz = sizeof(output_buffer);
  aom_codec_set_cx_data_buf(&encoder, &encoder_buf, 0, 0);

  // Create input image
  aom_image_t input_image;
  aom_img_alloc(&input_image, AOM_IMG_FMT_I420, 1280, 720, 16);

  // Encode the input image
  aom_enc_frame_flags_t flags = 0;
  aom_codec_pts_t pts = 0;
  unsigned long duration = 1;
  aom_codec_encode(&encoder, &input_image, pts, duration, flags);

  // Print build configuration
  printf("Build Config: %s\n", aom_codec_build_config());

  // Print number of metadata in the input image
  size_t num_metadata = aom_img_num_metadata(&input_image);
  printf("Number of Metadata: %zu\n", num_metadata);

  // Cleanup
  aom_img_free(&input_image);
  aom_codec_destroy(&encoder);

  return 0;
}