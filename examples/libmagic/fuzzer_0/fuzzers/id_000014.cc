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
//<ID> 757
//<Prompt> ["vpx_codec_err_to_string","vpx_codec_error","vpx_codec_vp8_cx","vpx_codec_enc_config_default","vpx_codec_set_frame_buffer_functions","vpx_codec_peek_stream_info","vpx_codec_set_cx_data_buf","vpx_read_tpl_gop_stats","vpx_img_alloc","vpx_codec_encode","vpx_codec_decode","vpx_codec_destroy","vpx_img_free"]
/*<Combination>: [const char *vpx_codec_err_to_string(vpx_codec_err_t err),
    const char *vpx_codec_error(const vpx_codec_ctx_t * ctx),
    vpx_codec_iface_t *vpx_codec_vp8_cx(),
    vpx_codec_err_t vpx_codec_enc_config_default(vpx_codec_iface_t * iface, vpx_codec_enc_cfg_t * cfg, unsigned int usage),
    vpx_codec_err_t vpx_codec_set_frame_buffer_functions(vpx_codec_ctx_t * ctx, vpx_get_frame_buffer_cb_fn_t cb_get, vpx_release_frame_buffer_cb_fn_t cb_release, void * cb_priv),
    vpx_codec_err_t vpx_codec_peek_stream_info(vpx_codec_iface_t * iface, const uint8_t * data, unsigned int data_sz, vpx_codec_stream_info_t * si),
    vpx_codec_err_t vpx_codec_set_cx_data_buf(vpx_codec_ctx_t * ctx, const vpx_fixed_buf_t * buf, unsigned int pad_before, unsigned int pad_after),
    vpx_codec_err_t vpx_read_tpl_gop_stats(FILE * tpl_file, VpxTplGopStats * tpl_gop_stats),
    vpx_image_t *vpx_img_alloc(vpx_image_t * img, vpx_img_fmt_t fmt, unsigned int d_w, unsigned int d_h, unsigned int align),
    vpx_codec_err_t vpx_codec_encode(vpx_codec_ctx_t * ctx, const vpx_image_t * img, vpx_codec_pts_t pts, unsigned long duration, vpx_enc_frame_flags_t flags, unsigned long deadline),
    vpx_codec_err_t vpx_codec_decode(vpx_codec_ctx_t * ctx, const uint8_t * data, unsigned int data_sz, void * user_priv, long deadline),
    vpx_codec_err_t vpx_codec_destroy(vpx_codec_ctx_t * ctx),
    void vpx_img_free(vpx_image_t * img)
*/
//<score> 5.25, nr_unique_branch: 3
//<Quality> {"density":7,"unique_branches":{"vpx_codec_set_cx_data_buf":[[292,15,292,25,0,0,4,0]],"vpx_read_tpl_gop_stats":[[61,7,61,23,0,0,4,1],[61,27,61,48,0,0,4,0]]},"library_calls":["vpx_codec_err_to_string","vpx_codec_error","vpx_codec_vp8_cx","vpx_codec_enc_config_default","vpx_codec_set_frame_buffer_functions","vpx_codec_peek_stream_info","vpx_codec_set_cx_data_buf","vpx_read_tpl_gop_stats","vpx_img_alloc","vpx_codec_encode","vpx_codec_decode","vpx_codec_destroy","vpx_img_free"],"critical_calls":["vpx_codec_err_to_string","vpx_codec_error","vpx_codec_vp8_cx","vpx_codec_enc_config_default","vpx_codec_set_frame_buffer_functions","vpx_codec_peek_stream_info","vpx_codec_set_cx_data_buf","vpx_read_tpl_gop_stats","vpx_img_alloc","vpx_codec_encode","vpx_codec_decode","vpx_codec_destroy","vpx_img_free"],"visited":3}
/**/



extern "C" int LLVMFuzzerTestOneInput_14(const uint8_t* data, size_t size) {
	if(size<0) return 0;

	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);

  // Step 4: Open file for input data
  FILE* in_file = fmemopen((void*)data, size, "rb");

  // Step 6: Define input file name
  const char* input_file = "input_file";

  // Step 7: Create variables for libvpx APIs
  vpx_codec_ctx_t encoder;
  vpx_codec_ctx_t decoder;
  vpx_codec_enc_cfg_t enc_cfg;
  vpx_codec_stream_info_t si;
  vpx_image_t img;

  // Step 2: Call libvpx APIs to achieve the event
  const char* error_string = vpx_codec_err_to_string(VPX_CODEC_OK);
  const char* error_detail = vpx_codec_error(&encoder);
  vpx_codec_iface_t* codec_iface = vpx_codec_vp8_cx();
  vpx_codec_enc_config_default(codec_iface, &enc_cfg, 0);
  vpx_codec_set_frame_buffer_functions(&encoder, nullptr, nullptr, nullptr);
  vpx_codec_peek_stream_info(codec_iface, data, size, &si);
  vpx_fixed_buf_t buf;
  vpx_codec_set_cx_data_buf(&encoder, &buf, 0, 0);
  vpx_read_tpl_gop_stats(in_file, nullptr);
  vpx_img_alloc(&img, VPX_IMG_FMT_I420, 1280, 720, 16);
  vpx_codec_encode(&encoder, &img, 0, 0, 0, 0);
  vpx_codec_decode(&decoder, data, size, nullptr, 0);
  vpx_codec_destroy(&encoder);
  vpx_img_free(&img);

  // Step 7: Close input file
  assert_file_closed(&in_file);;

  assert_file_name_closed("input_file");
	assert_file_closed(&in_file);
	return 0;
}