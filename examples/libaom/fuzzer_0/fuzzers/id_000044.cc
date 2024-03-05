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
//<ID> 1725
//<Prompt> ["aom_codec_err_to_string","aom_codec_get_caps","aom_codec_destroy","aom_codec_iface_name","aom_img_alloc_with_border","aom_img_add_metadata","aom_img_get_metadata","aom_codec_error_detail","aom_img_remove_metadata","aom_codec_error","aom_img_metadata_alloc"]
/*<Combination>: [const char *aom_codec_err_to_string(aom_codec_err_t err),
    aom_codec_caps_t aom_codec_get_caps(aom_codec_iface_t * iface),
    aom_codec_err_t aom_codec_destroy(aom_codec_ctx_t * ctx),
    const char *aom_codec_iface_name(aom_codec_iface_t * iface),
    aom_image_t *aom_img_alloc_with_border(aom_image_t * img, aom_img_fmt_t fmt, unsigned int d_w, unsigned int d_h, unsigned int align, unsigned int size_align, unsigned int border),
    int aom_img_add_metadata(aom_image_t * img, uint32_t type, const uint8_t * data, size_t sz, aom_metadata_insert_flags_t insert_flag),
    const aom_metadata_t *aom_img_get_metadata(const aom_image_t * img, size_t index),
    const char *aom_codec_error_detail(const aom_codec_ctx_t * ctx),
    void aom_img_remove_metadata(aom_image_t * img),
    const char *aom_codec_error(const aom_codec_ctx_t * ctx),
    aom_metadata_t *aom_img_metadata_alloc(uint32_t type, const uint8_t * data, size_t sz, aom_metadata_insert_flags_t insert_flag)
*/
//<score> 1.3, nr_unique_branch: 1, p_cov: 1
//<Quality> {"diversity":20,"density":13,"covered_api":["aom_codec_err_to_string","aom_codec_get_caps","aom_codec_destroy","aom_codec_iface_name","aom_img_alloc_with_border","aom_img_add_metadata","aom_img_get_metadata","aom_codec_error_detail","aom_img_remove_metadata","aom_codec_error","aom_img_metadata_alloc"],"uncovered_api":[],"unique_branches":{"aom_codec_error_detail":[[61,14,61,22,1]]},"library_calls":["aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_peek_stream_info","aom_codec_err_to_string","aom_codec_decode","aom_codec_get_caps","aom_codec_destroy","aom_codec_iface_name","aom_img_alloc_with_border","aom_img_add_metadata","aom_img_get_metadata","aom_img_remove_metadata","aom_codec_error_detail","aom_codec_error","aom_img_metadata_alloc","aom_img_metadata_free","aom_img_free","aom_codec_destroy"]}
/**/

extern "C" int LLVMFuzzerTestOneInput_44(const uint8_t *data, size_t size) {
	if(size<=0) return 0;

  // Step 4: Read the input data from the fuzzer
  FILE *in_file = fmemopen((void *)data, size, "rb");
  if (in_file == NULL) {
    assert_file_closed(&in_file);
	return 0;
  }

  // Step 5: Get the file descriptor for reading
  int input_fd = fuzz_fileno(in_file);

  // Initialize the libaom codec context
  aom_codec_ctx_t codec_ctx;
  aom_codec_iface_t *iface = aom_codec_av1_dx();
  aom_codec_dec_cfg_t cfg;
  if (aom_codec_dec_init_ver(&codec_ctx, iface, &cfg, 0, AOM_DECODER_ABI_VERSION)) {
    assert_file_closed(&in_file);;
    assert_file_closed(&in_file);
	assert_fd_closed(input_fd);
	return 0;
  }

  // Step 6: Create an output file for writing
  FILE *out_file = fopen("output_file", "wb");
  if (out_file == NULL) {
    aom_codec_destroy(&codec_ctx);
    assert_file_closed(&in_file);;
    assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(input_fd);
	return 0;
  }

  // Step 7: Get the file descriptor for writing
  int output_fd = fuzz_fileno(out_file);

  // Process the input stream using libaom
  aom_codec_stream_info_t si;
  if (aom_codec_peek_stream_info(iface, (const uint8_t *)data, size, &si) != AOM_CODEC_OK) {
    aom_codec_destroy(&codec_ctx);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(output_fd);
	assert_fd_closed(input_fd);
	return 0;
  }

  // Step 1: Convert error code to string
  const char *error_str = aom_codec_err_to_string(aom_codec_decode(&codec_ctx, (const uint8_t *)data, size, NULL));
  printf("Error: %s\n", error_str);

  // Step 2: Get codec capabilities
  aom_codec_caps_t caps = aom_codec_get_caps(iface);
  printf("Codec Capabilities: %lu\n", caps);

  // Step 3: Destroy the codec context
  aom_codec_destroy(&codec_ctx);

  // Step 6: Get the name of the codec interface
  const char *iface_name = aom_codec_iface_name(iface);
  printf("Codec Interface Name: %s\n", iface_name);

  // Step 8: Allocate an image with border
  aom_image_t img;
  unsigned int width = 640;
  unsigned int height = 480;
  unsigned int align = 32;
  unsigned int size_align = 16;
  unsigned int border = 8;
  if (aom_img_alloc_with_border(&img, AOM_IMG_FMT_I420, width, height, align, size_align, border) == NULL) {
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(output_fd);
	assert_fd_closed(input_fd);
	return 0;
  }

  // Step 9: Add metadata to the image
  uint32_t type = 1;
  const uint8_t metadata[] = {0x01, 0x02, 0x03};
  size_t metadata_size = sizeof(metadata);
  aom_metadata_insert_flags_t insert_flags = AOM_MIF_ANY_FRAME;
  aom_img_add_metadata(&img, type, metadata, metadata_size, insert_flags);

  // Step 10: Get metadata from the image
  const aom_metadata_t *metadata_ptr = aom_img_get_metadata(&img, 0);
  if (metadata_ptr != NULL) {
    printf("Metadata Type: %u\n", metadata_ptr->type);
    printf("Metadata Size: %zu\n", metadata_ptr->sz);
    // Process metadata payload
  }

  // Step 11: Remove metadata from the image
  aom_img_remove_metadata(&img);

  // Step 12: Get codec error detail
  const char *error_detail = aom_codec_error_detail(&codec_ctx);
  printf("Error Detail: %s\n", error_detail);

  // Step 13: Get codec error
  const char *error = aom_codec_error(&codec_ctx);
  printf("Error: %s\n", error);

  // Step 14: Allocate metadata
  aom_metadata_t *metadata_alloc = aom_img_metadata_alloc(type, metadata, metadata_size, insert_flags);
  if (metadata_alloc != NULL) {
    // Process allocated metadata
    aom_img_metadata_free(metadata_alloc);
  }

  // Step 7: Clean up resources
  aom_img_free(&img);
  aom_codec_destroy(&codec_ctx);
  assert_file_closed(&in_file);;
  assert_file_closed(&out_file);;

  assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(output_fd);
	assert_fd_closed(input_fd);
	return 0;
}