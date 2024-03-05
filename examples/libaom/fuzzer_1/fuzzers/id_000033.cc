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
//<ID> 1212
//<Prompt> ["aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_set_cx_data_buf","aom_img_alloc","aom_codec_encode","aom_img_plane_width"]
/*<Combination>: [aom_codec_err_t aom_codec_enc_config_default(aom_codec_iface_t * iface, aom_codec_enc_cfg_t * cfg, unsigned int usage),
    aom_codec_err_t aom_codec_enc_init_ver(aom_codec_ctx_t * ctx, aom_codec_iface_t * iface, const aom_codec_enc_cfg_t * cfg, aom_codec_flags_t flags, int ver),
    aom_codec_err_t aom_codec_set_cx_data_buf(aom_codec_ctx_t * ctx, const aom_fixed_buf_t * buf, unsigned int pad_before, unsigned int pad_after),
    aom_image_t *aom_img_alloc(aom_image_t * img, aom_img_fmt_t fmt, unsigned int d_w, unsigned int d_h, unsigned int align),
    aom_codec_err_t aom_codec_encode(aom_codec_ctx_t * ctx, const aom_image_t * img, aom_codec_pts_t pts, unsigned long duration, aom_enc_frame_flags_t flags),
    int aom_img_plane_width(const aom_image_t * img, int plane)
*/
//<score> 2.5, nr_unique_branch: 3, p_cov: 1
//<Quality> {"diversity":16,"density":10,"covered_api":["aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_set_cx_data_buf","aom_img_alloc","aom_codec_encode","aom_img_plane_width"],"uncovered_api":[],"unique_branches":{"aom_img_plane_height":[[299,7,299,16,0],[299,20,299,43,0]],"aom_img_plane_width":[[292,20,292,43,0]]},"library_calls":["aom_codec_enc_config_default","aom_codec_av1_cx","aom_codec_enc_init_ver","aom_codec_av1_cx","aom_codec_set_cx_data_buf","aom_img_alloc","aom_img_plane_height","aom_img_plane_width","aom_img_plane_height","aom_img_plane_width","aom_img_plane_height","aom_img_plane_width","aom_codec_encode","aom_img_free","aom_codec_destroy"]}
/**/



extern "C" int LLVMFuzzerTestOneInput_33(const uint8_t* f_data, size_t f_size) {
	if(f_size<=40) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_1, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_4, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_5, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_6, fdp);
	//fuzzer shim end}




  // Create input file stream
  FILE *in_file = fmemopen((void *)data, size, "rb");
  if (in_file == nullptr) {
    assert_file_closed(&in_file);
	return 0;
  }

  // Create output file stream
  FILE *out_file = fopen("output_file", "wb");
  if (out_file == nullptr) {
    assert_file_closed(&in_file);;
    assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
  }

  // Create codec context
  aom_codec_ctx_t codec_ctx;
  memset(&codec_ctx, 0, sizeof(aom_codec_ctx_t));

  // Initialize encoder configuration
  aom_codec_enc_cfg_t encoder_cfg;
  memset(&encoder_cfg, 0, sizeof(aom_codec_enc_cfg_t));
  aom_codec_enc_config_default(aom_codec_av1_cx(), &encoder_cfg, fuzz_uint32_t_1);

  // Initialize encoder
  aom_codec_enc_init_ver(&codec_ctx, aom_codec_av1_cx(), &encoder_cfg, fuzz_int64_t_2, AOM_ENCODER_ABI_VERSION);

  // Set compressed data buffer
  aom_fixed_buf_t cx_data_buf;
  uint8_t *cx_data = nullptr;
  size_t cx_data_size = 0;
  cx_data = (uint8_t *)malloc(size);
  cx_data_buf.buf = cx_data;
  cx_data_buf.sz = size;
  aom_codec_set_cx_data_buf(&codec_ctx, &cx_data_buf, 0, fuzz_uint32_t_3);

  // Allocate input image
  aom_image_t *input_image = nullptr;
  input_image = aom_img_alloc(nullptr, AOM_IMG_FMT_I420, 640, 480, 16);
  if (input_image == nullptr) {
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    aom_codec_destroy(&codec_ctx);
    assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
  }

  // Read input image data from file
  fread(input_image->planes[0], 1, aom_img_plane_width(input_image, 0) * aom_img_plane_height(input_image, 0), in_file);
  fread(input_image->planes[1], 1, aom_img_plane_width(input_image, 1) * aom_img_plane_height(input_image, 1), in_file);
  fread(input_image->planes[2], 1, aom_img_plane_width(input_image, 2) * aom_img_plane_height(input_image, 2), in_file);

  // Encode input image
  aom_codec_encode(&codec_ctx, input_image, fuzz_int64_t_4, fuzz_uint64_t_5, fuzz_int64_t_6);

  // Write compressed data to output file
  fwrite(cx_data, 1, cx_data_size, out_file);

  // Free allocated resources
  assert_file_closed(&in_file);;
  assert_file_closed(&out_file);;
  aom_img_free(input_image);
  free(cx_data);
  aom_codec_destroy(&codec_ctx);

  assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}