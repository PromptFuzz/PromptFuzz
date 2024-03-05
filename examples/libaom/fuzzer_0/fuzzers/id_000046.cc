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
//<ID> 1752
//<Prompt> ["aom_codec_err_to_string","aom_codec_get_caps","aom_codec_destroy","aom_codec_iface_name","aom_img_add_metadata","aom_codec_set_option","aom_img_get_metadata","aom_codec_error_detail","aom_img_remove_metadata","aom_codec_error","aom_img_metadata_alloc"]
/*<Combination>: [const char *aom_codec_err_to_string(aom_codec_err_t err),
    aom_codec_caps_t aom_codec_get_caps(aom_codec_iface_t * iface),
    aom_codec_err_t aom_codec_destroy(aom_codec_ctx_t * ctx),
    const char *aom_codec_iface_name(aom_codec_iface_t * iface),
    int aom_img_add_metadata(aom_image_t * img, uint32_t type, const uint8_t * data, size_t sz, aom_metadata_insert_flags_t insert_flag),
    aom_codec_err_t aom_codec_set_option(aom_codec_ctx_t * ctx, const char * name, const char * value),
    const aom_metadata_t *aom_img_get_metadata(const aom_image_t * img, size_t index),
    const char *aom_codec_error_detail(const aom_codec_ctx_t * ctx),
    void aom_img_remove_metadata(aom_image_t * img),
    const char *aom_codec_error(const aom_codec_ctx_t * ctx),
    aom_metadata_t *aom_img_metadata_alloc(uint32_t type, const uint8_t * data, size_t sz, aom_metadata_insert_flags_t insert_flag)
*/
//<score> 3.1168833, nr_unique_branch: 4, p_cov: 0.72727275
//<Quality> {"diversity":14,"density":12,"covered_api":["aom_codec_destroy","aom_img_add_metadata","aom_codec_set_option","aom_img_get_metadata","aom_codec_error_detail","aom_img_remove_metadata","aom_codec_error","aom_img_metadata_alloc"],"uncovered_api":["aom_codec_err_to_string","aom_codec_get_caps","aom_codec_iface_name"],"unique_branches":{"aom_img_remove_metadata":[[382,7,382,10,1]],"ctrl_set_byte_alignment":[[1524,7,1524,46,0],[1525,8,1525,43,0]],"aom_img_get_metadata":[[390,7,390,11,0]]},"library_calls":["aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_control","aom_codec_decode","aom_codec_get_frame","aom_img_add_metadata","aom_img_get_metadata","aom_codec_set_option","aom_codec_error_detail","aom_img_remove_metadata","aom_codec_error","aom_img_metadata_alloc","aom_codec_destroy","aom_img_metadata_free"]}
/*Below is a step-by-step implementation of the C++ program using the libaom library APIs:

*/


extern "C" int LLVMFuzzerTestOneInput_46(const uint8_t *data, size_t size) {
	if(size<=0) return 0;

  // Create input and output file variables
  FILE *in_file = fmemopen((void *)data, size, "rb");
  FILE *out_file = fopen("output_file", "wb");

  // Check if file variables are created successfully
  if (in_file == nullptr || out_file == nullptr) {
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
  }

  // Read the input data from file
  fseek(in_file, 0, SEEK_END);
  size_t input_size = ftell(in_file);
  fseek(in_file, 0, SEEK_SET);
  uint8_t *input_data = new uint8_t[input_size];
  fread(input_data, 1, input_size, in_file);

  // Create and initialize the libaom decoder
  aom_codec_iface_t *decoder = aom_codec_av1_dx();
  aom_codec_ctx_t decoder_ctx;
  aom_codec_dec_cfg_t decoder_cfg = {0};
  aom_codec_dec_init(&decoder_ctx, decoder, &decoder_cfg, 0);

  // Decode the input data
  aom_codec_control(&decoder_ctx, AV1_SET_BYTE_ALIGNMENT, 16);
  aom_codec_decode(&decoder_ctx, input_data, input_size, nullptr);

  // Get the decoded frame
  aom_codec_iter_t iter = nullptr;
  const aom_image_t *decoded_frame = aom_codec_get_frame(&decoder_ctx, &iter);

  // Add metadata to the decoded frame
  uint32_t metadata_type = 0;
  const uint8_t *metadata_data = nullptr;
  size_t metadata_size = 0;
  aom_metadata_insert_flags_t metadata_insert_flag = AOM_MIF_ANY_FRAME;
  aom_img_add_metadata(const_cast<aom_image_t *>(decoded_frame), metadata_type,
                       metadata_data, metadata_size, metadata_insert_flag);

  // Get the added metadata
  size_t metadata_index = 0;
  const aom_metadata_t *added_metadata =
      aom_img_get_metadata(decoded_frame, metadata_index);

  // Set decoder option
  const char *option_name = "option_name";
  const char *option_value = "option_value";
  aom_codec_set_option(&decoder_ctx, option_name, option_value);

  // Get decoder error detail
  const char *error_detail = aom_codec_error_detail(&decoder_ctx);

  // Remove metadata from the decoded frame
  aom_img_remove_metadata(const_cast<aom_image_t *>(decoded_frame));

  // Get decoder error
  const char *error = aom_codec_error(&decoder_ctx);

  // Create metadata
  uint32_t new_metadata_type = 1;
  const uint8_t *new_metadata_data = nullptr;
  size_t new_metadata_size = 0;
  aom_metadata_insert_flags_t new_metadata_insert_flag = AOM_MIF_ANY_FRAME;
  aom_metadata_t *new_metadata =
      aom_img_metadata_alloc(new_metadata_type, new_metadata_data,
                             new_metadata_size, new_metadata_insert_flag);

  // Release allocated resources
  assert_file_closed(&in_file);;
  assert_file_closed(&out_file);;
  delete[] input_data;
  aom_codec_destroy(&decoder_ctx);
  aom_img_metadata_free(new_metadata);

  assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}