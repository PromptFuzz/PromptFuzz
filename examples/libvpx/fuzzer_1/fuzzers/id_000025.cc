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
//<ID> 1159
//<Prompt> ["vpx_codec_vp8_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_set_frame_buffer_functions","vpx_codec_peek_stream_info","vpx_codec_enc_config_set","vpx_codec_set_cx_data_buf","vpx_codec_get_stream_info","vpx_codec_set_frame_buffer_functions","vpx_codec_set_cx_data_buf","vpx_codec_peek_stream_info","vpx_codec_destroy","vpx_img_free"]
/*<Combination>: [vpx_codec_iface_t *vpx_codec_vp8_cx(),
    vpx_codec_err_t vpx_codec_enc_config_default(vpx_codec_iface_t * iface, vpx_codec_enc_cfg_t * cfg, unsigned int usage),
    vpx_codec_err_t vpx_codec_enc_init_ver(vpx_codec_ctx_t * ctx, vpx_codec_iface_t * iface, const vpx_codec_enc_cfg_t * cfg, vpx_codec_flags_t flags, int ver),
    vpx_codec_err_t vpx_codec_set_frame_buffer_functions(vpx_codec_ctx_t * ctx, vpx_get_frame_buffer_cb_fn_t cb_get, vpx_release_frame_buffer_cb_fn_t cb_release, void * cb_priv),
    vpx_codec_err_t vpx_codec_peek_stream_info(vpx_codec_iface_t * iface, const uint8_t * data, unsigned int data_sz, vpx_codec_stream_info_t * si),
    vpx_codec_err_t vpx_codec_enc_config_set(vpx_codec_ctx_t * ctx, const vpx_codec_enc_cfg_t * cfg),
    vpx_codec_err_t vpx_codec_set_cx_data_buf(vpx_codec_ctx_t * ctx, const vpx_fixed_buf_t * buf, unsigned int pad_before, unsigned int pad_after),
    vpx_codec_err_t vpx_codec_get_stream_info(vpx_codec_ctx_t * ctx, vpx_codec_stream_info_t * si),
    vpx_codec_err_t vpx_codec_set_frame_buffer_functions(vpx_codec_ctx_t * ctx, vpx_get_frame_buffer_cb_fn_t cb_get, vpx_release_frame_buffer_cb_fn_t cb_release, void * cb_priv),
    vpx_codec_err_t vpx_codec_set_cx_data_buf(vpx_codec_ctx_t * ctx, const vpx_fixed_buf_t * buf, unsigned int pad_before, unsigned int pad_after),
    vpx_codec_err_t vpx_codec_peek_stream_info(vpx_codec_iface_t * iface, const uint8_t * data, unsigned int data_sz, vpx_codec_stream_info_t * si),
    vpx_codec_err_t vpx_codec_destroy(vpx_codec_ctx_t * ctx),
    void vpx_img_free(vpx_image_t * img)
*/
//<score> 9, nr_unique_branch: 3
//<Quality> {"density":9,"unique_branches":{"vpx_codec_set_frame_buffer_functions":[[181,14,181,25,0,0,4,1],[181,29,181,39,0,0,4,1],[183,14,183,71,0,0,4,0]]},"library_calls":["vpx_codec_vp8_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_set_frame_buffer_functions","vpx_codec_vp8_dx","vpx_codec_peek_stream_info","vpx_codec_enc_config_set","vpx_codec_set_cx_data_buf","vpx_codec_dec_init_ver","vpx_codec_destroy","vpx_codec_get_stream_info","vpx_codec_encode","vpx_codec_decode","vpx_codec_get_frame","vpx_codec_destroy","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp8_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_set_frame_buffer_functions","vpx_codec_vp8_dx","vpx_codec_peek_stream_info","vpx_codec_enc_config_set","vpx_codec_set_cx_data_buf","vpx_codec_dec_init_ver","vpx_codec_get_stream_info","vpx_codec_encode","vpx_codec_decode","vpx_codec_get_frame","vpx_codec_destroy","vpx_codec_destroy"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_25(const uint8_t* f_data, size_t f_size) {
	if(f_size<68) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_1, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_4, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_5, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_6, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_7, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_8, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_9, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_10, fdp);
	//fuzzer shim end}




    // Create a VP8 codec context
    vpx_codec_ctx_t encoder_ctx;
    vpx_codec_iface_t *encoder_iface = vpx_codec_vp8_cx();
    vpx_codec_enc_cfg_t encoder_cfg;
    vpx_codec_enc_config_default(encoder_iface, &encoder_cfg, fuzz_uint32_t_1);
    vpx_codec_enc_init_ver(&encoder_ctx, encoder_iface, &encoder_cfg, fuzz_int64_t_2, VPX_ENCODER_ABI_VERSION);

    // Set frame buffer functions for the encoder
    vpx_get_frame_buffer_cb_fn_t cb_get = [](void *cb_priv, size_t size, vpx_codec_frame_buffer_t *fb) {
        // Allocate memory for the frame buffer
        fb->data = new uint8_t[size];
        fb->size = size;
        return 0;
    };
    vpx_release_frame_buffer_cb_fn_t cb_release = [](void *cb_priv, vpx_codec_frame_buffer_t *fb) {
        // Free the memory of the frame buffer
        delete[] fb->data;
        return 0;
    };
    vpx_codec_set_frame_buffer_functions(&encoder_ctx, cb_get, cb_release, nullptr);

    // Peek stream information
    vpx_codec_iface_t *decoder_iface = vpx_codec_vp8_dx();
    vpx_codec_stream_info_t stream_info;
    vpx_codec_peek_stream_info(decoder_iface, data, size, &stream_info);

    // Set configuration for the encoder
    vpx_codec_enc_config_set(&encoder_ctx, &encoder_cfg);

    // Set compressed data buffer for the encoder
    vpx_fixed_buf_t compressed_data_buf;
    compressed_data_buf.buf = (void *)data;
    compressed_data_buf.sz = size;
    vpx_codec_set_cx_data_buf(&encoder_ctx, &compressed_data_buf, fuzz_uint32_t_3, fuzz_uint32_t_4);

    // Create a VP8 codec context for decoding
    vpx_codec_ctx_t decoder_ctx;
    vpx_codec_err_t decoder_err = vpx_codec_dec_init_ver(&decoder_ctx, decoder_iface, nullptr, fuzz_int64_t_5, VPX_DECODER_ABI_VERSION);
    if (decoder_err != VPX_CODEC_OK) {
        // Handle initialization error
        vpx_codec_destroy(&encoder_ctx);
        return 0;
    }

    // Get stream information for the decoder
    vpx_codec_get_stream_info(&decoder_ctx, &stream_info);

    // Encode and decode the frame
    vpx_codec_encode(&encoder_ctx, nullptr, fuzz_int64_t_6, fuzz_uint64_t_7, fuzz_int64_t_8, fuzz_uint64_t_9);
    vpx_codec_decode(&decoder_ctx, data, size, nullptr, fuzz_int64_t_10);

    // Get the decoded frame
    vpx_codec_iter_t iter = nullptr;
    const vpx_image_t *decoded_frame;
    while ((decoded_frame = vpx_codec_get_frame(&decoder_ctx, &iter)) != nullptr) {
        // Process the decoded frame
    }

    // Destroy codec contexts
    vpx_codec_destroy(&encoder_ctx);
    vpx_codec_destroy(&decoder_ctx);

    return 0;
}