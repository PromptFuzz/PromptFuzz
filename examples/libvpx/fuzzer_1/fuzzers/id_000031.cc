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
//<ID> 1396
//<Prompt> ["vpx_write_tpl_gop_stats","vpx_codec_control_","vpx_read_tpl_gop_stats","vpx_codec_enc_config_default","vpx_codec_enc_init_multi_ver","vpx_codec_get_stream_info","vpx_img_alloc","vpx_codec_dec_init_ver","vpx_codec_set_frame_buffer_functions"]
/*<Combination>: [vpx_codec_err_t vpx_write_tpl_gop_stats(FILE * tpl_file, const VpxTplGopStats * tpl_gop_stats),
    vpx_codec_err_t vpx_codec_control_(vpx_codec_ctx_t * ctx, int ctrl_id),
    vpx_codec_err_t vpx_read_tpl_gop_stats(FILE * tpl_file, VpxTplGopStats * tpl_gop_stats),
    vpx_codec_err_t vpx_codec_enc_config_default(vpx_codec_iface_t * iface, vpx_codec_enc_cfg_t * cfg, unsigned int usage),
    vpx_codec_err_t vpx_codec_enc_init_multi_ver(vpx_codec_ctx_t * ctx, vpx_codec_iface_t * iface, vpx_codec_enc_cfg_t * cfg, int num_enc, vpx_codec_flags_t flags, vpx_rational_t * dsf, int ver),
    vpx_codec_err_t vpx_codec_get_stream_info(vpx_codec_ctx_t * ctx, vpx_codec_stream_info_t * si),
    vpx_image_t *vpx_img_alloc(vpx_image_t * img, vpx_img_fmt_t fmt, unsigned int d_w, unsigned int d_h, unsigned int align),
    vpx_codec_err_t vpx_codec_dec_init_ver(vpx_codec_ctx_t * ctx, vpx_codec_iface_t * iface, const vpx_codec_dec_cfg_t * cfg, vpx_codec_flags_t flags, int ver),
    vpx_codec_err_t vpx_codec_set_frame_buffer_functions(vpx_codec_ctx_t * ctx, vpx_get_frame_buffer_cb_fn_t cb_get, vpx_release_frame_buffer_cb_fn_t cb_release, void * cb_priv)
*/
//<score> 5.3333335, nr_unique_branch: 1
//<Quality> {"density":8,"unique_branches":{"vpx_write_tpl_gop_stats":[[34,27,34,48,0,0,4,0]]},"library_calls":["vpx_codec_vp8_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_img_alloc","vpx_codec_encode","vpx_codec_get_stream_info","vpx_write_tpl_gop_stats","vpx_codec_vp8_dx","vpx_codec_dec_init_ver","vpx_codec_set_frame_buffer_functions","vpx_codec_decode","vpx_codec_get_frame","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp8_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_img_alloc","vpx_codec_encode","vpx_codec_get_stream_info","vpx_write_tpl_gop_stats","vpx_codec_vp8_dx","vpx_codec_dec_init_ver","vpx_codec_set_frame_buffer_functions","vpx_codec_decode","vpx_codec_get_frame","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy"],"visited":2}
/**/


extern "C" int LLVMFuzzerTestOneInput_31(const uint8_t* f_data, size_t f_size) {
	if(f_size<60) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_1, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_2, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_3, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_4, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_5, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_6, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_7, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_8, fdp);
	//fuzzer shim end}




  // Create a FILE * variable to read the input data
  FILE *in_file = fmemopen((void *)data, size, "rb");
  if (in_file == NULL) {
    assert_file_closed(&in_file);
	return 0;
  }

  // Create a FILE * variable to write output data
  FILE *out_file = fopen("output_file", "wb");
  if (out_file == NULL) {
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
  }

  // Get the file descriptor for reading and writing
  int in_fd = fuzz_fileno(in_file);
  int out_fd = fuzz_fileno(out_file);

  // Initialize the libvpx encoder
  vpx_codec_ctx_t encoder;
  vpx_codec_enc_cfg_t encoder_cfg;
  vpx_codec_iface_t *encoder_iface = vpx_codec_vp8_cx();

  // Set encoder configuration
  vpx_codec_enc_config_default(encoder_iface, &encoder_cfg, fuzz_uint32_t_1);
  encoder_cfg.g_w = 640;
  encoder_cfg.g_h = 480;

  // Initialize the encoder
  vpx_codec_enc_init_ver(&encoder, encoder_iface, &encoder_cfg, fuzz_int64_t_2, VPX_ENCODER_ABI_VERSION);

  // Create an input image
  vpx_image_t image;
  vpx_img_alloc(&image, VPX_IMG_FMT_I420, 640, 480, 16);

  // Encode the image
  vpx_codec_encode(&encoder, &image, fuzz_int64_t_3, fuzz_uint64_t_4, fuzz_int64_t_5, fuzz_uint64_t_6);

  // Get the stream info
  vpx_codec_stream_info_t stream_info;
  vpx_codec_get_stream_info(&encoder, &stream_info);

  // Write the stream stats to file
  vpx_write_tpl_gop_stats(out_file, NULL);

  // Initialize the libvpx decoder
  vpx_codec_ctx_t decoder;
  vpx_codec_dec_cfg_t decoder_cfg;
  vpx_codec_iface_t *decoder_iface = vpx_codec_vp8_dx();

  // Set decoder configuration
  decoder_cfg.w = stream_info.w;
  decoder_cfg.h = stream_info.h;

  // Initialize the decoder
  vpx_codec_dec_init_ver(&decoder, decoder_iface, &decoder_cfg, fuzz_int64_t_7, VPX_DECODER_ABI_VERSION);

  // Set frame buffer functions
  vpx_codec_set_frame_buffer_functions(&decoder, NULL, NULL, NULL);

  // Decode the stream
  vpx_codec_decode(&decoder, data, size, NULL, fuzz_int64_t_8);

  // Get the decoded image
  const vpx_image_t *decoded_image = vpx_codec_get_frame(&decoder, NULL);

  // Cleanup
  vpx_img_free(&image);
  vpx_codec_destroy(&encoder);
  vpx_codec_destroy(&decoder);
  assert_file_closed(&in_file);;
  assert_file_closed(&out_file);;

  assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}