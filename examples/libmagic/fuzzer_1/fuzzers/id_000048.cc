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
//<ID> 2129
//<Prompt> ["vpx_codec_vp8_cx","vpx_codec_vp8_dx","vpx_img_wrap","vpx_codec_peek_stream_info","vpx_write_tpl_gop_stats","vpx_codec_encode","vpx_codec_get_stream_info","vpx_write_tpl_gop_stats","vpx_codec_dec_init_ver","vpx_codec_set_frame_buffer_functions","vpx_codec_get_preview_frame","vpx_codec_get_cx_data","vpx_codec_destroy","vpx_img_free"]
/*<Combination>: [vpx_codec_iface_t *vpx_codec_vp8_cx(),
    vpx_codec_iface_t *vpx_codec_vp8_dx(),
    vpx_image_t *vpx_img_wrap(vpx_image_t * img, vpx_img_fmt_t fmt, unsigned int d_w, unsigned int d_h, unsigned int stride_align, unsigned char * img_data),
    vpx_codec_err_t vpx_codec_peek_stream_info(vpx_codec_iface_t * iface, const uint8_t * data, unsigned int data_sz, vpx_codec_stream_info_t * si),
    vpx_codec_err_t vpx_write_tpl_gop_stats(FILE * tpl_file, const VpxTplGopStats * tpl_gop_stats),
    vpx_codec_err_t vpx_codec_encode(vpx_codec_ctx_t * ctx, const vpx_image_t * img, vpx_codec_pts_t pts, unsigned long duration, vpx_enc_frame_flags_t flags, unsigned long deadline),
    vpx_codec_err_t vpx_codec_get_stream_info(vpx_codec_ctx_t * ctx, vpx_codec_stream_info_t * si),
    vpx_codec_err_t vpx_write_tpl_gop_stats(FILE * tpl_file, const VpxTplGopStats * tpl_gop_stats),
    vpx_codec_err_t vpx_codec_dec_init_ver(vpx_codec_ctx_t * ctx, vpx_codec_iface_t * iface, const vpx_codec_dec_cfg_t * cfg, vpx_codec_flags_t flags, int ver),
    vpx_codec_err_t vpx_codec_set_frame_buffer_functions(vpx_codec_ctx_t * ctx, vpx_get_frame_buffer_cb_fn_t cb_get, vpx_release_frame_buffer_cb_fn_t cb_release, void * cb_priv),
    const vpx_image_t *vpx_codec_get_preview_frame(vpx_codec_ctx_t * ctx),
    const vpx_codec_cx_pkt_t *vpx_codec_get_cx_data(vpx_codec_ctx_t * ctx, vpx_codec_iter_t * iter),
    vpx_codec_err_t vpx_codec_destroy(vpx_codec_ctx_t * ctx),
    void vpx_img_free(vpx_image_t * img)
*/
//<score> 14, nr_unique_branch: 0
//<Quality> {"density":14,"unique_branches":{},"library_calls":["vpx_codec_vp8_cx","vpx_codec_enc_init_ver","vpx_img_wrap","vpx_codec_enc_config_default","vpx_codec_enc_config_set","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_vp8_dx","vpx_codec_dec_init_ver","vpx_codec_decode","vpx_codec_get_frame","vpx_codec_destroy","vpx_img_free","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp8_cx","vpx_codec_enc_init_ver","vpx_img_wrap","vpx_codec_enc_config_default","vpx_codec_enc_config_set","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_vp8_dx","vpx_codec_dec_init_ver","vpx_codec_decode","vpx_codec_get_frame","vpx_codec_destroy","vpx_img_free","vpx_codec_destroy"],"visited":0}
/**/

extern "C" int LLVMFuzzerTestOneInput_48(const uint8_t* f_data, size_t f_size) {
	if(f_size<72) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int64_t, fuzz_int64_t_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_4, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_5, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_6, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_7, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_8, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_9, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_10, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_11, fdp);
	//fuzzer shim end}




    // Step 1: Create an encoder context
    vpx_codec_ctx_t enc_ctx;
    vpx_codec_iface_t *enc_iface = vpx_codec_vp8_cx();
    vpx_codec_enc_cfg_t enc_cfg;
    vpx_codec_enc_init_ver(&enc_ctx, enc_iface, &enc_cfg, fuzz_int64_t_1, VPX_ENCODER_ABI_VERSION);

    // Step 2: Create an image to encode
    vpx_image_t *enc_img = vpx_img_wrap(NULL, VPX_IMG_FMT_I420, fuzz_uint32_t_2, fuzz_uint32_t_3, fuzz_uint32_t_4, NULL);

    // Step 3: Set the encoding parameters
    vpx_codec_enc_config_default(enc_iface, &enc_cfg, fuzz_uint32_t_5);
    vpx_codec_enc_config_set(&enc_ctx, &enc_cfg);

    // Step 4: Encode the image
    vpx_codec_pts_t pts = 0;
    vpx_enc_frame_flags_t flags = 0;
    vpx_codec_encode(&enc_ctx, enc_img, fuzz_int64_t_6, fuzz_uint64_t_7, fuzz_int64_t_8, fuzz_uint64_t_9);

    // Step 5: Get the encoded data
    vpx_codec_iter_t iter = NULL;
    const vpx_codec_cx_pkt_t *pkt = vpx_codec_get_cx_data(&enc_ctx, &iter);

    // Step 6: Create a decoder context
    vpx_codec_ctx_t dec_ctx;
    vpx_codec_iface_t *dec_iface = vpx_codec_vp8_dx();
    vpx_codec_dec_cfg_t dec_cfg;
    vpx_codec_dec_init_ver(&dec_ctx, dec_iface, &dec_cfg, fuzz_int64_t_10, VPX_DECODER_ABI_VERSION);

    // Step 7: Decode the encoded data
    vpx_codec_decode(&dec_ctx, data, size, NULL, fuzz_int64_t_11);

    // Step 8: Get the decoded frame
    const vpx_image_t *dec_img = vpx_codec_get_frame(&dec_ctx, &iter);

    // Step 9: Cleanup
    vpx_codec_destroy(&enc_ctx);
    vpx_img_free(enc_img);
    vpx_codec_destroy(&dec_ctx);

    return 0;
}