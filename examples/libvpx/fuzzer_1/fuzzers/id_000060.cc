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
//<ID> 2459
//<Prompt> ["vpx_img_alloc","vpx_codec_vp9_dx","vpx_img_wrap","vpx_codec_encode","vpx_codec_dec_init_ver","vpx_codec_decode","vpx_codec_destroy","vpx_img_set_rect","vpx_img_free"]
/*<Combination>: [vpx_image_t *vpx_img_alloc(vpx_image_t * img, vpx_img_fmt_t fmt, unsigned int d_w, unsigned int d_h, unsigned int align),
    vpx_codec_iface_t *vpx_codec_vp9_dx(),
    vpx_image_t *vpx_img_wrap(vpx_image_t * img, vpx_img_fmt_t fmt, unsigned int d_w, unsigned int d_h, unsigned int stride_align, unsigned char * img_data),
    vpx_codec_err_t vpx_codec_encode(vpx_codec_ctx_t * ctx, const vpx_image_t * img, vpx_codec_pts_t pts, unsigned long duration, vpx_enc_frame_flags_t flags, unsigned long deadline),
    vpx_codec_err_t vpx_codec_dec_init_ver(vpx_codec_ctx_t * ctx, vpx_codec_iface_t * iface, const vpx_codec_dec_cfg_t * cfg, vpx_codec_flags_t flags, int ver),
    vpx_codec_err_t vpx_codec_decode(vpx_codec_ctx_t * ctx, const uint8_t * data, unsigned int data_sz, void * user_priv, long deadline),
    vpx_codec_err_t vpx_codec_destroy(vpx_codec_ctx_t * ctx),
    int vpx_img_set_rect(vpx_image_t * img, unsigned int x, unsigned int y, unsigned int w, unsigned int h),
    void vpx_img_free(vpx_image_t * img)
*/
//<score> 1.3333334, nr_unique_branch: 0
//<Quality> {"density":4,"unique_branches":{},"library_calls":["vpx_img_alloc","vpx_codec_dec_init_ver","vpx_codec_vp9_dx","vpx_codec_decode","vpx_codec_encode","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy"],"critical_calls":["vpx_img_alloc","vpx_codec_dec_init_ver","vpx_codec_vp9_dx","vpx_codec_decode","vpx_codec_encode","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy"],"visited":2}
/*Here is the C++ program using libvpx library APIs to achieve the given event:

*/


extern "C" int LLVMFuzzerTestOneInput_60(const uint8_t* f_data, size_t f_size) {
	if(f_size<48) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int64_t, fuzz_int64_t_1, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_2, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_3, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_4, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_5, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_6, fdp);
	//fuzzer shim end}




    // Create input and output file variables
    FILE *in_file = fmemopen((void *)data, size, "rb");
    FILE *out_file = fopen("output_file", "wb");
    
    // Create vpx_image and vpx_codec_ctx variables
    vpx_image_t img;
    vpx_codec_ctx_t encoder_ctx;
    vpx_codec_ctx_t decoder_ctx;
    
    // Initialize the vpx_image
    vpx_img_alloc(&img, VPX_IMG_FMT_YV12, 1920, 1080, 32);
    
    // Initialize the encoder context with VP9 interface
    vpx_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 1;
    dec_cfg.w = 1920;
    dec_cfg.h = 1080;
    vpx_codec_dec_init_ver(&decoder_ctx, vpx_codec_vp9_dx(), &dec_cfg, fuzz_int64_t_1, VPX_DECODER_ABI_VERSION);
    
    // Decode the input data using the decoder context
    vpx_codec_decode(&decoder_ctx, data, size, NULL, fuzz_int64_t_2);
    
    // Encode the decoded image using the encoder context
    vpx_codec_encode(&encoder_ctx, &img, fuzz_int64_t_3, fuzz_uint64_t_4, fuzz_int64_t_5, fuzz_uint64_t_6);
    
    // Release all allocated resources
    vpx_img_free(&img);
    vpx_codec_destroy(&encoder_ctx);
    vpx_codec_destroy(&decoder_ctx);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}