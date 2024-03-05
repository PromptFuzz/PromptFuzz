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
//<ID> 373
//<Prompt> ["vpx_codec_error","vpx_codec_peek_stream_info","vpx_codec_enc_init_multi_ver","vpx_read_tpl_gop_stats","vpx_codec_err_to_string"]
/*<Combination>: [const char *vpx_codec_error(const vpx_codec_ctx_t * ctx),
    vpx_codec_err_t vpx_codec_peek_stream_info(vpx_codec_iface_t * iface, const uint8_t * data, unsigned int data_sz, vpx_codec_stream_info_t * si),
    vpx_codec_err_t vpx_codec_enc_init_multi_ver(vpx_codec_ctx_t * ctx, vpx_codec_iface_t * iface, vpx_codec_enc_cfg_t * cfg, int num_enc, vpx_codec_flags_t flags, vpx_rational_t * dsf, int ver),
    vpx_codec_err_t vpx_read_tpl_gop_stats(FILE * tpl_file, VpxTplGopStats * tpl_gop_stats),
    const char *vpx_codec_err_to_string(vpx_codec_err_t err)
*/
//<score> 5, nr_unique_branch: 2
//<Quality> {"density":5,"unique_branches":{"vpx_codec_error":[[54,10,54,15,0,0,4,0]],"vpx_codec_enc_init_multi_ver":[[76,30,76,34,0,0,4,0]]},"library_calls":["vpx_codec_enc_init_multi_ver","vpx_codec_vp8_cx","vpx_codec_err_to_string","vpx_codec_peek_stream_info","vpx_codec_vp8_dx","vpx_codec_err_to_string","vpx_codec_error","vpx_codec_destroy"],"critical_calls":["vpx_codec_enc_init_multi_ver","vpx_codec_vp8_cx","vpx_codec_err_to_string","vpx_codec_peek_stream_info","vpx_codec_vp8_dx","vpx_codec_err_to_string","vpx_codec_error","vpx_codec_destroy"],"visited":2}
/*Here is an example program that achieves the desired event using the libvpx APIs:

*/


extern "C" int LLVMFuzzerTestOneInput_7(const uint8_t* f_data, size_t f_size) {
	if(f_size<12) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_2, fdp);
	//fuzzer shim end}




    // Create an input file
    FILE *in_file = fmemopen((void *)data, size, "rb");
    
    // Create an output file
    FILE *out_file = fopen("output_file", "wb");
    
    // Get the file descriptor for reading and writing
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);
    
    // Initialize libvpx codec
    vpx_codec_ctx_t codec_ctx;
    vpx_codec_err_t err;
    err = vpx_codec_enc_init_multi_ver(&codec_ctx, vpx_codec_vp8_cx(), NULL, fuzz_int32_t_1, fuzz_int64_t_2, NULL, VPX_ENCODER_ABI_VERSION);
    if (err != VPX_CODEC_OK) {
        const char *error_msg = vpx_codec_err_to_string(err);
        fprintf(stderr, "Error initializing libvpx codec: %s\n", error_msg);
    }
    
    // Read and decode stream info
    vpx_codec_stream_info_t stream_info;
    err = vpx_codec_peek_stream_info(vpx_codec_vp8_dx(), data, size, &stream_info);
    if (err != VPX_CODEC_OK) {
        const char *error_msg = vpx_codec_err_to_string(err);
        fprintf(stderr, "Error decoding stream info: %s\n", error_msg);
    }
    
    // Print error if any
    const char *error = vpx_codec_error(&codec_ctx);
    if (error != NULL) {
        fprintf(stderr, "Error: %s\n", error);
    }
    
    // Release resources
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    vpx_codec_destroy(&codec_ctx);
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}