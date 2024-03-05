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
//<ID> 145
//<Prompt> ["aom_img_add_metadata","aom_codec_control","aom_codec_version_str","aom_codec_set_cx_data_buf","aom_img_num_metadata","aom_img_set_rect"]
/*<Combination>: [int aom_img_add_metadata(aom_image_t * img, uint32_t type, const uint8_t * data, size_t sz, aom_metadata_insert_flags_t insert_flag),
    aom_codec_err_t aom_codec_control(aom_codec_ctx_t * ctx, int ctrl_id),
    const char *aom_codec_version_str(),
    aom_codec_err_t aom_codec_set_cx_data_buf(aom_codec_ctx_t * ctx, const aom_fixed_buf_t * buf, unsigned int pad_before, unsigned int pad_after),
    size_t aom_img_num_metadata(const aom_image_t * img),
    int aom_img_set_rect(aom_image_t * img, unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int border)
*/
//<score> 2.5, nr_unique_branch: 4, p_cov: 1
//<Quality> {"diversity":12,"density":6,"covered_api":["aom_img_add_metadata","aom_codec_control","aom_codec_version_str","aom_codec_set_cx_data_buf","aom_img_num_metadata","aom_img_set_rect"],"uncovered_api":[],"unique_branches":{"aom_codec_enc_init_ver":[[60,7,60,37,0],[60,41,60,74,0]],"aom_codec_control":[[88,7,88,11,1],[92,7,92,15,0]]},"library_calls":["aom_img_alloc","aom_img_add_metadata","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_set_cx_data_buf","aom_codec_version_str","aom_img_set_rect","aom_codec_control","aom_img_num_metadata","aom_img_free","aom_codec_destroy"]}
/*Here is the C++ program that achieves the event using the libaom APIs:

*/



extern "C" int LLVMFuzzerTestOneInput_5(const uint8_t* f_data, size_t f_size) {
	if(f_size<=24) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_4, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_5, fdp);
	//fuzzer shim end}




    // Create aom_image_t object
    aom_image_t image;
    aom_img_alloc(&image, AOM_IMG_FMT_I420, fuzz_uint32_t_1, fuzz_uint32_t_2, 32);

    // Add metadata to the image
    uint32_t type = 0;
    const uint8_t *metadata = data;
    size_t metadata_size = size;
    aom_img_add_metadata(&image, type, metadata, metadata_size, AOM_MIF_ANY_FRAME);

    // Create aom_codec_ctx_t object
    aom_codec_ctx_t codec_ctx;
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    aom_codec_enc_cfg_t enc_cfg;
    aom_codec_enc_config_default(iface, &enc_cfg, fuzz_uint32_t_3);
    aom_codec_enc_init_ver(&codec_ctx, iface, &enc_cfg, fuzz_int64_t_4, AOM_DECODER_ABI_VERSION);

    // Set custom data buffer
    aom_fixed_buf_t custom_data;
    custom_data.buf = (void *)data;
    custom_data.sz = size;
    aom_codec_set_cx_data_buf(&codec_ctx, &custom_data, 0, fuzz_uint32_t_5);

    // Get the version of the codec
    const char *version_str = aom_codec_version_str();

    // Set a rectangle region in the image
    unsigned int x = 0;
    unsigned int y = 0;
    unsigned int w = 100;
    unsigned int h = 100;
    unsigned int border = 0;
    aom_img_set_rect(&image, x, y, w, h, border);

    // Control aom_codec_ctx_t
    int ctrl_id = 0;
    aom_codec_control(&codec_ctx, ctrl_id);

    // Get the number of metadata in the image
    size_t num_metadata = aom_img_num_metadata(&image);

    // Print the version string
    printf("Version: %s\n", version_str);

    // Cleanup resources
    aom_img_free(&image);
    aom_codec_destroy(&codec_ctx);

    return 0;
}