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
//<ID> 1813
//<Prompt> ["vpx_codec_dec_init_ver","vpx_codec_vp9_dx","vpx_codec_peek_stream_info","vpx_codec_vp8_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_img_wrap","vpx_img_set_rect","vpx_codec_decode","vpx_codec_get_preview_frame","vpx_img_free","vpx_codec_destroy"]
/*<Combination>: [vpx_codec_err_t vpx_codec_dec_init_ver(vpx_codec_ctx_t * ctx, vpx_codec_iface_t * iface, const vpx_codec_dec_cfg_t * cfg, vpx_codec_flags_t flags, int ver),
    vpx_codec_iface_t *vpx_codec_vp9_dx(),
    vpx_codec_err_t vpx_codec_peek_stream_info(vpx_codec_iface_t * iface, const uint8_t * data, unsigned int data_sz, vpx_codec_stream_info_t * si),
    vpx_codec_iface_t *vpx_codec_vp8_cx(),
    vpx_codec_err_t vpx_codec_enc_config_default(vpx_codec_iface_t * iface, vpx_codec_enc_cfg_t * cfg, unsigned int usage),
    vpx_codec_err_t vpx_codec_enc_init_ver(vpx_codec_ctx_t * ctx, vpx_codec_iface_t * iface, const vpx_codec_enc_cfg_t * cfg, vpx_codec_flags_t flags, int ver),
    vpx_image_t *vpx_img_wrap(vpx_image_t * img, vpx_img_fmt_t fmt, unsigned int d_w, unsigned int d_h, unsigned int stride_align, unsigned char * img_data),
    int vpx_img_set_rect(vpx_image_t * img, unsigned int x, unsigned int y, unsigned int w, unsigned int h),
    vpx_codec_err_t vpx_codec_decode(vpx_codec_ctx_t * ctx, const uint8_t * data, unsigned int data_sz, void * user_priv, long deadline),
    const vpx_image_t *vpx_codec_get_preview_frame(vpx_codec_ctx_t * ctx),
    void vpx_img_free(vpx_image_t * img),
    vpx_codec_err_t vpx_codec_destroy(vpx_codec_ctx_t * ctx)
*/
//<score> 16, nr_unique_branch: 0
//<Quality> {"density":16,"unique_branches":{},"library_calls":["vpx_codec_dec_init_ver","vpx_codec_vp9_dx","vpx_codec_peek_stream_info","vpx_codec_vp9_dx","vpx_codec_enc_config_default","vpx_codec_vp8_cx","vpx_codec_enc_init_ver","vpx_codec_vp8_cx","vpx_img_wrap","vpx_img_set_rect","vpx_codec_decode","vpx_codec_get_preview_frame","vpx_codec_encode","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy"],"critical_calls":["vpx_codec_dec_init_ver","vpx_codec_vp9_dx","vpx_codec_peek_stream_info","vpx_codec_vp9_dx","vpx_codec_enc_config_default","vpx_codec_vp8_cx","vpx_codec_enc_init_ver","vpx_codec_vp8_cx","vpx_img_wrap","vpx_img_set_rect","vpx_codec_decode","vpx_codec_get_preview_frame","vpx_codec_encode","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy"],"visited":0}
/*Here is the complete C++ program that achieves the given event using the libvpx library APIs:

*/


extern "C" int LLVMFuzzerTestOneInput_36(const uint8_t *data, size_t size) {
	if(size<0) return 0;

    FILE *in_file = fmemopen((void *)data, size, "rb");
    FILE *out_file = fopen("output_file", "wb");

    // Decode Initialization
    vpx_codec_ctx_t decoder_ctx;
    vpx_codec_dec_cfg_t decoder_cfg;
    vpx_codec_stream_info_t stream_info;

    vpx_codec_dec_init_ver(&decoder_ctx, vpx_codec_vp9_dx(), &decoder_cfg, 0, VPX_DECODER_ABI_VERSION);
    vpx_codec_peek_stream_info(vpx_codec_vp9_dx(), data, size, &stream_info);

    // Encode Initialization
    vpx_codec_enc_cfg_t encoder_cfg;
    vpx_codec_ctx_t encoder_ctx;

    vpx_codec_enc_config_default(vpx_codec_vp8_cx(), &encoder_cfg, 0);
    vpx_codec_enc_init_ver(&encoder_ctx, vpx_codec_vp8_cx(), &encoder_cfg, 0, VPX_ENCODER_ABI_VERSION);

    // Create Image
    vpx_image_t image;
    vpx_img_wrap(&image, VPX_IMG_FMT_I420, stream_info.w, stream_info.h, 1, NULL);

    // Set Image Rectangle
    vpx_img_set_rect(&image, 0, 0, stream_info.w, stream_info.h);

    // Decode the Input Data
    vpx_codec_decode(&decoder_ctx, data, size, NULL, 0);

    // Get and Write Preview Frame
    const vpx_image_t *preview_frame = vpx_codec_get_preview_frame(&decoder_ctx);
    if (preview_frame) {
        fwrite(preview_frame->img_data, preview_frame->d_w * preview_frame->d_h * 3 / 2, 1, out_file);
    }

    // Encode the Image
    vpx_codec_encode(&encoder_ctx, &image, 0, 1, 0, -1);

    // Cleanup
    vpx_img_free(&image);
    vpx_codec_destroy(&encoder_ctx);
    vpx_codec_destroy(&decoder_ctx);

    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}