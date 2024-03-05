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
//<ID> 3026
//<Prompt> ["vpx_codec_dec_init_ver","vpx_codec_vp9_dx","vpx_codec_decode","vpx_codec_get_frame","vpx_codec_set_frame_buffer_functions","vpx_codec_enc_init_multi_ver","vpx_img_set_rect","vpx_img_free","vpx_codec_get_stream_info","vpx_codec_destroy"]
/*<Combination>: [vpx_codec_err_t vpx_codec_dec_init_ver(vpx_codec_ctx_t * ctx, vpx_codec_iface_t * iface, const vpx_codec_dec_cfg_t * cfg, vpx_codec_flags_t flags, int ver),
    vpx_codec_iface_t *vpx_codec_vp9_dx(),
    vpx_codec_err_t vpx_codec_decode(vpx_codec_ctx_t * ctx, const uint8_t * data, unsigned int data_sz, void * user_priv, long deadline),
    vpx_image_t *vpx_codec_get_frame(vpx_codec_ctx_t * ctx, vpx_codec_iter_t * iter),
    vpx_codec_err_t vpx_codec_set_frame_buffer_functions(vpx_codec_ctx_t * ctx, vpx_get_frame_buffer_cb_fn_t cb_get, vpx_release_frame_buffer_cb_fn_t cb_release, void * cb_priv),
    vpx_codec_err_t vpx_codec_enc_init_multi_ver(vpx_codec_ctx_t * ctx, vpx_codec_iface_t * iface, vpx_codec_enc_cfg_t * cfg, int num_enc, vpx_codec_flags_t flags, vpx_rational_t * dsf, int ver),
    int vpx_img_set_rect(vpx_image_t * img, unsigned int x, unsigned int y, unsigned int w, unsigned int h),
    void vpx_img_free(vpx_image_t * img),
    vpx_codec_err_t vpx_codec_get_stream_info(vpx_codec_ctx_t * ctx, vpx_codec_stream_info_t * si),
    vpx_codec_err_t vpx_codec_destroy(vpx_codec_ctx_t * ctx)
*/
//<score> 10, nr_unique_branch: 0
//<Quality> {"density":10,"unique_branches":{},"library_calls":["vpx_codec_vp9_dx","vpx_codec_dec_init_ver","vpx_codec_get_stream_info","vpx_img_alloc","vpx_codec_set_frame_buffer_functions","vpx_codec_decode","vpx_codec_get_frame","vpx_img_set_rect","vpx_codec_vp9_cx","vpx_codec_enc_init_multi_ver","vpx_codec_destroy","vpx_img_free","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp9_dx","vpx_codec_dec_init_ver","vpx_codec_get_stream_info","vpx_img_alloc","vpx_codec_set_frame_buffer_functions","vpx_codec_decode","vpx_codec_get_frame","vpx_img_set_rect","vpx_codec_vp9_cx","vpx_codec_enc_init_multi_ver","vpx_codec_destroy","vpx_img_free","vpx_codec_destroy"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_85(const uint8_t *data, size_t size) {
	if(size<0) return 0;

    const unsigned char *in_data = reinterpret_cast<const unsigned char *>(data);
    FILE *in_file = fmemopen((void *)in_data, size, "rb");
    FILE *out_file = fopen("output_file", "wb");
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);

    vpx_codec_ctx_t dec_ctx;
    vpx_codec_dec_cfg_t dec_cfg = {0};
    vpx_codec_iface_t *dec_iface = vpx_codec_vp9_dx();
    vpx_codec_dec_init_ver(&dec_ctx, dec_iface, &dec_cfg, 0, VPX_DECODER_ABI_VERSION);

    vpx_codec_stream_info_t stream_info;
    vpx_codec_get_stream_info(&dec_ctx, &stream_info);

    vpx_image_t *img = vpx_img_alloc(nullptr, VPX_IMG_FMT_I420, stream_info.w, stream_info.h, 1);
    vpx_codec_iter_t iter = nullptr;

    vpx_codec_set_frame_buffer_functions(&dec_ctx, nullptr, nullptr, nullptr);

    vpx_codec_err_t dec_err = vpx_codec_decode(&dec_ctx, in_data, size, nullptr, 0);

    if (dec_err == VPX_CODEC_OK) {
        const vpx_image_t *dec_img = vpx_codec_get_frame(&dec_ctx, &iter);
        if (dec_img) {
            vpx_img_set_rect(img, 0, 0, dec_img->d_w, dec_img->d_h);

            vpx_codec_ctx_t enc_ctx;
            vpx_codec_iface_t *enc_iface = vpx_codec_vp9_cx();
            vpx_codec_enc_cfg_t enc_cfg = {0};
            vpx_codec_enc_init_multi_ver(&enc_ctx, enc_iface, &enc_cfg, 1, 0, nullptr, 2);

            vpx_codec_destroy(&enc_ctx);
        }
    }

    vpx_img_free(img);
    vpx_codec_destroy(&dec_ctx);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}