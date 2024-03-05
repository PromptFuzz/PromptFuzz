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
//<ID> 301
//<Prompt> ["aom_obu_type_to_string","aom_codec_control","aom_codec_build_config","aom_codec_set_option","aom_codec_set_cx_data_buf","aom_img_num_metadata","aom_img_metadata_alloc","aom_codec_get_global_headers","aom_img_set_rect"]
/*<Combination>: [const char *aom_obu_type_to_string(OBU_TYPE type),
    aom_codec_err_t aom_codec_control(aom_codec_ctx_t * ctx, int ctrl_id),
    const char *aom_codec_build_config(),
    aom_codec_err_t aom_codec_set_option(aom_codec_ctx_t * ctx, const char * name, const char * value),
    aom_codec_err_t aom_codec_set_cx_data_buf(aom_codec_ctx_t * ctx, const aom_fixed_buf_t * buf, unsigned int pad_before, unsigned int pad_after),
    size_t aom_img_num_metadata(const aom_image_t * img),
    aom_metadata_t *aom_img_metadata_alloc(uint32_t type, const uint8_t * data, size_t sz, aom_metadata_insert_flags_t insert_flag),
    aom_fixed_buf_t *aom_codec_get_global_headers(aom_codec_ctx_t * ctx),
    int aom_img_set_rect(aom_image_t * img, unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int border)
*/
//<score> 1, nr_unique_branch: 1, p_cov: 1
//<Quality> {"diversity":16,"density":8,"covered_api":["aom_obu_type_to_string","aom_codec_control","aom_codec_build_config","aom_codec_set_option","aom_codec_set_cx_data_buf","aom_img_num_metadata","aom_img_metadata_alloc","aom_codec_get_global_headers","aom_img_set_rect"],"uncovered_api":[],"unique_branches":{"aom_codec_set_option":[[123,36,123,59,0]]},"library_calls":["aom_obu_type_to_string","aom_codec_dec_init_ver","aom_codec_av1_dx","aom_codec_control","aom_codec_build_config","aom_codec_enc_config_default","aom_codec_av1_cx","aom_codec_set_option","aom_codec_set_cx_data_buf","aom_img_num_metadata","aom_img_metadata_alloc","aom_codec_get_global_headers","aom_img_set_rect","aom_img_metadata_free","aom_codec_destroy"]}
/*Please find below the implementation of the C++ program that achieves the specified event using the libaom library APIs:

*/


extern "C" int LLVMFuzzerTestOneInput_9(const uint8_t *data, size_t size) {
	if(size<=0) return 0;

	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);


    // Step 1: Event - Convert OBU type to string
    const char *obuTypeStr = aom_obu_type_to_string(OBU_TYPE(0));

    // Step 1: Event - Control the codec
    aom_codec_ctx_t ctx;
    aom_codec_dec_cfg_t decCfg = {0};
    aom_codec_err_t decErr = aom_codec_dec_init_ver(&ctx, aom_codec_av1_dx(), &decCfg, 0, AOM_DECODER_ABI_VERSION);
    if (decErr != AOM_CODEC_OK) {
        return 0;
    }
    aom_codec_control(&ctx, 0);

    // Step 1: Event - Get build configuration
    const char *buildConfig = aom_codec_build_config();

    // Step 1: Event - Set option
    aom_codec_enc_cfg_t encCfg = {0};
    aom_codec_err_t encErr = aom_codec_enc_config_default(aom_codec_av1_cx(), &encCfg, 0);
    if (encErr != AOM_CODEC_OK) {
        aom_codec_destroy(&ctx);
        return 0;
    }
    aom_codec_set_option(&ctx, "optionName", "optionValue");

    // Step 1: Event - Set CX data buffer
    aom_fixed_buf_t cxDataBuf;
    aom_codec_set_cx_data_buf(&ctx, &cxDataBuf, 0, 0);

    // Step 1: Event - Get number of metadata in an image
    aom_image_t img;
    size_t numMetadata = aom_img_num_metadata(&img);

    // Step 1: Event - Allocate metadata
    aom_metadata_t *metadata = aom_img_metadata_alloc(0, data, size, AOM_MIF_ANY_FRAME);

    // Step 1: Event - Get global headers
    aom_fixed_buf_t *globalHeaders = aom_codec_get_global_headers(&ctx);

    // Step 1: Event - Set image rectangle
    aom_img_set_rect(&img, 0, 0, 100, 100, 0);

    // Step 2: Use the variables and APIs as per your requirement

    // Step 3: Consume input data
    FILE *inFile = fmemopen((void *)data, size, "rb");
    int inFileDesc = fuzz_fileno(inFile);

    // Step 4: Use inFile to read input data

    // Step 5: Use inFileDesc for any other operations

    // Step 6: Use file names directly
    const char *inputFileName = "input_file";
    const char *outputFileName = "output_file";

    // Step 7: Release allocated resources
    aom_img_metadata_free(metadata);
    aom_codec_destroy(&ctx);
    assert_file_closed(&inFile);;

    assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_closed(&inFile);
	assert_fd_closed(inFileDesc);
	return 0;
}