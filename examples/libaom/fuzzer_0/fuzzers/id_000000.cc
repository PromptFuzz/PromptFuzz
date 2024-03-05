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
//<ID> 2
//<Prompt> ["aom_codec_err_to_string","aom_uleb_decode","aom_img_metadata_free","aom_codec_enc_config_default","aom_codec_get_frame"]
/*<Combination>: [const char *aom_codec_err_to_string(aom_codec_err_t err),
    int aom_uleb_decode(const uint8_t * buffer, size_t available, uint64_t * value, size_t * length),
    void aom_img_metadata_free(aom_metadata_t * metadata),
    aom_codec_err_t aom_codec_enc_config_default(aom_codec_iface_t * iface, aom_codec_enc_cfg_t * cfg, unsigned int usage),
    aom_image_t *aom_codec_get_frame(aom_codec_ctx_t * ctx, aom_codec_iter_t * iter)
*/
//<score> 0.6, nr_unique_branch: 2, p_cov: 0.2
//<Quality> {"diversity":9,"density":9,"covered_api":["aom_codec_enc_config_default"],"uncovered_api":["aom_codec_err_to_string","aom_uleb_decode","aom_img_metadata_free","aom_codec_get_frame"],"unique_branches":{"aom_codec_enc_init_ver":[[68,12,68,40,0],[68,44,68,79,1]]},"library_calls":["aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_img_alloc","aom_codec_encode","aom_codec_get_cx_data","aom_img_free","aom_codec_destroy"]}
/**/


// Include libaom headers

extern "C" int LLVMFuzzerTestOneInput_0(const uint8_t *data, size_t size) {
	if(size<=0) return 0;

    // Create a file from the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Get the file descriptor for reading and writing
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);

    // Initialize libaom encoder context
    aom_codec_ctx_t encoder_ctx;
    memset(&encoder_ctx, 0, sizeof(aom_codec_ctx_t));
    aom_codec_iface_t *encoder_iface = aom_codec_av1_cx();

    // Set encoder configuration
    aom_codec_enc_cfg_t encoder_cfg;
    memset(&encoder_cfg, 0, sizeof(aom_codec_enc_cfg_t));
    aom_codec_enc_config_default(encoder_iface, &encoder_cfg, 0);

    // Initialize encoder
    aom_codec_err_t encoder_init_result = aom_codec_enc_init_ver(
            &encoder_ctx, encoder_iface, &encoder_cfg, AOM_CODEC_USE_PSNR, AOM_ENCODER_ABI_VERSION
    );
    if (encoder_init_result != AOM_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }

    // Create an image to hold the frame
    aom_image_t *frame = aom_img_alloc(NULL, AOM_IMG_FMT_I420, encoder_cfg.g_w, encoder_cfg.g_h, 32);
    if (!frame) {
        aom_codec_destroy(&encoder_ctx);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }

    // Encode the frame
    aom_codec_encode(&encoder_ctx, frame, 0, 0, 0);

    // Get the encoded data
    const aom_codec_cx_pkt_t *encoded_data;
    for (;;) {
        encoded_data = aom_codec_get_cx_data(&encoder_ctx, NULL);
        if (!encoded_data) {
            break;
        }

        // Write encoded data to output file
        fwrite(encoded_data->data.frame.buf, 1, encoded_data->data.frame.sz, out_file);
    }

    // Cleanup
    aom_img_free(frame);
    aom_codec_destroy(&encoder_ctx);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;

    assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}