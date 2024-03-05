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
//<ID> 2392
//<Prompt> ["vpx_codec_error","vpx_codec_control_","vpx_codec_vp8_cx","vpx_codec_enc_config_default","vpx_codec_peek_stream_info","vpx_codec_set_cx_data_buf","vpx_img_alloc","vpx_codec_encode","vpx_codec_decode","vpx_codec_err_to_string","vpx_codec_destroy","vpx_codec_register_put_slice_cb","vpx_img_free"]
/*<Combination>: [const char *vpx_codec_error(const vpx_codec_ctx_t * ctx),
    vpx_codec_err_t vpx_codec_control_(vpx_codec_ctx_t * ctx, int ctrl_id),
    vpx_codec_iface_t *vpx_codec_vp8_cx(),
    vpx_codec_err_t vpx_codec_enc_config_default(vpx_codec_iface_t * iface, vpx_codec_enc_cfg_t * cfg, unsigned int usage),
    vpx_codec_err_t vpx_codec_peek_stream_info(vpx_codec_iface_t * iface, const uint8_t * data, unsigned int data_sz, vpx_codec_stream_info_t * si),
    vpx_codec_err_t vpx_codec_set_cx_data_buf(vpx_codec_ctx_t * ctx, const vpx_fixed_buf_t * buf, unsigned int pad_before, unsigned int pad_after),
    vpx_image_t *vpx_img_alloc(vpx_image_t * img, vpx_img_fmt_t fmt, unsigned int d_w, unsigned int d_h, unsigned int align),
    vpx_codec_err_t vpx_codec_encode(vpx_codec_ctx_t * ctx, const vpx_image_t * img, vpx_codec_pts_t pts, unsigned long duration, vpx_enc_frame_flags_t flags, unsigned long deadline),
    vpx_codec_err_t vpx_codec_decode(vpx_codec_ctx_t * ctx, const uint8_t * data, unsigned int data_sz, void * user_priv, long deadline),
    const char *vpx_codec_err_to_string(vpx_codec_err_t err),
    vpx_codec_err_t vpx_codec_destroy(vpx_codec_ctx_t * ctx),
    vpx_codec_err_t vpx_codec_register_put_slice_cb(vpx_codec_ctx_t * ctx, vpx_codec_put_slice_cb_fn_t cb, void * user_priv),
    void vpx_img_free(vpx_image_t * img)
*/
//<score> 14, nr_unique_branch: 0
//<Quality> {"density":14,"unique_branches":{},"library_calls":["vpx_codec_enc_config_default","vpx_codec_vp8_cx","vpx_codec_enc_init_ver","vpx_codec_vp8_cx","vpx_codec_dec_init_ver","vpx_codec_vp8_dx","vpx_codec_destroy","vpx_codec_control_","vpx_codec_destroy","vpx_codec_destroy","vpx_img_alloc","vpx_codec_encode","vpx_codec_get_cx_data","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy","vpx_codec_peek_stream_info","vpx_codec_vp8_dx","vpx_codec_decode","vpx_codec_get_frame","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy"],"critical_calls":["vpx_codec_enc_config_default","vpx_codec_vp8_cx","vpx_codec_enc_init_ver","vpx_codec_vp8_cx","vpx_codec_dec_init_ver","vpx_codec_vp8_dx","vpx_codec_control_","vpx_img_alloc","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_peek_stream_info","vpx_codec_vp8_dx","vpx_codec_decode","vpx_codec_get_frame","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy"],"visited":0}
/**/

extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_56(const uint8_t *data, size_t size) {
	if(size<0) return 0;

    // Open input file
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file == nullptr) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Open output file
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

    // Initialize libvpx encoder
    vpx_codec_ctx_t encoder;
    vpx_codec_enc_cfg_t encoder_cfg;
    // Set encoder configuration
    vpx_codec_enc_config_default(vpx_codec_vp8_cx(), &encoder_cfg, 0);
    // Initialize encoder
    if (vpx_codec_enc_init(&encoder, vpx_codec_vp8_cx(), &encoder_cfg, 0) != VPX_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }

    // Initialize libvpx decoder
    vpx_codec_ctx_t decoder;
    if (vpx_codec_dec_init(&decoder, vpx_codec_vp8_dx(), nullptr, 0) != VPX_CODEC_OK) {
        vpx_codec_destroy(&encoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }

    // Set encoder control parameters
    vpx_codec_control_(&encoder, VP8E_SET_CPUUSED, -5);

    // Read input data
    fseek(in_file, 0, SEEK_END);
    long input_size = ftell(in_file);
    fseek(in_file, 0, SEEK_SET);
    uint8_t *input_data = (uint8_t *)malloc(input_size);
    if (input_data == nullptr) {
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
    fread(input_data, 1, input_size, in_file);

    // Encode
    vpx_image_t *raw_frame = vpx_img_alloc(nullptr, VPX_IMG_FMT_I420, encoder_cfg.g_w, encoder_cfg.g_h, 1);
    vpx_codec_encode(&encoder, raw_frame, 0, 1, 0, VPX_DL_GOOD_QUALITY);

    // Write encoded data to output file
    const vpx_codec_cx_pkt_t *packet;
    while ((packet = vpx_codec_get_cx_data(&encoder, nullptr)) != nullptr) {
        if (fwrite(packet->data.frame.buf, packet->data.frame.sz, 1, out_file) != 1) {
            break;
        }
    }

    // Decode
    uint8_t *output_data = (uint8_t *)malloc(size);
    if (output_data == nullptr) {
        vpx_img_free(raw_frame);
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

    vpx_image_t *decoded_frame = nullptr;

    vpx_codec_stream_info_t stream_info;
    vpx_codec_peek_stream_info(vpx_codec_vp8_dx(), output_data, size, &stream_info);
    vpx_codec_decode(&decoder, output_data, size, nullptr, 0);

    // Get decoded frame
    while ((decoded_frame = vpx_codec_get_frame(&decoder, nullptr)) != nullptr) {
        // Do something with the decoded frame
    }

    // Cleanup
    free(input_data);
    free(output_data);
    vpx_img_free(raw_frame);
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