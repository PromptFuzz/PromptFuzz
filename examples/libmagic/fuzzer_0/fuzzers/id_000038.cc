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
//<ID> 1848
//<Prompt> ["vpx_codec_register_put_slice_cb","vpx_write_tpl_gop_stats","vpx_read_tpl_gop_stats","vpx_codec_err_to_string","vpx_img_wrap"]
/*<Combination>: [vpx_codec_err_t vpx_codec_register_put_slice_cb(vpx_codec_ctx_t * ctx, vpx_codec_put_slice_cb_fn_t cb, void * user_priv),
    vpx_codec_err_t vpx_write_tpl_gop_stats(FILE * tpl_file, const VpxTplGopStats * tpl_gop_stats),
    vpx_codec_err_t vpx_read_tpl_gop_stats(FILE * tpl_file, VpxTplGopStats * tpl_gop_stats),
    const char *vpx_codec_err_to_string(vpx_codec_err_t err),
    vpx_image_t *vpx_img_wrap(vpx_image_t * img, vpx_img_fmt_t fmt, unsigned int d_w, unsigned int d_h, unsigned int stride_align, unsigned char * img_data)
*/
//<score> 6, nr_unique_branch: 1
//<Quality> {"density":6,"unique_branches":{"img_alloc_helper":[[40,7,40,40,0,0,4,0]]},"library_calls":["vpx_codec_vp9_cx","vpx_codec_enc_init_ver","vpx_img_wrap","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp9_cx","vpx_codec_enc_init_ver","vpx_img_wrap","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_destroy"],"visited":1}
/**/

extern "C" int LLVMFuzzerTestOneInput_38(const uint8_t *data, size_t size) {
	if(size<0) return 0;

    vpx_codec_iface_t *encoder_iface = vpx_codec_vp9_cx();
    vpx_codec_ctx_t encoder;
    vpx_codec_enc_cfg_t cfg;
    vpx_codec_enc_init(&encoder, encoder_iface, &cfg, 0);

    // Wrap input data
    const int width = 640;
    const int height = 480;
    const int stride = width;
    vpx_image_t img;
    vpx_img_wrap(&img, VPX_IMG_FMT_I420, width, height, stride, const_cast<uint8_t*>(data));

    // Encode frame
    const vpx_codec_cx_pkt_t *pkt;
    vpx_codec_encode(&encoder, &img, /* pts */ 0, /* duration */ 1, 0, VPX_DL_REALTIME);

    // Get encoded frame
    while ((pkt = vpx_codec_get_cx_data(&encoder, NULL)) != NULL) {
        if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
            // Process encoded frame
            // ...
        }
    }

    vpx_codec_destroy(&encoder);
    return 0;
}