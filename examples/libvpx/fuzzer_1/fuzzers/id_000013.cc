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
//<ID> 699
//<Prompt> ["vpx_codec_enc_config_default","vpx_codec_vp8_cx","vpx_codec_vp8_cx","vpx_codec_enc_init_ver","vpx_codec_set_frame_buffer_functions","vpx_codec_set_frame_buffer_functions","vpx_codec_peek_stream_info","vpx_codec_enc_config_set","vpx_codec_set_cx_data_buf","vpx_codec_get_stream_info","vpx_codec_encode","vpx_codec_decode","vpx_codec_destroy"]
/*<Combination>: [vpx_codec_err_t vpx_codec_enc_config_default(vpx_codec_iface_t * iface, vpx_codec_enc_cfg_t * cfg, unsigned int usage),
    vpx_codec_iface_t *vpx_codec_vp8_cx(),
    vpx_codec_iface_t *vpx_codec_vp8_cx(),
    vpx_codec_err_t vpx_codec_enc_init_ver(vpx_codec_ctx_t * ctx, vpx_codec_iface_t * iface, const vpx_codec_enc_cfg_t * cfg, vpx_codec_flags_t flags, int ver),
    vpx_codec_err_t vpx_codec_set_frame_buffer_functions(vpx_codec_ctx_t * ctx, vpx_get_frame_buffer_cb_fn_t cb_get, vpx_release_frame_buffer_cb_fn_t cb_release, void * cb_priv),
    vpx_codec_err_t vpx_codec_set_frame_buffer_functions(vpx_codec_ctx_t * ctx, vpx_get_frame_buffer_cb_fn_t cb_get, vpx_release_frame_buffer_cb_fn_t cb_release, void * cb_priv),
    vpx_codec_err_t vpx_codec_peek_stream_info(vpx_codec_iface_t * iface, const uint8_t * data, unsigned int data_sz, vpx_codec_stream_info_t * si),
    vpx_codec_err_t vpx_codec_enc_config_set(vpx_codec_ctx_t * ctx, const vpx_codec_enc_cfg_t * cfg),
    vpx_codec_err_t vpx_codec_set_cx_data_buf(vpx_codec_ctx_t * ctx, const vpx_fixed_buf_t * buf, unsigned int pad_before, unsigned int pad_after),
    vpx_codec_err_t vpx_codec_get_stream_info(vpx_codec_ctx_t * ctx, vpx_codec_stream_info_t * si),
    vpx_codec_err_t vpx_codec_encode(vpx_codec_ctx_t * ctx, const vpx_image_t * img, vpx_codec_pts_t pts, unsigned long duration, vpx_enc_frame_flags_t flags, unsigned long deadline),
    vpx_codec_err_t vpx_codec_decode(vpx_codec_ctx_t * ctx, const uint8_t * data, unsigned int data_sz, void * user_priv, long deadline),
    vpx_codec_err_t vpx_codec_destroy(vpx_codec_ctx_t * ctx)
*/
//<score> 6.5, nr_unique_branch: 2
//<Quality> {"density":13,"unique_branches":{"vp8_decode":[[283,35,283,47,0,0,4,0],[283,51,283,63,0,0,4,0]]},"library_calls":["vpx_codec_vp8_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_set_frame_buffer_functions","vpx_codec_set_cx_data_buf","vpx_codec_peek_stream_info","vpx_codec_enc_config_set","vpx_codec_encode","vpx_codec_vp8_dx","vpx_codec_dec_init_ver","vpx_codec_decode","vpx_codec_destroy","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp8_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_set_frame_buffer_functions","vpx_codec_set_cx_data_buf","vpx_codec_peek_stream_info","vpx_codec_enc_config_set","vpx_codec_encode","vpx_codec_vp8_dx","vpx_codec_dec_init_ver","vpx_codec_decode","vpx_codec_destroy","vpx_codec_destroy"],"visited":3}
/**/


extern "C" int LLVMFuzzerTestOneInput_13(const uint8_t* f_data, size_t f_size) {
	if(f_size<60) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_1, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_4, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_5, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_6, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_7, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_8, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_9, fdp);
	//fuzzer shim end}




    // Create a file to store the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file == nullptr) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Create an output file to store the encoded data
    FILE *out_file = fopen("output_file", "wb");
    if (out_file == nullptr) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Get the file descriptors
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);

    // Create the encoder instance
    vpx_codec_iface_t *encoder_iface = vpx_codec_vp8_cx();
    vpx_codec_ctx_t encoder_ctx;
    vpx_codec_enc_cfg_t encoder_cfg;
    vpx_codec_enc_config_default(encoder_iface, &encoder_cfg, fuzz_uint32_t_1);
    vpx_codec_enc_init_ver(&encoder_ctx, encoder_iface, &encoder_cfg, fuzz_int64_t_2, VPX_ENCODER_ABI_VERSION);

    // Set the frame buffer functions
    vpx_codec_set_frame_buffer_functions(&encoder_ctx, nullptr, nullptr, nullptr);

    // Set the coding data buffer
    vpx_fixed_buf_t cx_data_buf;
    vpx_codec_set_cx_data_buf(&encoder_ctx, &cx_data_buf, fuzz_uint32_t_3, fuzz_uint32_t_4);

    // Get the stream info
    vpx_codec_stream_info_t stream_info;
    vpx_codec_peek_stream_info(encoder_iface, nullptr, 0, &stream_info);
    vpx_codec_enc_config_set(&encoder_ctx, &encoder_cfg);

    // Encode the input data
    vpx_image_t image;
    vpx_codec_pts_t pts = 0;
    vpx_enc_frame_flags_t flags = 0;
    unsigned long deadline = 0;
    vpx_codec_encode(&encoder_ctx, &image, fuzz_int64_t_5, fuzz_uint64_t_6, fuzz_int64_t_7, fuzz_uint64_t_8);

    // Create the decoder instance
    vpx_codec_iface_t *decoder_iface = vpx_codec_vp8_dx();
    vpx_codec_ctx_t decoder_ctx;
    vpx_codec_dec_cfg_t decoder_cfg;
    vpx_codec_dec_init_ver(&decoder_ctx, decoder_iface, &decoder_cfg, 0, VPX_DECODER_ABI_VERSION);

    // Decode the encoded data
    vpx_codec_decode(&decoder_ctx, nullptr, 0, nullptr, fuzz_int64_t_9);

    // Destroy the encoder and decoder instances
    vpx_codec_destroy(&encoder_ctx);
    vpx_codec_destroy(&decoder_ctx);

    // Close the files and release the resources
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}