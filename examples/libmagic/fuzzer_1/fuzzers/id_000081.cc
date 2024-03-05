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
//<ID> 2919
//<Prompt> ["vpx_codec_vp9_dx","vpx_codec_dec_init_ver","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_set_frame_buffer_functions","vpx_codec_register_put_frame_cb","vpx_img_set_rect","vpx_codec_register_put_slice_cb","vpx_codec_encode","vpx_codec_control_","vpx_codec_decode","vpx_codec_get_stream_info","vpx_codec_err_to_string","vpx_img_free","vpx_codec_destroy"]
/*<Combination>: [vpx_codec_iface_t *vpx_codec_vp9_dx(),
    vpx_codec_err_t vpx_codec_dec_init_ver(vpx_codec_ctx_t * ctx, vpx_codec_iface_t * iface, const vpx_codec_dec_cfg_t * cfg, vpx_codec_flags_t flags, int ver),
    vpx_codec_iface_t *vpx_codec_vp9_cx(),
    vpx_codec_err_t vpx_codec_enc_config_default(vpx_codec_iface_t * iface, vpx_codec_enc_cfg_t * cfg, unsigned int usage),
    vpx_codec_err_t vpx_codec_enc_init_ver(vpx_codec_ctx_t * ctx, vpx_codec_iface_t * iface, const vpx_codec_enc_cfg_t * cfg, vpx_codec_flags_t flags, int ver),
    vpx_codec_err_t vpx_codec_set_frame_buffer_functions(vpx_codec_ctx_t * ctx, vpx_get_frame_buffer_cb_fn_t cb_get, vpx_release_frame_buffer_cb_fn_t cb_release, void * cb_priv),
    vpx_codec_err_t vpx_codec_register_put_frame_cb(vpx_codec_ctx_t * ctx, vpx_codec_put_frame_cb_fn_t cb, void * user_priv),
    int vpx_img_set_rect(vpx_image_t * img, unsigned int x, unsigned int y, unsigned int w, unsigned int h),
    vpx_codec_err_t vpx_codec_register_put_slice_cb(vpx_codec_ctx_t * ctx, vpx_codec_put_slice_cb_fn_t cb, void * user_priv),
    vpx_codec_err_t vpx_codec_encode(vpx_codec_ctx_t * ctx, const vpx_image_t * img, vpx_codec_pts_t pts, unsigned long duration, vpx_enc_frame_flags_t flags, unsigned long deadline),
    vpx_codec_err_t vpx_codec_control_(vpx_codec_ctx_t * ctx, int ctrl_id),
    vpx_codec_err_t vpx_codec_decode(vpx_codec_ctx_t * ctx, const uint8_t * data, unsigned int data_sz, void * user_priv, long deadline),
    vpx_codec_err_t vpx_codec_get_stream_info(vpx_codec_ctx_t * ctx, vpx_codec_stream_info_t * si),
    const char *vpx_codec_err_to_string(vpx_codec_err_t err),
    void vpx_img_free(vpx_image_t * img),
    vpx_codec_err_t vpx_codec_destroy(vpx_codec_ctx_t * ctx)
*/
//<score> 13.5, nr_unique_branch: 5
//<Quality> {"density":9,"unique_branches":{"set_good_speed_feature_framesize_dependent":[[89,7,89,24,0,0,4,0],[93,7,93,26,0,0,4,1],[106,7,106,25,0,0,4,1]],"vp9_noise_estimate_init":[[35,15,35,44,0,0,4,1],[41,7,41,36,0,0,4,0]]},"library_calls":["vpx_codec_vp9_dx","vpx_codec_dec_init_ver","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_set_frame_buffer_functions","vpx_codec_register_put_frame_cb","vpx_codec_register_put_slice_cb","vpx_codec_decode","vpx_codec_get_stream_info","vpx_codec_encode","vpx_codec_get_cx_data","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp9_dx","vpx_codec_dec_init_ver","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_set_frame_buffer_functions","vpx_codec_register_put_frame_cb","vpx_codec_register_put_slice_cb","vpx_codec_decode","vpx_codec_get_stream_info","vpx_codec_encode","vpx_codec_get_cx_data","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy"],"visited":1}
/*Here is the complete C++ program that achieves the event using the libvpx APIs:

*/


extern "C" int LLVMFuzzerTestOneInput_81(const uint8_t* f_data, size_t f_size) {
	if(f_size<60) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int64_t, fuzz_int64_t_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_3, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_4, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_5, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_6, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_7, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_8, fdp);
	//fuzzer shim end}




    // Create input file
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file == nullptr) {
        assert_file_closed(&in_file);
	return 0;
    }
    int in_fd = fuzz_fileno(in_file);

    // Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (out_file == nullptr) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
    }
    int out_fd = fuzz_fileno(out_file);

    // Initialize libvpx decoder
    vpx_codec_iface_t *decoder_iface = vpx_codec_vp9_dx();
    vpx_codec_ctx_t decoder_ctx;
    vpx_codec_dec_cfg_t decoder_cfg;
    vpx_codec_stream_info_t stream_info;
    memset(&decoder_cfg, 0, sizeof(decoder_cfg));
    memset(&stream_info, 0, sizeof(stream_info));
    decoder_cfg.w = 1920;
    decoder_cfg.h = 1080;
    vpx_codec_dec_init_ver(&decoder_ctx, decoder_iface, &decoder_cfg, fuzz_int64_t_1, VPX_DECODER_ABI_VERSION);

    // Initialize libvpx encoder
    vpx_codec_iface_t *encoder_iface = vpx_codec_vp9_cx();
    vpx_codec_ctx_t encoder_ctx;
    vpx_codec_enc_cfg_t encoder_cfg;
    memset(&encoder_cfg, 0, sizeof(encoder_cfg));
    vpx_codec_enc_config_default(encoder_iface, &encoder_cfg, fuzz_uint32_t_2);
    encoder_cfg.g_w = 1920;
    encoder_cfg.g_h = 1080;
    encoder_cfg.g_timebase.num = 1;
    encoder_cfg.g_timebase.den = 30;
    vpx_codec_enc_init_ver(&encoder_ctx, encoder_iface, &encoder_cfg, fuzz_int64_t_3, VPX_ENCODER_ABI_VERSION);

    // Set frame buffer functions
    vpx_codec_set_frame_buffer_functions(&decoder_ctx, nullptr, nullptr, nullptr);

    // Register frame encoding callback
    vpx_codec_register_put_frame_cb(&encoder_ctx, nullptr, nullptr);

    // Set frame slice callback
    vpx_codec_register_put_slice_cb(&encoder_ctx, nullptr, nullptr);

    // Decode input data
    uint8_t *input_data = new uint8_t[size];
    fread(input_data, 1, size, in_file);
    vpx_codec_decode(&decoder_ctx, input_data, size, nullptr, fuzz_int64_t_4);

    // Get stream info
    vpx_codec_get_stream_info(&decoder_ctx, &stream_info);

    // Encode frames
    vpx_image_t img;
    vpx_codec_pts_t pts = 0;
    unsigned long duration = 1;
    vpx_codec_encode(&encoder_ctx, &img, fuzz_int64_t_5, fuzz_uint64_t_6, fuzz_int64_t_7, fuzz_uint64_t_8);

    // Write output data
    const vpx_codec_cx_pkt_t *pkt;
    while ((pkt = vpx_codec_get_cx_data(&encoder_ctx, nullptr)) != nullptr) {
        fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
    }

    // Cleanup
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    delete[] input_data;
    vpx_img_free(&img);
    vpx_codec_destroy(&decoder_ctx);
    vpx_codec_destroy(&encoder_ctx);

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}