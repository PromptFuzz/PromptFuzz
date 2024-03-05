#include "FDSan.h"
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
//<ID> 2528
//<Prompt> ["vpx_codec_enc_init_multi_ver","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_register_put_frame_cb","vpx_img_alloc","vpx_codec_encode","vpx_codec_get_global_headers","vpx_codec_vp9_dx","vpx_codec_set_frame_buffer_functions","vpx_codec_decode","vpx_codec_destroy","vpx_codec_register_put_slice_cb","vpx_codec_get_stream_info","vpx_img_free"]
/*<Combination>: [vpx_codec_err_t vpx_codec_enc_init_multi_ver(vpx_codec_ctx_t * ctx, vpx_codec_iface_t * iface, vpx_codec_enc_cfg_t * cfg, int num_enc, vpx_codec_flags_t flags, vpx_rational_t * dsf, int ver),
    vpx_codec_iface_t *vpx_codec_vp9_cx(),
    vpx_codec_err_t vpx_codec_enc_config_default(vpx_codec_iface_t * iface, vpx_codec_enc_cfg_t * cfg, unsigned int usage),
    vpx_codec_err_t vpx_codec_register_put_frame_cb(vpx_codec_ctx_t * ctx, vpx_codec_put_frame_cb_fn_t cb, void * user_priv),
    vpx_image_t *vpx_img_alloc(vpx_image_t * img, vpx_img_fmt_t fmt, unsigned int d_w, unsigned int d_h, unsigned int align),
    vpx_codec_err_t vpx_codec_encode(vpx_codec_ctx_t * ctx, const vpx_image_t * img, vpx_codec_pts_t pts, unsigned long duration, vpx_enc_frame_flags_t flags, unsigned long deadline),
    vpx_fixed_buf_t *vpx_codec_get_global_headers(vpx_codec_ctx_t * ctx),
    vpx_codec_iface_t *vpx_codec_vp9_dx(),
    vpx_codec_err_t vpx_codec_set_frame_buffer_functions(vpx_codec_ctx_t * ctx, vpx_get_frame_buffer_cb_fn_t cb_get, vpx_release_frame_buffer_cb_fn_t cb_release, void * cb_priv),
    vpx_codec_err_t vpx_codec_decode(vpx_codec_ctx_t * ctx, const uint8_t * data, unsigned int data_sz, void * user_priv, long deadline),
    vpx_codec_err_t vpx_codec_destroy(vpx_codec_ctx_t * ctx),
    vpx_codec_err_t vpx_codec_register_put_slice_cb(vpx_codec_ctx_t * ctx, vpx_codec_put_slice_cb_fn_t cb, void * user_priv),
    vpx_codec_err_t vpx_codec_get_stream_info(vpx_codec_ctx_t * ctx, vpx_codec_stream_info_t * si),
    void vpx_img_free(vpx_image_t * img)
*/
//<score> 5.3333335, nr_unique_branch: 2
//<Quality> {"density":8,"unique_branches":{"decoder_decode":[[351,7,351,19,0,0,4,0],[351,23,351,35,0,0,4,0]]},"library_calls":["vpx_codec_vp9_dx","vpx_codec_dec_init_ver","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_set_frame_buffer_functions","vpx_codec_set_frame_buffer_functions","vpx_codec_register_put_frame_cb","vpx_codec_register_put_slice_cb","vpx_codec_encode","vpx_codec_decode","vpx_codec_get_stream_info","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp9_dx","vpx_codec_dec_init_ver","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_set_frame_buffer_functions","vpx_codec_set_frame_buffer_functions","vpx_codec_register_put_frame_cb","vpx_codec_register_put_slice_cb","vpx_codec_encode","vpx_codec_decode","vpx_codec_get_stream_info","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy"],"visited":2}
/*Here is the C++ program that achieves the event using the provided libvpx APIs:

*/


extern "C" int LLVMFuzzerTestOneInput_64(const uint8_t *data, size_t size) {
	if(size<0) return 0;

	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);

    // Step 4: Create a FILE * variable to read the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    
    // Step 5: Create a file descriptor for reading
    int in_fd = fuzz_fileno(in_file);
    
    // Step 6: Specify the input file name
    const char *input_file = "input_file";
    
    // Step 1: Initialize the decoder
    vpx_codec_ctx_t decoder;
    vpx_codec_iface_t *decoder_interface = vpx_codec_vp9_dx();
    vpx_codec_dec_cfg_t decoder_config = {0};
    vpx_codec_err_t decoder_init_result = vpx_codec_dec_init_ver(&decoder, decoder_interface, &decoder_config, 0, VPX_DECODER_ABI_VERSION);
    if (decoder_init_result != VPX_CODEC_OK) {
        assert_file_closed(&in_file);;  // Step 7: Release allocated resources before return
        assert_file_name_closed("input_file");
	assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Step 2: Create a FILE * variable to write output data
    FILE *out_file = fopen("output_file", "wb");
    
    // Step 5: Create a file descriptor for writing
    int out_fd = fuzz_fileno(out_file);
    
    // Step 6: Specify the output file name
    const char *output_file = "output_file";
    
    // Step 3: Initialize the encoder
    vpx_codec_ctx_t encoder;
    vpx_codec_iface_t *encoder_interface = vpx_codec_vp9_cx();
    vpx_codec_enc_cfg_t encoder_config;
    vpx_codec_enc_config_default(encoder_interface, &encoder_config, 0);
    vpx_codec_enc_init_ver(&encoder, encoder_interface, &encoder_config, 0, VPX_ENCODER_ABI_VERSION);
    
    // Step 8: Set frame buffer functions for encoder and decoder
    vpx_codec_set_frame_buffer_functions(&decoder, NULL, NULL, NULL);
    vpx_codec_set_frame_buffer_functions(&encoder, NULL, NULL, NULL);
    
    // Step 9: Register put_frame_cb and put_slice_cb for the encoder
    vpx_codec_register_put_frame_cb(&encoder, NULL, NULL);
    vpx_codec_register_put_slice_cb(&encoder, NULL, NULL);
    
    // Step 10: Encode the input data
    vpx_codec_pts_t pts = 0;
    vpx_codec_encode(&encoder, NULL, pts, 1, 0, 0);
    
    // Step 11: Decode the encoded data
    vpx_codec_err_t decode_result = vpx_codec_decode(&decoder, NULL, 0, NULL, 0);
    
    // Step 12: Get stream info from the decoder
    vpx_codec_stream_info_t stream_info;
    vpx_codec_get_stream_info(&decoder, &stream_info);
    
    // Step 13: Free the allocated images
    vpx_img_free(NULL);
    
    // Step 14: Destroy the encoder and decoder
    vpx_codec_destroy(&encoder);
    vpx_codec_destroy(&decoder);
    
    // Step 7: Release allocated resources before return
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}