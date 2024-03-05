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
//<ID> 3186
//<Prompt> ["vpx_codec_vp8_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_multi_ver","vpx_codec_enc_init_ver","vpx_read_tpl_gop_stats","vpx_img_alloc","vpx_img_free","vpx_img_set_rect","vpx_codec_encode","vpx_codec_get_stream_info","vpx_codec_get_cx_data","vpx_codec_destroy"]
/*<Combination>: [vpx_codec_iface_t *vpx_codec_vp8_cx(),
    vpx_codec_err_t vpx_codec_enc_config_default(vpx_codec_iface_t * iface, vpx_codec_enc_cfg_t * cfg, unsigned int usage),
    vpx_codec_err_t vpx_codec_enc_init_multi_ver(vpx_codec_ctx_t * ctx, vpx_codec_iface_t * iface, vpx_codec_enc_cfg_t * cfg, int num_enc, vpx_codec_flags_t flags, vpx_rational_t * dsf, int ver),
    vpx_codec_err_t vpx_codec_enc_init_ver(vpx_codec_ctx_t * ctx, vpx_codec_iface_t * iface, const vpx_codec_enc_cfg_t * cfg, vpx_codec_flags_t flags, int ver),
    vpx_codec_err_t vpx_read_tpl_gop_stats(FILE * tpl_file, VpxTplGopStats * tpl_gop_stats),
    vpx_image_t *vpx_img_alloc(vpx_image_t * img, vpx_img_fmt_t fmt, unsigned int d_w, unsigned int d_h, unsigned int align),
    void vpx_img_free(vpx_image_t * img),
    int vpx_img_set_rect(vpx_image_t * img, unsigned int x, unsigned int y, unsigned int w, unsigned int h),
    vpx_codec_err_t vpx_codec_encode(vpx_codec_ctx_t * ctx, const vpx_image_t * img, vpx_codec_pts_t pts, unsigned long duration, vpx_enc_frame_flags_t flags, unsigned long deadline),
    vpx_codec_err_t vpx_codec_get_stream_info(vpx_codec_ctx_t * ctx, vpx_codec_stream_info_t * si),
    const vpx_codec_cx_pkt_t *vpx_codec_get_cx_data(vpx_codec_ctx_t * ctx, vpx_codec_iter_t * iter),
    vpx_codec_err_t vpx_codec_destroy(vpx_codec_ctx_t * ctx)
*/
//<score> 10, nr_unique_branch: 0
//<Quality> {"density":10,"unique_branches":{},"library_calls":["vpx_codec_vp8_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_read_tpl_gop_stats","vpx_img_alloc","vpx_img_set_rect","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_get_stream_info","vpx_codec_destroy","vpx_img_free","vpx_free_tpl_gop_stats"],"critical_calls":["vpx_codec_vp8_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_read_tpl_gop_stats","vpx_img_alloc","vpx_img_set_rect","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_get_stream_info","vpx_codec_destroy","vpx_img_free","vpx_free_tpl_gop_stats"],"visited":0}
/*Here is the complete C++ program that achieves the event using the libvpx APIs:

*/



extern "C" int LLVMFuzzerTestOneInput_90(const uint8_t* f_data, size_t f_size) {
	if(f_size<52) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_1, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_4, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_5, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_6, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_7, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_8, fdp);
	//fuzzer shim end}




    // Step 1: Create a VPX encoder context
    vpx_codec_ctx_t encoder;
    vpx_codec_iface_t *iface = vpx_codec_vp8_cx();
    vpx_codec_enc_cfg_t cfg;
    vpx_codec_enc_config_default(iface, &cfg, fuzz_uint32_t_1);
    vpx_codec_enc_init_ver(&encoder, iface, &cfg, fuzz_int64_t_2, VPX_ENCODER_ABI_VERSION);

    // Step 2: Read GOP statistics from template file
    FILE *tpl_file = fmemopen((void *)data, size, "rb");
    VpxTplGopStats tpl_gop_stats;
    vpx_read_tpl_gop_stats(tpl_file, &tpl_gop_stats);
    assert_file_closed(&tpl_file);;

    // Step 3: Allocate image and set its properties
    vpx_image_t image;
    vpx_img_alloc(&image, VPX_IMG_FMT_I420, cfg.g_w, cfg.g_h, 1);

    // Step 4: Set a specific region of the image
    vpx_img_set_rect(&image, fuzz_uint32_t_3, fuzz_uint32_t_4, cfg.g_w, cfg.g_h);

    // Step 5: Encode the image
    vpx_codec_pts_t pts = 0;
    vpx_codec_encode(&encoder, &image, fuzz_int64_t_5, fuzz_uint64_t_6, fuzz_int64_t_7, fuzz_uint64_t_8);

    // Step 6: Get the encoded data from the encoder
    const vpx_codec_cx_pkt_t *cx_pkt;
    vpx_codec_iter_t iter = NULL;
    while ((cx_pkt = vpx_codec_get_cx_data(&encoder, &iter)) != NULL) {
        // Process or save the encoded data
        // For example, fwrite(cx_pkt->data.frame.buf, cx_pkt->data.frame.sz, 1, out_file);
    }
    
    // Step 7: Get stream information from the encoder
    vpx_codec_stream_info_t si;
    vpx_codec_get_stream_info(&encoder, &si);

    // Step 8: Destroy the encoder and free resources
    vpx_codec_destroy(&encoder);
    vpx_img_free(&image);
    vpx_free_tpl_gop_stats(&tpl_gop_stats);

    assert_file_closed(&tpl_file);
	return 0;
}