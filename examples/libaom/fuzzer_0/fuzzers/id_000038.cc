#include "FDSan.h"
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
//<ID> 1353
//<Prompt> ["aom_codec_enc_init_ver","aom_codec_set_cx_data_buf","aom_codec_control","aom_codec_enc_config_set","aom_codec_encode","aom_img_plane_width"]
/*<Combination>: [aom_codec_err_t aom_codec_enc_init_ver(aom_codec_ctx_t * ctx, aom_codec_iface_t * iface, const aom_codec_enc_cfg_t * cfg, aom_codec_flags_t flags, int ver),
    aom_codec_err_t aom_codec_set_cx_data_buf(aom_codec_ctx_t * ctx, const aom_fixed_buf_t * buf, unsigned int pad_before, unsigned int pad_after),
    aom_codec_err_t aom_codec_control(aom_codec_ctx_t * ctx, int ctrl_id),
    aom_codec_err_t aom_codec_enc_config_set(aom_codec_ctx_t * ctx, const aom_codec_enc_cfg_t * cfg),
    aom_codec_err_t aom_codec_encode(aom_codec_ctx_t * ctx, const aom_image_t * img, aom_codec_pts_t pts, unsigned long duration, aom_enc_frame_flags_t flags),
    int aom_img_plane_width(const aom_image_t * img, int plane)
*/
//<score> 2, nr_unique_branch: 1, p_cov: 1
//<Quality> {"diversity":11,"density":11,"covered_api":["aom_codec_enc_init_ver","aom_codec_set_cx_data_buf","aom_codec_control","aom_codec_enc_config_set","aom_codec_encode","aom_img_plane_width"],"uncovered_api":[],"unique_branches":{"aom_img_plane_width":[[292,20,292,43,1]]},"library_calls":["aom_codec_av1_cx","aom_codec_enc_init_ver","aom_codec_set_cx_data_buf","aom_codec_control","aom_codec_enc_config_set","aom_img_alloc","aom_img_plane_width","aom_img_plane_width","aom_codec_encode","aom_codec_destroy","aom_img_free"]}
/*Here is a C++ program that uses the libaom library APIs to achieve the event described above:

*/


extern "C" int LLVMFuzzerTestOneInput_38(const uint8_t *data, size_t size) {
	if(size<=0) return 0;

    // Create codec context
    aom_codec_ctx_t codec_ctx;
    aom_codec_iface_t *codec_iface = aom_codec_av1_cx();
    aom_codec_enc_cfg_t codec_cfg;
    aom_codec_enc_init_ver(&codec_ctx, codec_iface, &codec_cfg, 0, AOM_ENCODER_ABI_VERSION);

    // Set data buffer
    aom_fixed_buf_t buf;
    buf.buf = (void*)data;
    buf.sz = size;
    aom_codec_set_cx_data_buf(&codec_ctx, &buf, 0, 0);

    // Set control parameters
    aom_codec_control(&codec_ctx, AOME_SET_CPUUSED, 4);

    // Set encoder configuration
    aom_codec_enc_config_set(&codec_ctx, &codec_cfg);

    // Create image
    aom_image_t img;
    aom_img_alloc(&img, AOM_IMG_FMT_I420, aom_img_plane_width(&img, 0), aom_img_plane_width(&img, 1), 32);

    // Encode image
    aom_codec_pts_t pts = 0;
    aom_enc_frame_flags_t flags = 0;
    aom_codec_encode(&codec_ctx, &img, pts, 1, flags);

    // Release resources
    aom_codec_destroy(&codec_ctx);
    aom_img_free(&img);

    return 0;
}