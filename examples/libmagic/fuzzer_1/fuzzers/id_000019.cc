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
//<ID> 1058
//<Prompt> ["vpx_read_tpl_gop_stats","vpx_codec_register_put_slice_cb","vpx_codec_control_","vpx_img_set_rect","vpx_codec_encode","vpx_write_tpl_gop_stats"]
/*<Combination>: [vpx_codec_err_t vpx_read_tpl_gop_stats(FILE * tpl_file, VpxTplGopStats * tpl_gop_stats),
    vpx_codec_err_t vpx_codec_register_put_slice_cb(vpx_codec_ctx_t * ctx, vpx_codec_put_slice_cb_fn_t cb, void * user_priv),
    vpx_codec_err_t vpx_codec_control_(vpx_codec_ctx_t * ctx, int ctrl_id),
    int vpx_img_set_rect(vpx_image_t * img, unsigned int x, unsigned int y, unsigned int w, unsigned int h),
    vpx_codec_err_t vpx_codec_encode(vpx_codec_ctx_t * ctx, const vpx_image_t * img, vpx_codec_pts_t pts, unsigned long duration, vpx_enc_frame_flags_t flags, unsigned long deadline),
    vpx_codec_err_t vpx_write_tpl_gop_stats(FILE * tpl_file, const VpxTplGopStats * tpl_gop_stats)
*/
//<score> 4, nr_unique_branch: 0
//<Quality> {"density":8,"unique_branches":{},"library_calls":["vpx_codec_enc_config_default","vpx_codec_vp8_cx","vpx_codec_enc_init_ver","vpx_codec_vp8_cx","vpx_img_alloc","vpx_codec_encode","vpx_codec_decode","vpx_codec_get_frame","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy"],"critical_calls":["vpx_codec_enc_config_default","vpx_codec_vp8_cx","vpx_codec_enc_init_ver","vpx_codec_vp8_cx","vpx_img_alloc","vpx_codec_encode","vpx_codec_decode","vpx_codec_get_frame","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_19(const uint8_t* f_data, size_t f_size) {
	if(f_size<44) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_1, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_2, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_3, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_4, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_5, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_6, fdp);
	//fuzzer shim end}




    // Step 1: Creating and Initializing Variables
    vpx_codec_ctx_t encoder;
    vpx_codec_ctx_t decoder;
    vpx_codec_enc_cfg_t cfg;
    vpx_image_t raw;

    // Step 2: Initializing Encoder
    vpx_codec_enc_config_default(vpx_codec_vp8_cx(), &cfg, fuzz_uint32_t_1);
    cfg.g_w = cfg.g_h = 256;

    vpx_codec_enc_init(&encoder, vpx_codec_vp8_cx(), &cfg, 0);

    // Step 3: Creating and Initializing Frame
    vpx_img_alloc(&raw, VPX_IMG_FMT_I420, cfg.g_w, cfg.g_h, 1);

    // Step 4: Encoding Frame
    vpx_codec_encode(&encoder, &raw, fuzz_int64_t_2, fuzz_uint64_t_3, fuzz_int64_t_4, fuzz_uint64_t_5);

    // Step 5: Decoding Frame
    vpx_codec_decode(&decoder, data, size, NULL, fuzz_int64_t_6);

    // Step 6: Getting Decoded Frame
    const vpx_image_t *decoded_frame = vpx_codec_get_frame(&decoder, NULL);

    // Step 7: Cleaning Up
    vpx_img_free(&raw);
    vpx_codec_destroy(&encoder);
    vpx_codec_destroy(&decoder);

    return 0;
}