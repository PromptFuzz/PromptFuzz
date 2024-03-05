#include "FDSan.h"
#include "FuzzedDataProvider.h"
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
//<ID> 2859
//<Prompt> ["vpx_codec_control_","vpx_codec_get_cx_data","vpx_codec_enc_config_set","vpx_img_set_rect","vpx_codec_set_cx_data_buf","vpx_read_tpl_gop_stats","vpx_codec_get_stream_info"]
/*<Combination>: [vpx_codec_err_t vpx_codec_control_(vpx_codec_ctx_t * ctx, int ctrl_id),
    const vpx_codec_cx_pkt_t *vpx_codec_get_cx_data(vpx_codec_ctx_t * ctx, vpx_codec_iter_t * iter),
    vpx_codec_err_t vpx_codec_enc_config_set(vpx_codec_ctx_t * ctx, const vpx_codec_enc_cfg_t * cfg),
    int vpx_img_set_rect(vpx_image_t * img, unsigned int x, unsigned int y, unsigned int w, unsigned int h),
    vpx_codec_err_t vpx_codec_set_cx_data_buf(vpx_codec_ctx_t * ctx, const vpx_fixed_buf_t * buf, unsigned int pad_before, unsigned int pad_after),
    vpx_codec_err_t vpx_read_tpl_gop_stats(FILE * tpl_file, VpxTplGopStats * tpl_gop_stats),
    vpx_codec_err_t vpx_codec_get_stream_info(vpx_codec_ctx_t * ctx, vpx_codec_stream_info_t * si)
*/
//<score> 13, nr_unique_branch: 5
//<Quality> {"density":13,"unique_branches":{"encoder_set_config":[[797,7,797,31,0,0,4,1],[797,35,797,59,0,0,4,1],[810,7,810,54,0,0,4,1],[814,7,814,26,0,0,4,1],[833,7,833,16,0,0,4,1]]},"library_calls":["vpx_codec_enc_config_default","vpx_codec_vp9_cx","vpx_codec_enc_init_ver","vpx_codec_vp9_cx","vpx_codec_enc_config_set","vpx_read_tpl_gop_stats","vpx_codec_set_cx_data_buf","vpx_codec_get_stream_info","vpx_img_alloc","vpx_img_set_rect","vpx_codec_encode","vpx_codec_get_cx_data","vpx_img_free","vpx_free_tpl_gop_stats","vpx_codec_destroy"],"critical_calls":["vpx_codec_enc_config_default","vpx_codec_vp9_cx","vpx_codec_enc_init_ver","vpx_codec_vp9_cx","vpx_codec_enc_config_set","vpx_read_tpl_gop_stats","vpx_codec_set_cx_data_buf","vpx_codec_get_stream_info","vpx_img_alloc","vpx_img_set_rect","vpx_codec_encode","vpx_codec_get_cx_data","vpx_img_free","vpx_free_tpl_gop_stats","vpx_codec_destroy"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_77(const uint8_t* f_data, size_t f_size) {
	if(f_size<68) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_1, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_4, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_5, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_6, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_7, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_8, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_9, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_10, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_11, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_12, fdp);
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    // Step 4: Open input file
    FILE *in_file = fmemopen((void *)data, size, "rb");

    // Step 5: Get file descriptor for input file
    int in_fd = fuzz_fileno(in_file);

    // Step 6: Set input file name
    const char *in_file_name = "input_file";

    // Initialize libvpx variables
    vpx_codec_ctx_t codec;
    vpx_codec_enc_cfg_t enc_config;
    vpx_codec_stream_info_t stream_info;

    // Step 1: Set up libvpx codec configuration
    vpx_codec_enc_config_default(vpx_codec_vp9_cx(), &enc_config, fuzz_uint32_t_1);
    enc_config.g_w = 640;
    enc_config.g_h = 480;

    // Step 2: Initialize libvpx codec
    vpx_codec_enc_init_ver(&codec, vpx_codec_vp9_cx(), &enc_config, fuzz_int64_t_2, VPX_ENCODER_ABI_VERSION);

    // Step 3: Set encoding configuration
    vpx_codec_enc_config_set(&codec, &enc_config);

    // Step 7: Read GOP stats
    FILE *tpl_file = fopen("gop_stats_file", "rb");
    VpxTplGopStats tpl_gop_stats;
    vpx_read_tpl_gop_stats(tpl_file, &tpl_gop_stats);
    assert_file_closed(&tpl_file);;

    // Step 8: Set CX data buffer
    vpx_fixed_buf_t cx_data_buf;
    vpx_codec_set_cx_data_buf(&codec, &cx_data_buf, fuzz_uint32_t_3, fuzz_uint32_t_4);

    // Step 9: Get stream info
    vpx_codec_get_stream_info(&codec, &stream_info);

    // Step 10: Set image rect
    vpx_image_t img;
    vpx_img_alloc(&img, VPX_IMG_FMT_I420, 640, 480, 16);
    vpx_img_set_rect(&img, fuzz_uint32_t_5, fuzz_uint32_t_6, fuzz_uint32_t_7, fuzz_uint32_t_8);

    // Step 11: Encode frame
    vpx_codec_encode(&codec, &img, fuzz_int64_t_9, fuzz_uint64_t_10, fuzz_int64_t_11, fuzz_uint64_t_12);

    // Step 12: Get CX data
    vpx_codec_iter_t iter = NULL;
    const vpx_codec_cx_pkt_t *cx_pkt;
    while ((cx_pkt = vpx_codec_get_cx_data(&codec, &iter)) != NULL) {
        // Process CX data
    }

    // Step 13: Free resources
    vpx_img_free(&img);
    vpx_free_tpl_gop_stats(&tpl_gop_stats);
    vpx_codec_destroy(&codec);
    assert_file_closed(&in_file);;

    // Step 14: Return 0 to indicate successful execution
    assert_file_closed(&tpl_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_fd_closed(in_fd);
	return 0;
}