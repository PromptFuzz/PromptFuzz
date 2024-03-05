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
//<ID> 3375
//<Prompt> ["vpx_codec_vp9_dx","vpx_codec_dec_init_ver","vpx_codec_vp9_cx","vpx_codec_enc_config_set","vpx_img_wrap","vpx_codec_register_put_slice_cb","vpx_codec_set_frame_buffer_functions","vpx_img_alloc","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_decode","vpx_codec_get_frame","vpx_img_free","vpx_codec_destroy"]
/*<Combination>: [vpx_codec_iface_t *vpx_codec_vp9_dx(),
    vpx_codec_err_t vpx_codec_dec_init_ver(vpx_codec_ctx_t * ctx, vpx_codec_iface_t * iface, const vpx_codec_dec_cfg_t * cfg, vpx_codec_flags_t flags, int ver),
    vpx_codec_iface_t *vpx_codec_vp9_cx(),
    vpx_codec_err_t vpx_codec_enc_config_set(vpx_codec_ctx_t * ctx, const vpx_codec_enc_cfg_t * cfg),
    vpx_image_t *vpx_img_wrap(vpx_image_t * img, vpx_img_fmt_t fmt, unsigned int d_w, unsigned int d_h, unsigned int stride_align, unsigned char * img_data),
    vpx_codec_err_t vpx_codec_register_put_slice_cb(vpx_codec_ctx_t * ctx, vpx_codec_put_slice_cb_fn_t cb, void * user_priv),
    vpx_codec_err_t vpx_codec_set_frame_buffer_functions(vpx_codec_ctx_t * ctx, vpx_get_frame_buffer_cb_fn_t cb_get, vpx_release_frame_buffer_cb_fn_t cb_release, void * cb_priv),
    vpx_image_t *vpx_img_alloc(vpx_image_t * img, vpx_img_fmt_t fmt, unsigned int d_w, unsigned int d_h, unsigned int align),
    vpx_codec_err_t vpx_codec_encode(vpx_codec_ctx_t * ctx, const vpx_image_t * img, vpx_codec_pts_t pts, unsigned long duration, vpx_enc_frame_flags_t flags, unsigned long deadline),
    const vpx_codec_cx_pkt_t *vpx_codec_get_cx_data(vpx_codec_ctx_t * ctx, vpx_codec_iter_t * iter),
    vpx_codec_err_t vpx_codec_decode(vpx_codec_ctx_t * ctx, const uint8_t * data, unsigned int data_sz, void * user_priv, long deadline),
    vpx_image_t *vpx_codec_get_frame(vpx_codec_ctx_t * ctx, vpx_codec_iter_t * iter),
    void vpx_img_free(vpx_image_t * img),
    vpx_codec_err_t vpx_codec_destroy(vpx_codec_ctx_t * ctx)
*/
//<score> 12, nr_unique_branch: 0
//<Quality> {"density":12,"unique_branches":{},"library_calls":["vpx_codec_dec_init_ver","vpx_codec_vp9_dx","vpx_codec_decode","vpx_codec_get_frame","vpx_codec_enc_config_default","vpx_codec_vp9_cx","vpx_codec_enc_init_ver","vpx_codec_vp9_cx","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_destroy","vpx_codec_destroy"],"critical_calls":["vpx_codec_dec_init_ver","vpx_codec_vp9_dx","vpx_codec_decode","vpx_codec_get_frame","vpx_codec_enc_config_default","vpx_codec_vp9_cx","vpx_codec_enc_init_ver","vpx_codec_vp9_cx","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_destroy","vpx_codec_destroy"],"visited":0}
/**/

extern "C" int LLVMFuzzerTestOneInput_96(const uint8_t* f_data, size_t f_size) {
	if(f_size<60) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int64_t, fuzz_int64_t_1, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_4, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_5, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_6, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_7, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_8, fdp);
	//fuzzer shim end}




  // Open input file
  FILE *in_file = fmemopen((void *)data, size, "rb");
  if (in_file == nullptr) {
    assert_file_closed(&in_file);
	return 0;
  }

  // Create decoder
  vpx_codec_ctx_t decoder;
  vpx_codec_err_t status =
    vpx_codec_dec_init_ver(&decoder, vpx_codec_vp9_dx(), nullptr, fuzz_int64_t_1, VPX_DECODER_ABI_VERSION);
  if (status != VPX_CODEC_OK) {
    assert_file_closed(&in_file);;
    assert_file_closed(&in_file);
	return 0;
  }

  // Read input data
  fseek(in_file, 0, SEEK_END);
  size_t in_size = ftell(in_file);
  fseek(in_file, 0, SEEK_SET);
  uint8_t *in_data = new uint8_t[in_size];
  fread(in_data, sizeof(uint8_t), in_size, in_file);

  // Decode input data
  vpx_codec_decode(&decoder, in_data, in_size, nullptr, fuzz_int64_t_2);
  delete[] in_data;

  // Get decoded frames
  vpx_codec_iter_t iter = nullptr;
  const vpx_image_t *decoded_frame = nullptr;
  while ((decoded_frame = vpx_codec_get_frame(&decoder, &iter)) != nullptr) {
    // Create encoder
    vpx_codec_ctx_t encoder;
    vpx_codec_enc_cfg_t encoder_config;
    vpx_codec_enc_config_default(vpx_codec_vp9_cx(), &encoder_config, fuzz_uint32_t_3);
    vpx_codec_enc_init_ver(&encoder, vpx_codec_vp9_cx(), &encoder_config, fuzz_int64_t_4, VPX_ENCODER_ABI_VERSION);

    // Encode frame
    vpx_codec_encode(&encoder, decoded_frame, fuzz_int64_t_5, fuzz_uint64_t_6, fuzz_int64_t_7, fuzz_uint64_t_8);

    // Get encoded data
    const vpx_codec_cx_pkt_t *encoded_packet = nullptr;
    while ((encoded_packet = vpx_codec_get_cx_data(&encoder, &iter)) != nullptr) {
      // Process encoded data
      // ...
    }

    // Destroy encoder
    vpx_codec_destroy(&encoder);
  }

  // Close input file
  assert_file_closed(&in_file);;

  // Destroy decoder
  vpx_codec_destroy(&decoder);

  assert_file_closed(&in_file);
	return 0;
}