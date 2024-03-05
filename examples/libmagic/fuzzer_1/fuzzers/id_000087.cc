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
//<ID> 3036
//<Prompt> ["vpx_img_alloc","vpx_codec_enc_config_default","vpx_codec_err_to_string","vpx_codec_vp9_cx","vpx_codec_get_cx_data","vpx_codec_enc_init_ver","vpx_codec_decode","vpx_codec_dec_init_ver","vpx_codec_vp9_dx","vpx_img_free","vpx_codec_destroy"]
/*<Combination>: [vpx_image_t *vpx_img_alloc(vpx_image_t * img, vpx_img_fmt_t fmt, unsigned int d_w, unsigned int d_h, unsigned int align),
    vpx_codec_err_t vpx_codec_enc_config_default(vpx_codec_iface_t * iface, vpx_codec_enc_cfg_t * cfg, unsigned int usage),
    const char *vpx_codec_err_to_string(vpx_codec_err_t err),
    vpx_codec_iface_t *vpx_codec_vp9_cx(),
    const vpx_codec_cx_pkt_t *vpx_codec_get_cx_data(vpx_codec_ctx_t * ctx, vpx_codec_iter_t * iter),
    vpx_codec_err_t vpx_codec_enc_init_ver(vpx_codec_ctx_t * ctx, vpx_codec_iface_t * iface, const vpx_codec_enc_cfg_t * cfg, vpx_codec_flags_t flags, int ver),
    vpx_codec_err_t vpx_codec_decode(vpx_codec_ctx_t * ctx, const uint8_t * data, unsigned int data_sz, void * user_priv, long deadline),
    vpx_codec_err_t vpx_codec_dec_init_ver(vpx_codec_ctx_t * ctx, vpx_codec_iface_t * iface, const vpx_codec_dec_cfg_t * cfg, vpx_codec_flags_t flags, int ver),
    vpx_codec_iface_t *vpx_codec_vp9_dx(),
    void vpx_img_free(vpx_image_t * img),
    vpx_codec_err_t vpx_codec_destroy(vpx_codec_ctx_t * ctx)
*/
//<score> 8, nr_unique_branch: 0
//<Quality> {"density":8,"unique_branches":{},"library_calls":["vpx_img_alloc","vpx_codec_enc_config_default","vpx_codec_vp9_cx","vpx_codec_err_to_string","vpx_codec_enc_init_ver","vpx_codec_vp9_cx","vpx_codec_dec_init_ver","vpx_codec_vp9_dx","vpx_codec_encode","vpx_codec_decode","vpx_codec_get_frame","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy"],"critical_calls":["vpx_img_alloc","vpx_codec_enc_config_default","vpx_codec_vp9_cx","vpx_codec_err_to_string","vpx_codec_enc_init_ver","vpx_codec_vp9_cx","vpx_codec_dec_init_ver","vpx_codec_vp9_dx","vpx_codec_encode","vpx_codec_decode","vpx_codec_get_frame","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy"],"visited":0}
/**/

extern "C" int LLVMFuzzerTestOneInput_87(const uint8_t* f_data, size_t f_size) {
	if(f_size<60) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_1, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_2, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_3, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_4, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_5, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_6, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_7, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_8, fdp);
	//fuzzer shim end}




    // Step 1: Create a vpx_image_t variable
    vpx_image_t img;
    vpx_img_alloc(&img, VPX_IMG_FMT_I420, 640, 480, 32);
    
    // Step 2: Create a vpx_codec_enc_cfg_t variable and initialize it with default values
    vpx_codec_enc_cfg_t enc_cfg;
    vpx_codec_enc_config_default(vpx_codec_vp9_cx(), &enc_cfg, fuzz_uint32_t_1);
    
    // Step 3: Convert the error code to a string
    const char* err_str = vpx_codec_err_to_string(VPX_CODEC_OK);
    
    // Step 4: Initialize the encoder context
    vpx_codec_ctx_t enc_ctx;
    vpx_codec_enc_init_ver(&enc_ctx, vpx_codec_vp9_cx(), &enc_cfg, fuzz_int64_t_2, VPX_ENCODER_ABI_VERSION);
    
    // Step 5: Initialize the decoder context
    vpx_codec_ctx_t dec_ctx;
    vpx_codec_dec_cfg_t dec_cfg;
    vpx_codec_dec_init_ver(&dec_ctx, vpx_codec_vp9_dx(), &dec_cfg, fuzz_int64_t_3, VPX_DECODER_ABI_VERSION);
    
    // Step 6: Use the input data to encode a frame
    vpx_codec_encode(&enc_ctx, &img, fuzz_int64_t_4, fuzz_uint64_t_5, fuzz_int64_t_6, fuzz_uint64_t_7);
    
    // Step 7: Decode the encoded frame
    vpx_codec_decode(&dec_ctx, data, size, nullptr, fuzz_int64_t_8);
    
    // Step 8: Get the decoded frame from the decoder context
    const vpx_image_t* dec_img = vpx_codec_get_frame(&dec_ctx, nullptr);
    
    // Step 9: Free the allocated image
    vpx_img_free(&img);
    
    // Step 10: Destroy the encoder context
    vpx_codec_destroy(&enc_ctx);
    
    // Step 11: Destroy the decoder context
    vpx_codec_destroy(&dec_ctx);
    
    return 0;
}