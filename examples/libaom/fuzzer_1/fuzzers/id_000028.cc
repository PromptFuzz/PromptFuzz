#include "FDSan.h"
#include "FuzzedDataProvider.h"
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
//<ID> 1092
//<Prompt> ["aom_codec_enc_init_ver","aom_img_alloc","aom_img_set_rect","aom_codec_get_caps","aom_codec_encode","aom_img_free"]
/*<Combination>: [aom_codec_err_t aom_codec_enc_init_ver(aom_codec_ctx_t * ctx, aom_codec_iface_t * iface, const aom_codec_enc_cfg_t * cfg, aom_codec_flags_t flags, int ver),
    aom_image_t *aom_img_alloc(aom_image_t * img, aom_img_fmt_t fmt, unsigned int d_w, unsigned int d_h, unsigned int align),
    int aom_img_set_rect(aom_image_t * img, unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int border),
    aom_codec_caps_t aom_codec_get_caps(aom_codec_iface_t * iface),
    aom_codec_err_t aom_codec_encode(aom_codec_ctx_t * ctx, const aom_image_t * img, aom_codec_pts_t pts, unsigned long duration, aom_enc_frame_flags_t flags),
    void aom_img_free(aom_image_t * img)
*/
//<score> 1.6666666, nr_unique_branch: 1, p_cov: 0.8333333
//<Quality> {"diversity":14,"density":14,"covered_api":["aom_codec_enc_init_ver","aom_img_alloc","aom_img_set_rect","aom_codec_encode","aom_img_free"],"uncovered_api":["aom_codec_get_caps"],"unique_branches":{"set_bitstream_level_tier":[[392,14,393,33,0]]},"library_calls":["aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_img_alloc","aom_img_set_rect","aom_codec_encode","aom_codec_get_cx_data","aom_img_free","aom_codec_destroy"]}
/**/


extern "C" int LLVMFuzzerTestOneInput_28(const uint8_t* f_data, size_t f_size) {
	if(f_size<=28) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_1, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_4, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_5, fdp);
	//fuzzer shim end}




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
    assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
  }
  
  // Initialize libaom encoder
  aom_codec_iface_t *encoder_iface = aom_codec_av1_cx();
  aom_codec_ctx_t encoder_ctx;
  aom_codec_enc_cfg_t encoder_cfg;
  
  if (aom_codec_enc_config_default(encoder_iface, &encoder_cfg, fuzz_uint32_t_1) != AOM_CODEC_OK) {
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
  }

  encoder_cfg.g_w = 1280;  // Width of the input frame
  encoder_cfg.g_h = 720;   // Height of the input frame
  encoder_cfg.rc_target_bitrate = 1000;  // Target bitrate

  if (aom_codec_enc_init_ver(&encoder_ctx, encoder_iface, &encoder_cfg, fuzz_int64_t_2, AOM_ENCODER_ABI_VERSION) !=
      AOM_CODEC_OK) {
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
  }

  // Create input image
  aom_image_t *img = NULL;
  img = aom_img_alloc(img, AOM_IMG_FMT_I420, encoder_cfg.g_w, encoder_cfg.g_h, 16);

  if (img == NULL) {
    aom_codec_destroy(&encoder_ctx);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
  }

  // Read input frame data from file
  size_t read_size = fread(img->planes[0], 1, img->sz, in_file);
  assert_file_closed(&in_file);;

  // Set the entire frame as the region of interest
  if (aom_img_set_rect(img, fuzz_uint32_t_3, fuzz_uint32_t_4, img->d_w, img->d_h, 0) != 0) {
    aom_img_free(img);
    aom_codec_destroy(&encoder_ctx);
    assert_file_closed(&out_file);;
    assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
  }

  // Encode frame
  aom_enc_frame_flags_t flags = 0;
  aom_codec_pts_t pts = 0;
  
  if (aom_codec_encode(&encoder_ctx, img, pts, fuzz_uint64_t_5, flags) != AOM_CODEC_OK) {
    aom_img_free(img);
    aom_codec_destroy(&encoder_ctx);
    assert_file_closed(&out_file);;
    assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
  }

  // Get compressed data
  const aom_codec_cx_pkt_t *pkt = NULL;
  
  while ((pkt = aom_codec_get_cx_data(&encoder_ctx, NULL)) != NULL) {
    if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
      fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
    }
  }

  // Free resources
  aom_img_free(img);
  aom_codec_destroy(&encoder_ctx);
  assert_file_closed(&out_file);;
  
  assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}