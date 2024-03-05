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
//<ID> 1631
//<Prompt> ["aom_codec_err_to_string","aom_codec_get_caps","aom_codec_destroy","aom_uleb_size_in_bytes","aom_codec_iface_name","aom_img_alloc_with_border","aom_img_metadata_alloc","aom_codec_build_config"]
/*<Combination>: [const char *aom_codec_err_to_string(aom_codec_err_t err),
    aom_codec_caps_t aom_codec_get_caps(aom_codec_iface_t * iface),
    aom_codec_err_t aom_codec_destroy(aom_codec_ctx_t * ctx),
    size_t aom_uleb_size_in_bytes(uint64_t value),
    const char *aom_codec_iface_name(aom_codec_iface_t * iface),
    aom_image_t *aom_img_alloc_with_border(aom_image_t * img, aom_img_fmt_t fmt, unsigned int d_w, unsigned int d_h, unsigned int align, unsigned int size_align, unsigned int border),
    aom_metadata_t *aom_img_metadata_alloc(uint32_t type, const uint8_t * data, size_t sz, aom_metadata_insert_flags_t insert_flag),
    const char *aom_codec_build_config()
*/
//<score> 0.54545456, nr_unique_branch: 1, p_cov: 1
//<Quality> {"diversity":11,"density":3,"covered_api":["aom_codec_err_to_string","aom_codec_get_caps","aom_codec_destroy","aom_uleb_size_in_bytes","aom_codec_iface_name","aom_img_alloc_with_border","aom_img_metadata_alloc","aom_codec_build_config"],"uncovered_api":[],"unique_branches":{"img_alloc_helper":[[57,7,57,18,0]]},"library_calls":["aom_codec_err_to_string","aom_codec_av1_dx","aom_codec_get_caps","aom_codec_destroy","aom_uleb_size_in_bytes","aom_codec_iface_name","aom_img_alloc_with_border","aom_img_metadata_alloc","aom_codec_build_config","aom_img_metadata_free","aom_img_free"]}
/*Here is a C++ program that achieves the given event by using the libaom library APIs:

*/


extern "C" int LLVMFuzzerTestOneInput_42(const uint8_t *data, size_t size) {
	if(size<=0) return 0;

	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);

    // Step 1: Use the required APIs to achieve the event

    // aom_codec_err_to_string
    const char *err_str = aom_codec_err_to_string(AOM_CODEC_OK);
    printf("Error string: %s\n", err_str);

    // aom_codec_get_caps
    aom_codec_iface_t *iface = aom_codec_av1_dx();
    aom_codec_caps_t caps = aom_codec_get_caps(iface);
    printf("Codec capabilities: %lx\n", caps);

    // aom_codec_destroy
    aom_codec_ctx_t ctx;
    aom_codec_destroy(&ctx);

    // aom_uleb_size_in_bytes
    uint64_t value = 12345;
    size_t uleb_size = aom_uleb_size_in_bytes(value);
    printf("ULEB size: %zu\n", uleb_size);

    // aom_codec_iface_name
    const char *iface_name = aom_codec_iface_name(iface);
    printf("Codec interface name: %s\n", iface_name);

    // aom_img_alloc_with_border
    aom_img_fmt_t fmt = AOM_IMG_FMT_I420;
    unsigned int d_w = 1280;
    unsigned int d_h = 720;
    unsigned int align = 32;
    unsigned int size_align = 0;
    unsigned int border = 16;
    aom_image_t img;
    aom_img_alloc_with_border(&img, fmt, d_w, d_h, align, size_align, border);

    // aom_img_metadata_alloc
    uint32_t type = 1;
    const uint8_t metadata_data[] = {0x01, 0x02, 0x03};
    size_t metadata_sz = sizeof(metadata_data);
    aom_metadata_insert_flags_t insert_flag = AOM_MIF_ANY_FRAME;
    aom_metadata_t *metadata = aom_img_metadata_alloc(type, metadata_data, metadata_sz, insert_flag);

    // aom_codec_build_config
    const char *build_config = aom_codec_build_config();
    printf("Build config: %s\n", build_config);

    // Step 2: Use the parameters and variables in your fuzzing scenario

    // Reading the input data from the file
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file == nullptr) {
        printf("Failed to open input file\n");
        assert_file_closed(&in_file);
	return 0;
    }

    // Writing output data to a file
    FILE *out_file = fopen("output_file", "wb");
    if (out_file == nullptr) {
        printf("Failed to open output file\n");
        assert_file_closed(&in_file);;
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Using the file descriptor for reading or writing
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);
    printf("Input file descriptor: %d\n", in_fd);
    printf("Output file descriptor: %d\n", out_fd);

    // Using the file name directly
    const char *input_file = "input_file";
    const char *output_file = "output_file";
    printf("Input file name: %s\n", input_file);
    printf("Output file name: %s\n", output_file);

    // Step 3: Release all allocated resources before returning

    aom_img_metadata_free(metadata);
    aom_img_free(&img);

    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;

    assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}