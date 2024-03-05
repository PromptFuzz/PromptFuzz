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
//<ID> 1768
//<Prompt> ["vpx_codec_get_global_headers","vpx_codec_vp9_dx","vpx_codec_dec_init_ver","vpx_codec_err_to_string","vpx_codec_peek_stream_info","vpx_codec_register_put_frame_cb","vpx_codec_enc_init_multi_ver","vpx_img_wrap","vpx_codec_set_frame_buffer_functions","vpx_img_set_rect","vpx_codec_decode","vpx_codec_get_preview_frame","vpx_img_free"]
/*<Combination>: [vpx_fixed_buf_t *vpx_codec_get_global_headers(vpx_codec_ctx_t * ctx),
    vpx_codec_iface_t *vpx_codec_vp9_dx(),
    vpx_codec_err_t vpx_codec_dec_init_ver(vpx_codec_ctx_t * ctx, vpx_codec_iface_t * iface, const vpx_codec_dec_cfg_t * cfg, vpx_codec_flags_t flags, int ver),
    const char *vpx_codec_err_to_string(vpx_codec_err_t err),
    vpx_codec_err_t vpx_codec_peek_stream_info(vpx_codec_iface_t * iface, const uint8_t * data, unsigned int data_sz, vpx_codec_stream_info_t * si),
    vpx_codec_err_t vpx_codec_register_put_frame_cb(vpx_codec_ctx_t * ctx, vpx_codec_put_frame_cb_fn_t cb, void * user_priv),
    vpx_codec_err_t vpx_codec_enc_init_multi_ver(vpx_codec_ctx_t * ctx, vpx_codec_iface_t * iface, vpx_codec_enc_cfg_t * cfg, int num_enc, vpx_codec_flags_t flags, vpx_rational_t * dsf, int ver),
    vpx_image_t *vpx_img_wrap(vpx_image_t * img, vpx_img_fmt_t fmt, unsigned int d_w, unsigned int d_h, unsigned int stride_align, unsigned char * img_data),
    vpx_codec_err_t vpx_codec_set_frame_buffer_functions(vpx_codec_ctx_t * ctx, vpx_get_frame_buffer_cb_fn_t cb_get, vpx_release_frame_buffer_cb_fn_t cb_release, void * cb_priv),
    int vpx_img_set_rect(vpx_image_t * img, unsigned int x, unsigned int y, unsigned int w, unsigned int h),
    vpx_codec_err_t vpx_codec_decode(vpx_codec_ctx_t * ctx, const uint8_t * data, unsigned int data_sz, void * user_priv, long deadline),
    const vpx_image_t *vpx_codec_get_preview_frame(vpx_codec_ctx_t * ctx),
    void vpx_img_free(vpx_image_t * img)
*/
//<score> 5.6, nr_unique_branch: 1
//<Quality> {"density":14,"unique_branches":{"vpx_codec_enc_config_default":[[162,12,162,50,0,0,4,0]]},"library_calls":["vpx_codec_dec_init_ver","vpx_codec_vp9_dx","vpx_codec_peek_stream_info","vpx_codec_vp9_dx","vpx_codec_register_put_frame_cb","vpx_codec_vp9_dx","vpx_codec_enc_config_default","vpx_codec_enc_init_multi_ver","vpx_img_wrap","vpx_img_set_rect","vpx_codec_decode","vpx_codec_get_preview_frame","vpx_img_free","vpx_codec_destroy"],"critical_calls":["vpx_codec_dec_init_ver","vpx_codec_vp9_dx","vpx_codec_peek_stream_info","vpx_codec_vp9_dx","vpx_codec_register_put_frame_cb","vpx_codec_vp9_dx","vpx_codec_enc_config_default","vpx_codec_enc_init_multi_ver","vpx_img_wrap","vpx_img_set_rect","vpx_codec_decode","vpx_codec_get_preview_frame","vpx_img_free","vpx_codec_destroy"],"visited":4}
/*Here is the C++ program that follows the instructions provided:

*/



// Declare the LLVMFuzzerTestOneInput_35 function
extern "C" int LLVMFuzzerTestOneInput_35(const uint8_t* f_data, size_t f_size) {
	if(f_size<44) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int64_t, fuzz_int64_t_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_4, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_5, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_6, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_7, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_8, fdp);
	//fuzzer shim end}




  // Create a vpx_codec_ctx_t variable
  vpx_codec_ctx_t codec_ctx;

  // Initialize the codec context
  vpx_codec_dec_cfg_t cfg;
  vpx_codec_dec_init_ver(&codec_ctx, vpx_codec_vp9_dx(), &cfg, fuzz_int64_t_1, VPX_DECODER_ABI_VERSION);

  // Create a vpx_codec_stream_info_t variable
  vpx_codec_stream_info_t stream_info;

  // Peek the stream information from the input data
  vpx_codec_peek_stream_info(vpx_codec_vp9_dx(), data, size, &stream_info);

  // Register the put frame callback function
  vpx_codec_register_put_frame_cb(&codec_ctx, nullptr, nullptr);

  // Create a vpx_codec_iface_t variable
  vpx_codec_iface_t *iface = vpx_codec_vp9_dx();

  // Create a vpx_codec_enc_cfg_t variable
  vpx_codec_enc_cfg_t enc_cfg;

  // Initialize the encoder configuration
  vpx_codec_enc_config_default(iface, &enc_cfg, fuzz_uint32_t_2);

  // Create a vpx_rational_t variable
  vpx_rational_t dsf;

  // Initialize the dsf
  dsf.num = 1;
  dsf.den = 1;

  // Initialize the encoder
  vpx_codec_enc_init_multi_ver(&codec_ctx, iface, &enc_cfg, fuzz_int32_t_3, fuzz_int64_t_4, &dsf, VPX_ENCODER_ABI_VERSION);

  // Create and wrap a vpx_image_t variable
  vpx_image_t image;
  vpx_img_wrap(&image, VPX_IMG_FMT_I420, enc_cfg.g_w, enc_cfg.g_h, fuzz_uint32_t_5, nullptr);

  // Set the rectangle of the image
  vpx_img_set_rect(&image, fuzz_uint32_t_6, fuzz_uint32_t_7, enc_cfg.g_w, enc_cfg.g_h);

  // Decode the input data
  vpx_codec_decode(&codec_ctx, data, size, nullptr, fuzz_int64_t_8);

  // Get the preview frame
  const vpx_image_t *preview_image = vpx_codec_get_preview_frame(&codec_ctx);

  // Free the image
  vpx_img_free(&image);

  // Destroy the codec context
  vpx_codec_destroy(&codec_ctx);

  // Return 0 to indicate successful execution
  return 0;
}