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
//<ID> 908
//<Prompt> ["aom_uleb_encode_fixed_size","aom_img_alloc","aom_codec_set_frame_buffer_functions","aom_img_remove_metadata","aom_codec_av1_cx","aom_codec_get_stream_info"]
/*<Combination>: [int aom_uleb_encode_fixed_size(uint64_t value, size_t available, size_t pad_to_size, uint8_t * coded_value, size_t * coded_size),
    aom_image_t *aom_img_alloc(aom_image_t * img, aom_img_fmt_t fmt, unsigned int d_w, unsigned int d_h, unsigned int align),
    aom_codec_err_t aom_codec_set_frame_buffer_functions(aom_codec_ctx_t * ctx, aom_get_frame_buffer_cb_fn_t cb_get, aom_release_frame_buffer_cb_fn_t cb_release, void * cb_priv),
    void aom_img_remove_metadata(aom_image_t * img),
    aom_codec_iface_t *aom_codec_av1_cx(),
    aom_codec_err_t aom_codec_get_stream_info(aom_codec_ctx_t * ctx, aom_codec_stream_info_t * si)
*/
//<score> 8.333334, nr_unique_branch: 14, p_cov: 1
//<Quality> {"diversity":9,"density":5,"covered_api":["aom_uleb_encode_fixed_size","aom_img_alloc","aom_codec_set_frame_buffer_functions","aom_img_remove_metadata","aom_codec_av1_cx","aom_codec_get_stream_info"],"uncovered_api":[],"unique_branches":{"aom_uleb_encode_fixed_size":[[82,7,82,34,0],[82,7,82,34,1],[82,38,82,50,1],[82,54,82,65,1],[83,7,83,30,0],[83,7,83,30,1],[83,34,83,66,0],[83,34,83,66,1],[87,7,87,21,0],[87,7,87,21,1],[92,22,92,37,0],[92,22,92,37,1],[96,9,96,28,0],[96,9,96,28,1]]},"library_calls":["aom_uleb_encode_fixed_size","aom_img_alloc","aom_codec_set_frame_buffer_functions","aom_img_remove_metadata","aom_codec_av1_cx","aom_codec_dec_init_ver","aom_codec_get_stream_info","aom_img_free","aom_codec_destroy"]}
/*Below is the implementation of the fuzz driver using the libaom library APIs:

*/



extern "C" int LLVMFuzzerTestOneInput_24(const uint8_t* f_data, size_t f_size) {
	if(f_size<=16) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_3, fdp);
	//fuzzer shim end}




  FILE *in_file = fmemopen((void *)data, size, "rb");
  FILE *out_file = fopen("output_file", "wb");
  
  // Get the file descriptor for reading and writing
  int in_fd = fuzz_fileno(in_file);
  int out_fd = fuzz_fileno(out_file);
  
  // Read and process the input data
  uint64_t value = 0;
  size_t pad_to_size = 0;
  uint8_t coded_value[64];
  size_t coded_size = 0;
  
  if (fread(&value, sizeof(uint64_t), 1, in_file) != 1) {
    // Handle error
  }
  
  if (fread(&pad_to_size, sizeof(size_t), 1, in_file) != 1) {
    // Handle error
  }
  
  aom_uleb_encode_fixed_size(value, sizeof(coded_value), pad_to_size, coded_value, &coded_size);
  
  fwrite(coded_value, sizeof(uint8_t), coded_size, out_file);
  
  // Allocate and initialize an aom_image_t
  aom_image_t img;
  aom_img_alloc(&img, AOM_IMG_FMT_I420, fuzz_uint32_t_1, fuzz_uint32_t_2, 16);
  
  // Set frame buffer functions for the codec context
  aom_codec_ctx_t ctx;
  aom_codec_set_frame_buffer_functions(&ctx, NULL, NULL, NULL);
  
  // Remove metadata from the image
  aom_img_remove_metadata(&img);
  
  // Initialize the AV1 codec
  aom_codec_err_t codec_err;
  aom_codec_iface_t *codec_iface = aom_codec_av1_cx();
  codec_err = aom_codec_dec_init_ver(&ctx, codec_iface, NULL, fuzz_int64_t_3, AOM_DECODER_ABI_VERSION);
  
  // Get the stream info from the codec context
  aom_codec_stream_info_t si;
  codec_err = aom_codec_get_stream_info(&ctx, &si);
  
  // Free the allocated resources
  assert_file_closed(&in_file);;
  assert_file_closed(&out_file);;
  aom_img_free(&img);
  aom_codec_destroy(&ctx);
  
  assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}