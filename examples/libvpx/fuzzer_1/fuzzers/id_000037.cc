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
//<ID> 1843
//<Prompt> ["vpx_codec_register_put_slice_cb","vpx_write_tpl_gop_stats","vpx_read_tpl_gop_stats","vpx_codec_err_to_string","vpx_img_wrap"]
/*<Combination>: [vpx_codec_err_t vpx_codec_register_put_slice_cb(vpx_codec_ctx_t * ctx, vpx_codec_put_slice_cb_fn_t cb, void * user_priv),
    vpx_codec_err_t vpx_write_tpl_gop_stats(FILE * tpl_file, const VpxTplGopStats * tpl_gop_stats),
    vpx_codec_err_t vpx_read_tpl_gop_stats(FILE * tpl_file, VpxTplGopStats * tpl_gop_stats),
    const char *vpx_codec_err_to_string(vpx_codec_err_t err),
    vpx_image_t *vpx_img_wrap(vpx_image_t * img, vpx_img_fmt_t fmt, unsigned int d_w, unsigned int d_h, unsigned int stride_align, unsigned char * img_data)
*/
//<score> 9, nr_unique_branch: 0
//<Quality> {"density":9,"unique_branches":{},"library_calls":["vpx_img_wrap","vpx_codec_enc_config_default","vpx_codec_vp9_cx","vpx_codec_enc_init_ver","vpx_codec_vp9_cx","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_destroy","vpx_img_free"],"critical_calls":["vpx_img_wrap","vpx_codec_enc_config_default","vpx_codec_vp9_cx","vpx_codec_enc_init_ver","vpx_codec_vp9_cx","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_destroy","vpx_img_free"],"visited":0}
/*Here is the completed C++ program that achieves the desired event using the libvpx APIs:

*/


extern "C" int LLVMFuzzerTestOneInput_37(const uint8_t* f_data, size_t f_size) {
	if(f_size<56) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_4, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_5, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_6, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_7, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_8, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_9, fdp);
	//fuzzer shim end}




    // Create a FILE pointer to read the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    
    // Create a FILE pointer to write output data
    FILE *out_file = fopen("output_file", "wb");

    // Create a vpx_image_t variable to wrap the input data
    vpx_image_t img;
    vpx_img_wrap(&img, VPX_IMG_FMT_NONE, fuzz_uint32_t_1, fuzz_uint32_t_2, fuzz_uint32_t_3, (unsigned char *)data);

    // Create a vpx_codec_ctx_t variable to encode the wrapped image
    vpx_codec_ctx_t ctx;
    vpx_codec_enc_cfg_t cfg;
    vpx_codec_enc_config_default(vpx_codec_vp9_cx(), &cfg, fuzz_uint32_t_4);
    vpx_codec_enc_init_ver(&ctx, vpx_codec_vp9_cx(), &cfg, fuzz_int64_t_5, VPX_ENCODER_ABI_VERSION);
    
    // Encode the image using the codec context
    vpx_codec_encode(&ctx, &img, fuzz_int64_t_6, fuzz_uint64_t_7, fuzz_int64_t_8, fuzz_uint64_t_9);
    
    // Get the encoded data from the codec context
    const vpx_codec_cx_pkt_t *pkt;
    vpx_codec_iter_t iter = NULL;
    while ((pkt = vpx_codec_get_cx_data(&ctx, &iter))) {
        fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
    }
    
    // Destroy the codec context and free the wrapped image
    vpx_codec_destroy(&ctx);
    vpx_img_free(&img);
    
    // Close the input and output files
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}