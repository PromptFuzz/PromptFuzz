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
//<ID> 1241
//<Prompt> ["vpx_codec_err_to_string","vpx_codec_error","vpx_codec_vp8_cx","vpx_codec_enc_config_default","vpx_codec_set_frame_buffer_functions","vpx_codec_peek_stream_info","vpx_codec_decode","vpx_codec_get_frame","vpx_img_free","vpx_codec_encode","vpx_codec_decode","vpx_codec_destroy","vpx_img_free"]
/*<Combination>: [const char *vpx_codec_err_to_string(vpx_codec_err_t err),
    const char *vpx_codec_error(const vpx_codec_ctx_t * ctx),
    vpx_codec_iface_t *vpx_codec_vp8_cx(),
    vpx_codec_err_t vpx_codec_enc_config_default(vpx_codec_iface_t * iface, vpx_codec_enc_cfg_t * cfg, unsigned int usage),
    vpx_codec_err_t vpx_codec_set_frame_buffer_functions(vpx_codec_ctx_t * ctx, vpx_get_frame_buffer_cb_fn_t cb_get, vpx_release_frame_buffer_cb_fn_t cb_release, void * cb_priv),
    vpx_codec_err_t vpx_codec_peek_stream_info(vpx_codec_iface_t * iface, const uint8_t * data, unsigned int data_sz, vpx_codec_stream_info_t * si),
    vpx_codec_err_t vpx_codec_decode(vpx_codec_ctx_t * ctx, const uint8_t * data, unsigned int data_sz, void * user_priv, long deadline),
    vpx_image_t *vpx_codec_get_frame(vpx_codec_ctx_t * ctx, vpx_codec_iter_t * iter),
    void vpx_img_free(vpx_image_t * img),
    vpx_codec_err_t vpx_codec_encode(vpx_codec_ctx_t * ctx, const vpx_image_t * img, vpx_codec_pts_t pts, unsigned long duration, vpx_enc_frame_flags_t flags, unsigned long deadline),
    vpx_codec_err_t vpx_codec_decode(vpx_codec_ctx_t * ctx, const uint8_t * data, unsigned int data_sz, void * user_priv, long deadline),
    vpx_codec_err_t vpx_codec_destroy(vpx_codec_ctx_t * ctx),
    void vpx_img_free(vpx_image_t * img)
*/
//<score> 2, nr_unique_branch: 1
//<Quality> {"density":1,"unique_branches":{"vpx_codec_error":[[54,10,54,15,0,0,4,1]]},"library_calls":["vpx_codec_err_to_string","vpx_codec_error"],"critical_calls":["vpx_codec_err_to_string","vpx_codec_error"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_27(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Step 4: Create input file
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    // Step 6: Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Step 5: Get file descriptors
    int in_file_fd = fuzz_fileno(in_file);
    int out_file_fd = fuzz_fileno(out_file);
    
    // Step 1: Error handling
    const char *error_string = vpx_codec_err_to_string(VPX_CODEC_OK);
    const vpx_codec_ctx_t *codec_ctx;
    const char *error_detail = vpx_codec_error(codec_ctx);
    
    // Step 7: Release resources
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(out_file_fd);
	assert_fd_closed(in_file_fd);
	return 0;
}