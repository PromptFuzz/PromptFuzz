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
//<ID> 2197
//<Prompt> ["vpx_codec_vp9_dx","vpx_codec_dec_init_ver","vpx_codec_enc_init_multi_ver","vpx_codec_get_stream_info","vpx_codec_decode","vpx_codec_get_frame","vpx_codec_err_to_string","vpx_codec_destroy"]
/*<Combination>: [vpx_codec_iface_t *vpx_codec_vp9_dx(),
    vpx_codec_err_t vpx_codec_dec_init_ver(vpx_codec_ctx_t * ctx, vpx_codec_iface_t * iface, const vpx_codec_dec_cfg_t * cfg, vpx_codec_flags_t flags, int ver),
    vpx_codec_err_t vpx_codec_enc_init_multi_ver(vpx_codec_ctx_t * ctx, vpx_codec_iface_t * iface, vpx_codec_enc_cfg_t * cfg, int num_enc, vpx_codec_flags_t flags, vpx_rational_t * dsf, int ver),
    vpx_codec_err_t vpx_codec_get_stream_info(vpx_codec_ctx_t * ctx, vpx_codec_stream_info_t * si),
    vpx_codec_err_t vpx_codec_decode(vpx_codec_ctx_t * ctx, const uint8_t * data, unsigned int data_sz, void * user_priv, long deadline),
    vpx_image_t *vpx_codec_get_frame(vpx_codec_ctx_t * ctx, vpx_codec_iter_t * iter),
    const char *vpx_codec_err_to_string(vpx_codec_err_t err),
    vpx_codec_err_t vpx_codec_destroy(vpx_codec_ctx_t * ctx)
*/
//<score> 7, nr_unique_branch: 0
//<Quality> {"density":7,"unique_branches":{},"library_calls":["vpx_codec_vp9_dx","vpx_codec_dec_init_ver","vpx_codec_get_stream_info","vpx_codec_decode","vpx_codec_get_frame","vpx_codec_error_detail","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp9_dx","vpx_codec_dec_init_ver","vpx_codec_get_stream_info","vpx_codec_decode","vpx_codec_get_frame","vpx_codec_error_detail","vpx_codec_destroy"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_51(const uint8_t *data, size_t size) {
	if(size<0) return 0;

    // Step 1: Create a VP9 decoder
    vpx_codec_iface_t *iface = vpx_codec_vp9_dx();
    vpx_codec_ctx_t decoder;
    vpx_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.w = 1920;
    dec_cfg.h = 1080;
    vpx_codec_dec_init_ver(&decoder, iface, &dec_cfg, 0, VPX_DECODER_ABI_VERSION);

    // Step 2: Decode the input data
    vpx_codec_stream_info_t si;
    vpx_codec_get_stream_info(&decoder, &si);
    vpx_codec_decode(&decoder, data, size, NULL, -1);

    // Step 3: Get the decoded frame image
    vpx_codec_iter_t iter = NULL;
    vpx_image_t *img = vpx_codec_get_frame(&decoder, &iter);

    if (img != NULL) {
        // Do something with the decoded frame image
    }

    // Step 4: Print any error that occurred during decoding
    const char *error_detail = vpx_codec_error_detail(&decoder);
    if (error_detail != NULL) {
        printf("Decoding error: %s\n", error_detail);
    }

    // Step 5: Destroy the decoder
    vpx_codec_destroy(&decoder);

    return 0;
}