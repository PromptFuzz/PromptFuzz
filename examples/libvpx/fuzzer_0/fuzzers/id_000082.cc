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
//<ID> 2997
//<Prompt> ["vpx_codec_vp9_dx","vpx_codec_dec_init_ver","vpx_codec_peek_stream_info","vpx_write_tpl_gop_stats","vpx_codec_register_put_frame_cb","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_peek_stream_info","vpx_codec_decode","vpx_codec_get_preview_frame","vpx_img_free","vpx_codec_destroy"]
/*<Combination>: [vpx_codec_iface_t *vpx_codec_vp9_dx(),
    vpx_codec_err_t vpx_codec_dec_init_ver(vpx_codec_ctx_t * ctx, vpx_codec_iface_t * iface, const vpx_codec_dec_cfg_t * cfg, vpx_codec_flags_t flags, int ver),
    vpx_codec_err_t vpx_codec_peek_stream_info(vpx_codec_iface_t * iface, const uint8_t * data, unsigned int data_sz, vpx_codec_stream_info_t * si),
    vpx_codec_err_t vpx_write_tpl_gop_stats(FILE * tpl_file, const VpxTplGopStats * tpl_gop_stats),
    vpx_codec_err_t vpx_codec_register_put_frame_cb(vpx_codec_ctx_t * ctx, vpx_codec_put_frame_cb_fn_t cb, void * user_priv),
    vpx_codec_err_t vpx_codec_enc_config_default(vpx_codec_iface_t * iface, vpx_codec_enc_cfg_t * cfg, unsigned int usage),
    vpx_codec_err_t vpx_codec_enc_init_ver(vpx_codec_ctx_t * ctx, vpx_codec_iface_t * iface, const vpx_codec_enc_cfg_t * cfg, vpx_codec_flags_t flags, int ver),
    vpx_codec_err_t vpx_codec_peek_stream_info(vpx_codec_iface_t * iface, const uint8_t * data, unsigned int data_sz, vpx_codec_stream_info_t * si),
    vpx_codec_err_t vpx_codec_decode(vpx_codec_ctx_t * ctx, const uint8_t * data, unsigned int data_sz, void * user_priv, long deadline),
    const vpx_image_t *vpx_codec_get_preview_frame(vpx_codec_ctx_t * ctx),
    void vpx_img_free(vpx_image_t * img),
    vpx_codec_err_t vpx_codec_destroy(vpx_codec_ctx_t * ctx)
*/
//<score> 7, nr_unique_branch: 0
//<Quality> {"density":7,"unique_branches":{},"library_calls":["vpx_codec_vp9_dx","vpx_codec_dec_init_ver","vpx_codec_peek_stream_info","vpx_codec_decode","vpx_codec_get_preview_frame","vpx_img_free","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp9_dx","vpx_codec_dec_init_ver","vpx_codec_peek_stream_info","vpx_codec_decode","vpx_codec_get_preview_frame","vpx_img_free","vpx_codec_destroy"],"visited":0}
/**/

// Include the libvpx headers

// Event: Initialize the decoder, decode a frame and get the preview frame
extern "C" int LLVMFuzzerTestOneInput_82(const uint8_t *data, size_t size) {
	if(size<0) return 0;

    // Create a decoder context
    vpx_codec_ctx_t decoder_ctx;
    vpx_codec_dec_cfg_t decoder_cfg;
    vpx_codec_iface_t *decoder_iface = vpx_codec_vp9_dx();
    vpx_codec_dec_init_ver(&decoder_ctx, decoder_iface, &decoder_cfg, 0, VPX_DECODER_ABI_VERSION);

    // Create a stream info structure for peeking the stream information
    vpx_codec_stream_info_t stream_info;
    vpx_codec_peek_stream_info(decoder_iface, data, static_cast<unsigned int>(size), &stream_info);

    // Decode the frame
    vpx_codec_decode(&decoder_ctx, data, static_cast<unsigned int>(size), nullptr, 0);

    // Get the preview frame
    const vpx_image_t *preview_frame = vpx_codec_get_preview_frame(&decoder_ctx);

    // Do something with the preview frame

    // Free the preview frame
    vpx_img_free(const_cast<vpx_image_t *>(preview_frame));

    // Destroy the decoder context
    vpx_codec_destroy(&decoder_ctx);

    return 0;
}