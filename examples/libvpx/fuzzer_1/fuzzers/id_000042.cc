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
//<ID> 1995
//<Prompt> ["vpx_codec_dec_init_ver","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_register_put_frame_cb","vpx_codec_get_stream_info","vpx_read_tpl_gop_stats","vpx_img_wrap","vpx_img_set_rect","vpx_img_alloc","vpx_codec_enc_init_multi_ver","vpx_codec_decode","vpx_codec_get_preview_frame","vpx_codec_encode","vpx_codec_destroy","vpx_img_free"]
/*<Combination>: [vpx_codec_err_t vpx_codec_dec_init_ver(vpx_codec_ctx_t * ctx, vpx_codec_iface_t * iface, const vpx_codec_dec_cfg_t * cfg, vpx_codec_flags_t flags, int ver),
    vpx_codec_err_t vpx_codec_enc_config_default(vpx_codec_iface_t * iface, vpx_codec_enc_cfg_t * cfg, unsigned int usage),
    vpx_codec_err_t vpx_codec_enc_init_ver(vpx_codec_ctx_t * ctx, vpx_codec_iface_t * iface, const vpx_codec_enc_cfg_t * cfg, vpx_codec_flags_t flags, int ver),
    vpx_codec_err_t vpx_codec_register_put_frame_cb(vpx_codec_ctx_t * ctx, vpx_codec_put_frame_cb_fn_t cb, void * user_priv),
    vpx_codec_err_t vpx_codec_get_stream_info(vpx_codec_ctx_t * ctx, vpx_codec_stream_info_t * si),
    vpx_codec_err_t vpx_read_tpl_gop_stats(FILE * tpl_file, VpxTplGopStats * tpl_gop_stats),
    vpx_image_t *vpx_img_wrap(vpx_image_t * img, vpx_img_fmt_t fmt, unsigned int d_w, unsigned int d_h, unsigned int stride_align, unsigned char * img_data),
    int vpx_img_set_rect(vpx_image_t * img, unsigned int x, unsigned int y, unsigned int w, unsigned int h),
    vpx_image_t *vpx_img_alloc(vpx_image_t * img, vpx_img_fmt_t fmt, unsigned int d_w, unsigned int d_h, unsigned int align),
    vpx_codec_err_t vpx_codec_enc_init_multi_ver(vpx_codec_ctx_t * ctx, vpx_codec_iface_t * iface, vpx_codec_enc_cfg_t * cfg, int num_enc, vpx_codec_flags_t flags, vpx_rational_t * dsf, int ver),
    vpx_codec_err_t vpx_codec_decode(vpx_codec_ctx_t * ctx, const uint8_t * data, unsigned int data_sz, void * user_priv, long deadline),
    const vpx_image_t *vpx_codec_get_preview_frame(vpx_codec_ctx_t * ctx),
    vpx_codec_err_t vpx_codec_encode(vpx_codec_ctx_t * ctx, const vpx_image_t * img, vpx_codec_pts_t pts, unsigned long duration, vpx_enc_frame_flags_t flags, unsigned long deadline),
    vpx_codec_err_t vpx_codec_destroy(vpx_codec_ctx_t * ctx),
    void vpx_img_free(vpx_image_t * img)
*/
//<score> 15, nr_unique_branch: 0
//<Quality> {"density":15,"unique_branches":{},"library_calls":["vpx_codec_dec_init_ver","vpx_codec_vp9_dx","vpx_codec_get_stream_info","vpx_codec_enc_config_default","vpx_codec_vp9_cx","vpx_codec_enc_init_ver","vpx_codec_vp9_cx","vpx_img_wrap","vpx_codec_encode","vpx_codec_decode","vpx_codec_get_preview_frame","vpx_codec_encode","vpx_codec_destroy","vpx_codec_destroy","vpx_img_free"],"critical_calls":["vpx_codec_dec_init_ver","vpx_codec_vp9_dx","vpx_codec_get_stream_info","vpx_codec_enc_config_default","vpx_codec_vp9_cx","vpx_codec_enc_init_ver","vpx_codec_vp9_cx","vpx_img_wrap","vpx_codec_encode","vpx_codec_decode","vpx_codec_get_preview_frame","vpx_codec_encode","vpx_codec_destroy","vpx_codec_destroy","vpx_img_free"],"visited":0}
/*Here is the C++ program that achieves the event described above:

*/


extern "C" int LLVMFuzzerTestOneInput_42(const uint8_t* f_data, size_t f_size) {
	if(f_size<96) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int64_t, fuzz_int64_t_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_3, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_4, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_5, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_6, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_7, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_8, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_9, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_10, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_11, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_12, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_13, fdp);
	//fuzzer shim end}




    vpx_codec_ctx_t dec_ctx, enc_ctx;
    vpx_codec_dec_cfg_t dec_cfg;
    vpx_codec_enc_cfg_t enc_cfg;
    vpx_codec_stream_info_t stream_info;
    FILE *in_file = fmemopen((void *)data, size, "rb");
    FILE *out_file = fopen("output_file", "wb");

    // Initialize the decoder context
    vpx_codec_dec_init_ver(&dec_ctx, vpx_codec_vp9_dx(), &dec_cfg, fuzz_int64_t_1, VPX_DECODER_ABI_VERSION);

    // Read stream info from input file
    vpx_codec_get_stream_info(&dec_ctx, &stream_info);

    // Initialize the encoder context
    vpx_codec_enc_config_default(vpx_codec_vp9_cx(), &enc_cfg, fuzz_uint32_t_2);
    vpx_codec_enc_init_ver(&enc_ctx, vpx_codec_vp9_cx(), &enc_cfg, fuzz_int64_t_3, VPX_ENCODER_ABI_VERSION);

    // Set the input image
    vpx_image_t img;
    vpx_img_wrap(&img, VPX_IMG_FMT_I420, stream_info.w, stream_info.h, fuzz_uint32_t_4, nullptr);

    // Encode the input image
    vpx_codec_encode(&enc_ctx, &img, fuzz_int64_t_5, fuzz_uint64_t_6, fuzz_int64_t_7, fuzz_uint64_t_8);

    // Decode the input data
    vpx_codec_decode(&dec_ctx, data, size, nullptr, fuzz_int64_t_9);

    // Get the preview frame
    const vpx_image_t *preview_frame = vpx_codec_get_preview_frame(&dec_ctx);

    // Encode the preview frame
    vpx_codec_encode(&enc_ctx, preview_frame, fuzz_int64_t_10, fuzz_uint64_t_11, fuzz_int64_t_12, fuzz_uint64_t_13);

    // Destroy the encoder and decoder contexts
    vpx_codec_destroy(&enc_ctx);
    vpx_codec_destroy(&dec_ctx);

    // Free the image allocated memory
    vpx_img_free(&img);

    // Close the input and output files
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}