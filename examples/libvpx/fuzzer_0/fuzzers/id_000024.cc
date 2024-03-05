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
//<ID> 1140
//<Prompt> ["vpx_codec_enc_config_default","vpx_codec_vp9_cx","vpx_codec_set_frame_buffer_functions","vpx_codec_enc_init_ver","vpx_codec_control_","vpx_codec_version_extra_str","vpx_codec_error","vpx_codec_get_cx_data","vpx_codec_destroy","vpx_codec_register_put_slice_cb","vpx_img_free"]
/*<Combination>: [vpx_codec_err_t vpx_codec_enc_config_default(vpx_codec_iface_t * iface, vpx_codec_enc_cfg_t * cfg, unsigned int usage),
    vpx_codec_iface_t *vpx_codec_vp9_cx(),
    vpx_codec_err_t vpx_codec_set_frame_buffer_functions(vpx_codec_ctx_t * ctx, vpx_get_frame_buffer_cb_fn_t cb_get, vpx_release_frame_buffer_cb_fn_t cb_release, void * cb_priv),
    vpx_codec_err_t vpx_codec_enc_init_ver(vpx_codec_ctx_t * ctx, vpx_codec_iface_t * iface, const vpx_codec_enc_cfg_t * cfg, vpx_codec_flags_t flags, int ver),
    vpx_codec_err_t vpx_codec_control_(vpx_codec_ctx_t * ctx, int ctrl_id),
    const char *vpx_codec_version_extra_str(),
    const char *vpx_codec_error(const vpx_codec_ctx_t * ctx),
    const vpx_codec_cx_pkt_t *vpx_codec_get_cx_data(vpx_codec_ctx_t * ctx, vpx_codec_iter_t * iter),
    vpx_codec_err_t vpx_codec_destroy(vpx_codec_ctx_t * ctx),
    vpx_codec_err_t vpx_codec_register_put_slice_cb(vpx_codec_ctx_t * ctx, vpx_codec_put_slice_cb_fn_t cb, void * user_priv),
    void vpx_img_free(vpx_image_t * img)
*/
//<score> 7.5, nr_unique_branch: 2
//<Quality> {"density":10,"unique_branches":{"encoder_encode":[[1331,7,1331,35,0,0,4,1]],"vpx_codec_control_":[[91,15,91,23,0,0,4,0]]},"library_calls":["vpx_codec_enc_config_default","vpx_codec_vp9_cx","vpx_codec_set_frame_buffer_functions","vpx_codec_enc_init_ver","vpx_codec_vp9_cx","vpx_codec_register_put_slice_cb","vpx_codec_version_extra_str","vpx_codec_control_","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_destroy","vpx_img_free"],"critical_calls":["vpx_codec_enc_config_default","vpx_codec_vp9_cx","vpx_codec_set_frame_buffer_functions","vpx_codec_enc_init_ver","vpx_codec_vp9_cx","vpx_codec_register_put_slice_cb","vpx_codec_version_extra_str","vpx_codec_control_","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_destroy","vpx_img_free"],"visited":3}
/**/

extern "C" int LLVMFuzzerTestOneInput_24(const uint8_t *data, size_t size) {
	if(size<0) return 0;

    // Create a FILE * variable for reading input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file == nullptr) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Create a FILE * variable for writing output data
    FILE *out_file = fopen("output_file", "wb");
    if (out_file == nullptr) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Create vpx_codec_enc_cfg_t variable and set default configurations
    vpx_codec_enc_cfg_t enc_cfg;
    vpx_codec_enc_config_default(vpx_codec_vp9_cx(), &enc_cfg, 0);

    // Create a vpx_codec_ctx_t variable
    vpx_codec_ctx_t ctx;

    // Set frame buffer functions
    vpx_codec_set_frame_buffer_functions(&ctx, nullptr, nullptr, nullptr);

    // Initialize encoder with the configurations
    if (vpx_codec_enc_init_ver(&ctx, vpx_codec_vp9_cx(), &enc_cfg, 0, VPX_ENCODER_ABI_VERSION) != VPX_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Register put slice callback function
    vpx_codec_register_put_slice_cb(&ctx, nullptr, nullptr);

    // Get codec version extra string
    const char *version_extra_str = vpx_codec_version_extra_str();

    // Control the codec context
    vpx_codec_control_(&ctx, 0);

    // Read input data and encode frames
    while (!feof(in_file)) {
        // Read input data from file
        uint8_t input_data[4096];
        size_t input_size = fread(input_data, 1, sizeof(input_data), in_file);

        // Encode the frame
        vpx_codec_encode(&ctx, nullptr, 0, 0, 0, VPX_DL_REALTIME);

        // Get encoded data
        vpx_codec_iter_t iter = nullptr;
        const vpx_codec_cx_pkt_t *pkt = nullptr;
        while ((pkt = vpx_codec_get_cx_data(&ctx, &iter)) != nullptr) {
            if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
                // Write encoded data to output file
                fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
            }
        }
    }

    // Destroy the codec context
    vpx_codec_destroy(&ctx);

    // Free allocated image memory
    vpx_image_t img;
    vpx_img_free(&img);

    // Close input and output files
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}