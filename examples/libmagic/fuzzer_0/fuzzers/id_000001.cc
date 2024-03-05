#include "FDSan.h"
#include <vpx/vp8dx.h>
#include <vpx/vp8cx.h>
#include <vpx/vpx_decoder.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 99
//<Prompt> ["vpx_codec_set_cx_data_buf","vpx_codec_enc_init_multi_ver","vpx_codec_get_frame","vpx_codec_err_to_string","vpx_codec_get_preview_frame"]
/*<Combination>: [vpx_codec_err_t vpx_codec_set_cx_data_buf(vpx_codec_ctx_t * ctx, const vpx_fixed_buf_t * buf, unsigned int pad_before, unsigned int pad_after),
    vpx_codec_err_t vpx_codec_enc_init_multi_ver(vpx_codec_ctx_t * ctx, vpx_codec_iface_t * iface, vpx_codec_enc_cfg_t * cfg, int num_enc, vpx_codec_flags_t flags, vpx_rational_t * dsf, int ver),
    vpx_image_t *vpx_codec_get_frame(vpx_codec_ctx_t * ctx, vpx_codec_iter_t * iter),
    const char *vpx_codec_err_to_string(vpx_codec_err_t err),
    const vpx_image_t *vpx_codec_get_preview_frame(vpx_codec_ctx_t * ctx)
*/
//<score> 10, nr_unique_branch: 3
//<Quality> {"density":5,"unique_branches":{"vp9_remove_compressor":[[2776,7,2776,11,0,0,4,0]],"vpx_codec_enc_init_multi_ver":[[23,32,23,37,9,0,4,1]],"encoder_init":[[1125,9,1125,28,0,0,4,1]]},"library_calls":["vpx_codec_enc_init_multi_ver","vpx_codec_vp9_cx","vpx_codec_enc_init_ver","vpx_codec_vp9_cx","vpx_codec_destroy"],"critical_calls":["vpx_codec_enc_init_multi_ver","vpx_codec_vp9_cx","vpx_codec_enc_init_ver","vpx_codec_vp9_cx","vpx_codec_destroy"],"visited":1}
/**/

extern "C" int LLVMFuzzerTestOneInput_1(const uint8_t *data, size_t size) {
	if(size<0) return 0;

	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);

    // Step 4: Create a FILE * variable to read the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    
    // Step 6: Specify the input file name
    const char *input_file = "input_file";
    
    // Step 5: Get the file descriptor for reading
    int in_fd = fuzz_fileno(in_file);
    
    // Step 1: Set up the libvpx encoder configuration
    vpx_codec_enc_cfg_t enc_cfg;
    vpx_codec_enc_init_multi_ver(NULL, vpx_codec_vp9_cx(), &enc_cfg, 1, 0, NULL, 1);
    
    // Step 2: Create a libvpx encoder context
    vpx_codec_ctx_t encoder;
    vpx_codec_enc_init_ver(&encoder, vpx_codec_vp9_cx(), &enc_cfg, 0, VPX_ENCODER_ABI_VERSION);
    
    // Step 7: Release the allocated resources
    vpx_codec_destroy(&encoder);
    assert_file_closed(&in_file);;
    
    assert_file_name_closed("input_file");
	assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
}