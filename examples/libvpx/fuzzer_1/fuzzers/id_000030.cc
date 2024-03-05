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
//<ID> 1324
//<Prompt> ["vpx_codec_enc_config_default","vpx_codec_vp8_cx","vpx_codec_enc_init_ver","vpx_codec_get_caps","vpx_codec_register_put_frame_cb","vpx_codec_error_detail","vpx_codec_peek_stream_info","vpx_codec_enc_config_set","vpx_codec_set_cx_data_buf","vpx_codec_get_stream_info","vpx_codec_encode","vpx_codec_decode","vpx_codec_destroy"]
/*<Combination>: [vpx_codec_err_t vpx_codec_enc_config_default(vpx_codec_iface_t * iface, vpx_codec_enc_cfg_t * cfg, unsigned int usage),
    vpx_codec_iface_t *vpx_codec_vp8_cx(),
    vpx_codec_err_t vpx_codec_enc_init_ver(vpx_codec_ctx_t * ctx, vpx_codec_iface_t * iface, const vpx_codec_enc_cfg_t * cfg, vpx_codec_flags_t flags, int ver),
    vpx_codec_caps_t vpx_codec_get_caps(vpx_codec_iface_t * iface),
    vpx_codec_err_t vpx_codec_register_put_frame_cb(vpx_codec_ctx_t * ctx, vpx_codec_put_frame_cb_fn_t cb, void * user_priv),
    const char *vpx_codec_error_detail(const vpx_codec_ctx_t * ctx),
    vpx_codec_err_t vpx_codec_peek_stream_info(vpx_codec_iface_t * iface, const uint8_t * data, unsigned int data_sz, vpx_codec_stream_info_t * si),
    vpx_codec_err_t vpx_codec_enc_config_set(vpx_codec_ctx_t * ctx, const vpx_codec_enc_cfg_t * cfg),
    vpx_codec_err_t vpx_codec_set_cx_data_buf(vpx_codec_ctx_t * ctx, const vpx_fixed_buf_t * buf, unsigned int pad_before, unsigned int pad_after),
    vpx_codec_err_t vpx_codec_get_stream_info(vpx_codec_ctx_t * ctx, vpx_codec_stream_info_t * si),
    vpx_codec_err_t vpx_codec_encode(vpx_codec_ctx_t * ctx, const vpx_image_t * img, vpx_codec_pts_t pts, unsigned long duration, vpx_enc_frame_flags_t flags, unsigned long deadline),
    vpx_codec_err_t vpx_codec_decode(vpx_codec_ctx_t * ctx, const uint8_t * data, unsigned int data_sz, void * user_priv, long deadline),
    vpx_codec_err_t vpx_codec_destroy(vpx_codec_ctx_t * ctx)
*/
//<score> 7.5, nr_unique_branch: 1
//<Quality> {"density":15,"unique_branches":{"vpx_codec_enc_init_ver":[[37,30,37,34,0,0,4,0]]},"library_calls":["vpx_codec_vp8_cx","vpx_codec_vp8_dx","vpx_codec_enc_config_default","vpx_codec_enc_config_set","vpx_codec_enc_init_ver","vpx_codec_dec_init_ver","vpx_codec_get_caps","vpx_codec_register_put_frame_cb","vpx_codec_peek_stream_info","vpx_codec_set_cx_data_buf","vpx_codec_get_stream_info","vpx_codec_encode","vpx_codec_decode","vpx_codec_destroy","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp8_cx","vpx_codec_vp8_dx","vpx_codec_enc_config_default","vpx_codec_enc_config_set","vpx_codec_enc_init_ver","vpx_codec_dec_init_ver","vpx_codec_get_caps","vpx_codec_register_put_frame_cb","vpx_codec_peek_stream_info","vpx_codec_set_cx_data_buf","vpx_codec_get_stream_info","vpx_codec_encode","vpx_codec_decode","vpx_codec_destroy","vpx_codec_destroy"],"visited":3}
/*Here is an example program that demonstrates the usage of the libvpx library APIs:

*/


extern "C" int LLVMFuzzerTestOneInput_30(const uint8_t* f_data, size_t f_size) {
	if(f_size<68) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_1, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_2, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_3, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_4, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_5, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_6, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_7, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_8, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_9, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_10, fdp);
	//fuzzer shim end}




    vpx_codec_ctx_t encoder;
    vpx_codec_enc_cfg_t encoder_cfg;
    vpx_codec_ctx_t decoder;

    vpx_codec_iface_t *encoder_iface = vpx_codec_vp8_cx();
    vpx_codec_iface_t *decoder_iface = vpx_codec_vp8_dx();

    // Initialize encoder configuration with default values
    vpx_codec_enc_config_default(encoder_iface, &encoder_cfg, fuzz_uint32_t_1);

    // Set encoder configuration
    vpx_codec_enc_config_set(&encoder, &encoder_cfg);

    // Initialize encoder
    vpx_codec_enc_init_ver(&encoder, encoder_iface, nullptr, fuzz_int64_t_2, VPX_ENCODER_ABI_VERSION);

    // Initialize decoder
    vpx_codec_dec_init_ver(&decoder, decoder_iface, nullptr, fuzz_int64_t_3, VPX_DECODER_ABI_VERSION);

    // Get encoder's capabilities
    vpx_codec_caps_t encoder_caps = vpx_codec_get_caps(encoder_iface);

    // Register frame callback function
    vpx_codec_register_put_frame_cb(&encoder, nullptr, nullptr);

    // Peek stream info
    vpx_codec_stream_info_t stream_info;
    vpx_codec_peek_stream_info(encoder_iface, data, size, &stream_info);

    // Set coding data buffer
    vpx_fixed_buf_t data_buf;
    vpx_codec_set_cx_data_buf(&encoder, &data_buf, fuzz_uint32_t_4, fuzz_uint32_t_5);

    // Get stream info from encoder
    vpx_codec_stream_info_t encoder_stream_info;
    vpx_codec_get_stream_info(&encoder, &encoder_stream_info);

    // Encode image
    vpx_image_t image;
    vpx_codec_encode(&encoder, &image, fuzz_int64_t_6, fuzz_uint64_t_7, fuzz_int64_t_8, fuzz_uint64_t_9);

    // Decode stream
    vpx_codec_decode(&decoder, data, size, nullptr, fuzz_int64_t_10);

    // Destroy encoder and decoder
    vpx_codec_destroy(&encoder);
    vpx_codec_destroy(&decoder);

    return 0;
}