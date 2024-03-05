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
//<ID> 1092
//<Prompt> ["vpx_img_wrap","vpx_write_tpl_gop_stats","vpx_codec_encode","vpx_free_tpl_gop_stats","vpx_codec_error","vpx_read_tpl_gop_stats"]
/*<Combination>: [vpx_image_t *vpx_img_wrap(vpx_image_t * img, vpx_img_fmt_t fmt, unsigned int d_w, unsigned int d_h, unsigned int stride_align, unsigned char * img_data),
    vpx_codec_err_t vpx_write_tpl_gop_stats(FILE * tpl_file, const VpxTplGopStats * tpl_gop_stats),
    vpx_codec_err_t vpx_codec_encode(vpx_codec_ctx_t * ctx, const vpx_image_t * img, vpx_codec_pts_t pts, unsigned long duration, vpx_enc_frame_flags_t flags, unsigned long deadline),
    void vpx_free_tpl_gop_stats(VpxTplGopStats * tpl_gop_stats),
    const char *vpx_codec_error(const vpx_codec_ctx_t * ctx),
    vpx_codec_err_t vpx_read_tpl_gop_stats(FILE * tpl_file, VpxTplGopStats * tpl_gop_stats)
*/
//<score> 10, nr_unique_branch: 10
//<Quality> {"density":8,"unique_branches":{"vpx_read_tpl_gop_stats":[[66,7,66,46,0,0,4,1],[69,15,69,34,0,0,4,0],[69,15,69,34,0,0,4,1],[26,9,26,31,1,0,4,1],[26,9,26,31,2,0,4,0]],"vpx_free_tpl_gop_stats":[[103,19,103,46,0,0,4,0]],"check_size_argument_overflow":[[31,7,31,17,0,0,4,0]],"vpx_write_tpl_gop_stats":[[37,15,37,38,0,0,4,0],[43,21,43,39,0,0,4,1],[19,9,19,17,2,0,4,1]]},"library_calls":["vpx_img_wrap","vpx_read_tpl_gop_stats","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_encode","vpx_write_tpl_gop_stats","vpx_free_tpl_gop_stats","vpx_codec_error","vpx_codec_destroy","vpx_img_free"],"critical_calls":["vpx_img_wrap","vpx_read_tpl_gop_stats","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_encode","vpx_write_tpl_gop_stats","vpx_free_tpl_gop_stats","vpx_codec_error","vpx_codec_destroy","vpx_img_free"],"visited":3}
/**/


// Include libvpx headers

extern "C" int LLVMFuzzerTestOneInput_21(const uint8_t *data, size_t size) {
	if(size<0) return 0;


    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Create an image using vpx_img_wrap
    vpx_image_t img;
    vpx_img_wrap(&img, VPX_IMG_FMT_I420, 1280, 720, 32, NULL);

    // Read GOP statistics from input file
    VpxTplGopStats tpl_gop_stats;
    vpx_read_tpl_gop_stats(in_file, &tpl_gop_stats);

    // Encode the image using vpx_codec_encode
    vpx_codec_ctx_t ctx;
    vpx_codec_iface_t *iface = vpx_codec_vp9_cx();
    vpx_codec_enc_cfg_t cfg;
    vpx_codec_enc_config_default(iface, &cfg, 0);
    vpx_codec_enc_init_ver(&ctx, iface, &cfg, 0, 4);
    vpx_codec_pts_t pts = 0;
    vpx_codec_encode(&ctx, &img, pts, 0, 0, VPX_DL_GOOD_QUALITY);

    // Write GOP statistics to output file
    vpx_write_tpl_gop_stats(out_file, &tpl_gop_stats);

    // Free GOP statistics memory
    vpx_free_tpl_gop_stats(&tpl_gop_stats);

    // Get and print codec error
    const char *error = vpx_codec_error(&ctx);
    printf("Codec error: %s\n", error);

    // Cleanup resources
    vpx_codec_destroy(&ctx);
    vpx_img_free(&img);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}