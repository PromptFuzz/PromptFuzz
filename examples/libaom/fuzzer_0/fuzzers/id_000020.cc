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
//<ID> 821
//<Prompt> ["aom_codec_enc_config_set","aom_img_metadata_alloc","aom_codec_peek_stream_info","aom_codec_destroy","aom_codec_av1_cx"]
/*<Combination>: [aom_codec_err_t aom_codec_enc_config_set(aom_codec_ctx_t * ctx, const aom_codec_enc_cfg_t * cfg),
    aom_metadata_t *aom_img_metadata_alloc(uint32_t type, const uint8_t * data, size_t sz, aom_metadata_insert_flags_t insert_flag),
    aom_codec_err_t aom_codec_peek_stream_info(aom_codec_iface_t * iface, const uint8_t * data, size_t data_sz, aom_codec_stream_info_t * si),
    aom_codec_err_t aom_codec_destroy(aom_codec_ctx_t * ctx),
    aom_codec_iface_t *aom_codec_av1_cx()
*/
//<score> 0.87272733, nr_unique_branch: 1, p_cov: 0.8
//<Quality> {"diversity":11,"density":6,"covered_api":["aom_codec_enc_config_set","aom_codec_peek_stream_info","aom_codec_destroy","aom_codec_av1_cx"],"uncovered_api":["aom_img_metadata_alloc"],"unique_branches":{"aom_codec_enc_config_set":[[297,44,297,48,0]]},"library_calls":["aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_peek_stream_info","aom_codec_enc_init_ver","aom_codec_enc_config_set","aom_codec_destroy","aom_codec_destroy"]}
/**/


extern "C" int LLVMFuzzerTestOneInput_20(const uint8_t *data, size_t size) {
	if(size<=0) return 0;

    FILE *in_file = fmemopen((void *)data, size, "rb");
    FILE *out_file = fopen("output_file", "wb");

    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);

    aom_codec_ctx_t encoder;
    aom_codec_iface_t *encoder_iface = aom_codec_av1_cx();

    aom_codec_enc_cfg_t encoder_cfg;
    aom_codec_err_t encoder_cfg_result = aom_codec_enc_config_default(encoder_iface, &encoder_cfg, 0);
    if (encoder_cfg_result != AOM_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0; // Error handling
    }

    aom_codec_ctx_t decoder;
    aom_codec_iface_t *decoder_iface = aom_codec_av1_dx();

    aom_codec_err_t decoder_init_result = aom_codec_dec_init_ver(&decoder, decoder_iface, NULL, 0, AOM_DECODER_ABI_VERSION);
    if (decoder_init_result != AOM_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0; // Error handling
    }

    aom_codec_stream_info_t stream_info;
    aom_codec_err_t stream_info_result = aom_codec_peek_stream_info(decoder_iface, (const uint8_t *)data, size, &stream_info);
    if (stream_info_result != AOM_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        aom_codec_destroy(&decoder);
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0; // Error handling
    }

    aom_codec_ctx_t *encoder_ctx = &encoder;
    aom_codec_err_t encoder_init_result = aom_codec_enc_init_ver(encoder_ctx, encoder_iface, &encoder_cfg, 0, AOM_ENCODER_ABI_VERSION);
    if (encoder_init_result != AOM_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        aom_codec_destroy(&decoder);
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0; // Error handling
    }

    aom_codec_enc_cfg_t *encoder_cfg_ptr;
    aom_codec_enc_config_set(encoder_ctx, encoder_cfg_ptr);

    // Perform encoding and decoding operations
    // ...

    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    aom_codec_destroy(&encoder);
    aom_codec_destroy(&decoder);

    assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}