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
//<ID> 1869
//<Prompt> ["vpx_codec_enc_config_default","vpx_codec_vp9_cx","vpx_codec_enc_init_ver","vpx_codec_set_cx_data_buf","vpx_codec_register_put_frame_cb","vpx_codec_version_extra_str","vpx_codec_iface_name","vpx_codec_set_frame_buffer_functions","vpx_codec_enc_init_multi_ver","vpx_codec_vp8_dx","vpx_img_free"]
/*<Combination>: [vpx_codec_err_t vpx_codec_enc_config_default(vpx_codec_iface_t * iface, vpx_codec_enc_cfg_t * cfg, unsigned int usage),
    vpx_codec_iface_t *vpx_codec_vp9_cx(),
    vpx_codec_err_t vpx_codec_enc_init_ver(vpx_codec_ctx_t * ctx, vpx_codec_iface_t * iface, const vpx_codec_enc_cfg_t * cfg, vpx_codec_flags_t flags, int ver),
    vpx_codec_err_t vpx_codec_set_cx_data_buf(vpx_codec_ctx_t * ctx, const vpx_fixed_buf_t * buf, unsigned int pad_before, unsigned int pad_after),
    vpx_codec_err_t vpx_codec_register_put_frame_cb(vpx_codec_ctx_t * ctx, vpx_codec_put_frame_cb_fn_t cb, void * user_priv),
    const char *vpx_codec_version_extra_str(),
    const char *vpx_codec_iface_name(vpx_codec_iface_t * iface),
    vpx_codec_err_t vpx_codec_set_frame_buffer_functions(vpx_codec_ctx_t * ctx, vpx_get_frame_buffer_cb_fn_t cb_get, vpx_release_frame_buffer_cb_fn_t cb_release, void * cb_priv),
    vpx_codec_err_t vpx_codec_enc_init_multi_ver(vpx_codec_ctx_t * ctx, vpx_codec_iface_t * iface, vpx_codec_enc_cfg_t * cfg, int num_enc, vpx_codec_flags_t flags, vpx_rational_t * dsf, int ver),
    vpx_codec_iface_t *vpx_codec_vp8_dx(),
    void vpx_img_free(vpx_image_t * img)
*/
//<score> 19, nr_unique_branch: 0
//<Quality> {"density":19,"unique_branches":{},"library_calls":["vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_set_cx_data_buf","vpx_codec_register_put_frame_cb","vpx_codec_version_extra_str","vpx_codec_iface_name","vpx_codec_set_frame_buffer_functions","vpx_codec_enc_init_multi_ver","vpx_codec_vp9_dx","vpx_codec_dec_init_ver","vpx_codec_peek_stream_info","vpx_codec_get_stream_info","vpx_codec_decode","vpx_img_alloc","vpx_codec_encode","vpx_codec_get_cx_data","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_set_cx_data_buf","vpx_codec_register_put_frame_cb","vpx_codec_version_extra_str","vpx_codec_iface_name","vpx_codec_set_frame_buffer_functions","vpx_codec_enc_init_multi_ver","vpx_codec_vp9_dx","vpx_codec_dec_init_ver","vpx_codec_peek_stream_info","vpx_codec_get_stream_info","vpx_codec_decode","vpx_img_alloc","vpx_codec_encode","vpx_codec_get_cx_data","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_39(const uint8_t* data, size_t size)
{
	if(size<0) return 0;

    // Create a file for reading the input data
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (in_file == NULL) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Create an output file for writing the encoded data
    FILE* out_file = fopen("output_file", "wb");
    if (out_file == NULL) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Get the file descriptors
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);

    // Initialize libvpx encoder
    vpx_codec_ctx_t encoder;
    vpx_codec_iface_t* encoder_iface = vpx_codec_vp9_cx();
    vpx_codec_enc_cfg_t encoder_cfg;
    vpx_codec_enc_config_default(encoder_iface, &encoder_cfg, 0);
    vpx_codec_enc_init_ver(&encoder, encoder_iface, &encoder_cfg, 0, VPX_ENCODER_ABI_VERSION);
    vpx_codec_set_cx_data_buf(&encoder, NULL, 0, 0);
    vpx_codec_register_put_frame_cb(&encoder, NULL, NULL);
    const char* version_extra_str = vpx_codec_version_extra_str();
    const char* iface_name = vpx_codec_iface_name(encoder_iface);
    vpx_codec_set_frame_buffer_functions(&encoder, NULL, NULL, NULL);
    vpx_codec_enc_init_multi_ver(&encoder, encoder_iface, &encoder_cfg, 1, 0, NULL, VPX_ENCODER_ABI_VERSION);

    // Decode the input data
    vpx_codec_ctx_t decoder;
    vpx_codec_iface_t* decoder_iface = vpx_codec_vp9_dx();
    vpx_codec_dec_init_ver(&decoder, decoder_iface, NULL, 0, VPX_DECODER_ABI_VERSION);
    vpx_codec_stream_info_t si;
    vpx_codec_peek_stream_info(decoder_iface, data, size, &si);
    vpx_codec_get_stream_info(&decoder, &si);
    vpx_codec_decode(&decoder, data, size, NULL, -1);

    // Create an image buffer
    vpx_image_t image;
    vpx_img_alloc(&image, VPX_IMG_FMT_I420, si.w, si.h, 1);

    // Encode the image
    vpx_codec_encode(&encoder, &image, 0, 0, 0, -1);

    // Get the encoded data
    const vpx_codec_cx_pkt_t* pkt;
    vpx_codec_iter_t iter = NULL;
    while ((pkt = vpx_codec_get_cx_data(&encoder, &iter)) != NULL) {
        if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
            // Write the encoded data to the output file
            fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
        }
    }

    // Free the image buffer
    vpx_img_free(&image);

    // Release all allocated resources
    vpx_codec_destroy(&encoder);
    vpx_codec_destroy(&decoder);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}