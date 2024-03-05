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
//<ID> 3020
//<Prompt> ["vpx_codec_dec_init_ver","vpx_codec_vp9_dx","vpx_codec_decode","vpx_codec_get_frame","vpx_codec_set_frame_buffer_functions","vpx_codec_enc_init_multi_ver","vpx_img_set_rect","vpx_img_free","vpx_codec_get_stream_info","vpx_codec_destroy"]
/*<Combination>: [vpx_codec_err_t vpx_codec_dec_init_ver(vpx_codec_ctx_t * ctx, vpx_codec_iface_t * iface, const vpx_codec_dec_cfg_t * cfg, vpx_codec_flags_t flags, int ver),
    vpx_codec_iface_t *vpx_codec_vp9_dx(),
    vpx_codec_err_t vpx_codec_decode(vpx_codec_ctx_t * ctx, const uint8_t * data, unsigned int data_sz, void * user_priv, long deadline),
    vpx_image_t *vpx_codec_get_frame(vpx_codec_ctx_t * ctx, vpx_codec_iter_t * iter),
    vpx_codec_err_t vpx_codec_set_frame_buffer_functions(vpx_codec_ctx_t * ctx, vpx_get_frame_buffer_cb_fn_t cb_get, vpx_release_frame_buffer_cb_fn_t cb_release, void * cb_priv),
    vpx_codec_err_t vpx_codec_enc_init_multi_ver(vpx_codec_ctx_t * ctx, vpx_codec_iface_t * iface, vpx_codec_enc_cfg_t * cfg, int num_enc, vpx_codec_flags_t flags, vpx_rational_t * dsf, int ver),
    int vpx_img_set_rect(vpx_image_t * img, unsigned int x, unsigned int y, unsigned int w, unsigned int h),
    void vpx_img_free(vpx_image_t * img),
    vpx_codec_err_t vpx_codec_get_stream_info(vpx_codec_ctx_t * ctx, vpx_codec_stream_info_t * si),
    vpx_codec_err_t vpx_codec_destroy(vpx_codec_ctx_t * ctx)
*/
//<score> 12, nr_unique_branch: 0
//<Quality> {"density":12,"unique_branches":{},"library_calls":["vpx_codec_vp9_dx","vpx_codec_dec_init_ver","vpx_codec_vp9_cx","vpx_codec_enc_init_multi_ver","vpx_codec_set_frame_buffer_functions","vpx_codec_set_frame_buffer_functions","vpx_codec_decode","vpx_codec_get_frame","vpx_img_set_rect","vpx_codec_encode","vpx_codec_destroy","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp9_dx","vpx_codec_dec_init_ver","vpx_codec_vp9_cx","vpx_codec_enc_init_multi_ver","vpx_codec_set_frame_buffer_functions","vpx_codec_set_frame_buffer_functions","vpx_codec_decode","vpx_codec_get_frame","vpx_img_set_rect","vpx_codec_encode","vpx_codec_destroy","vpx_codec_destroy"],"visited":0}
/*Here is an example of a fuzz driver that uses the libvpx library APIs to achieve the event described:

*/



extern "C" int LLVMFuzzerTestOneInput_84(const uint8_t *data, size_t size) {
	if(size<0) return 0;

  vpx_codec_ctx_t decoder_ctx;
  vpx_codec_ctx_t encoder_ctx;

  // Initialize decoder
  vpx_codec_iface_t *decoder_iface = vpx_codec_vp9_dx();
  vpx_codec_dec_cfg_t decoder_cfg{};
  vpx_codec_dec_init_ver(&decoder_ctx, decoder_iface, &decoder_cfg, 0, VPX_DECODER_ABI_VERSION);

  // Initialize encoder
  vpx_codec_iface_t *encoder_iface = vpx_codec_vp9_cx();
  vpx_codec_enc_cfg_t encoder_cfg{};
  vpx_codec_enc_init_multi_ver(&encoder_ctx, encoder_iface, &encoder_cfg, 1, 0, nullptr, VPX_ENCODER_ABI_VERSION);

  // Create a file to read input data
  FILE *in_file = fmemopen((void *)data, size, "rb");
  if (!in_file) {
    assert_file_closed(&in_file);
	return 0;
  }

  // Set the file as the input source for the decoder
  int in_file_fd = fuzz_fileno(in_file);
  vpx_codec_set_frame_buffer_functions(&decoder_ctx, nullptr, nullptr, (void *)&in_file_fd);

  // Create a file to write output data
  FILE *out_file = fopen("output_file", "wb");
  if (!out_file) {
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(in_file_fd);
	return 0;
  }

  // Set the file as the output destination for the encoder
  int out_file_fd = fuzz_fileno(out_file);
  vpx_codec_set_frame_buffer_functions(&encoder_ctx, nullptr, nullptr, (void *)&out_file_fd);

  // Decode the input data
  uint8_t buffer[4096];
  size_t bytes_read;
  do {
    bytes_read = fread(buffer, sizeof(uint8_t), sizeof(buffer), in_file);
    vpx_codec_decode(&decoder_ctx, buffer, bytes_read, nullptr, 0);
  } while (bytes_read == sizeof(buffer));

  // Get frames from the decoder
  vpx_codec_iter_t decoder_iter = nullptr;
  const vpx_image_t *decoder_frame;
  while ((decoder_frame = vpx_codec_get_frame(&decoder_ctx, &decoder_iter)) != nullptr) {
    // Set a rectangular region on the frame
    vpx_img_set_rect(const_cast<vpx_image_t *>(decoder_frame), 0, 0, 10, 10);

    // Encode the frame
    vpx_codec_encode(&encoder_ctx, decoder_frame, 0, 1, 0, 0);
  }

  // Destroy the codec contexts
  vpx_codec_destroy(&decoder_ctx);
  vpx_codec_destroy(&encoder_ctx);

  // Close the input and output files
  assert_file_closed(&in_file);;
  assert_file_closed(&out_file);;

  assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_file_fd);
	assert_fd_closed(in_file_fd);
	return 0;
}