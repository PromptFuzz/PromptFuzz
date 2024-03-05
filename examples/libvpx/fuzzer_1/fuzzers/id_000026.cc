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
//<ID> 1207
//<Prompt> ["vpx_img_set_rect","vpx_codec_vp8_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_vp8_dx","vpx_codec_dec_init_ver","vpx_img_alloc","vpx_codec_register_put_frame_cb","vpx_codec_get_stream_info","vpx_codec_encode","vpx_codec_register_put_slice_cb","vpx_codec_get_preview_frame","vpx_codec_decode","vpx_codec_get_frame","vpx_codec_get_global_headers","vpx_codec_destroy","vpx_img_free"]
/*<Combination>: [int vpx_img_set_rect(vpx_image_t * img, unsigned int x, unsigned int y, unsigned int w, unsigned int h),
    vpx_codec_iface_t *vpx_codec_vp8_cx(),
    vpx_codec_err_t vpx_codec_enc_config_default(vpx_codec_iface_t * iface, vpx_codec_enc_cfg_t * cfg, unsigned int usage),
    vpx_codec_err_t vpx_codec_enc_init_ver(vpx_codec_ctx_t * ctx, vpx_codec_iface_t * iface, const vpx_codec_enc_cfg_t * cfg, vpx_codec_flags_t flags, int ver),
    vpx_codec_iface_t *vpx_codec_vp8_dx(),
    vpx_codec_err_t vpx_codec_dec_init_ver(vpx_codec_ctx_t * ctx, vpx_codec_iface_t * iface, const vpx_codec_dec_cfg_t * cfg, vpx_codec_flags_t flags, int ver),
    vpx_image_t *vpx_img_alloc(vpx_image_t * img, vpx_img_fmt_t fmt, unsigned int d_w, unsigned int d_h, unsigned int align),
    vpx_codec_err_t vpx_codec_register_put_frame_cb(vpx_codec_ctx_t * ctx, vpx_codec_put_frame_cb_fn_t cb, void * user_priv),
    vpx_codec_err_t vpx_codec_get_stream_info(vpx_codec_ctx_t * ctx, vpx_codec_stream_info_t * si),
    vpx_codec_err_t vpx_codec_encode(vpx_codec_ctx_t * ctx, const vpx_image_t * img, vpx_codec_pts_t pts, unsigned long duration, vpx_enc_frame_flags_t flags, unsigned long deadline),
    vpx_codec_err_t vpx_codec_register_put_slice_cb(vpx_codec_ctx_t * ctx, vpx_codec_put_slice_cb_fn_t cb, void * user_priv),
    const vpx_image_t *vpx_codec_get_preview_frame(vpx_codec_ctx_t * ctx),
    vpx_codec_err_t vpx_codec_decode(vpx_codec_ctx_t * ctx, const uint8_t * data, unsigned int data_sz, void * user_priv, long deadline),
    vpx_image_t *vpx_codec_get_frame(vpx_codec_ctx_t * ctx, vpx_codec_iter_t * iter),
    vpx_fixed_buf_t *vpx_codec_get_global_headers(vpx_codec_ctx_t * ctx),
    vpx_codec_err_t vpx_codec_destroy(vpx_codec_ctx_t * ctx),
    void vpx_img_free(vpx_image_t * img)
*/
//<score> 11, nr_unique_branch: 1
//<Quality> {"density":11,"unique_branches":{"vpx_codec_get_stream_info":[[89,15,89,18,0,0,4,0]]},"library_calls":["vpx_codec_vp8_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_vp8_dx","vpx_codec_dec_init_ver","vpx_img_alloc","vpx_codec_register_put_frame_cb","vpx_codec_get_stream_info","vpx_codec_encode","vpx_codec_register_put_slice_cb","vpx_codec_get_preview_frame","vpx_codec_decode","vpx_codec_get_frame","vpx_codec_decode","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp8_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_vp8_dx","vpx_codec_dec_init_ver","vpx_img_alloc","vpx_codec_register_put_frame_cb","vpx_codec_get_stream_info","vpx_codec_encode","vpx_codec_register_put_slice_cb","vpx_codec_get_preview_frame","vpx_codec_decode","vpx_codec_get_frame","vpx_codec_decode","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy"],"visited":1}
/**/

extern "C" int LLVMFuzzerTestOneInput_26(const uint8_t* f_data, size_t f_size) {
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
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





  // Step 4: Create a FILE pointer to read input data
  FILE *in_file = fmemopen((void *)data, size, "rb");
  
  // Step 5: Create a file descriptor for reading
  int in_fd = fuzz_fileno(in_file);

  // Step 6: Create a string for the input file name
  const char *in_filename = "input_file";

  // Step 1: Create and initialize variables using libvpx APIs
  vpx_codec_ctx_t enc_ctx;
  vpx_codec_iface_t *enc_iface = vpx_codec_vp8_cx();
  vpx_codec_enc_cfg_t enc_cfg;
  vpx_codec_enc_config_default(enc_iface, &enc_cfg, fuzz_uint32_t_1);
  vpx_codec_enc_init_ver(&enc_ctx, enc_iface, &enc_cfg, fuzz_int64_t_2, VPX_ENCODER_ABI_VERSION);
  
  vpx_codec_ctx_t dec_ctx;
  vpx_codec_iface_t *dec_iface = vpx_codec_vp8_dx();
  vpx_codec_dec_cfg_t dec_cfg;
  dec_cfg.w = enc_cfg.g_w;
  dec_cfg.h = enc_cfg.g_h;
  vpx_codec_dec_init_ver(&dec_ctx, dec_iface, &dec_cfg, 0, VPX_DECODER_ABI_VERSION);

  vpx_image_t enc_img;
  vpx_image_t dec_img;

  // Step 2: Use the libvpx APIs to achieve the event
  // Set up encoding parameters
  vpx_img_alloc(&enc_img, VPX_IMG_FMT_I420, enc_cfg.g_w, enc_cfg.g_h, 1);
  vpx_codec_register_put_frame_cb(&enc_ctx, NULL, NULL);
  vpx_codec_get_stream_info(&enc_ctx, NULL);
  vpx_codec_encode(&enc_ctx, &enc_img, fuzz_int64_t_3, fuzz_uint64_t_4, fuzz_int64_t_5, fuzz_uint64_t_6);
  vpx_codec_register_put_slice_cb(&enc_ctx, NULL, NULL);
  vpx_codec_get_preview_frame(&enc_ctx);
  
  // Set up decoding parameters
  vpx_codec_decode(&dec_ctx, NULL, 0, NULL, fuzz_int64_t_7);
  vpx_codec_get_frame(&dec_ctx, NULL);
  
  // Step 3: Consume the input data
  vpx_codec_decode(&dec_ctx, data, size, NULL, fuzz_int64_t_8);

  // Step 7: Release allocated resources
  vpx_img_free(&enc_img);
  vpx_codec_destroy(&enc_ctx);
  vpx_codec_destroy(&dec_ctx);
  
  assert_file_closed(&in_file);;

  assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_fd_closed(in_fd);
	return 0;
}