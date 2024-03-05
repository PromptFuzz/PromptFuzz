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
//<ID> 2755
//<Prompt> ["vpx_codec_enc_config_set","vpx_codec_vp9_dx","vpx_codec_dec_init_ver","vpx_codec_register_put_slice_cb","vpx_codec_peek_stream_info","vpx_codec_register_put_frame_cb","vpx_img_wrap","vpx_codec_decode","vpx_codec_set_frame_buffer_functions","vpx_codec_get_preview_frame","vpx_img_free","vpx_codec_destroy"]
/*<Combination>: [vpx_codec_err_t vpx_codec_enc_config_set(vpx_codec_ctx_t * ctx, const vpx_codec_enc_cfg_t * cfg),
    vpx_codec_iface_t *vpx_codec_vp9_dx(),
    vpx_codec_err_t vpx_codec_dec_init_ver(vpx_codec_ctx_t * ctx, vpx_codec_iface_t * iface, const vpx_codec_dec_cfg_t * cfg, vpx_codec_flags_t flags, int ver),
    vpx_codec_err_t vpx_codec_register_put_slice_cb(vpx_codec_ctx_t * ctx, vpx_codec_put_slice_cb_fn_t cb, void * user_priv),
    vpx_codec_err_t vpx_codec_peek_stream_info(vpx_codec_iface_t * iface, const uint8_t * data, unsigned int data_sz, vpx_codec_stream_info_t * si),
    vpx_codec_err_t vpx_codec_register_put_frame_cb(vpx_codec_ctx_t * ctx, vpx_codec_put_frame_cb_fn_t cb, void * user_priv),
    vpx_image_t *vpx_img_wrap(vpx_image_t * img, vpx_img_fmt_t fmt, unsigned int d_w, unsigned int d_h, unsigned int stride_align, unsigned char * img_data),
    vpx_codec_err_t vpx_codec_decode(vpx_codec_ctx_t * ctx, const uint8_t * data, unsigned int data_sz, void * user_priv, long deadline),
    vpx_codec_err_t vpx_codec_set_frame_buffer_functions(vpx_codec_ctx_t * ctx, vpx_get_frame_buffer_cb_fn_t cb_get, vpx_release_frame_buffer_cb_fn_t cb_release, void * cb_priv),
    const vpx_image_t *vpx_codec_get_preview_frame(vpx_codec_ctx_t * ctx),
    void vpx_img_free(vpx_image_t * img),
    vpx_codec_err_t vpx_codec_destroy(vpx_codec_ctx_t * ctx)
*/
//<score> 7.5, nr_unique_branch: 0
//<Quality> {"density":15,"unique_branches":{},"library_calls":["vpx_codec_vp9_cx","vpx_codec_vp9_dx","vpx_codec_enc_config_default","vpx_codec_enc_config_set","vpx_codec_dec_init_ver","vpx_codec_register_put_slice_cb","vpx_codec_peek_stream_info","vpx_codec_register_put_frame_cb","vpx_img_wrap","vpx_codec_decode","vpx_codec_set_frame_buffer_functions","vpx_codec_get_preview_frame","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp9_cx","vpx_codec_vp9_dx","vpx_codec_enc_config_default","vpx_codec_enc_config_set","vpx_codec_dec_init_ver","vpx_codec_register_put_slice_cb","vpx_codec_peek_stream_info","vpx_codec_register_put_frame_cb","vpx_img_wrap","vpx_codec_decode","vpx_codec_set_frame_buffer_functions","vpx_codec_get_preview_frame","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy"],"visited":1}
/*Here is the complete implementation of the C++ program that uses the libvpx library APIs:

*/


extern "C" int LLVMFuzzerTestOneInput_71(const uint8_t* f_data, size_t f_size) {
	if(f_size<24) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_1, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_4, fdp);
	//fuzzer shim end}




    // Step 1: Create vpx_codec_ctx_t
    vpx_codec_ctx_t encoder_ctx;
    vpx_codec_ctx_t decoder_ctx;
    vpx_codec_iface_t *encoder_iface = vpx_codec_vp9_cx();
    vpx_codec_iface_t *decoder_iface = vpx_codec_vp9_dx();
    
    // Step 2: Initialize encoder configuration
    vpx_codec_enc_cfg_t encoder_cfg;
    memset(&encoder_cfg, 0, sizeof(encoder_cfg));
    vpx_codec_enc_config_default(encoder_iface, &encoder_cfg, fuzz_uint32_t_1);
    vpx_codec_enc_config_set(&encoder_ctx, &encoder_cfg);
    
    // Step 3: Initialize decoder configuration
    vpx_codec_dec_cfg_t decoder_cfg;
    memset(&decoder_cfg, 0, sizeof(decoder_cfg));
    decoder_cfg.w = 640;
    decoder_cfg.h = 480;
    vpx_codec_dec_init_ver(&decoder_ctx, decoder_iface, &decoder_cfg, fuzz_int64_t_2, VPX_DECODER_ABI_VERSION);
    
    // Step 4: Register put_slice callback
    vpx_codec_register_put_slice_cb(&decoder_ctx, nullptr, nullptr);
    
    // Step 5: Peek stream information
    vpx_codec_stream_info_t stream_info;
    vpx_codec_peek_stream_info(encoder_iface, data, size, &stream_info);
    
    // Step 6: Register put_frame callback
    vpx_codec_register_put_frame_cb(&encoder_ctx, nullptr, nullptr);
    
    // Step 7: Wrap input data as vpx_image_t
    vpx_image_t input_image;
    memset(&input_image, 0, sizeof(input_image));
    vpx_img_wrap(&input_image, VPX_IMG_FMT_I420, decoder_cfg.w, decoder_cfg.h, fuzz_uint32_t_3, const_cast<uint8_t *>(data));
    
    // Step 8: Decode input data
    vpx_codec_decode(&decoder_ctx, data, size, nullptr, fuzz_int64_t_4);
    
    // Step 9: Set frame buffer functions
    vpx_codec_set_frame_buffer_functions(&decoder_ctx, nullptr, nullptr, nullptr);
    
    // Step 10: Get preview frame
    vpx_codec_get_preview_frame(&decoder_ctx);
    
    // Step 11: Free the input image
    vpx_img_free(&input_image);
    
    // Step 12: Destroy codec contexts
    vpx_codec_destroy(&encoder_ctx);
    vpx_codec_destroy(&decoder_ctx);
    
    return 0;
}