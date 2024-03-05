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
//<ID> 1242
//<Prompt> ["aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_set_cx_data_buf","aom_img_alloc","aom_codec_encode","aom_img_plane_width"]
/*<Combination>: [aom_codec_err_t aom_codec_enc_config_default(aom_codec_iface_t * iface, aom_codec_enc_cfg_t * cfg, unsigned int usage),
    aom_codec_err_t aom_codec_enc_init_ver(aom_codec_ctx_t * ctx, aom_codec_iface_t * iface, const aom_codec_enc_cfg_t * cfg, aom_codec_flags_t flags, int ver),
    aom_codec_err_t aom_codec_set_cx_data_buf(aom_codec_ctx_t * ctx, const aom_fixed_buf_t * buf, unsigned int pad_before, unsigned int pad_after),
    aom_image_t *aom_img_alloc(aom_image_t * img, aom_img_fmt_t fmt, unsigned int d_w, unsigned int d_h, unsigned int align),
    aom_codec_err_t aom_codec_encode(aom_codec_ctx_t * ctx, const aom_image_t * img, aom_codec_pts_t pts, unsigned long duration, aom_enc_frame_flags_t flags),
    int aom_img_plane_width(const aom_image_t * img, int plane)
*/
//<score> 2.222222, nr_unique_branch: 2, p_cov: 0.8333333
//<Quality> {"diversity":9,"density":8,"covered_api":["aom_codec_enc_init_ver","aom_codec_set_cx_data_buf","aom_img_alloc","aom_codec_encode","aom_img_plane_width"],"uncovered_api":["aom_codec_enc_config_default"],"unique_branches":{"aom_codec_enc_init_ver":[[62,12,62,16,0],[31,32,31,35,1]]},"library_calls":["aom_codec_av1_cx","aom_codec_enc_init_ver","aom_img_alloc","aom_codec_set_cx_data_buf","aom_codec_enc_init_ver","aom_codec_encode","aom_img_plane_width","aom_img_free","aom_codec_destroy"]}
/*Here is the C++ program that implements the steps mentioned above:

*/


extern "C" int LLVMFuzzerTestOneInput_34(const uint8_t *data, size_t size) {
	if(size<=0) return 0;

    aom_codec_iface_t* iface = aom_codec_av1_cx();
    aom_codec_enc_cfg_t cfg;
    aom_codec_enc_init_ver(NULL, iface, &cfg, 0, AOM_ENCODER_ABI_VERSION);

    aom_image_t img;
    aom_img_alloc(&img, AOM_IMG_FMT_I420, 1920, 1080, 16);

    FILE* in_file = fmemopen((void *)data, size, "rb");
    FILE* out_file = fopen("output_file", "wb");

    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);

    aom_fixed_buf_t cx_buf;
    aom_codec_set_cx_data_buf(NULL, &cx_buf, 0, 0);

    aom_codec_ctx_t ctx;
    aom_codec_enc_init_ver(&ctx, iface, &cfg, 0, AOM_ENCODER_ABI_VERSION);
    aom_codec_encode(&ctx, &img, 0, 0, 0);

    int plane_width = aom_img_plane_width(&img, 0);

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