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
//<ID> 212
//<Prompt> ["vpx_codec_vp8_cx","vpx_codec_vp8_dx","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_register_put_frame_cb","vpx_codec_control_","vpx_codec_get_global_headers","vpx_img_alloc","vpx_img_free","vpx_codec_destroy"]
/*<Combination>: [vpx_codec_iface_t *vpx_codec_vp8_cx(),
    vpx_codec_iface_t *vpx_codec_vp8_dx(),
    vpx_codec_iface_t *vpx_codec_vp9_cx(),
    vpx_codec_err_t vpx_codec_enc_config_default(vpx_codec_iface_t * iface, vpx_codec_enc_cfg_t * cfg, unsigned int usage),
    vpx_codec_err_t vpx_codec_enc_init_ver(vpx_codec_ctx_t * ctx, vpx_codec_iface_t * iface, const vpx_codec_enc_cfg_t * cfg, vpx_codec_flags_t flags, int ver),
    vpx_codec_err_t vpx_codec_register_put_frame_cb(vpx_codec_ctx_t * ctx, vpx_codec_put_frame_cb_fn_t cb, void * user_priv),
    vpx_codec_err_t vpx_codec_control_(vpx_codec_ctx_t * ctx, int ctrl_id),
    vpx_fixed_buf_t *vpx_codec_get_global_headers(vpx_codec_ctx_t * ctx),
    vpx_image_t *vpx_img_alloc(vpx_image_t * img, vpx_img_fmt_t fmt, unsigned int d_w, unsigned int d_h, unsigned int align),
    void vpx_img_free(vpx_image_t * img),
    vpx_codec_err_t vpx_codec_destroy(vpx_codec_ctx_t * ctx)
*/
//<score> 0.8888889, nr_unique_branch: 0
//<Quality> {"density":8,"unique_branches":{},"library_calls":["vpx_codec_vp8_cx","vpx_codec_vp8_dx","vpx_codec_enc_config_default","vpx_img_alloc","vpx_codec_enc_init_ver","vpx_codec_dec_init_ver","vpx_codec_register_put_frame_cb","vpx_codec_encode","vpx_codec_decode","vpx_codec_get_frame","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp8_cx","vpx_codec_vp8_dx","vpx_codec_enc_config_default","vpx_img_alloc","vpx_codec_enc_init_ver","vpx_codec_dec_init_ver","vpx_codec_register_put_frame_cb","vpx_codec_encode","vpx_codec_decode","vpx_codec_get_frame","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy"],"visited":8}
/*Here is the implementation of the fuzz driver that follows the user's instructions:

*/


extern "C" int LLVMFuzzerTestOneInput_5(const uint8_t* f_data, size_t f_size) {
	if(f_size<60) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_1, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_2, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_3, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_4, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_5, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_6, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_7, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_8, fdp);
	//fuzzer shim end}




  // Create input file
  FILE *in_file = fmemopen((void *)data, size, "rb");
  if (in_file == nullptr) {
    assert_file_closed(&in_file);
	return 0;
  }

  // Create output file
  FILE *out_file = fopen("output_file", "wb");
  if (out_file == nullptr) {
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
  }

  // Get file descriptors
  int in_fd = fuzz_fileno(in_file);
  int out_fd = fuzz_fileno(out_file);

  // Initialize libvpx codecs
  vpx_codec_iface_t *encoder_iface = vpx_codec_vp8_cx();
  vpx_codec_iface_t *decoder_iface = vpx_codec_vp8_dx();
  vpx_codec_ctx_t encoder;
  vpx_codec_ctx_t decoder;

  vpx_codec_enc_cfg_t encoder_cfg;
  vpx_codec_enc_config_default(encoder_iface, &encoder_cfg, fuzz_uint32_t_1);
  encoder_cfg.rc_target_bitrate = 1000;

  vpx_image_t *frame = vpx_img_alloc(nullptr, VPX_IMG_FMT_I420, encoder_cfg.g_w,
                                     encoder_cfg.g_h, 1);
  
  vpx_codec_enc_init_ver(&encoder, encoder_iface, &encoder_cfg, fuzz_int64_t_2, VPX_ENCODER_ABI_VERSION);
  vpx_codec_dec_init_ver(&decoder, decoder_iface, nullptr, fuzz_int64_t_3, VPX_DECODER_ABI_VERSION);

  // Set put frame callback function for encoder
  vpx_codec_register_put_frame_cb(&encoder, [](void *priv, const vpx_image_t *img) {
    FILE *out_file = (FILE *)priv;
    fwrite(img->planes[0], 1, img->h * img->stride[0], out_file);
  }, out_file);

  // Encode frames
  vpx_codec_encode(&encoder, frame, fuzz_int64_t_4, fuzz_uint64_t_5, fuzz_int64_t_6, fuzz_uint64_t_7);

  // Decode frames
  uint8_t *buffer = new uint8_t[encoder_cfg.g_w * encoder_cfg.g_h * 3 / 2];
  while (true) {
    size_t bytes_read = fread(buffer, 1, encoder_cfg.g_w * encoder_cfg.g_h * 3 / 2, in_file);
    if (bytes_read == 0) {
      break;
    }
    vpx_codec_decode(&decoder, buffer, bytes_read, nullptr, fuzz_int64_t_8);
    const vpx_image_t *decoded_frame;
    while ((decoded_frame = vpx_codec_get_frame(&decoder, nullptr)) != nullptr) {
      // Consume decoded frame
    }
  }

  // Cleanup
  delete[] buffer;
  vpx_img_free(frame);
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