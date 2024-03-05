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
//<ID> 150
//<Prompt> ["aom_img_add_metadata","aom_codec_control","aom_img_flip","aom_codec_version_str","aom_codec_set_cx_data_buf","aom_img_num_metadata","aom_img_set_rect"]
/*<Combination>: [int aom_img_add_metadata(aom_image_t * img, uint32_t type, const uint8_t * data, size_t sz, aom_metadata_insert_flags_t insert_flag),
    aom_codec_err_t aom_codec_control(aom_codec_ctx_t * ctx, int ctrl_id),
    void aom_img_flip(aom_image_t * img),
    const char *aom_codec_version_str(),
    aom_codec_err_t aom_codec_set_cx_data_buf(aom_codec_ctx_t * ctx, const aom_fixed_buf_t * buf, unsigned int pad_before, unsigned int pad_after),
    size_t aom_img_num_metadata(const aom_image_t * img),
    int aom_img_set_rect(aom_image_t * img, unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int border)
*/
//<score> 0.2857143, nr_unique_branch: 1, p_cov: 0.14285715
//<Quality> {"diversity":10,"density":10,"covered_api":["aom_img_set_rect"],"uncovered_api":["aom_img_add_metadata","aom_codec_control","aom_img_flip","aom_codec_version_str","aom_codec_set_cx_data_buf","aom_img_num_metadata"],"unique_branches":{"validate_img":[[918,7,918,31,0]]},"library_calls":["aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_img_alloc","aom_img_set_rect","aom_codec_encode","aom_codec_get_cx_data","aom_img_free","aom_codec_destroy"]}
/*Here is a C++ program that implements the LLVMFuzzerTestOneInput_6 function using the libaom library APIs to achieve the event described above:

*/



// Include libaom headers

extern "C" int LLVMFuzzerTestOneInput_6(const uint8_t *data, size_t size) {
	if(size<=0) return 0;

  // Create an input file from the data buffer
  FILE *in_file = fmemopen((void *)data, size, "rb");
  if (!in_file) {
    assert_file_closed(&in_file);
	return 0;
  }

  // Create an output file to write the result
  FILE *out_file = fopen("output_file", "wb");
  if (!out_file) {
    assert_file_closed(&in_file);;
    assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
  }

  // Set the input file as the source for the codec
  int in_file_descriptor = fuzz_fileno(in_file);
  int out_file_descriptor = fuzz_fileno(out_file);

  // Initialize the codec context
  aom_codec_ctx_t codec_ctx;
  memset(&codec_ctx, 0, sizeof(codec_ctx));

  // Open the codec
  aom_codec_iface_t *codec_iface = aom_codec_av1_cx();
  aom_codec_enc_cfg_t codec_cfg;
  aom_codec_enc_config_default(codec_iface, &codec_cfg, 0);
  if (aom_codec_enc_init_ver(&codec_ctx, codec_iface, &codec_cfg, 0, AOM_ENCODER_ABI_VERSION) != AOM_CODEC_OK) {
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_file_descriptor);
	assert_fd_closed(in_file_descriptor);
	return 0;
  }

  // Create an image
  aom_image_t *image = aom_img_alloc(NULL, AOM_IMG_FMT_I420, 1280, 720, 16);
  if (!image) {
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    aom_codec_destroy(&codec_ctx);
    assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_file_descriptor);
	assert_fd_closed(in_file_descriptor);
	return 0;
  }

  // Set the image rectangle
  aom_img_set_rect(image, 0, 0, 1280, 720, 0);

  // Encode the image
  aom_codec_pts_t pts = 0;
  aom_codec_encode(&codec_ctx, image, pts, 1, 0);

  // Get the compressed data
  const aom_codec_cx_pkt_t *pkt;
  while ((pkt = aom_codec_get_cx_data(&codec_ctx, NULL)) != NULL) {
    if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
      // Write the compressed data to the output file
      fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
    }
  }

  // Clean up
  aom_img_free(image);
  aom_codec_destroy(&codec_ctx);
  assert_file_closed(&in_file);;
  assert_file_closed(&out_file);;

  assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_file_descriptor);
	assert_fd_closed(in_file_descriptor);
	return 0;
}