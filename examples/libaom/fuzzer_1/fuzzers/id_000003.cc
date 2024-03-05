#include "FDSan.h"
#include "FuzzedDataProvider.h"
#include <aom/aom_decoder.h>
#include <aom/aomcx.h>
#include <aom/aomdx.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 46
//<Prompt> ["aom_codec_get_preview_frame","aom_codec_get_caps","aom_codec_err_to_string","aom_uleb_decode","aom_codec_get_frame"]
/*<Combination>: [const aom_image_t *aom_codec_get_preview_frame(aom_codec_ctx_t * ctx),
    aom_codec_caps_t aom_codec_get_caps(aom_codec_iface_t * iface),
    const char *aom_codec_err_to_string(aom_codec_err_t err),
    int aom_uleb_decode(const uint8_t * buffer, size_t available, uint64_t * value, size_t * length),
    aom_image_t *aom_codec_get_frame(aom_codec_ctx_t * ctx, aom_codec_iter_t * iter)
*/
//<score> 2, nr_unique_branch: 4, p_cov: 0.8
//<Quality> {"diversity":12,"density":6,"covered_api":["aom_codec_get_preview_frame","aom_codec_get_caps","aom_codec_err_to_string","aom_codec_get_frame"],"uncovered_api":["aom_uleb_decode"],"unique_branches":{"aom_codec_get_preview_frame":[[265,14,265,57,1],[267,14,267,42,1]],"encoder_get_preview":[[3435,7,3435,57,1]],"av1_get_preview_raw_frame":[[5101,7,5101,22,0]]},"library_calls":["aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_get_caps","aom_codec_err_to_string","aom_codec_av1_dx","aom_codec_enc_init_ver","aom_codec_dec_init_ver","aom_codec_decode","aom_codec_get_frame","aom_codec_get_preview_frame","aom_codec_destroy","aom_codec_destroy"]}
/**/


extern "C" int LLVMFuzzerTestOneInput_3(const uint8_t* f_data, size_t f_size) {
	if(f_size<=20) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_1, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_2, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_3, fdp);
	//fuzzer shim end}




    // Step 1: Initialization
    aom_codec_ctx_t encoder_ctx;
    aom_codec_iface_t *encoder_iface = aom_codec_av1_cx();
    aom_codec_enc_cfg_t encoder_cfg;
    aom_codec_enc_config_default(encoder_iface, &encoder_cfg, fuzz_uint32_t_1);
    
    aom_codec_caps_t encoder_caps = aom_codec_get_caps(encoder_iface);
    const char *err_str = aom_codec_err_to_string(AOM_CODEC_OK);
    
    aom_codec_ctx_t decoder_ctx;
    aom_codec_iface_t *decoder_iface = aom_codec_av1_dx();
    aom_codec_dec_cfg_t decoder_cfg = {0};
    
    // Step 2: Encoding and Decoding
    // Encoding
    aom_codec_enc_init_ver(&encoder_ctx, encoder_iface, &encoder_cfg, fuzz_int64_t_2, AOM_ENCODER_ABI_VERSION);
    
    // Decoding
    aom_codec_dec_init_ver(&decoder_ctx, decoder_iface, &decoder_cfg, fuzz_int64_t_3, AOM_DECODER_ABI_VERSION);
    
    // Step 3: Consume the input data
    aom_codec_decode(&decoder_ctx, data, size, NULL);
    
    // Step 4: Get the frame from decoder
    const aom_image_t *frame = NULL;
    aom_codec_iter_t iter = NULL;
    
    while ((frame = aom_codec_get_frame(&decoder_ctx, &iter))) {
        // Do something with the decoded frame
    }
    
    // Step 5: Get the preview frame from encoder
    const aom_image_t *preview_frame = aom_codec_get_preview_frame(&encoder_ctx);
    
    // Step 6: Cleanup
    aom_codec_destroy(&encoder_ctx);
    aom_codec_destroy(&decoder_ctx);
    
    // Step 7: Return
    return 0;
}