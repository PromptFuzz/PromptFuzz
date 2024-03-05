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
//<ID> 2761
//<Prompt> ["vpx_img_alloc","vpx_codec_dec_init_ver","vpx_codec_vp9_dx","vpx_codec_get_preview_frame","vpx_codec_get_caps","vpx_codec_register_put_frame_cb","vpx_codec_destroy"]
/*<Combination>: [vpx_image_t *vpx_img_alloc(vpx_image_t * img, vpx_img_fmt_t fmt, unsigned int d_w, unsigned int d_h, unsigned int align),
    vpx_codec_err_t vpx_codec_dec_init_ver(vpx_codec_ctx_t * ctx, vpx_codec_iface_t * iface, const vpx_codec_dec_cfg_t * cfg, vpx_codec_flags_t flags, int ver),
    vpx_codec_iface_t *vpx_codec_vp9_dx(),
    const vpx_image_t *vpx_codec_get_preview_frame(vpx_codec_ctx_t * ctx),
    vpx_codec_caps_t vpx_codec_get_caps(vpx_codec_iface_t * iface),
    vpx_codec_err_t vpx_codec_register_put_frame_cb(vpx_codec_ctx_t * ctx, vpx_codec_put_frame_cb_fn_t cb, void * user_priv),
    vpx_codec_err_t vpx_codec_destroy(vpx_codec_ctx_t * ctx)
*/
//<score> 8, nr_unique_branch: 0
//<Quality> {"density":8,"unique_branches":{},"library_calls":["vpx_img_alloc","vpx_codec_vp9_dx","vpx_codec_dec_init_ver","vpx_codec_register_put_frame_cb","vpx_codec_peek_stream_info","vpx_codec_decode","vpx_codec_get_preview_frame","vpx_codec_get_caps","vpx_codec_destroy","vpx_img_free"],"critical_calls":["vpx_img_alloc","vpx_codec_vp9_dx","vpx_codec_dec_init_ver","vpx_codec_register_put_frame_cb","vpx_codec_peek_stream_info","vpx_codec_decode","vpx_codec_get_preview_frame","vpx_codec_get_caps","vpx_codec_destroy","vpx_img_free"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_72(const uint8_t *data, size_t size) {
	if(size<0) return 0;

  FILE * in_file = fmemopen((void *)data, size, "rb");
  FILE * out_file = fopen("output_file", "wb");
  int in_fd = fuzz_fileno(in_file);
  int out_fd = fuzz_fileno(out_file);

  // 1. Allocate vpx_image
  vpx_image_t image;
  vpx_img_alloc(&image, VPX_IMG_FMT_I420, 640, 480, 32);

  // 2. Initialize VP9 decoder
  vpx_codec_iface_t *iface = vpx_codec_vp9_dx();
  vpx_codec_ctx_t decoder;
  vpx_codec_dec_cfg_t cfg;
  cfg.w = 640;
  cfg.h = 480;
  vpx_codec_dec_init_ver(&decoder, iface, &cfg, 0, VPX_DECODER_ABI_VERSION);

  // 3. Register put_frame callback
  vpx_codec_put_frame_cb_fn_t put_frame_cb;
  vpx_codec_register_put_frame_cb(&decoder, put_frame_cb, NULL);

  // 4. Read input data and decode frames
  vpx_codec_stream_info_t stream_info;
  vpx_codec_peek_stream_info(iface, data, size, &stream_info);
  vpx_codec_decode(&decoder, data, size, NULL, -1);

  // 5. Get decoded frames
  const vpx_image_t *preview_frame = vpx_codec_get_preview_frame(&decoder);

  // 6. Get VP9 decoder capabilities
  vpx_codec_caps_t caps = vpx_codec_get_caps(iface);

  // 7. Destroy the decoder and free resources
  vpx_codec_destroy(&decoder);
  vpx_img_free(&image);
  assert_file_closed(&in_file);;
  assert_file_closed(&out_file);;

  assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}