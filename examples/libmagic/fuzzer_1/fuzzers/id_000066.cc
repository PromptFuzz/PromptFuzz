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
//<ID> 2609
//<Prompt> ["vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_multi_ver","vpx_codec_register_put_frame_cb","vpx_img_alloc","vpx_codec_encode","vpx_codec_get_global_headers","vpx_codec_register_put_frame_cb","vpx_codec_encode","vpx_codec_decode","vpx_codec_get_stream_info","vpx_codec_decode","vpx_img_free","vpx_codec_destroy"]
/*<Combination>: [vpx_codec_iface_t *vpx_codec_vp9_cx(),
    vpx_codec_err_t vpx_codec_enc_config_default(vpx_codec_iface_t * iface, vpx_codec_enc_cfg_t * cfg, unsigned int usage),
    vpx_codec_err_t vpx_codec_enc_init_multi_ver(vpx_codec_ctx_t * ctx, vpx_codec_iface_t * iface, vpx_codec_enc_cfg_t * cfg, int num_enc, vpx_codec_flags_t flags, vpx_rational_t * dsf, int ver),
    vpx_codec_err_t vpx_codec_register_put_frame_cb(vpx_codec_ctx_t * ctx, vpx_codec_put_frame_cb_fn_t cb, void * user_priv),
    vpx_image_t *vpx_img_alloc(vpx_image_t * img, vpx_img_fmt_t fmt, unsigned int d_w, unsigned int d_h, unsigned int align),
    vpx_codec_err_t vpx_codec_encode(vpx_codec_ctx_t * ctx, const vpx_image_t * img, vpx_codec_pts_t pts, unsigned long duration, vpx_enc_frame_flags_t flags, unsigned long deadline),
    vpx_fixed_buf_t *vpx_codec_get_global_headers(vpx_codec_ctx_t * ctx),
    vpx_codec_err_t vpx_codec_register_put_frame_cb(vpx_codec_ctx_t * ctx, vpx_codec_put_frame_cb_fn_t cb, void * user_priv),
    vpx_codec_err_t vpx_codec_encode(vpx_codec_ctx_t * ctx, const vpx_image_t * img, vpx_codec_pts_t pts, unsigned long duration, vpx_enc_frame_flags_t flags, unsigned long deadline),
    vpx_codec_err_t vpx_codec_decode(vpx_codec_ctx_t * ctx, const uint8_t * data, unsigned int data_sz, void * user_priv, long deadline),
    vpx_codec_err_t vpx_codec_get_stream_info(vpx_codec_ctx_t * ctx, vpx_codec_stream_info_t * si),
    vpx_codec_err_t vpx_codec_decode(vpx_codec_ctx_t * ctx, const uint8_t * data, unsigned int data_sz, void * user_priv, long deadline),
    void vpx_img_free(vpx_image_t * img),
    vpx_codec_err_t vpx_codec_destroy(vpx_codec_ctx_t * ctx)
*/
//<score> 9, nr_unique_branch: 0
//<Quality> {"density":9,"unique_branches":{},"library_calls":["vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_multi_ver","vpx_codec_register_put_frame_cb","vpx_img_alloc","vpx_codec_encode","vpx_codec_get_global_headers","vpx_codec_vp9_dx","vpx_codec_dec_init_ver","vpx_codec_decode","vpx_codec_get_stream_info","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_multi_ver","vpx_codec_register_put_frame_cb","vpx_img_alloc","vpx_codec_encode","vpx_codec_get_global_headers","vpx_codec_vp9_dx","vpx_codec_dec_init_ver","vpx_codec_decode","vpx_codec_get_stream_info","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy"],"visited":0}
/**/

extern "C" int LLVMFuzzerTestOneInput_66(const uint8_t* f_data, size_t f_size) {
	if(f_size<68) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_5, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_6, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_7, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_8, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_9, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_10, fdp);
	//fuzzer shim end}




    // Create a VP9 encoder
    vpx_codec_ctx_t enc_ctx;
    vpx_codec_iface_t *iface = vpx_codec_vp9_cx();
    vpx_codec_enc_cfg_t enc_cfg;
    vpx_codec_enc_config_default(iface, &enc_cfg, fuzz_uint32_t_1);
    vpx_codec_enc_init_multi_ver(&enc_ctx, iface, &enc_cfg, fuzz_int32_t_2, fuzz_int64_t_3, NULL, fuzz_int32_t_4);
    
    // Register a frame callback
    vpx_codec_register_put_frame_cb(&enc_ctx, [](void *user_priv, const vpx_image_t *img) -> void {
        // Handle the frame callback
    }, NULL);
    
    // Allocate an image
    vpx_image_t img;
    vpx_img_alloc(&img, VPX_IMG_FMT_I420, enc_cfg.g_w, enc_cfg.g_h, 32);
    
    // Encode the image
    vpx_codec_encode(&enc_ctx, &img, fuzz_int64_t_5, fuzz_uint64_t_6, fuzz_int64_t_7, fuzz_uint64_t_8);
    
    // Get the global headers
    vpx_fixed_buf_t *global_headers = vpx_codec_get_global_headers(&enc_ctx);
    
    // Create a VP9 decoder
    vpx_codec_ctx_t dec_ctx;
    vpx_codec_iface_t *dec_iface = vpx_codec_vp9_dx();
    vpx_codec_dec_cfg_t dec_cfg;
    vpx_codec_dec_init_ver(&dec_ctx, dec_iface, &dec_cfg, fuzz_int64_t_9, VPX_DECODER_ABI_VERSION);
    
    // Decode the data
    vpx_codec_decode(&dec_ctx, data, size, NULL, fuzz_int64_t_10);
    
    // Get the stream info
    vpx_codec_stream_info_t si;
    vpx_codec_get_stream_info(&dec_ctx, &si);
    
    // Cleanup
    vpx_img_free(&img);
    vpx_codec_destroy(&enc_ctx);
    vpx_codec_destroy(&dec_ctx);
    
    return 0;
}