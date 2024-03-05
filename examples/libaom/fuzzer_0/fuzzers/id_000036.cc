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
//<ID> 1285
//<Prompt> ["aom_codec_enc_config_set","aom_codec_iface_name","aom_codec_enc_init_ver","aom_codec_set_cx_data_buf","aom_img_alloc","aom_codec_encode","aom_img_plane_width"]
/*<Combination>: [aom_codec_err_t aom_codec_enc_config_set(aom_codec_ctx_t * ctx, const aom_codec_enc_cfg_t * cfg),
    const char *aom_codec_iface_name(aom_codec_iface_t * iface),
    aom_codec_err_t aom_codec_enc_init_ver(aom_codec_ctx_t * ctx, aom_codec_iface_t * iface, const aom_codec_enc_cfg_t * cfg, aom_codec_flags_t flags, int ver),
    aom_codec_err_t aom_codec_set_cx_data_buf(aom_codec_ctx_t * ctx, const aom_fixed_buf_t * buf, unsigned int pad_before, unsigned int pad_after),
    aom_image_t *aom_img_alloc(aom_image_t * img, aom_img_fmt_t fmt, unsigned int d_w, unsigned int d_h, unsigned int align),
    aom_codec_err_t aom_codec_encode(aom_codec_ctx_t * ctx, const aom_image_t * img, aom_codec_pts_t pts, unsigned long duration, aom_enc_frame_flags_t flags),
    int aom_img_plane_width(const aom_image_t * img, int plane)
*/
//<score> 2.5714285, nr_unique_branch: 2, p_cov: 0.85714287
//<Quality> {"diversity":9,"density":9,"covered_api":["aom_codec_enc_config_set","aom_codec_enc_init_ver","aom_codec_set_cx_data_buf","aom_img_alloc","aom_codec_encode","aom_img_plane_width"],"uncovered_api":["aom_codec_iface_name"],"unique_branches":{"aom_codec_enc_config_default":[[104,7,104,13,0]],"aom_codec_enc_init_ver":[[62,20,62,26,0]]},"library_calls":["aom_codec_enc_config_default","aom_codec_enc_config_set","aom_codec_enc_init_ver","aom_codec_set_cx_data_buf","aom_img_alloc","aom_codec_encode","aom_img_plane_width","aom_img_free","aom_codec_destroy"]}
/*Here is the completed C++ program that achieves the event using the libaom APIs:

*/


extern "C" int LLVMFuzzerTestOneInput_36(const uint8_t *data, size_t size) {
	if(size<=0) return 0;

	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);

    // Create input file pointer
    FILE *in_file = fmemopen((void *)data, size, "rb");

    // Create output file pointer
    FILE *out_file = fopen("output_file", "wb");

    // Create input file descriptor
    int in_fd = fuzz_fileno(in_file);

    // Create output file descriptor
    int out_fd = fuzz_fileno(out_file);

    // Create input and output file names
    const char *input_file = "input_file";
    const char *output_file = "output_file";

    // Initialize codec context
    aom_codec_ctx_t codec_ctx;
    aom_codec_iface_t *codec_iface;
    aom_codec_enc_cfg_t codec_cfg;
    aom_codec_flags_t codec_flags = 0;
    int codec_ver = AOM_ENCODER_ABI_VERSION;

    // Initialize encoder configuration
    aom_codec_enc_config_default(codec_iface, &codec_cfg, 0);

    // Set encoder configuration
    aom_codec_enc_config_set(&codec_ctx, &codec_cfg);

    // Initialize encoder
    aom_codec_enc_init_ver(&codec_ctx, codec_iface, &codec_cfg, codec_flags, codec_ver);

    // Set output buffer
    aom_fixed_buf_t output_buffer;
    unsigned int pad_before = 0;
    unsigned int pad_after = 0;
    aom_codec_set_cx_data_buf(&codec_ctx, &output_buffer, pad_before, pad_after);

    // Allocate input image
    aom_img_fmt_t img_fmt = AOM_IMG_FMT_I420;
    unsigned int img_width = 1920;
    unsigned int img_height = 1080;
    unsigned int img_align = 16;
    aom_image_t *input_img = aom_img_alloc(NULL, img_fmt, img_width, img_height, img_align);

    // Encode image
    aom_enc_frame_flags_t enc_flags = 0;
    aom_codec_pts_t pts = 0;
    unsigned long duration = 0;
    aom_codec_encode(&codec_ctx, input_img, pts, duration, enc_flags);

    // Get plane width
    int plane_index = 0;
    int plane_width = aom_img_plane_width(input_img, plane_index);

    // Release resources
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    aom_img_free(input_img);
    aom_codec_destroy(&codec_ctx);

    assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}