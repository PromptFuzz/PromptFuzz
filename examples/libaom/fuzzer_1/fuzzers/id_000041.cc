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
//<ID> 1618
//<Prompt> ["aom_codec_err_to_string","aom_codec_destroy","aom_uleb_size_in_bytes","aom_codec_iface_name","aom_img_wrap","aom_img_metadata_alloc","aom_codec_build_config"]
/*<Combination>: [const char *aom_codec_err_to_string(aom_codec_err_t err),
    aom_codec_err_t aom_codec_destroy(aom_codec_ctx_t * ctx),
    size_t aom_uleb_size_in_bytes(uint64_t value),
    const char *aom_codec_iface_name(aom_codec_iface_t * iface),
    aom_image_t *aom_img_wrap(aom_image_t * img, aom_img_fmt_t fmt, unsigned int d_w, unsigned int d_h, unsigned int align, unsigned char * img_data),
    aom_metadata_t *aom_img_metadata_alloc(uint32_t type, const uint8_t * data, size_t sz, aom_metadata_insert_flags_t insert_flag),
    const char *aom_codec_build_config()
*/
//<score> 0.44444445, nr_unique_branch: 1, p_cov: 1
//<Quality> {"diversity":9,"density":2,"covered_api":["aom_codec_err_to_string","aom_codec_destroy","aom_uleb_size_in_bytes","aom_codec_iface_name","aom_img_wrap","aom_img_metadata_alloc","aom_codec_build_config"],"uncovered_api":[],"unique_branches":{"aom_codec_iface_name":[[32,10,32,15,1]]},"library_calls":["aom_codec_err_to_string","aom_codec_destroy","aom_uleb_size_in_bytes","aom_codec_iface_name","aom_img_wrap","aom_img_metadata_alloc","aom_codec_build_config","aom_img_metadata_free","aom_img_free"]}
/**/


extern "C" int LLVMFuzzerTestOneInput_41(const uint8_t* f_data, size_t f_size) {
	if(f_size<=20) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_4, fdp);
	//fuzzer shim end}




    // Step 4: Create FILE pointers for input and output
    FILE *in_file = fmemopen((void *)data, size, "rb");
    FILE *out_file = fopen("output_file", "wb");

    if (in_file == NULL || out_file == NULL) {
        // Error handling
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);

    // Step 1: Define the event you want to achieve
    const char *error_string;
    aom_codec_err_t error;
    size_t uleb_size;
    const char *iface_name;
    aom_image_t *image;
    aom_metadata_t *metadata;
    const char *build_config;

    // Step 2: Call libaom APIs to achieve the event
    error_string = aom_codec_err_to_string(error);
    error = aom_codec_destroy(NULL);
    uleb_size = aom_uleb_size_in_bytes(fuzz_uint64_t_1);
    iface_name = aom_codec_iface_name(NULL);
    image = aom_img_wrap(NULL, AOM_IMG_FMT_I420, fuzz_uint32_t_2, fuzz_uint32_t_3, 1, NULL);
    metadata = aom_img_metadata_alloc(fuzz_uint32_t_4, data, size, AOM_MIF_KEY_FRAME);
    build_config = aom_codec_build_config();

    // Step 7: Release allocated resources
    aom_img_metadata_free(metadata);
    aom_img_free(image);
    assert_file_closed(&out_file);;
    assert_file_closed(&in_file);;

    assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}