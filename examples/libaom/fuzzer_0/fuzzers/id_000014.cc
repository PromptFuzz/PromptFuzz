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
//<ID> 564
//<Prompt> ["aom_obu_type_to_string","aom_codec_encode","aom_codec_control","aom_codec_build_config","aom_codec_set_cx_data_buf","aom_img_num_metadata"]
/*<Combination>: [const char *aom_obu_type_to_string(OBU_TYPE type),
    aom_codec_err_t aom_codec_encode(aom_codec_ctx_t * ctx, const aom_image_t * img, aom_codec_pts_t pts, unsigned long duration, aom_enc_frame_flags_t flags),
    aom_codec_err_t aom_codec_control(aom_codec_ctx_t * ctx, int ctrl_id),
    const char *aom_codec_build_config(),
    aom_codec_err_t aom_codec_set_cx_data_buf(aom_codec_ctx_t * ctx, const aom_fixed_buf_t * buf, unsigned int pad_before, unsigned int pad_after),
    size_t aom_img_num_metadata(const aom_image_t * img)
*/
//<score> 3.5, nr_unique_branch: 6, p_cov: 0.5
//<Quality> {"diversity":10,"density":10,"covered_api":["aom_codec_encode","aom_codec_control","aom_codec_set_cx_data_buf"],"uncovered_api":["aom_obu_type_to_string","aom_codec_build_config","aom_img_num_metadata"],"unique_branches":{"av1_change_config_seq":[[740,37,740,67,0]],"av1_select_sb_size":[[854,9,854,25,0],[854,29,854,46,0]],"av1_setup_sms_tree":[[273,29,273,43,1]],"av1_change_config":[[910,36,910,54,0],[911,9,911,46,0]]},"library_calls":["aom_codec_enc_config_default","aom_codec_av1_cx","aom_codec_enc_init_ver","aom_codec_av1_cx","aom_codec_control","aom_codec_control","aom_codec_set_cx_data_buf","aom_codec_encode","aom_codec_get_cx_data","aom_codec_destroy"]}
/**/


extern "C" int LLVMFuzzerTestOneInput_14(const uint8_t *data, size_t size) {
	if(size<=0) return 0;

    // Create input file stream
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Create output file stream
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Get file descriptors
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);

    // Initialize libaom
    aom_codec_ctx_t encoder;
    aom_codec_enc_cfg_t cfg;
    aom_codec_enc_config_default(aom_codec_av1_cx(), &cfg, 0);
    aom_codec_enc_init_ver(&encoder, aom_codec_av1_cx(), &cfg, 0, AOM_ENCODER_ABI_VERSION);

    // Configure encoder
    aom_codec_control(&encoder, AOME_SET_CPUUSED, 8);
    aom_codec_control(&encoder, AV1E_SET_TILE_COLUMNS, 2);

    // Create buffer for input data
    aom_fixed_buf_t input_buf;
    input_buf.buf = (void *)data;
    input_buf.sz = size;

    // Set input buffer
    aom_codec_set_cx_data_buf(&encoder, &input_buf, 0, 0);

    // Encode frame
    aom_codec_encode(&encoder, NULL, 0, 0, 0);

    // Get compressed data
    const aom_codec_cx_pkt_t *pkt;
    while ((pkt = aom_codec_get_cx_data(&encoder, NULL)) != NULL) {
        // Write compressed data to output file
        fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
    }

    // Release resources
    aom_codec_destroy(&encoder);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;

    assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}