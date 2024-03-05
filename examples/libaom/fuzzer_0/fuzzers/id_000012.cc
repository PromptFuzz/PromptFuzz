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
//<ID> 465
//<Prompt> ["aom_obu_type_to_string","aom_codec_control","aom_codec_build_config","aom_codec_set_cx_data_buf","aom_img_num_metadata","aom_codec_get_global_headers","aom_img_set_rect"]
/*<Combination>: [const char *aom_obu_type_to_string(OBU_TYPE type),
    aom_codec_err_t aom_codec_control(aom_codec_ctx_t * ctx, int ctrl_id),
    const char *aom_codec_build_config(),
    aom_codec_err_t aom_codec_set_cx_data_buf(aom_codec_ctx_t * ctx, const aom_fixed_buf_t * buf, unsigned int pad_before, unsigned int pad_after),
    size_t aom_img_num_metadata(const aom_image_t * img),
    aom_fixed_buf_t *aom_codec_get_global_headers(aom_codec_ctx_t * ctx),
    int aom_img_set_rect(aom_image_t * img, unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int border)
*/
//<score> 2, nr_unique_branch: 3, p_cov: 0.5714286
//<Quality> {"diversity":8,"density":7,"covered_api":["aom_codec_control","aom_codec_build_config","aom_codec_set_cx_data_buf","aom_codec_get_global_headers"],"uncovered_api":["aom_obu_type_to_string","aom_img_num_metadata","aom_img_set_rect"],"unique_branches":{"at_ctrl_map_end":[[193,10,193,25,0],[193,29,193,42,0]],"aom_codec_control":[[104,8,104,31,1]]},"library_calls":["aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_set_cx_data_buf","aom_codec_get_cx_data","aom_codec_get_global_headers","aom_codec_build_config","aom_codec_control","aom_codec_destroy"]}
/**/



extern "C" int LLVMFuzzerTestOneInput_12(const uint8_t *data, size_t size) {
	if(size<=0) return 0;

    // Step 1: Create aom_codec_ctx_t object
    aom_codec_ctx_t codec;
    
    // Step 2: Initialize the codec context
    aom_codec_iface_t *iface = aom_codec_av1_dx();
    aom_codec_dec_cfg_t dec_cfg = {0}; // Set the configuration options accordingly
    aom_codec_err_t res = aom_codec_dec_init_ver(&codec, iface, &dec_cfg, 0, AOM_DECODER_ABI_VERSION);
    if (res != AOM_CODEC_OK) {
        // Handle initialization error
        return 0;
    }
    
    // Step 3: Set input buffer
    aom_fixed_buf_t input_buf = { (void*)data, size };
    res = aom_codec_set_cx_data_buf(&codec, &input_buf, 0, 0);
    if (res != AOM_CODEC_OK) {
        // Handle setting input buffer error
        return 0;
    }
    
    // Step 4: Decode the frame
    while (1) {
        aom_codec_iter_t iter = NULL;
        const aom_codec_cx_pkt_t *pkt = aom_codec_get_cx_data(&codec, &iter);
        if (pkt == NULL) {
            // No more output packets
            break;
        }
        if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
            // Process decoded frame pkt
            // ...
        }
    }
    
    // Step 5: Get and process global headers
    aom_fixed_buf_t *global_headers = aom_codec_get_global_headers(&codec);
    if (global_headers) {
        // Process global headers
        // ...
    }
    
    // Step 6: Control the codec
    const char *config_str = aom_codec_build_config();
    aom_codec_control(&codec, AOME_SET_CPUUSED, 4); // Example of setting encoder options
    
    // Step 7: Cleanup
    aom_codec_destroy(&codec);
    
    return 0;
}