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
//<ID> 1180
//<Prompt> ["aom_img_plane_width","aom_codec_av1_dx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_set_frame_buffer_functions","aom_codec_destroy"]
/*<Combination>: [int aom_img_plane_width(const aom_image_t * img, int plane),
    aom_codec_iface_t *aom_codec_av1_dx(),
    aom_codec_err_t aom_codec_enc_config_default(aom_codec_iface_t * iface, aom_codec_enc_cfg_t * cfg, unsigned int usage),
    aom_codec_err_t aom_codec_enc_init_ver(aom_codec_ctx_t * ctx, aom_codec_iface_t * iface, const aom_codec_enc_cfg_t * cfg, aom_codec_flags_t flags, int ver),
    aom_codec_err_t aom_codec_set_frame_buffer_functions(aom_codec_ctx_t * ctx, aom_get_frame_buffer_cb_fn_t cb_get, aom_release_frame_buffer_cb_fn_t cb_release, void * cb_priv),
    aom_codec_err_t aom_codec_destroy(aom_codec_ctx_t * ctx)
*/
//<score> 1.6666666, nr_unique_branch: 1, p_cov: 1
//<Quality> {"diversity":6,"density":5,"covered_api":["aom_img_plane_width","aom_codec_av1_dx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_set_frame_buffer_functions","aom_codec_destroy"],"uncovered_api":[],"unique_branches":{"aom_codec_enc_init_ver":[[66,12,66,50,0]]},"library_calls":["aom_img_plane_width","aom_codec_av1_dx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_set_frame_buffer_functions","aom_codec_destroy"]}
/**/


extern "C" int LLVMFuzzerTestOneInput_32(const uint8_t *data, size_t size) {
	if(size<=0) return 0;

	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);

    // Step 4: Create FILE variables for input and output
    FILE *in_file = fmemopen((void *)data, size, "rb");
    FILE *out_file = fopen("output_file", "wb");

    // Step 5: Get file descriptors
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);

    // Step 1: Determine the width of a plane in the image
    aom_image_t img;
    int plane_width = aom_img_plane_width(&img, 0);

    // Step 2: Create a codec interface for AV1 decoding
    aom_codec_iface_t *codec_iface = aom_codec_av1_dx();

    // Step 3: Configure the encoding parameters
    aom_codec_enc_cfg_t enc_cfg;
    aom_codec_enc_config_default(codec_iface, &enc_cfg, 0);

    // Step 6: Use file names directly
    const char *input_file = "input_file";
    const char *output_file = "output_file";

    // Step 2: Create a codec context for encoding
    aom_codec_ctx_t codec_ctx;
    aom_codec_enc_init_ver(&codec_ctx, codec_iface, &enc_cfg, 0, AOM_ENCODER_ABI_VERSION);

    // Step 5: Set frame buffer functions
    aom_codec_set_frame_buffer_functions(&codec_ctx, nullptr, nullptr, nullptr);

    // Step 3: Destroy the codec context
    aom_codec_destroy(&codec_ctx);

    // Step 7: Release resources
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