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
//<ID> 1120
//<Prompt> ["aom_codec_enc_init_ver","aom_uleb_encode","aom_img_alloc","aom_img_set_rect","aom_codec_encode","aom_img_free"]
/*<Combination>: [aom_codec_err_t aom_codec_enc_init_ver(aom_codec_ctx_t * ctx, aom_codec_iface_t * iface, const aom_codec_enc_cfg_t * cfg, aom_codec_flags_t flags, int ver),
    int aom_uleb_encode(uint64_t value, size_t available, uint8_t * coded_value, size_t * coded_size),
    aom_image_t *aom_img_alloc(aom_image_t * img, aom_img_fmt_t fmt, unsigned int d_w, unsigned int d_h, unsigned int align),
    int aom_img_set_rect(aom_image_t * img, unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int border),
    aom_codec_err_t aom_codec_encode(aom_codec_ctx_t * ctx, const aom_image_t * img, aom_codec_pts_t pts, unsigned long duration, aom_enc_frame_flags_t flags),
    void aom_img_free(aom_image_t * img)
*/
//<score> 6.6666665, nr_unique_branch: 7, p_cov: 0.8333333
//<Quality> {"diversity":8,"density":8,"covered_api":["aom_codec_enc_init_ver","aom_img_alloc","aom_img_set_rect","aom_codec_encode","aom_img_free"],"uncovered_api":["aom_uleb_encode"],"unique_branches":{"img_alloc_helper":[[64,5,64,26,1],[65,5,65,26,0],[82,5,82,26,1],[83,5,83,26,0],[95,5,95,26,1],[96,5,96,26,0]],"aom_img_set_rect":[[242,18,242,51,1]]},"library_calls":["aom_codec_av1_cx","aom_codec_enc_init_ver","aom_img_alloc","aom_img_set_rect","aom_codec_encode","aom_codec_get_cx_data","aom_img_free","aom_codec_destroy"]}
/*Here is the implementation of the C++ program that follows the instructions provided:

*/


extern "C" int LLVMFuzzerTestOneInput_29(const uint8_t* f_data, size_t f_size) {
	if(f_size<=28) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int64_t, fuzz_int64_t_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_4, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_5, fdp);
	//fuzzer shim end}




    // Create a file descriptor for reading
    FILE *in_file = fmemopen((void *)data, size, "rb");
    int in_fd = fuzz_fileno(in_file);

    // Create a file descriptor for writing
    FILE *out_file = fopen("output_file", "wb");
    int out_fd = fuzz_fileno(out_file);

    // Initialize libaom encoder
    aom_codec_ctx_t encoder;
    aom_codec_enc_cfg_t cfg;
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    aom_codec_err_t ret = aom_codec_enc_init_ver(&encoder, iface, &cfg, fuzz_int64_t_1, AOM_ENCODER_ABI_VERSION);

    // Read input data and create aom_image
    unsigned char *img_data = nullptr; // Replace nullptr with valid input data
    unsigned int width = 128; // Replace with valid width
    unsigned int height = 128; // Replace with valid height
    aom_image_t img;
    aom_img_alloc(&img, AOM_IMG_FMT_YV12, width, height, 16);
    aom_img_set_rect(&img, fuzz_uint32_t_2, fuzz_uint32_t_3, width, height, fuzz_uint32_t_4);

    // Encode the image
    aom_codec_pts_t pts = 0;
    aom_enc_frame_flags_t flags = 0;
    ret = aom_codec_encode(&encoder, &img, pts, fuzz_uint64_t_5, flags);

    // Write the encoded data to output file
    aom_codec_iter_t iter = nullptr;
    const aom_codec_cx_pkt_t *pkt = nullptr;
    while ((pkt = aom_codec_get_cx_data(&encoder, &iter))) {
        fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
    }

    // Free resources
    assert_file_closed(&out_file);;
    assert_file_closed(&in_file);;
    aom_img_free(&img);
    aom_codec_destroy(&encoder);

    assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}