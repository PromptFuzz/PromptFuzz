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
//<ID> 780
//<Prompt> ["aom_codec_dec_init_ver","aom_codec_av1_cx","aom_img_metadata_alloc","aom_codec_enc_config_default","aom_img_alloc_with_border"]
/*<Combination>: [aom_codec_err_t aom_codec_dec_init_ver(aom_codec_ctx_t * ctx, aom_codec_iface_t * iface, const aom_codec_dec_cfg_t * cfg, aom_codec_flags_t flags, int ver),
    aom_codec_iface_t *aom_codec_av1_cx(),
    aom_metadata_t *aom_img_metadata_alloc(uint32_t type, const uint8_t * data, size_t sz, aom_metadata_insert_flags_t insert_flag),
    aom_codec_err_t aom_codec_enc_config_default(aom_codec_iface_t * iface, aom_codec_enc_cfg_t * cfg, unsigned int usage),
    aom_image_t *aom_img_alloc_with_border(aom_image_t * img, aom_img_fmt_t fmt, unsigned int d_w, unsigned int d_h, unsigned int align, unsigned int size_align, unsigned int border)
*/
//<score> 1.0181818, nr_unique_branch: 1, p_cov: 0.8
//<Quality> {"diversity":11,"density":7,"covered_api":["aom_codec_dec_init_ver","aom_codec_av1_cx","aom_codec_enc_config_default","aom_img_alloc_with_border"],"uncovered_api":["aom_img_metadata_alloc"],"unique_branches":{"align_image_dimension":[[27,11,27,35,0]]},"library_calls":["aom_codec_dec_init_ver","aom_codec_av1_dx","aom_codec_enc_config_default","aom_codec_av1_cx","aom_img_alloc_with_border","aom_codec_decode","aom_codec_encode","aom_codec_get_cx_data","aom_codec_destroy","aom_codec_destroy","aom_img_free"]}
/*Here is the completed C++ program that utilizes libaom library APIs to achieve the desired event:

*/


extern "C" int LLVMFuzzerTestOneInput_19(const uint8_t* f_data, size_t f_size) {
	if(f_size<=44) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int64_t, fuzz_int64_t_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_4, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_5, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_6, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_7, fdp);
	//fuzzer shim end}




    // Step 4: Read input data from the provided buffer (`data`) using `fmemopen`
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    int in_fd = fuzz_fileno(in_file); // File descriptor for reading

    // Step 5: Initialize aom decoder using `aom_codec_dec_init_ver`
    aom_codec_ctx_t decoder;
    aom_codec_dec_cfg_t dec_cfg = {0};
    aom_codec_dec_init_ver(&decoder, aom_codec_av1_dx(), &dec_cfg, fuzz_int64_t_1, AOM_DECODER_ABI_VERSION);

    // Step 6: Initialize aom encoder using `aom_codec_enc_config_default`
    aom_codec_ctx_t encoder;
    aom_codec_enc_cfg_t enc_cfg;
    aom_codec_enc_config_default(aom_codec_av1_cx(), &enc_cfg, fuzz_uint32_t_2);

    // Step 7: Allocate image with border using `aom_img_alloc_with_border`
    aom_image_t image;
    aom_img_alloc_with_border(&image, AOM_IMG_FMT_I420, enc_cfg.g_w, enc_cfg.g_h, 16, fuzz_uint32_t_3, fuzz_uint32_t_4);

    // Step 8: Read input from file descriptor and decode using `aom_codec_decode`
    uint8_t buffer[4096];
    size_t bytes_read = fread(buffer, 1, sizeof(buffer), in_file);
    aom_codec_decode(&decoder, buffer, bytes_read, NULL);

    // Step 9: Encode the decoded image using `aom_codec_encode`
    aom_codec_encode(&encoder, &image, fuzz_int64_t_5, fuzz_uint64_t_6, fuzz_int64_t_7);

    // Step 10: Get the encoded data using `aom_codec_get_cx_data`
    const aom_codec_cx_pkt_t *pkt;
    while ((pkt = aom_codec_get_cx_data(&encoder, NULL))) {
        if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
            // Step 11: Write the encoded data to an output file
            FILE *out_file = fopen("output_file", "wb");
            if (!out_file) {
                break;
            }
            int out_fd = fuzz_fileno(out_file); // File descriptor for writing
            size_t bytes_written = fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
            assert_file_closed(&out_file);;
            break;
        }
    }

    // Step 12: Release all allocated resources
    assert_file_closed(&in_file);;
    aom_codec_destroy(&decoder);
    aom_codec_destroy(&encoder);
    aom_img_free(&image);

    assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
}