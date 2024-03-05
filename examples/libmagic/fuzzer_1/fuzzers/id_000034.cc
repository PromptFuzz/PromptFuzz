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
//<ID> 1564
//<Prompt> ["vpx_codec_vp8_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_peek_stream_info","vpx_codec_enc_config_set","vpx_codec_set_cx_data_buf","vpx_codec_encode","vpx_codec_vp9_dx","vpx_codec_dec_init_ver","vpx_codec_get_stream_info","vpx_codec_destroy"]
/*<Combination>: [vpx_codec_iface_t *vpx_codec_vp8_cx(),
    vpx_codec_err_t vpx_codec_enc_config_default(vpx_codec_iface_t * iface, vpx_codec_enc_cfg_t * cfg, unsigned int usage),
    vpx_codec_err_t vpx_codec_enc_init_ver(vpx_codec_ctx_t * ctx, vpx_codec_iface_t * iface, const vpx_codec_enc_cfg_t * cfg, vpx_codec_flags_t flags, int ver),
    vpx_codec_err_t vpx_codec_peek_stream_info(vpx_codec_iface_t * iface, const uint8_t * data, unsigned int data_sz, vpx_codec_stream_info_t * si),
    vpx_codec_err_t vpx_codec_enc_config_set(vpx_codec_ctx_t * ctx, const vpx_codec_enc_cfg_t * cfg),
    vpx_codec_err_t vpx_codec_set_cx_data_buf(vpx_codec_ctx_t * ctx, const vpx_fixed_buf_t * buf, unsigned int pad_before, unsigned int pad_after),
    vpx_codec_err_t vpx_codec_encode(vpx_codec_ctx_t * ctx, const vpx_image_t * img, vpx_codec_pts_t pts, unsigned long duration, vpx_enc_frame_flags_t flags, unsigned long deadline),
    vpx_codec_iface_t *vpx_codec_vp9_dx(),
    vpx_codec_err_t vpx_codec_dec_init_ver(vpx_codec_ctx_t * ctx, vpx_codec_iface_t * iface, const vpx_codec_dec_cfg_t * cfg, vpx_codec_flags_t flags, int ver),
    vpx_codec_err_t vpx_codec_get_stream_info(vpx_codec_ctx_t * ctx, vpx_codec_stream_info_t * si),
    vpx_codec_err_t vpx_codec_destroy(vpx_codec_ctx_t * ctx)
*/
//<score> 3.5, nr_unique_branch: 0
//<Quality> {"density":7,"unique_branches":{},"library_calls":["vpx_codec_vp8_cx","vpx_codec_vp9_dx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_peek_stream_info","vpx_codec_dec_init_ver","vpx_codec_enc_config_set","vpx_codec_set_cx_data_buf","vpx_codec_encode","vpx_codec_decode","vpx_codec_get_stream_info","vpx_codec_destroy","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp8_cx","vpx_codec_vp9_dx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_peek_stream_info","vpx_codec_dec_init_ver","vpx_codec_enc_config_set","vpx_codec_set_cx_data_buf","vpx_codec_encode","vpx_codec_decode","vpx_codec_get_stream_info","vpx_codec_destroy","vpx_codec_destroy"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_34(const uint8_t* f_data, size_t f_size) {
	if(f_size<68) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_1, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_2, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_3, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_4, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_5, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_6, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_7, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_8, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_9, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_10, fdp);
	//fuzzer shim end}




    // Create input file to read the data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    
    // Create output file to write the data
    FILE *out_file = fopen("output_file", "wb");
    
    // Read the input file descriptor
    int in_fd = fuzz_fileno(in_file);
    
    // Write the output file descriptor
    int out_fd = fuzz_fileno(out_file);
    
    // Initialize libvpx codec
    vpx_codec_iface_t *encoder_if = vpx_codec_vp8_cx();
    vpx_codec_iface_t *decoder_if = vpx_codec_vp9_dx();
    
    vpx_codec_ctx_t encoder_ctx, decoder_ctx;
    vpx_codec_enc_cfg_t encoder_cfg;
    vpx_codec_dec_cfg_t decoder_cfg;
    vpx_codec_stream_info_t stream_info;
    
    // Set default encoder configuration
    vpx_codec_enc_config_default(encoder_if, &encoder_cfg, fuzz_uint32_t_1);
    
    // Initialize encoder
    vpx_codec_enc_init_ver(&encoder_ctx, encoder_if, &encoder_cfg, fuzz_int64_t_2, VPX_ENCODER_ABI_VERSION);
    
    // Set stream info with input data
    vpx_codec_peek_stream_info(decoder_if, data, static_cast<unsigned int>(size), &stream_info);
    
    // Initialize decoder
    vpx_codec_dec_init_ver(&decoder_ctx, decoder_if, &decoder_cfg, fuzz_int64_t_3, VPX_DECODER_ABI_VERSION);
    
    // Set encoder configuration
    vpx_codec_enc_config_set(&encoder_ctx, &encoder_cfg);
    
    // Set buffer for coded data
    vpx_fixed_buf_t cx_data_buf;
    cx_data_buf.buf = malloc(size);
    cx_data_buf.sz = size;
    vpx_codec_set_cx_data_buf(&encoder_ctx, &cx_data_buf, fuzz_uint32_t_4, fuzz_uint32_t_5);
    
    // Encode frame
    vpx_codec_encode(&encoder_ctx, nullptr, fuzz_int64_t_6, fuzz_uint64_t_7, fuzz_int64_t_8, fuzz_uint64_t_9);
    
    // Decode frame
    vpx_codec_decode(&decoder_ctx, data, static_cast<unsigned int>(size), nullptr, fuzz_int64_t_10);
    
    // Get stream info from decoder
    vpx_codec_get_stream_info(&decoder_ctx, &stream_info);
    
    // Destroy encoder and decoder
    vpx_codec_destroy(&encoder_ctx);
    vpx_codec_destroy(&decoder_ctx);
    
    // Release allocated resources
    free(cx_data_buf.buf);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}