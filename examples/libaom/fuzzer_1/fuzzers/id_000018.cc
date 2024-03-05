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
//<ID> 773
//<Prompt> ["aom_img_metadata_alloc","aom_codec_control","aom_codec_build_config","aom_img_alloc","aom_codec_error"]
/*<Combination>: [aom_metadata_t *aom_img_metadata_alloc(uint32_t type, const uint8_t * data, size_t sz, aom_metadata_insert_flags_t insert_flag),
    aom_codec_err_t aom_codec_control(aom_codec_ctx_t * ctx, int ctrl_id),
    const char *aom_codec_build_config(),
    aom_image_t *aom_img_alloc(aom_image_t * img, aom_img_fmt_t fmt, unsigned int d_w, unsigned int d_h, unsigned int align),
    const char *aom_codec_error(const aom_codec_ctx_t * ctx)
*/
//<score> 0.8000001, nr_unique_branch: 2, p_cov: 0.8
//<Quality> {"diversity":6,"density":2,"covered_api":["aom_img_metadata_alloc","aom_codec_build_config","aom_img_alloc","aom_codec_error"],"uncovered_api":["aom_codec_control"],"unique_branches":{"aom_codec_err_to_string":[[36,11,36,14,0],[49,5,49,28,0]]},"library_calls":["aom_img_metadata_alloc","aom_codec_build_config","aom_img_alloc","aom_codec_error","aom_img_metadata_free","aom_img_free"]}
/*Here is the C++ program implementing the steps mentioned above:

*/


extern "C" int LLVMFuzzerTestOneInput_18(const uint8_t* f_data, size_t f_size) {
	if(f_size<=12) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
	//fuzzer shim end}




    // Step 4: Open input file for reading
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file == NULL) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Step 5: Get file descriptor for reading
    int in_fd = fuzz_fileno(in_file);
    
    // Step 6: Set output file name
    const char *out_file_name = "output_file";
    
    // Step 1: An event achieved using libaom APIs
    aom_metadata_t *metadata = aom_img_metadata_alloc(fuzz_uint32_t_1, NULL, 0, AOM_MIF_ANY_FRAME);
    aom_codec_ctx_t *ctx = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
    const char *build_config = aom_codec_build_config(); 
    aom_image_t *img = aom_img_alloc(NULL, AOM_IMG_FMT_I420, fuzz_uint32_t_2, fuzz_uint32_t_3, 16);
    const char *error_msg = aom_codec_error(ctx);
    
    // Step 2: Complete the event using libaom APIs
    // TODO: Add necessary function calls here
    
    // Step 7: Release allocated resources
    aom_img_metadata_free(metadata);
    free(ctx);
    aom_img_free(img);
    assert_file_closed(&in_file);;
    
    assert_file_name_closed("output_file");
	assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
}