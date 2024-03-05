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
//<ID> 3329
//<Prompt> ["vpx_codec_enc_config_default","vpx_codec_vp8_cx","vpx_codec_encode","vpx_codec_enc_init_ver","vpx_codec_decode","vpx_codec_get_stream_info","vpx_img_alloc","vpx_img_free","vpx_codec_destroy"]
/*<Combination>: [vpx_codec_err_t vpx_codec_enc_config_default(vpx_codec_iface_t * iface, vpx_codec_enc_cfg_t * cfg, unsigned int usage),
    vpx_codec_iface_t *vpx_codec_vp8_cx(),
    vpx_codec_err_t vpx_codec_encode(vpx_codec_ctx_t * ctx, const vpx_image_t * img, vpx_codec_pts_t pts, unsigned long duration, vpx_enc_frame_flags_t flags, unsigned long deadline),
    vpx_codec_err_t vpx_codec_enc_init_ver(vpx_codec_ctx_t * ctx, vpx_codec_iface_t * iface, const vpx_codec_enc_cfg_t * cfg, vpx_codec_flags_t flags, int ver),
    vpx_codec_err_t vpx_codec_decode(vpx_codec_ctx_t * ctx, const uint8_t * data, unsigned int data_sz, void * user_priv, long deadline),
    vpx_codec_err_t vpx_codec_get_stream_info(vpx_codec_ctx_t * ctx, vpx_codec_stream_info_t * si),
    vpx_image_t *vpx_img_alloc(vpx_image_t * img, vpx_img_fmt_t fmt, unsigned int d_w, unsigned int d_h, unsigned int align),
    void vpx_img_free(vpx_image_t * img),
    vpx_codec_err_t vpx_codec_destroy(vpx_codec_ctx_t * ctx)
*/
//<score> 7, nr_unique_branch: 0
//<Quality> {"density":7,"unique_branches":{},"library_calls":["vpx_codec_vp8_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_vp8_dx","vpx_codec_dec_init_ver","vpx_img_alloc","vpx_codec_encode","vpx_codec_decode","vpx_codec_get_stream_info","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp8_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_vp8_dx","vpx_codec_dec_init_ver","vpx_img_alloc","vpx_codec_encode","vpx_codec_decode","vpx_codec_get_stream_info","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_95(const uint8_t* f_data, size_t f_size) {
	if(f_size<60) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_1, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_2, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_3, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_4, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_5, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_6, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_7, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_8, fdp);
	//fuzzer shim end}




    // Create input file for reading
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Create output file for writing
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Get the file descriptors
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);
    
    // Initialize libvpx
    vpx_codec_ctx_t encoder_ctx;
    vpx_codec_iface_t *encoder_iface = vpx_codec_vp8_cx();
    vpx_codec_enc_cfg_t encoder_cfg;
    vpx_codec_enc_config_default(encoder_iface, &encoder_cfg, fuzz_uint32_t_1);
    vpx_codec_enc_init_ver(&encoder_ctx, encoder_iface, &encoder_cfg, fuzz_int64_t_2, VPX_ENCODER_ABI_VERSION);
    
    vpx_codec_ctx_t decoder_ctx;
    vpx_codec_iface_t *decoder_iface = vpx_codec_vp8_dx();
    vpx_codec_dec_cfg_t decoder_cfg;
    vpx_codec_dec_init_ver(&decoder_ctx, decoder_iface, &decoder_cfg, fuzz_int64_t_3, VPX_DECODER_ABI_VERSION);
    
    // Read input data
    fseek(in_file, 0, SEEK_END);
    long input_size = ftell(in_file);
    fseek(in_file, 0, SEEK_SET);
    uint8_t *input_data = new uint8_t[input_size];
    fread(input_data, 1, input_size, in_file);
    
    // Encode input data
    vpx_image_t *input_image = vpx_img_alloc(NULL, VPX_IMG_FMT_I420, 320, 240, 16);
    vpx_codec_encode(&encoder_ctx, input_image, fuzz_int64_t_4, fuzz_uint64_t_5, fuzz_int64_t_6, fuzz_uint64_t_7);
    
    // Decode encoded data
    vpx_codec_decode(&decoder_ctx, input_data, input_size, NULL, fuzz_int64_t_8);
    
    // Get stream info
    vpx_codec_stream_info_t stream_info;
    vpx_codec_get_stream_info(&decoder_ctx, &stream_info);
    
    // Write output data
    fwrite(input_data, 1, input_size, out_file);
    
    // Free resources
    delete[] input_data;
    vpx_img_free(input_image);
    vpx_codec_destroy(&encoder_ctx);
    vpx_codec_destroy(&decoder_ctx);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}