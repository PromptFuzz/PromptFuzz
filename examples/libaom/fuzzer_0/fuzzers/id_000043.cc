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
//<ID> 1707
//<Prompt> ["aom_codec_err_to_string","aom_codec_get_caps","aom_codec_destroy","aom_codec_iface_name","aom_img_add_metadata","aom_img_get_metadata","aom_codec_error_detail","aom_codec_error","aom_img_metadata_alloc"]
/*<Combination>: [const char *aom_codec_err_to_string(aom_codec_err_t err),
    aom_codec_caps_t aom_codec_get_caps(aom_codec_iface_t * iface),
    aom_codec_err_t aom_codec_destroy(aom_codec_ctx_t * ctx),
    const char *aom_codec_iface_name(aom_codec_iface_t * iface),
    int aom_img_add_metadata(aom_image_t * img, uint32_t type, const uint8_t * data, size_t sz, aom_metadata_insert_flags_t insert_flag),
    const aom_metadata_t *aom_img_get_metadata(const aom_image_t * img, size_t index),
    const char *aom_codec_error_detail(const aom_codec_ctx_t * ctx),
    const char *aom_codec_error(const aom_codec_ctx_t * ctx),
    aom_metadata_t *aom_img_metadata_alloc(uint32_t type, const uint8_t * data, size_t sz, aom_metadata_insert_flags_t insert_flag)
*/
//<score> 0.7692308, nr_unique_branch: 1, p_cov: 1
//<Quality> {"diversity":13,"density":5,"covered_api":["aom_codec_err_to_string","aom_codec_get_caps","aom_codec_destroy","aom_codec_iface_name","aom_img_add_metadata","aom_img_get_metadata","aom_codec_error_detail","aom_codec_error","aom_img_metadata_alloc"],"uncovered_api":[],"unique_branches":{"aom_codec_error_detail":[[62,12,62,21,1]]},"library_calls":["aom_codec_err_to_string","aom_codec_get_caps","aom_codec_av1_dx","aom_codec_av1_dx","aom_codec_destroy","aom_codec_iface_name","aom_img_add_metadata","aom_img_get_metadata","aom_codec_error_detail","aom_codec_error","aom_img_metadata_alloc","aom_img_free","aom_img_metadata_free"]}
/**/



extern "C" int LLVMFuzzerTestOneInput_43(const uint8_t *data, size_t size) {
	if(size<=0) return 0;

	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);


    // Step 4: Create a FILE pointer to read the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file == nullptr) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Step 5: Get the file descriptor for reading
    int in_fd = fuzz_fileno(in_file);

    // Step 6: Create a string for the input file name
    const char *in_filename = "input_file";

    // Step 1: Define the event and use libaom APIs to achieve it
    const char *error_string = aom_codec_err_to_string(AOM_CODEC_OK);
    aom_codec_caps_t codec_caps = aom_codec_get_caps(aom_codec_av1_dx());
    aom_codec_iface_t *codec_iface = aom_codec_av1_dx();
    aom_codec_ctx_t codec_ctx;
    aom_codec_err_t codec_err = aom_codec_destroy(&codec_ctx);
    const char *iface_name = aom_codec_iface_name(codec_iface);
    aom_image_t img;
    memset(&img, 0, sizeof(aom_image_t));
    uint8_t metadata_payload[10];
    size_t metadata_size = sizeof(metadata_payload);
    aom_metadata_insert_flags_t insert_flag = AOM_MIF_NON_KEY_FRAME;
    aom_img_add_metadata(&img, 1, metadata_payload, metadata_size, insert_flag);
    const aom_metadata_t *metadata = aom_img_get_metadata(&img, 0);
    const char *error_detail = aom_codec_error_detail(&codec_ctx);
    const char *error = aom_codec_error(&codec_ctx);
    aom_metadata_t *metadata_alloc = aom_img_metadata_alloc(1, metadata_payload, metadata_size, insert_flag);

    // Step 7: Release allocated resources
    assert_file_closed(&in_file);;
    aom_img_free(&img);
    aom_img_metadata_free(metadata_alloc);

    assert_file_name_closed("input_file");
	assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
}