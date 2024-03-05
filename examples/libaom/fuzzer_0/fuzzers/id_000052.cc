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
//<ID> 2040
//<Prompt> ["aom_img_add_metadata","aom_uleb_encode","aom_codec_error_detail","aom_img_metadata_alloc","aom_codec_av1_cx"]
/*<Combination>: [int aom_img_add_metadata(aom_image_t * img, uint32_t type, const uint8_t * data, size_t sz, aom_metadata_insert_flags_t insert_flag),
    int aom_uleb_encode(uint64_t value, size_t available, uint8_t * coded_value, size_t * coded_size),
    const char *aom_codec_error_detail(const aom_codec_ctx_t * ctx),
    aom_metadata_t *aom_img_metadata_alloc(uint32_t type, const uint8_t * data, size_t sz, aom_metadata_insert_flags_t insert_flag),
    aom_codec_iface_t *aom_codec_av1_cx()
*/
//<score> 0.96000004, nr_unique_branch: 1, p_cov: 0.6
//<Quality> {"diversity":5,"density":4,"covered_api":["aom_img_add_metadata","aom_codec_error_detail","aom_codec_av1_cx"],"uncovered_api":["aom_uleb_encode","aom_img_metadata_alloc"],"unique_branches":{"aom_codec_enc_init_ver":[[62,30,62,34,0]]},"library_calls":["aom_img_add_metadata","aom_codec_av1_cx","aom_codec_enc_init_ver","aom_codec_error_detail","aom_codec_destroy"]}
/**/

extern "C" int LLVMFuzzerTestOneInput_52(const uint8_t *data, size_t size) {
	if(size<=0) return 0;

	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);

    // Step 4: Create a FILE * variable to read input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file == nullptr) {
        // Error handling
        assert_file_closed(&in_file);
	return 0;
    }

    // Step 6: Define input file name
    const char *input_file = "input_file";

    // Step 7: Read data from input file
    size_t fread_result;
    uint8_t buffer[1024];
    fread_result = fread(buffer, 1, sizeof(buffer), in_file);

    // Step 5: Get file descriptor for reading
    int input_fd = fuzz_fileno(in_file);

    // Step 1: Add metadata to an image
    aom_image_t *img = nullptr; // Initialize image variable
    uint32_t metadata_type = 1; // Set the type of metadata
    const uint8_t metadata_data[] = {0x01, 0x02, 0x03}; // Set the metadata payload
    size_t metadata_size = sizeof(metadata_data); // Set the size of metadata payload
    aom_metadata_insert_flags_t metadata_flag = AOM_MIF_KEY_FRAME; // Set the metadata insert flag
    int add_metadata_result = aom_img_add_metadata(img, metadata_type, metadata_data, metadata_size, metadata_flag);
    if (add_metadata_result != 0) {
        // Error handling
    }

    // Step 2: Encode the image using libaom
    aom_codec_iface_t *iface = aom_codec_av1_cx(); // Get the AV1 codec interface
    aom_codec_ctx_t ctx; // Create codec context
    aom_codec_err_t codec_err = aom_codec_enc_init(&ctx, iface, nullptr, 0); // Initialize encoder
    if (codec_err != AOM_CODEC_OK) {
        // Error handling
    }

    // Step 3: Get error details from codec context
    const char *error_detail = aom_codec_error_detail(&ctx);
    if (error_detail != nullptr) {
        // Error handling
    }

    // Step 7: Release resources
    aom_codec_destroy(&ctx); // Destroy codec context
    assert_file_closed(&in_file);; // Close input file

    assert_file_name_closed("input_file");
	assert_file_closed(&in_file);
	assert_fd_closed(input_fd);
	return 0;
}