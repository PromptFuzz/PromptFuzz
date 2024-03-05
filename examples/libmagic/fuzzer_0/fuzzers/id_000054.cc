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
//<ID> 2318
//<Prompt> ["vpx_read_tpl_gop_stats","vpx_write_tpl_gop_stats","vpx_img_set_rect","vpx_codec_control_","vpx_codec_get_global_headers"]
/*<Combination>: [vpx_codec_err_t vpx_read_tpl_gop_stats(FILE * tpl_file, VpxTplGopStats * tpl_gop_stats),
    vpx_codec_err_t vpx_write_tpl_gop_stats(FILE * tpl_file, const VpxTplGopStats * tpl_gop_stats),
    int vpx_img_set_rect(vpx_image_t * img, unsigned int x, unsigned int y, unsigned int w, unsigned int h),
    vpx_codec_err_t vpx_codec_control_(vpx_codec_ctx_t * ctx, int ctrl_id),
    vpx_fixed_buf_t *vpx_codec_get_global_headers(vpx_codec_ctx_t * ctx)
*/
//<score> 6, nr_unique_branch: 3
//<Quality> {"density":3,"unique_branches":{"vpx_codec_get_global_headers":[[329,24,329,34,0,0,4,0]],"vpx_codec_destroy":[[70,27,70,37,0,0,4,0]],"vpx_codec_control_":[[93,27,93,37,0,0,4,0]]},"library_calls":["vpx_write_tpl_gop_stats","vpx_read_tpl_gop_stats","vpx_img_alloc","vpx_img_set_rect","vpx_codec_vp8_dx","vpx_codec_control_","vpx_codec_get_global_headers","vpx_free_tpl_gop_stats","vpx_free_tpl_gop_stats","vpx_img_free","vpx_codec_destroy"],"critical_calls":["vpx_write_tpl_gop_stats","vpx_read_tpl_gop_stats","vpx_img_alloc","vpx_img_set_rect","vpx_codec_vp8_dx","vpx_codec_control_","vpx_codec_get_global_headers","vpx_free_tpl_gop_stats","vpx_free_tpl_gop_stats","vpx_img_free","vpx_codec_destroy"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_54(const uint8_t *data, size_t size) {
	if(size<0) return 0;

    // Step 1: Call vpx_write_tpl_gop_stats to write GOP stats to file
    FILE *tpl_file = fopen("tpl_file", "wb");
    VpxTplGopStats tpl_gop_stats;
    vpx_write_tpl_gop_stats(tpl_file, &tpl_gop_stats);
    assert_file_closed(&tpl_file);;

    // Step 2: Call vpx_read_tpl_gop_stats to read GOP stats from file
    tpl_file = fopen("tpl_file", "rb");
    VpxTplGopStats read_tpl_gop_stats;
    vpx_read_tpl_gop_stats(tpl_file, &read_tpl_gop_stats);
    assert_file_closed(&tpl_file);;

    // Step 3: Create and initialize vpx_image_t
    vpx_image_t img;
    vpx_img_alloc(&img, VPX_IMG_FMT_I420, 1920, 1080, 1);

    // Step 4: Set the rectangle of the image
    vpx_img_set_rect(&img, 0, 0, 1920, 1080);

    // Step 5: Create a vpx_codec_ctx_t
    vpx_codec_ctx_t ctx;
    ctx.iface = vpx_codec_vp8_dx();
    ctx.err = VPX_CODEC_OK;
    ctx.err_detail = NULL;

    // Step 6: Call vpx_codec_control_ to control the codec context
    vpx_codec_control_(&ctx, 1);

    // Step 7: Get global headers from codec context
    vpx_fixed_buf_t *global_headers = vpx_codec_get_global_headers(&ctx);

    // Step 8: Clean up resources
    vpx_free_tpl_gop_stats(&tpl_gop_stats);
    vpx_free_tpl_gop_stats(&read_tpl_gop_stats);
    vpx_img_free(&img);
    vpx_codec_destroy(&ctx);

    assert_file_closed(&tpl_file);
	return 0;
}