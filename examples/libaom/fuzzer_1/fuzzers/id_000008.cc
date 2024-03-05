#include "FDSan.h"
#include "FuzzedDataProvider.h"
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
//<ID> 288
//<Prompt> ["aom_obu_type_to_string","aom_codec_control","aom_codec_set_option","aom_codec_version_str","aom_codec_set_cx_data_buf","aom_img_num_metadata","aom_codec_get_global_headers","aom_img_set_rect"]
/*<Combination>: [const char *aom_obu_type_to_string(OBU_TYPE type),
    aom_codec_err_t aom_codec_control(aom_codec_ctx_t * ctx, int ctrl_id),
    aom_codec_err_t aom_codec_set_option(aom_codec_ctx_t * ctx, const char * name, const char * value),
    const char *aom_codec_version_str(),
    aom_codec_err_t aom_codec_set_cx_data_buf(aom_codec_ctx_t * ctx, const aom_fixed_buf_t * buf, unsigned int pad_before, unsigned int pad_after),
    size_t aom_img_num_metadata(const aom_image_t * img),
    aom_fixed_buf_t *aom_codec_get_global_headers(aom_codec_ctx_t * ctx),
    int aom_img_set_rect(aom_image_t * img, unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int border)
*/
//<score> 1.5, nr_unique_branch: 2, p_cov: 1
//<Quality> {"diversity":8,"density":4,"covered_api":["aom_obu_type_to_string","aom_codec_control","aom_codec_set_option","aom_codec_version_str","aom_codec_set_cx_data_buf","aom_img_num_metadata","aom_codec_get_global_headers","aom_img_set_rect"],"uncovered_api":[],"unique_branches":{"aom_codec_get_global_headers":[[280,9,280,20,0]],"aom_codec_set_option":[[123,7,123,18,0]]},"library_calls":["aom_obu_type_to_string","aom_codec_control","aom_codec_set_option","aom_codec_version_str","aom_codec_set_cx_data_buf","aom_img_num_metadata","aom_codec_get_global_headers","aom_img_set_rect"]}
/**/


extern "C" int LLVMFuzzerTestOneInput_8(const uint8_t* f_data, size_t f_size) {
	if(f_size<=4) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
	//fuzzer shim end}




    FILE *in_file = fmemopen((void *)data, size, "rb");
    FILE *out_file = fopen("output_file", "wb");
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);

    // 1. Call aom_obu_type_to_string function
    OBU_TYPE obu_type = OBU_TYPE(1);
    const char *obu_type_str = aom_obu_type_to_string(obu_type);

    // 2. Call aom_codec_control function
    aom_codec_ctx_t ctx;
    aom_codec_err_t err = aom_codec_control(&ctx, fuzz_int32_t_1);

    // 3. Call aom_codec_set_option function
    const char *name = "option_name";
    const char *value = "option_value";
    err = aom_codec_set_option(&ctx, name, value);

    // 4. Call aom_codec_version_str function
    const char *version_str = aom_codec_version_str();
    fprintf(out_file, "Version: %s\n", version_str);

    // 5. Call aom_codec_set_cx_data_buf function
    aom_fixed_buf_t buf;
    unsigned int pad_before = 0;
    unsigned int pad_after = 0;
    err = aom_codec_set_cx_data_buf(&ctx, &buf, pad_before, pad_after);

    // 6. Call aom_img_num_metadata function
    aom_image_t image;
    size_t num_metadata = aom_img_num_metadata(&image);

    // 7. Call aom_codec_get_global_headers function
    aom_fixed_buf_t *global_headers = aom_codec_get_global_headers(&ctx);

    // 8. Call aom_img_set_rect function
    unsigned int x = 0;
    unsigned int y = 0;
    unsigned int w = 128;
    unsigned int h = 128;
    unsigned int border = 0;
    aom_img_set_rect(&image, x, y, w, h, border);

    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;

    assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}