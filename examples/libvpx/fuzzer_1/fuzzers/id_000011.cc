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
//<ID> 516
//<Prompt> ["vpx_codec_enc_config_set","vpx_img_wrap","vpx_codec_register_put_slice_cb","vpx_codec_control_","vpx_codec_get_preview_frame","vpx_codec_error","vpx_free_tpl_gop_stats","vpx_codec_get_cx_data","vpx_img_free"]
/*<Combination>: [vpx_codec_err_t vpx_codec_enc_config_set(vpx_codec_ctx_t * ctx, const vpx_codec_enc_cfg_t * cfg),
    vpx_image_t *vpx_img_wrap(vpx_image_t * img, vpx_img_fmt_t fmt, unsigned int d_w, unsigned int d_h, unsigned int stride_align, unsigned char * img_data),
    vpx_codec_err_t vpx_codec_register_put_slice_cb(vpx_codec_ctx_t * ctx, vpx_codec_put_slice_cb_fn_t cb, void * user_priv),
    vpx_codec_err_t vpx_codec_control_(vpx_codec_ctx_t * ctx, int ctrl_id),
    const vpx_image_t *vpx_codec_get_preview_frame(vpx_codec_ctx_t * ctx),
    const char *vpx_codec_error(const vpx_codec_ctx_t * ctx),
    void vpx_free_tpl_gop_stats(VpxTplGopStats * tpl_gop_stats),
    const vpx_codec_cx_pkt_t *vpx_codec_get_cx_data(vpx_codec_ctx_t * ctx, vpx_codec_iter_t * iter),
    void vpx_img_free(vpx_image_t * img)
*/
//<score> 5.5, nr_unique_branch: 1
//<Quality> {"density":11,"unique_branches":{"vpx_codec_register_put_slice_cb":[[161,12,161,23,0,0,4,0]]},"library_calls":["vpx_codec_enc_config_default","vpx_codec_vp8_cx","vpx_codec_enc_config_set","vpx_img_wrap","vpx_codec_register_put_slice_cb","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_get_preview_frame","vpx_codec_error","vpx_img_free","vpx_codec_destroy"],"critical_calls":["vpx_codec_enc_config_default","vpx_codec_vp8_cx","vpx_codec_enc_config_set","vpx_img_wrap","vpx_codec_register_put_slice_cb","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_get_preview_frame","vpx_codec_error","vpx_img_free","vpx_codec_destroy"],"visited":3}
/*Here is an example implementation of the LLVMFuzzerTestOneInput_11 function that uses the libvpx APIs to achieve the event:

*/


extern "C" int LLVMFuzzerTestOneInput_11(const uint8_t* f_data, size_t f_size) {
	if(f_size<40) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_3, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_4, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_5, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_6, fdp);
	//fuzzer shim end}




    // Create the vpx_codec_ctx_t structure
    vpx_codec_ctx_t codec;
    
    // Initialize the encoder configuration
    vpx_codec_enc_cfg_t enc_cfg;
    vpx_codec_enc_config_default(vpx_codec_vp8_cx(), &enc_cfg, fuzz_uint32_t_1);
    
    // Set the encoder configuration
    vpx_codec_enc_config_set(&codec, &enc_cfg);

    // Wrap the input data as a vpx_image_t
    vpx_image_t image;
    vpx_img_wrap(&image, VPX_IMG_FMT_I420, enc_cfg.g_w, enc_cfg.g_h, fuzz_uint32_t_2, const_cast<uint8_t*>(data));

    // Register the put_slice callback function
    vpx_codec_register_put_slice_cb(&codec, [](void* , const vpx_image_t *, const vpx_image_rect_t*, const vpx_image_rect_t*){}, nullptr);
    
    // Encode the frame
    vpx_codec_cx_pkt_t pkt;
    vpx_codec_encode(&codec, &image, fuzz_int64_t_3, fuzz_uint64_t_4, fuzz_int64_t_5, fuzz_uint64_t_6);
    
    // Get the compressed data
    const vpx_codec_cx_pkt_t *cx_pkt = nullptr;
    vpx_codec_iter_t iter = nullptr;
    while ((cx_pkt = vpx_codec_get_cx_data(&codec, &iter)) != nullptr) {
        if (cx_pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
            pkt = *cx_pkt;
            break;
        }
    }
    
    // Get the preview frame
    const vpx_image_t *preview_frame = vpx_codec_get_preview_frame(&codec);
    
    // Get the error message
    const char *error_msg = vpx_codec_error(&codec);
    
    // Free the vpx_image_t structure
    vpx_img_free(&image);
    
    // Destroy the codec
    vpx_codec_destroy(&codec);
    
    // Process the output data
    
    return 0;
}