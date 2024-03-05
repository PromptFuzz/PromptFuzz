#include "FDSan.h"
#include "FuzzedDataProvider.h"
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
//<ID> 2274
//<Prompt> ["vpx_codec_vp8_cx","vpx_codec_vp8_dx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_img_alloc","vpx_write_tpl_gop_stats","vpx_codec_set_frame_buffer_functions","vpx_codec_get_stream_info","vpx_codec_dec_init_ver","vpx_codec_get_preview_frame","vpx_read_tpl_gop_stats","vpx_codec_get_cx_data","vpx_codec_destroy","vpx_img_free"]
/*<Combination>: [vpx_codec_iface_t *vpx_codec_vp8_cx(),
    vpx_codec_iface_t *vpx_codec_vp8_dx(),
    vpx_codec_err_t vpx_codec_enc_config_default(vpx_codec_iface_t * iface, vpx_codec_enc_cfg_t * cfg, unsigned int usage),
    vpx_codec_err_t vpx_codec_enc_init_ver(vpx_codec_ctx_t * ctx, vpx_codec_iface_t * iface, const vpx_codec_enc_cfg_t * cfg, vpx_codec_flags_t flags, int ver),
    vpx_image_t *vpx_img_alloc(vpx_image_t * img, vpx_img_fmt_t fmt, unsigned int d_w, unsigned int d_h, unsigned int align),
    vpx_codec_err_t vpx_write_tpl_gop_stats(FILE * tpl_file, const VpxTplGopStats * tpl_gop_stats),
    vpx_codec_err_t vpx_codec_set_frame_buffer_functions(vpx_codec_ctx_t * ctx, vpx_get_frame_buffer_cb_fn_t cb_get, vpx_release_frame_buffer_cb_fn_t cb_release, void * cb_priv),
    vpx_codec_err_t vpx_codec_get_stream_info(vpx_codec_ctx_t * ctx, vpx_codec_stream_info_t * si),
    vpx_codec_err_t vpx_codec_dec_init_ver(vpx_codec_ctx_t * ctx, vpx_codec_iface_t * iface, const vpx_codec_dec_cfg_t * cfg, vpx_codec_flags_t flags, int ver),
    const vpx_image_t *vpx_codec_get_preview_frame(vpx_codec_ctx_t * ctx),
    vpx_codec_err_t vpx_read_tpl_gop_stats(FILE * tpl_file, VpxTplGopStats * tpl_gop_stats),
    const vpx_codec_cx_pkt_t *vpx_codec_get_cx_data(vpx_codec_ctx_t * ctx, vpx_codec_iter_t * iter),
    vpx_codec_err_t vpx_codec_destroy(vpx_codec_ctx_t * ctx),
    void vpx_img_free(vpx_image_t * img)
*/
//<score> 12, nr_unique_branch: 3
//<Quality> {"density":6,"unique_branches":{"vp8_alloc_compressor_data":[[1145,7,1145,26,0,0,4,0],[1215,14,1215,27,0,0,4,1],[1217,14,1217,27,0,0,4,0]]},"library_calls":["vpx_codec_vp8_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_img_alloc","vpx_img_free","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp8_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_img_alloc","vpx_img_free","vpx_codec_destroy"],"visited":0}
/**/


// Include the necessary libvpx headers

// Define the input and output file names
#define INPUT_FILE "input_file"
#define OUTPUT_FILE "output_file"

extern "C" int LLVMFuzzerTestOneInput_52(const uint8_t* f_data, size_t f_size) {
	if(f_size<12) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_1, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_2, fdp);
	//fuzzer shim end}




  // Open the input file
  FILE *in_file = fmemopen((void *)data, size, "rb");
  if (in_file == nullptr) {
    assert_file_closed(&in_file);
	return 0; // Error handling
  }

  // Open the output file
  FILE *out_file = fopen(OUTPUT_FILE, "wb");
  if (out_file == nullptr) {
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0; // Error handling
  }

  // Read the input data from the file
  uint8_t buffer[4096];
  size_t bytes_read = fread(buffer, 1, sizeof(buffer), in_file);
  assert_file_closed(&in_file);;

  // Initialize the libvpx codec
  vpx_codec_ctx_t encoder;
  vpx_codec_enc_cfg_t encoder_config;
  vpx_codec_iface_t *encoder_interface = vpx_codec_vp8_cx();

  // Set default encoder configuration
  vpx_codec_err_t encoder_status =
      vpx_codec_enc_config_default(encoder_interface, &encoder_config, fuzz_uint32_t_1);
  if (encoder_status != VPX_CODEC_OK) {
    assert_file_closed(&out_file);;
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0; // Error handling
  }

  // Initialize the encoder
  encoder_config.g_w = 1920; // Set width
  encoder_config.g_h = 1080; // Set height
  encoder_status = vpx_codec_enc_init_ver(
      &encoder, encoder_interface, &encoder_config, fuzz_int64_t_2, VPX_ENCODER_ABI_VERSION);
  if (encoder_status != VPX_CODEC_OK) {
    assert_file_closed(&out_file);;
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0; // Error handling
  }

  // Initialize the input image
  vpx_image_t input_image;
  vpx_img_alloc(&input_image, VPX_IMG_FMT_I420, encoder_config.g_w,
                encoder_config.g_h, 1);
  memcpy(input_image.planes[0], buffer, bytes_read);
  assert_file_closed(&out_file);;

  // Clean up resources
  vpx_img_free(&input_image);
  vpx_codec_destroy(&encoder);

  assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}