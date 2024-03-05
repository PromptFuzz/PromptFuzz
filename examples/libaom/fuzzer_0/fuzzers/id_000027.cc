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
//<ID> 1060
//<Prompt> ["aom_codec_error","aom_codec_enc_init_ver","aom_img_alloc","aom_img_set_rect","aom_img_get_metadata","aom_img_metadata_free","aom_codec_encode","aom_img_free"]
/*<Combination>: [const char *aom_codec_error(const aom_codec_ctx_t * ctx),
    aom_codec_err_t aom_codec_enc_init_ver(aom_codec_ctx_t * ctx, aom_codec_iface_t * iface, const aom_codec_enc_cfg_t * cfg, aom_codec_flags_t flags, int ver),
    aom_image_t *aom_img_alloc(aom_image_t * img, aom_img_fmt_t fmt, unsigned int d_w, unsigned int d_h, unsigned int align),
    int aom_img_set_rect(aom_image_t * img, unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int border),
    const aom_metadata_t *aom_img_get_metadata(const aom_image_t * img, size_t index),
    void aom_img_metadata_free(aom_metadata_t * metadata),
    aom_codec_err_t aom_codec_encode(aom_codec_ctx_t * ctx, const aom_image_t * img, aom_codec_pts_t pts, unsigned long duration, aom_enc_frame_flags_t flags),
    void aom_img_free(aom_image_t * img)
*/
//<score> 1.75, nr_unique_branch: 1, p_cov: 0.875
//<Quality> {"diversity":10,"density":10,"covered_api":["aom_codec_enc_init_ver","aom_img_alloc","aom_img_set_rect","aom_img_get_metadata","aom_img_metadata_free","aom_codec_encode","aom_img_free"],"uncovered_api":["aom_codec_error"],"unique_branches":{"aom_img_get_metadata":[[392,7,392,12,1]]},"library_calls":["aom_codec_av1_cx","aom_codec_enc_init_ver","aom_img_alloc","aom_img_set_rect","aom_img_get_metadata","aom_img_metadata_free","aom_codec_encode","aom_codec_get_cx_data","aom_img_free","aom_codec_destroy"]}
/**/


extern "C" int LLVMFuzzerTestOneInput_27(const uint8_t *data, size_t size) {
	if(size<=0) return 0;

  // Step 4: Create input file
  FILE *in_file = fmemopen((void *)data, size, "rb");
  if (!in_file) {
    assert_file_closed(&in_file);
	return 0;
  }

  // Step 5: Get file descriptor for reading
  int in_fd = fuzz_fileno(in_file);

  // Step 6: Create output file
  FILE *out_file = fopen("output_file", "wb");
  if (!out_file) {
    assert_file_closed(&in_file);;
    assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
  }

  // Step 7: Get file descriptor for writing
  int out_fd = fuzz_fileno(out_file);

  // Step 2: Initialize encoder context and configure it
  aom_codec_ctx_t encoder;
  aom_codec_iface_t *encoder_iface = aom_codec_av1_cx();
  aom_codec_enc_cfg_t encoder_cfg = {0};
  aom_codec_enc_init_ver(&encoder, encoder_iface, &encoder_cfg, 0, AOM_DECODER_ABI_VERSION);

  // Step 3: Allocate input image frame
  aom_image_t input_image;
  aom_img_alloc(&input_image, AOM_IMG_FMT_I420, 640, 480, 16);

  // Step 4: Set input image rectangle
  aom_img_set_rect(&input_image, 0, 0, 640, 480, 2);

  // Step 5: Get metadata from input image
  const aom_metadata_t *metadata = aom_img_get_metadata(&input_image, 0);
  if (metadata) {
    // Perform operations with metadata
  }

  // Step 6: Free metadata
  aom_img_metadata_free((aom_metadata_t *)metadata);

  // Step 7: Encode input image
  aom_codec_pts_t pts = 0;
  aom_enc_frame_flags_t flags = 0;
  aom_codec_encode(&encoder, &input_image, pts, 0, flags);

  // Step 7: Get encoded output data
  const aom_codec_cx_pkt_t *packet = nullptr;
  while ((packet = aom_codec_get_cx_data(&encoder, nullptr)) != nullptr) {
    if (packet->kind == AOM_CODEC_CX_FRAME_PKT) {
      // Write encoded frame data to output file
      fwrite(packet->data.frame.buf, packet->data.frame.sz, 1, out_file);
    }
  }

  // Step 6: Free input image frame
  aom_img_free(&input_image);

  // Step 7: Destroy encoder context
  aom_codec_destroy(&encoder);

  // Step 7: Close input and output files
  assert_file_closed(&in_file);;
  assert_file_closed(&out_file);;

  assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}