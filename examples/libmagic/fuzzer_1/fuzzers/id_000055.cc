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
//<ID> 2338
//<Prompt> ["vpx_codec_vp9_dx","vpx_img_wrap","vpx_codec_peek_stream_info","vpx_codec_get_stream_info","vpx_codec_get_preview_frame","vpx_img_alloc","vpx_codec_get_frame","vpx_img_free","vpx_codec_destroy"]
/*<Combination>: [vpx_codec_iface_t *vpx_codec_vp9_dx(),
    vpx_image_t *vpx_img_wrap(vpx_image_t * img, vpx_img_fmt_t fmt, unsigned int d_w, unsigned int d_h, unsigned int stride_align, unsigned char * img_data),
    vpx_codec_err_t vpx_codec_peek_stream_info(vpx_codec_iface_t * iface, const uint8_t * data, unsigned int data_sz, vpx_codec_stream_info_t * si),
    vpx_codec_err_t vpx_codec_get_stream_info(vpx_codec_ctx_t * ctx, vpx_codec_stream_info_t * si),
    const vpx_image_t *vpx_codec_get_preview_frame(vpx_codec_ctx_t * ctx),
    vpx_image_t *vpx_img_alloc(vpx_image_t * img, vpx_img_fmt_t fmt, unsigned int d_w, unsigned int d_h, unsigned int align),
    vpx_image_t *vpx_codec_get_frame(vpx_codec_ctx_t * ctx, vpx_codec_iter_t * iter),
    void vpx_img_free(vpx_image_t * img),
    vpx_codec_err_t vpx_codec_destroy(vpx_codec_ctx_t * ctx)
*/
//<score> 6, nr_unique_branch: 0
//<Quality> {"density":6,"unique_branches":{},"library_calls":["vpx_codec_vp9_dx","vpx_codec_peek_stream_info","vpx_codec_get_stream_info","vpx_codec_dec_init_ver","vpx_codec_get_frame","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp9_dx","vpx_codec_peek_stream_info","vpx_codec_get_stream_info","vpx_codec_dec_init_ver","vpx_codec_get_frame","vpx_codec_destroy"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_55(const uint8_t* f_data, size_t f_size) {
	if(f_size<8) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int64_t, fuzz_int64_t_1, fdp);
	//fuzzer shim end}




    FILE *in_file = fmemopen((void *)data, size, "rb");
    
    // Read the input data using in_file
    
    vpx_codec_iface_t *decoder_codec = vpx_codec_vp9_dx();
    vpx_codec_ctx_t decoder_ctx;
    vpx_codec_stream_info_t decoder_stream_info;
    vpx_codec_dec_cfg_t decoder_cfg = {0};
    
    if (vpx_codec_peek_stream_info(decoder_codec, data, size, &decoder_stream_info) != VPX_CODEC_OK) {
        // Handle error
    }
    
    if (vpx_codec_get_stream_info(&decoder_ctx, &decoder_stream_info) != VPX_CODEC_OK) {
        // Handle error
    }
    
    if (vpx_codec_dec_init_ver(&decoder_ctx, decoder_codec, &decoder_cfg, fuzz_int64_t_1, VPX_DECODER_ABI_VERSION) != VPX_CODEC_OK) {
        // Handle error
    }
    
    vpx_codec_iter_t decoder_iter = NULL;
    const vpx_image_t *decoded_image;
    
    while ((decoded_image = vpx_codec_get_frame(&decoder_ctx, &decoder_iter)) != NULL) {
        // Process decoded_image
    }
    
    vpx_codec_destroy(&decoder_ctx);
    
    FILE *out_file = fopen("output_file", "wb");
    
    // Write output data using out_file
    
    assert_file_closed(&out_file);;
    assert_file_closed(&in_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}