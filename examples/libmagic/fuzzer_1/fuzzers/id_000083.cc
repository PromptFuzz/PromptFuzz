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
//<ID> 2998
//<Prompt> ["vpx_codec_vp9_dx","vpx_codec_dec_init_ver","vpx_codec_peek_stream_info","vpx_write_tpl_gop_stats","vpx_codec_register_put_frame_cb","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_peek_stream_info","vpx_codec_decode","vpx_codec_get_preview_frame","vpx_img_free","vpx_codec_destroy"]
/*<Combination>: [vpx_codec_iface_t *vpx_codec_vp9_dx(),
    vpx_codec_err_t vpx_codec_dec_init_ver(vpx_codec_ctx_t * ctx, vpx_codec_iface_t * iface, const vpx_codec_dec_cfg_t * cfg, vpx_codec_flags_t flags, int ver),
    vpx_codec_err_t vpx_codec_peek_stream_info(vpx_codec_iface_t * iface, const uint8_t * data, unsigned int data_sz, vpx_codec_stream_info_t * si),
    vpx_codec_err_t vpx_write_tpl_gop_stats(FILE * tpl_file, const VpxTplGopStats * tpl_gop_stats),
    vpx_codec_err_t vpx_codec_register_put_frame_cb(vpx_codec_ctx_t * ctx, vpx_codec_put_frame_cb_fn_t cb, void * user_priv),
    vpx_codec_err_t vpx_codec_enc_config_default(vpx_codec_iface_t * iface, vpx_codec_enc_cfg_t * cfg, unsigned int usage),
    vpx_codec_err_t vpx_codec_enc_init_ver(vpx_codec_ctx_t * ctx, vpx_codec_iface_t * iface, const vpx_codec_enc_cfg_t * cfg, vpx_codec_flags_t flags, int ver),
    vpx_codec_err_t vpx_codec_peek_stream_info(vpx_codec_iface_t * iface, const uint8_t * data, unsigned int data_sz, vpx_codec_stream_info_t * si),
    vpx_codec_err_t vpx_codec_decode(vpx_codec_ctx_t * ctx, const uint8_t * data, unsigned int data_sz, void * user_priv, long deadline),
    const vpx_image_t *vpx_codec_get_preview_frame(vpx_codec_ctx_t * ctx),
    void vpx_img_free(vpx_image_t * img),
    vpx_codec_err_t vpx_codec_destroy(vpx_codec_ctx_t * ctx)
*/
//<score> 6, nr_unique_branch: 0
//<Quality> {"density":6,"unique_branches":{},"library_calls":["vpx_codec_vp9_dx","vpx_codec_peek_stream_info","vpx_codec_dec_init_ver","vpx_codec_decode","vpx_codec_get_preview_frame","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_register_put_frame_cb","vpx_codec_destroy","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp9_dx","vpx_codec_peek_stream_info","vpx_codec_dec_init_ver","vpx_codec_decode","vpx_codec_get_preview_frame","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_register_put_frame_cb","vpx_codec_destroy","vpx_codec_destroy"],"visited":0}
/*Here is the C++ program that follows the instructions provided:

*/


extern "C" int LLVMFuzzerTestOneInput_83(const uint8_t* f_data, size_t f_size) {
	if(f_size<28) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int64_t, fuzz_int64_t_1, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_4, fdp);
	//fuzzer shim end}




    // Initialize libvpx decoder
    vpx_codec_ctx_t decoder;
    vpx_codec_dec_cfg_t dec_cfg;
    vpx_codec_iface_t *decoder_iface = vpx_codec_vp9_dx();
    vpx_codec_peek_stream_info(decoder_iface, data, size, nullptr);
    vpx_codec_dec_init_ver(&decoder, decoder_iface, &dec_cfg, fuzz_int64_t_1, VPX_DECODER_ABI_VERSION);

    // Decode the input data
    vpx_codec_decode(&decoder, data, size, nullptr, fuzz_int64_t_2);

    // Get the decoded preview frame
    const vpx_image_t *preview_frame = vpx_codec_get_preview_frame(&decoder);

    // Initialize libvpx encoder
    vpx_codec_ctx_t encoder;
    vpx_codec_enc_cfg_t enc_cfg;
    vpx_codec_iface_t *encoder_iface = vpx_codec_vp9_cx();
    vpx_codec_enc_config_default(encoder_iface, &enc_cfg, fuzz_uint32_t_3);
    vpx_codec_enc_init_ver(&encoder, encoder_iface, &enc_cfg, fuzz_int64_t_4, VPX_ENCODER_ABI_VERSION);

    // Register put frame callback function
    vpx_codec_register_put_frame_cb(&encoder, nullptr, nullptr);

    // Destroy the encoder and decoder
    vpx_codec_destroy(&encoder);
    vpx_codec_destroy(&decoder);
    
    return 0;
}