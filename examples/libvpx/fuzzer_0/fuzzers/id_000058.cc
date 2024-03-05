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
//<ID> 2456
//<Prompt> ["vpx_img_alloc","vpx_codec_vp9_dx","vpx_img_wrap","vpx_codec_encode","vpx_codec_dec_init_ver","vpx_codec_decode","vpx_codec_destroy","vpx_img_set_rect","vpx_img_free"]
/*<Combination>: [vpx_image_t *vpx_img_alloc(vpx_image_t * img, vpx_img_fmt_t fmt, unsigned int d_w, unsigned int d_h, unsigned int align),
    vpx_codec_iface_t *vpx_codec_vp9_dx(),
    vpx_image_t *vpx_img_wrap(vpx_image_t * img, vpx_img_fmt_t fmt, unsigned int d_w, unsigned int d_h, unsigned int stride_align, unsigned char * img_data),
    vpx_codec_err_t vpx_codec_encode(vpx_codec_ctx_t * ctx, const vpx_image_t * img, vpx_codec_pts_t pts, unsigned long duration, vpx_enc_frame_flags_t flags, unsigned long deadline),
    vpx_codec_err_t vpx_codec_dec_init_ver(vpx_codec_ctx_t * ctx, vpx_codec_iface_t * iface, const vpx_codec_dec_cfg_t * cfg, vpx_codec_flags_t flags, int ver),
    vpx_codec_err_t vpx_codec_decode(vpx_codec_ctx_t * ctx, const uint8_t * data, unsigned int data_sz, void * user_priv, long deadline),
    vpx_codec_err_t vpx_codec_destroy(vpx_codec_ctx_t * ctx),
    int vpx_img_set_rect(vpx_image_t * img, unsigned int x, unsigned int y, unsigned int w, unsigned int h),
    void vpx_img_free(vpx_image_t * img)
*/
//<score> 9, nr_unique_branch: 0
//<Quality> {"density":9,"unique_branches":{},"library_calls":["vpx_img_alloc","vpx_codec_dec_init_ver","vpx_codec_vp9_dx","vpx_codec_decode","vpx_codec_get_frame","vpx_codec_enc_init_ver","vpx_codec_vp9_dx","vpx_codec_encode","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy"],"critical_calls":["vpx_img_alloc","vpx_codec_dec_init_ver","vpx_codec_vp9_dx","vpx_codec_decode","vpx_codec_get_frame","vpx_codec_enc_init_ver","vpx_codec_vp9_dx","vpx_codec_encode","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy"],"visited":0}
/**/


// Include the libvpx headers
extern "C" {
}

// The LLVMFuzzerTestOneInput_58 function
extern "C" int LLVMFuzzerTestOneInput_58(const uint8_t *data, size_t size) {
	if(size<0) return 0;

    // Create a FILE pointer for reading the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file == NULL) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Open an output file for writing
    FILE *out_file = fopen("output_file", "wb");
    if (out_file == NULL) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Get the file descriptor for reading and writing
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);

    // Create the vpx_image_t structure
    vpx_image_t image;
    vpx_img_fmt_t fmt = VPX_IMG_FMT_I420;
    unsigned int width = 1280;
    unsigned int height = 720;
    unsigned int align = 32;
    vpx_img_alloc(&image, fmt, width, height, align);

    // Initialize the VP9 decoder
    vpx_codec_ctx_t codec_ctx;
    vpx_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 1;
    dec_cfg.w = width;
    dec_cfg.h = height;
    vpx_codec_dec_init_ver(&codec_ctx, vpx_codec_vp9_dx(), &dec_cfg, 0, VPX_DECODER_ABI_VERSION);

    // Read the input data and decode the frames
    uint8_t *buffer = new uint8_t[size];
    fread(buffer, 1, size, in_file);

    vpx_codec_decode(&codec_ctx, buffer, size, NULL, 0);

    // Get the decoded frame
    vpx_codec_iter_t iter = NULL;
    const vpx_image_t *decoded_frame = vpx_codec_get_frame(&codec_ctx, &iter);

    // Encode the decoded frame
    vpx_codec_ctx_t enc_ctx;
    vpx_codec_enc_cfg_t enc_cfg = {0};
    enc_cfg.g_w = width;
    enc_cfg.g_h = height;
    vpx_codec_enc_init_ver(&enc_ctx, vpx_codec_vp9_dx(), &enc_cfg, 0, VPX_ENCODER_ABI_VERSION);
    
    vpx_codec_encode(&enc_ctx, decoded_frame, 0, 0, 0, 0);

    // Cleanup
    delete[] buffer;
    vpx_img_free(&image);
    vpx_codec_destroy(&codec_ctx);
    vpx_codec_destroy(&enc_ctx);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}