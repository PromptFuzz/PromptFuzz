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
//<ID> 2444
//<Prompt> ["vpx_codec_dec_init_ver","vpx_codec_err_to_string","vpx_codec_control_","vpx_codec_get_cx_data","vpx_codec_peek_stream_info"]
/*<Combination>: [vpx_codec_err_t vpx_codec_dec_init_ver(vpx_codec_ctx_t * ctx, vpx_codec_iface_t * iface, const vpx_codec_dec_cfg_t * cfg, vpx_codec_flags_t flags, int ver),
    const char *vpx_codec_err_to_string(vpx_codec_err_t err),
    vpx_codec_err_t vpx_codec_control_(vpx_codec_ctx_t * ctx, int ctrl_id),
    const vpx_codec_cx_pkt_t *vpx_codec_get_cx_data(vpx_codec_ctx_t * ctx, vpx_codec_iter_t * iter),
    vpx_codec_err_t vpx_codec_peek_stream_info(vpx_codec_iface_t * iface, const uint8_t * data, unsigned int data_sz, vpx_codec_stream_info_t * si)
*/
//<score> 12, nr_unique_branch: 1
//<Quality> {"density":6,"unique_branches":{"vpx_codec_peek_stream_info":[[72,7,72,47,0,0,4,1]]},"library_calls":["vpx_codec_vp8_dx","vpx_codec_dec_init_ver","vpx_codec_err_to_string","vpx_codec_peek_stream_info","vpx_codec_err_to_string","vpx_codec_destroy","vpx_codec_get_cx_data","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp8_dx","vpx_codec_dec_init_ver","vpx_codec_peek_stream_info","vpx_codec_get_cx_data","vpx_codec_destroy"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_57(const uint8_t* f_data, size_t f_size) {
	if(f_size<8) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int64_t, fuzz_int64_t_1, fdp);
	//fuzzer shim end}




    // Create a vpx_codec_ctx_t variable
    vpx_codec_ctx_t codec_ctx;
    
    // Create a vpx_codec_iface_t variable
    const vpx_codec_iface_t *codec_iface = vpx_codec_vp8_dx();
    
    // Create a vpx_codec_dec_cfg_t variable
    vpx_codec_dec_cfg_t codec_cfg;
    codec_cfg.threads = 1;
    codec_cfg.w = 0;
    codec_cfg.h = 0;
    
    // Initialize the decoder
    vpx_codec_err_t init_res = vpx_codec_dec_init_ver(&codec_ctx, codec_iface, &codec_cfg, fuzz_int64_t_1, VPX_DECODER_ABI_VERSION);
    if (init_res != VPX_CODEC_OK) {
        std::cout << "Error initializing decoder: " << vpx_codec_err_to_string(init_res) << std::endl;
        return 0;
    }
    
    // Create a vpx_codec_stream_info_t variable
    vpx_codec_stream_info_t stream_info;
    stream_info.sz = sizeof(stream_info);
    stream_info.w = 0;
    stream_info.h = 0;
    stream_info.is_kf = 0;
    
    // Peek stream info from the input data
    vpx_codec_err_t peek_res = vpx_codec_peek_stream_info(codec_iface, data, static_cast<unsigned int>(size), &stream_info);
    if (peek_res != VPX_CODEC_OK) {
        std::cout << "Error peeking stream info: " << vpx_codec_err_to_string(peek_res) << std::endl;
        vpx_codec_destroy(&codec_ctx);
        return 0;
    }
    
    // Process the input data
    vpx_codec_iter_t iter = nullptr;
    const vpx_codec_cx_pkt_t *cx_data;
    while ((cx_data = vpx_codec_get_cx_data(&codec_ctx, &iter)) != nullptr) {
        // Process cx_data
    }
    
    // Destroy the decoder
    vpx_codec_destroy(&codec_ctx);
    
    return 0;
}