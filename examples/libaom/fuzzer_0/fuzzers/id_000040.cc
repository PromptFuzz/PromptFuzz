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
//<ID> 1537
//<Prompt> ["aom_codec_set_option","aom_codec_enc_config_default","aom_uleb_decode","aom_codec_enc_init_ver","aom_codec_get_global_headers"]
/*<Combination>: [aom_codec_err_t aom_codec_set_option(aom_codec_ctx_t * ctx, const char * name, const char * value),
    aom_codec_err_t aom_codec_enc_config_default(aom_codec_iface_t * iface, aom_codec_enc_cfg_t * cfg, unsigned int usage),
    int aom_uleb_decode(const uint8_t * buffer, size_t available, uint64_t * value, size_t * length),
    aom_codec_err_t aom_codec_enc_init_ver(aom_codec_ctx_t * ctx, aom_codec_iface_t * iface, const aom_codec_enc_cfg_t * cfg, aom_codec_flags_t flags, int ver),
    aom_fixed_buf_t *aom_codec_get_global_headers(aom_codec_ctx_t * ctx)
*/
//<score> 2.694737, nr_unique_branch: 3, p_cov: 0.8
//<Quality> {"diversity":19,"density":16,"covered_api":["aom_codec_set_option","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_get_global_headers"],"uncovered_api":["aom_uleb_decode"],"unique_branches":{"ctrl_set_tile_rows":[[1665,7,1665,44,1]],"set_tile_info":[[30,23,30,34,1],[30,23,30,34,1]]},"library_calls":["aom_codec_set_option","aom_codec_enc_config_default","aom_codec_av1_cx","aom_codec_peek_stream_info","aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_av1_dx","aom_codec_decode","aom_codec_get_frame","aom_codec_enc_init_ver","aom_codec_av1_cx","aom_codec_control","aom_codec_control","aom_codec_encode","aom_codec_get_cx_data","aom_codec_get_global_headers","aom_codec_destroy","aom_codec_destroy","aom_codec_destroy"]}
/*Here is a step-by-step implementation of the fuzz driver using the libaom library APIs:

*/


extern "C" int LLVMFuzzerTestOneInput_40(const uint8_t *data, size_t size) {
	if(size<=0) return 0;

    // Create FILE * for reading input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file == nullptr) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Create aom_codec_ctx_t for encoding/decoding
    aom_codec_ctx_t codec_ctx;
    memset(&codec_ctx, 0, sizeof(codec_ctx));

    // Set encoding/decoding options
    aom_codec_set_option(&codec_ctx, "option_name", "option_value");

    // Set encoding configuration
    aom_codec_enc_cfg_t enc_cfg;
    aom_codec_enc_config_default(aom_codec_av1_cx(), &enc_cfg, 0);

    // Decode the input data
    aom_codec_stream_info_t stream_info;
    aom_codec_peek_stream_info(aom_codec_av1_dx(), data, size, &stream_info);
    aom_codec_ctx_t dec_ctx;
    memset(&dec_ctx, 0, sizeof(dec_ctx));
    aom_codec_dec_init_ver(&dec_ctx, aom_codec_av1_dx(), nullptr, 0, AOM_DECODER_ABI_VERSION);
    aom_codec_decode(&dec_ctx, data, size, nullptr);
    const aom_image_t *decoded_image = aom_codec_get_frame(&dec_ctx, nullptr);

    // Encode the decoded image
    aom_codec_ctx_t enc_ctx;
    memset(&enc_ctx, 0, sizeof(enc_ctx));
    aom_codec_enc_init_ver(&enc_ctx, aom_codec_av1_cx(), &enc_cfg, 0, AOM_ENCODER_ABI_VERSION);
    aom_codec_control(&enc_ctx, AV1E_SET_TILE_COLUMNS, 2);
    aom_codec_control(&enc_ctx, AV1E_SET_TILE_ROWS, 2);
    aom_codec_encode(&enc_ctx, decoded_image, 0, 0, 0);
    const aom_codec_cx_pkt_t *encoded_pkt;
    while ((encoded_pkt = aom_codec_get_cx_data(&enc_ctx, nullptr)) != nullptr) {
        if (encoded_pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
            // Write encoded data to output file
            FILE *out_file = fopen("output_file", "wb");
            if (out_file == nullptr) {
                break;
            }
            fwrite(encoded_pkt->data.frame.buf, 1, encoded_pkt->data.frame.sz, out_file);
            assert_file_closed(&out_file);;
        }
    }

    // Get global headers
    aom_fixed_buf_t *global_headers = aom_codec_get_global_headers(&codec_ctx);

    // Cleanup
    assert_file_closed(&in_file);;
    aom_codec_destroy(&codec_ctx);
    aom_codec_destroy(&dec_ctx);
    aom_codec_destroy(&enc_ctx);

    assert_file_closed(&in_file);
	return 0;
}