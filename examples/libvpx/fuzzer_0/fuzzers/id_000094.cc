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
//<ID> 3278
//<Prompt> ["vpx_codec_dec_init_ver","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_register_put_frame_cb","vpx_read_tpl_gop_stats","vpx_img_wrap","vpx_img_set_rect","vpx_codec_enc_init_multi_ver","vpx_codec_set_frame_buffer_functions","vpx_read_tpl_gop_stats","vpx_codec_destroy","vpx_img_free"]
/*<Combination>: [vpx_codec_err_t vpx_codec_dec_init_ver(vpx_codec_ctx_t * ctx, vpx_codec_iface_t * iface, const vpx_codec_dec_cfg_t * cfg, vpx_codec_flags_t flags, int ver),
    vpx_codec_err_t vpx_codec_enc_config_default(vpx_codec_iface_t * iface, vpx_codec_enc_cfg_t * cfg, unsigned int usage),
    vpx_codec_err_t vpx_codec_enc_init_ver(vpx_codec_ctx_t * ctx, vpx_codec_iface_t * iface, const vpx_codec_enc_cfg_t * cfg, vpx_codec_flags_t flags, int ver),
    vpx_codec_err_t vpx_codec_register_put_frame_cb(vpx_codec_ctx_t * ctx, vpx_codec_put_frame_cb_fn_t cb, void * user_priv),
    vpx_codec_err_t vpx_read_tpl_gop_stats(FILE * tpl_file, VpxTplGopStats * tpl_gop_stats),
    vpx_image_t *vpx_img_wrap(vpx_image_t * img, vpx_img_fmt_t fmt, unsigned int d_w, unsigned int d_h, unsigned int stride_align, unsigned char * img_data),
    int vpx_img_set_rect(vpx_image_t * img, unsigned int x, unsigned int y, unsigned int w, unsigned int h),
    vpx_codec_err_t vpx_codec_enc_init_multi_ver(vpx_codec_ctx_t * ctx, vpx_codec_iface_t * iface, vpx_codec_enc_cfg_t * cfg, int num_enc, vpx_codec_flags_t flags, vpx_rational_t * dsf, int ver),
    vpx_codec_err_t vpx_codec_set_frame_buffer_functions(vpx_codec_ctx_t * ctx, vpx_get_frame_buffer_cb_fn_t cb_get, vpx_release_frame_buffer_cb_fn_t cb_release, void * cb_priv),
    vpx_codec_err_t vpx_read_tpl_gop_stats(FILE * tpl_file, VpxTplGopStats * tpl_gop_stats),
    vpx_codec_err_t vpx_codec_destroy(vpx_codec_ctx_t * ctx),
    void vpx_img_free(vpx_image_t * img)
*/
//<score> 10, nr_unique_branch: 0
//<Quality> {"density":10,"unique_branches":{},"library_calls":["vpx_codec_dec_init_ver","vpx_codec_vp9_dx","vpx_codec_enc_config_default","vpx_codec_vp9_cx","vpx_codec_enc_init_ver","vpx_codec_vp9_cx","vpx_img_wrap","vpx_img_set_rect","vpx_codec_encode","vpx_codec_decode","vpx_codec_get_cx_data","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy"],"critical_calls":["vpx_codec_dec_init_ver","vpx_codec_vp9_dx","vpx_codec_enc_config_default","vpx_codec_vp9_cx","vpx_codec_enc_init_ver","vpx_codec_vp9_cx","vpx_img_wrap","vpx_img_set_rect","vpx_codec_encode","vpx_codec_decode","vpx_codec_get_cx_data","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy"],"visited":0}
/**/


// Define the input and output file names
const std::string  id_000094_inputFileName = "input_file";
const std::string  id_000094_outputFileName = "output_file";

extern "C" int LLVMFuzzerTestOneInput_94(const uint8_t *data, size_t size) {
	if(size<0) return 0;

    // Create and open the input file
    FILE *inFile = fmemopen((void *)data, size, "rb");
    if (inFile == nullptr) {
        std::cerr << "Failed to open input file" << std::endl;
        assert_file_closed(&inFile);
	return 0;
    }

    // Create and open the output file
    FILE *outFile = fopen( id_000094_outputFileName.c_str(), "wb");
    if (outFile == nullptr) {
        std::cerr << "Failed to open output file" << std::endl;
        assert_file_closed(&inFile);;
        assert_file_closed(&outFile);
	assert_file_closed(&inFile);
	return 0;
    }

    // Create a decoder context
    vpx_codec_ctx_t decoder;
    vpx_codec_dec_cfg_t decoderConfig;
    decoderConfig.w = 640;
    decoderConfig.h = 480;
    vpx_codec_dec_init_ver(&decoder, vpx_codec_vp9_dx(), &decoderConfig, 0, VPX_DECODER_ABI_VERSION);

    // Create an encoder context
    vpx_codec_ctx_t encoder;
    vpx_codec_enc_cfg_t encoderConfig;
    vpx_codec_enc_config_default(vpx_codec_vp9_cx(), &encoderConfig, 0);
    vpx_codec_enc_init_ver(&encoder, vpx_codec_vp9_cx(), &encoderConfig, 0, VPX_ENCODER_ABI_VERSION);

    // Define the image format and dimensions
    vpx_image_t image;
    vpx_img_fmt_t fmt = VPX_IMG_FMT_I420;
    unsigned int width = 640;
    unsigned int height = 480;

    // Wrap the image data with the defined format and dimensions
    vpx_img_wrap(&image, fmt, width, height, 1, nullptr);

    // Set the image rectangle
    vpx_img_set_rect(&image, 0, 0, width, height);

    // Encode the image
    vpx_codec_encode(&encoder, &image, 0, 1, 0, 0);

    // Decode the encoded data
    vpx_codec_decode(&decoder, data, size, nullptr, 0);

    // Write the output data to the output file
    const vpx_codec_cx_pkt_t *pkt = nullptr;
    while ((pkt = vpx_codec_get_cx_data(&encoder, nullptr)) != nullptr) {
        fwrite(pkt->data.frame.buf, pkt->data.frame.sz, 1, outFile);
    }

    // Free the image
    vpx_img_free(&image);

    // Destroy the encoder and decoder contexts
    vpx_codec_destroy(&encoder);
    vpx_codec_destroy(&decoder);

    // Close the input and output files
    assert_file_closed(&inFile);;
    assert_file_closed(&outFile);;

    assert_file_closed(&outFile);
	assert_file_closed(&inFile);
	return 0;
}