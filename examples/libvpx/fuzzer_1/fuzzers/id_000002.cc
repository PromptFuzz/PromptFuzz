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
//<ID> 148
//<Prompt> ["vpx_codec_register_put_frame_cb","vpx_codec_enc_init_ver","vpx_codec_control_","vpx_codec_err_to_string","vpx_codec_get_global_headers","vpx_img_free"]
/*<Combination>: [vpx_codec_err_t vpx_codec_register_put_frame_cb(vpx_codec_ctx_t * ctx, vpx_codec_put_frame_cb_fn_t cb, void * user_priv),
    vpx_codec_err_t vpx_codec_enc_init_ver(vpx_codec_ctx_t * ctx, vpx_codec_iface_t * iface, const vpx_codec_enc_cfg_t * cfg, vpx_codec_flags_t flags, int ver),
    vpx_codec_err_t vpx_codec_control_(vpx_codec_ctx_t * ctx, int ctrl_id),
    const char *vpx_codec_err_to_string(vpx_codec_err_t err),
    vpx_fixed_buf_t *vpx_codec_get_global_headers(vpx_codec_ctx_t * ctx),
    void vpx_img_free(vpx_image_t * img)
*/
//<score> 5.25, nr_unique_branch: 14
//<Quality> {"density":7,"unique_branches":{"vpx_codec_get_global_headers":[[329,9,329,20,0,0,4,1],[329,24,329,34,0,0,4,1],[331,14,331,57,0,0,4,1],[333,14,333,44,0,0,4,0]],"vpx_codec_control_":[[91,7,91,11,0,0,4,1],[91,15,91,23,0,0,4,1],[93,12,93,23,0,0,4,1],[93,27,93,37,0,0,4,1],[93,41,93,63,0,0,4,1],[100,41,100,50,0,0,4,0],[100,41,100,50,0,0,4,1],[101,11,101,26,0,0,4,1],[101,30,101,55,0,0,4,1],[21,32,21,35,1,0,4,0]]},"library_calls":["vpx_codec_vp8_cx","vpx_codec_vp8_dx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_dec_init_ver","vpx_codec_register_put_frame_cb","vpx_codec_control_","vpx_codec_get_global_headers","vpx_img_alloc","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp8_cx","vpx_codec_vp8_dx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_dec_init_ver","vpx_codec_register_put_frame_cb","vpx_codec_control_","vpx_codec_get_global_headers","vpx_img_alloc","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy"],"visited":3}
/*Here is the C++ program that achieves the event using the libvpx APIs:

*/



extern "C" int LLVMFuzzerTestOneInput_2(const uint8_t* f_data, size_t f_size) {
	if(f_size<20) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_1, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_2, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_3, fdp);
	//fuzzer shim end}




    // Step 1: Create a vpx_codec_ctx_t
    vpx_codec_ctx_t encoder;
    vpx_codec_ctx_t decoder;

    // Step 2: Initialize the decoder and encoder
    vpx_codec_iface_t *encoder_iface = vpx_codec_vp8_cx();
    vpx_codec_iface_t *decoder_iface = vpx_codec_vp8_dx();

    vpx_codec_enc_cfg_t encoder_cfg;
    vpx_codec_enc_config_default(encoder_iface, &encoder_cfg, fuzz_uint32_t_1);
    vpx_codec_enc_init_ver(&encoder, encoder_iface, &encoder_cfg, fuzz_int64_t_2, VPX_ENCODER_ABI_VERSION);

    vpx_codec_dec_cfg_t decoder_cfg;
    vpx_codec_dec_init_ver(&decoder, decoder_iface, &decoder_cfg, fuzz_int64_t_3, VPX_DECODER_ABI_VERSION);

    // Step 3: Process the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    vpx_codec_put_frame_cb_fn_t put_frame_cb = [](void *user_priv, const vpx_image_t *img) {};
    vpx_codec_register_put_frame_cb(&encoder, put_frame_cb, nullptr);
    vpx_codec_control_(&encoder, VP9E_SET_ROI_MAP);

    // Step 4: Get the global headers
    vpx_fixed_buf_t *global_headers = vpx_codec_get_global_headers(&encoder);

    // Step 5: Free the vpx_image_t
    vpx_image_t *image = vpx_img_alloc(nullptr, VPX_IMG_FMT_I420, 640, 480, 16);
    vpx_img_free(image);

    // Step 6: Convert vpx_fixed_buf_t to file descriptor
    int input_fd = fuzz_fileno(in_file);
    FILE *out_file = fopen("output_file", "wb");
    int output_fd = fuzz_fileno(out_file);

    // Step 7: Release resources
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    vpx_codec_destroy(&encoder);
    vpx_codec_destroy(&decoder);

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(output_fd);
	assert_fd_closed(input_fd);
	return 0;
}