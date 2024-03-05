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
//<ID> 3048
//<Prompt> ["vpx_codec_enc_config_default","vpx_codec_vp8_cx","vpx_codec_enc_init_ver","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_vp8_dx","vpx_img_wrap","vpx_codec_peek_stream_info","vpx_write_tpl_gop_stats","vpx_codec_get_cx_data","vpx_codec_destroy","vpx_img_free"]
/*<Combination>: [vpx_codec_err_t vpx_codec_enc_config_default(vpx_codec_iface_t * iface, vpx_codec_enc_cfg_t * cfg, unsigned int usage),
    vpx_codec_iface_t *vpx_codec_vp8_cx(),
    vpx_codec_err_t vpx_codec_enc_init_ver(vpx_codec_ctx_t * ctx, vpx_codec_iface_t * iface, const vpx_codec_enc_cfg_t * cfg, vpx_codec_flags_t flags, int ver),
    vpx_codec_err_t vpx_codec_enc_config_default(vpx_codec_iface_t * iface, vpx_codec_enc_cfg_t * cfg, unsigned int usage),
    vpx_codec_err_t vpx_codec_enc_init_ver(vpx_codec_ctx_t * ctx, vpx_codec_iface_t * iface, const vpx_codec_enc_cfg_t * cfg, vpx_codec_flags_t flags, int ver),
    vpx_codec_iface_t *vpx_codec_vp8_dx(),
    vpx_image_t *vpx_img_wrap(vpx_image_t * img, vpx_img_fmt_t fmt, unsigned int d_w, unsigned int d_h, unsigned int stride_align, unsigned char * img_data),
    vpx_codec_err_t vpx_codec_peek_stream_info(vpx_codec_iface_t * iface, const uint8_t * data, unsigned int data_sz, vpx_codec_stream_info_t * si),
    vpx_codec_err_t vpx_write_tpl_gop_stats(FILE * tpl_file, const VpxTplGopStats * tpl_gop_stats),
    const vpx_codec_cx_pkt_t *vpx_codec_get_cx_data(vpx_codec_ctx_t * ctx, vpx_codec_iter_t * iter),
    vpx_codec_err_t vpx_codec_destroy(vpx_codec_ctx_t * ctx),
    void vpx_img_free(vpx_image_t * img)
*/
//<score> 35.75, nr_unique_branch: 57
//<Quality> {"density":11,"unique_branches":{"calc_partial_ssl_err":[[68,17,68,28,0,0,4,0],[80,15,80,30,0,0,4,0],[80,15,80,30,0,0,4,1],[81,17,81,36,0,0,4,0],[81,17,81,36,0,0,4,1]],"vp8_loopfilter_frame":[[3134,9,3134,33,0,0,4,0],[3136,11,3136,38,0,0,4,1]],"vp8_set_speed_features":[[943,11,943,20,0,0,4,0],[1030,7,1030,37,0,0,4,1],[1033,14,1033,46,0,0,4,1],[1056,7,1056,39,0,0,4,1],[1058,14,1058,42,0,0,4,0]],"vp8_auto_select_speed":[[284,9,284,37,0,0,4,1],[287,11,287,70,0,0,4,0],[287,11,287,70,0,0,4,1],[292,13,292,28,0,0,4,1],[297,11,298,63,0,0,4,0],[297,11,298,63,0,0,4,1],[304,13,304,27,0,0,4,0],[304,13,304,27,0,0,4,1]],"vp8e_encode":[[904,7,904,13,0,0,4,1],[947,17,947,45,0,0,4,0]],"vp8cx_pick_filter_level_fast":[[145,7,145,34,0,0,4,0],[151,7,151,54,0,0,4,1],[159,7,159,42,0,0,4,1],[161,14,161,49,0,0,4,1],[179,10,179,38,0,0,4,0],[188,9,188,28,0,0,4,0],[188,9,188,28,0,0,4,1],[202,7,202,40,0,0,4,0],[202,7,202,40,0,0,4,1],[206,12,206,39,0,0,4,0],[216,11,216,30,0,0,4,1],[234,7,234,42,0,0,4,1],[236,7,236,42,0,0,4,1]],"encode_frame_to_data_rate":[[3276,41,3276,75,0,0,4,0]],"yv12_copy_partial_frame":[[41,17,41,28,0,0,4,0]],"vp8_pack_tokens":[[167,28,167,48,0,0,4,0]],"vp8_loop_filter_partial_frame":[[504,17,504,28,0,0,4,0],[511,20,511,47,0,0,4,0],[511,20,511,47,0,0,4,1],[512,22,512,38,0,0,4,0],[512,22,512,38,0,0,4,1],[513,22,513,60,0,0,4,0],[513,22,513,60,0,0,4,1],[514,22,514,61,0,0,4,0],[515,22,515,59,0,0,4,1],[523,11,523,23,0,0,4,0],[524,13,524,49,0,0,4,0],[531,15,531,25,0,0,4,0],[531,15,531,25,0,0,4,1],[534,15,534,23,0,0,4,0],[539,15,539,23,0,0,4,0]],"vp8_get_compressed_data":[[5065,11,5065,36,0,0,4,1],[5074,13,5074,41,0,0,4,1]],"calc_iframe_target_size":[[324,13,324,55,0,0,4,0],[347,9,347,22,0,0,4,0]]},"library_calls":["vpx_codec_enc_config_default","vpx_codec_vp8_cx","vpx_codec_enc_init_ver","vpx_codec_vp8_cx","vpx_codec_destroy","vpx_img_wrap","vpx_codec_destroy","vpx_codec_encode","vpx_codec_destroy","vpx_codec_get_cx_data","vpx_codec_destroy"],"critical_calls":["vpx_codec_enc_config_default","vpx_codec_vp8_cx","vpx_codec_enc_init_ver","vpx_codec_vp8_cx","vpx_img_wrap","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_destroy"],"visited":3}
/**/

extern "C" int LLVMFuzzerTestOneInput_88(const uint8_t *data, size_t size) {
	if(size<0) return 0;

    // Step 1: Initialize variables
    vpx_codec_ctx_t ctx;
    vpx_codec_err_t res;
    vpx_image_t img;

    // Step 2: Create a file to read the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Step 3: Set up codec configuration
    vpx_codec_enc_cfg_t cfg;
    res = vpx_codec_enc_config_default(vpx_codec_vp8_cx(), &cfg, 0);
    if (res != VPX_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // Step 4: Initialize encoder
    res = vpx_codec_enc_init_ver(&ctx, vpx_codec_vp8_cx(), &cfg, 0, VPX_ENCODER_ABI_VERSION);
    if (res != VPX_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // Step 5: Create a file to write the output data
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        vpx_codec_destroy(&ctx);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Step 6: Encode frames
    // Read first frame from input file
    vpx_img_fmt_t fmt = VPX_IMG_FMT_I420;
    unsigned int width = cfg.g_w;
    unsigned int height = cfg.g_h;
    unsigned int stride_align = 1;
    unsigned char *img_data = NULL;
    vpx_codec_stream_info_t si;
    while (fread(&si, sizeof(si), 1, in_file) == 1) {
        img_data = (unsigned char *)realloc(img_data, width * height * 3 / 2);
        fread(img_data, sizeof(unsigned char), width * height * 3 / 2, in_file);

        // Wrap the image data
        vpx_image_t *img_wrap = vpx_img_wrap(&img, fmt, width, height, stride_align, img_data);
        if (!img_wrap) {
            free(img_data);
            vpx_codec_destroy(&ctx);
            assert_file_closed(&in_file);;
            assert_file_closed(&out_file);;
            assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
        }

        // Encode the frame
        res = vpx_codec_encode(&ctx, img_wrap, 0, 1, VPX_EFLAG_FORCE_KF, VPX_DL_REALTIME);
        if (res != VPX_CODEC_OK) {
            free(img_data);
            vpx_codec_destroy(&ctx);
            assert_file_closed(&in_file);;
            assert_file_closed(&out_file);;
            assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
        }

        // Get compressed data
        const vpx_codec_cx_pkt_t *pkt;
        vpx_codec_iter_t iter = NULL;
        while ((pkt = vpx_codec_get_cx_data(&ctx, &iter)) != NULL) {
            if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
                fwrite(pkt->data.frame.buf, sizeof(unsigned char), pkt->data.frame.sz, out_file);
            }
        }
    }

    // Step 7: Release resources
    free(img_data);
    vpx_codec_destroy(&ctx);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}