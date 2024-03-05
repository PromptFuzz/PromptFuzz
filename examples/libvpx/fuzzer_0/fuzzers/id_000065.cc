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
//<ID> 2546
//<Prompt> ["vpx_codec_vp8_cx","vpx_codec_peek_stream_info","vpx_codec_decode","vpx_codec_error","vpx_img_alloc","vpx_codec_encode","vpx_codec_get_stream_info","vpx_write_tpl_gop_stats","vpx_codec_dec_init_ver","vpx_codec_set_frame_buffer_functions","vpx_codec_get_preview_frame","vpx_codec_get_cx_data","vpx_codec_destroy","vpx_img_free"]
/*<Combination>: [vpx_codec_iface_t *vpx_codec_vp8_cx(),
    vpx_codec_err_t vpx_codec_peek_stream_info(vpx_codec_iface_t * iface, const uint8_t * data, unsigned int data_sz, vpx_codec_stream_info_t * si),
    vpx_codec_err_t vpx_codec_decode(vpx_codec_ctx_t * ctx, const uint8_t * data, unsigned int data_sz, void * user_priv, long deadline),
    const char *vpx_codec_error(const vpx_codec_ctx_t * ctx),
    vpx_image_t *vpx_img_alloc(vpx_image_t * img, vpx_img_fmt_t fmt, unsigned int d_w, unsigned int d_h, unsigned int align),
    vpx_codec_err_t vpx_codec_encode(vpx_codec_ctx_t * ctx, const vpx_image_t * img, vpx_codec_pts_t pts, unsigned long duration, vpx_enc_frame_flags_t flags, unsigned long deadline),
    vpx_codec_err_t vpx_codec_get_stream_info(vpx_codec_ctx_t * ctx, vpx_codec_stream_info_t * si),
    vpx_codec_err_t vpx_write_tpl_gop_stats(FILE * tpl_file, const VpxTplGopStats * tpl_gop_stats),
    vpx_codec_err_t vpx_codec_dec_init_ver(vpx_codec_ctx_t * ctx, vpx_codec_iface_t * iface, const vpx_codec_dec_cfg_t * cfg, vpx_codec_flags_t flags, int ver),
    vpx_codec_err_t vpx_codec_set_frame_buffer_functions(vpx_codec_ctx_t * ctx, vpx_get_frame_buffer_cb_fn_t cb_get, vpx_release_frame_buffer_cb_fn_t cb_release, void * cb_priv),
    const vpx_image_t *vpx_codec_get_preview_frame(vpx_codec_ctx_t * ctx),
    const vpx_codec_cx_pkt_t *vpx_codec_get_cx_data(vpx_codec_ctx_t * ctx, vpx_codec_iter_t * iter),
    vpx_codec_err_t vpx_codec_destroy(vpx_codec_ctx_t * ctx),
    void vpx_img_free(vpx_image_t * img)
*/
//<score> 8, nr_unique_branch: 1
//<Quality> {"density":12,"unique_branches":{"vpx_codec_dec_init_ver":[[45,12,45,50,0,0,4,0]]},"library_calls":["vpx_codec_vp8_cx","vpx_codec_peek_stream_info","vpx_codec_dec_init_ver","vpx_img_alloc","vpx_codec_vp8_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_encode","vpx_codec_get_cx_data","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp8_cx","vpx_codec_peek_stream_info","vpx_codec_dec_init_ver","vpx_img_alloc","vpx_codec_vp8_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_encode","vpx_codec_get_cx_data","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy"],"visited":2}
/**/

extern "C" int LLVMFuzzerTestOneInput_65(const uint8_t *data, size_t size) {
	if(size<0) return 0;

	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);

    // Step 4: Create file streams for input and output
    FILE *in_file = fmemopen((void *)data, size, "rb");
    FILE *out_file = fopen("output_file", "wb");

    // Step 5: Get file descriptors for input and output
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);

    // Step 6: Specify file names
    const char *in_file_name = "input_file";
    const char *out_file_name = "output_file";

    // Step 1: Decode the compressed video stream
    vpx_codec_iface_t *decoder_iface = vpx_codec_vp8_cx();
    vpx_codec_stream_info_t stream_info;
    vpx_codec_err_t stream_info_err = vpx_codec_peek_stream_info(
        decoder_iface, data, size, &stream_info);
    if (stream_info_err != VPX_CODEC_OK) {
        // Handle error
    }

    vpx_codec_ctx_t decoder_ctx;
    vpx_codec_dec_cfg_t decoder_cfg = {0};
    decoder_cfg.w = stream_info.w;
    decoder_cfg.h = stream_info.h;
    vpx_codec_flags_t decoder_flags = 0;
    int decoder_ver = VPX_DECODER_ABI_VERSION;
    vpx_codec_err_t decoder_init_err = vpx_codec_dec_init_ver(
        &decoder_ctx, decoder_iface, &decoder_cfg, decoder_flags, decoder_ver);
    if (decoder_init_err != VPX_CODEC_OK) {
        // Handle error
    }

    // Step 2: Encode the decoded video frames
    vpx_image_t *image = vpx_img_alloc(NULL, VPX_IMG_FMT_I420, decoder_cfg.w, decoder_cfg.h, 1);
    vpx_codec_ctx_t encoder_ctx;
    vpx_codec_iface_t *encoder_iface = vpx_codec_vp8_cx();
    vpx_codec_enc_cfg_t encoder_cfg;
    vpx_codec_enc_config_default(encoder_iface, &encoder_cfg, 0);
    encoder_cfg.g_w = decoder_cfg.w;
    encoder_cfg.g_h = decoder_cfg.h;
    vpx_codec_err_t encoder_init_err = vpx_codec_enc_init_ver(
        &encoder_ctx, encoder_iface, &encoder_cfg, 0, VPX_ENCODER_ABI_VERSION);
    if (encoder_init_err != VPX_CODEC_OK) {
        // Handle error
    }
    vpx_codec_frame_flags_t frame_flags = 0;
    vpx_codec_pts_t pts = 0;
    unsigned long duration = 1;
    unsigned long deadline = 0;
    vpx_enc_frame_flags_t enc_flags = 0;
    vpx_codec_err_t encode_err = vpx_codec_encode(
        &encoder_ctx, image, pts, duration, enc_flags, deadline);
    if (encode_err != VPX_CODEC_OK) {
        // Handle error
    }

    // Step 3: Process the output of the encoder
    const vpx_codec_cx_pkt_t *cx_pkt = NULL;
    vpx_codec_iter_t iter = NULL;
    while ((cx_pkt = vpx_codec_get_cx_data(&encoder_ctx, &iter)) != NULL) {
        if (cx_pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
            // Handle encoded frames
        }
    }

    // Step 7: Release resources
    vpx_img_free(image);
    vpx_codec_destroy(&encoder_ctx);
    vpx_codec_destroy(&decoder_ctx);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;

    assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}