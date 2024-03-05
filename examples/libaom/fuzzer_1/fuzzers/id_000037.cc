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
//<ID> 1330
//<Prompt> ["aom_codec_enc_init_ver","aom_codec_set_cx_data_buf","aom_img_alloc","aom_codec_enc_config_set","aom_codec_encode","aom_img_plane_width"]
/*<Combination>: [aom_codec_err_t aom_codec_enc_init_ver(aom_codec_ctx_t * ctx, aom_codec_iface_t * iface, const aom_codec_enc_cfg_t * cfg, aom_codec_flags_t flags, int ver),
    aom_codec_err_t aom_codec_set_cx_data_buf(aom_codec_ctx_t * ctx, const aom_fixed_buf_t * buf, unsigned int pad_before, unsigned int pad_after),
    aom_image_t *aom_img_alloc(aom_image_t * img, aom_img_fmt_t fmt, unsigned int d_w, unsigned int d_h, unsigned int align),
    aom_codec_err_t aom_codec_enc_config_set(aom_codec_ctx_t * ctx, const aom_codec_enc_cfg_t * cfg),
    aom_codec_err_t aom_codec_encode(aom_codec_ctx_t * ctx, const aom_image_t * img, aom_codec_pts_t pts, unsigned long duration, aom_enc_frame_flags_t flags),
    int aom_img_plane_width(const aom_image_t * img, int plane)
*/
//<score> 13, nr_unique_branch: 12, p_cov: 1
//<Quality> {"diversity":10,"density":10,"covered_api":["aom_codec_enc_init_ver","aom_codec_set_cx_data_buf","aom_img_alloc","aom_codec_enc_config_set","aom_codec_encode","aom_img_plane_width"],"uncovered_api":[],"unique_branches":{"encoder_set_config":[[1495,7,1495,31,1],[1495,35,1495,59,1],[1506,7,1506,30,1],[1516,7,1516,54,1],[1519,7,1519,55,1],[1525,7,1525,26,0],[1532,21,1532,50,0],[1532,21,1532,50,1],[1536,9,1536,34,0],[1541,7,1541,16,1]],"aom_codec_enc_config_set":[[297,44,297,48,1],[299,12,299,55,1]]},"library_calls":["aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_set_cx_data_buf","aom_img_alloc","aom_codec_enc_config_set","aom_codec_encode","aom_img_plane_width","aom_img_free","aom_codec_destroy"]}
/*Here is the code that implements the steps mentioned above:

*/


extern "C" int LLVMFuzzerTestOneInput_37(const uint8_t* f_data, size_t f_size) {
	if(f_size<=40) return 0;

	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(f_data, sizeof(uint8_t), f_size, input_file_ptr);
	fclose(input_file_ptr);

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_1, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_4, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_5, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_6, fdp);
	//fuzzer shim end}




    // Step 4: Open input file
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file == NULL) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Step 5: Get file descriptor for input file
    int in_fd = fuzz_fileno(in_file);

    // Step 6: Set input file name
    const char *input_file = "input_file";

    // Step 7: Open output file
    FILE *out_file = fopen("output_file", "wb");
    if (out_file == NULL) {
        assert_file_closed(&in_file);;
        assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
    }

    // Step 8: Get file descriptor for output file
    int out_fd = fuzz_fileno(out_file);

    // Step 1: Initialize encoder context
    aom_codec_ctx_t encoder_ctx;
    aom_codec_iface_t *encoder_iface = aom_codec_av1_cx();
    aom_codec_enc_cfg_t encoder_cfg;
    aom_codec_enc_config_default(encoder_iface, &encoder_cfg, fuzz_uint32_t_1);
    aom_codec_enc_init_ver(&encoder_ctx, encoder_iface, &encoder_cfg, fuzz_int64_t_2, AOM_ENCODER_ABI_VERSION);

    // Step 2: Set output buffer for encoder
    aom_fixed_buf_t output_buf;
    output_buf.buf = malloc(size);
    output_buf.sz = size;
    aom_codec_set_cx_data_buf(&encoder_ctx, &output_buf, 0, fuzz_uint32_t_3);

    // Step 3: Allocate image buffer
    aom_image_t image;
    aom_img_fmt_t img_fmt = AOM_IMG_FMT_I420;
    unsigned int img_width = 640;
    unsigned int img_height = 480;
    unsigned int img_align = 32;
    aom_img_alloc(&image, img_fmt, img_width, img_height, img_align);

    // Step 9: Set input frame
    aom_codec_enc_config_set(&encoder_ctx, &encoder_cfg);
    aom_codec_encode(&encoder_ctx, &image, fuzz_int64_t_4, fuzz_uint64_t_5, fuzz_int64_t_6);

    // Step 10: Get plane width
    int plane_width = aom_img_plane_width(&image, 0);

    // Step 11: Release allocated resources
    aom_img_free(&image);
    aom_codec_destroy(&encoder_ctx);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    free(output_buf.buf);

    assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}