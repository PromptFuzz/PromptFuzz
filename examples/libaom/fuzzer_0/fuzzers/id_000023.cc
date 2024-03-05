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
//<ID> 883
//<Prompt> ["aom_codec_decode","aom_codec_enc_init_ver","aom_codec_set_frame_buffer_functions","aom_img_wrap","aom_codec_get_frame"]
/*<Combination>: [aom_codec_err_t aom_codec_decode(aom_codec_ctx_t * ctx, const uint8_t * data, size_t data_sz, void * user_priv),
    aom_codec_err_t aom_codec_enc_init_ver(aom_codec_ctx_t * ctx, aom_codec_iface_t * iface, const aom_codec_enc_cfg_t * cfg, aom_codec_flags_t flags, int ver),
    aom_codec_err_t aom_codec_set_frame_buffer_functions(aom_codec_ctx_t * ctx, aom_get_frame_buffer_cb_fn_t cb_get, aom_release_frame_buffer_cb_fn_t cb_release, void * cb_priv),
    aom_image_t *aom_img_wrap(aom_image_t * img, aom_img_fmt_t fmt, unsigned int d_w, unsigned int d_h, unsigned int align, unsigned char * img_data),
    aom_image_t *aom_codec_get_frame(aom_codec_ctx_t * ctx, aom_codec_iter_t * iter)
*/
//<score> 28.285715, nr_unique_branch: 98, p_cov: 0.4
//<Quality> {"diversity":7,"density":5,"covered_api":["aom_codec_enc_init_ver","aom_codec_set_frame_buffer_functions"],"uncovered_api":["aom_codec_decode","aom_img_wrap","aom_codec_get_frame"],"unique_branches":{"validate_config":[[622,46,622,48,0]],"encoder_destroy":[[2843,9,2843,21,1]],"av1_select_sb_size":[[827,14,827,36,0],[828,9,828,53,1],[841,14,841,41,0]],"av1_set_speed_features_framesize_independent":[[2342,5,2342,14,1],[2348,5,2348,18,0],[2384,10,2384,43,1],[2384,47,2384,77,1],[2414,40,2414,63,0],[30,23,30,34,1]],"set_encoder_config":[[1053,5,1053,28,0],[1055,5,1055,12,1],[1146,7,1146,43,0],[1173,36,1173,70,1],[1261,7,1261,29,0],[1261,33,1261,67,0],[1262,7,1262,22,0],[1274,27,1274,51,0],[1355,7,1355,41,0],[1355,45,1355,61,0],[1356,7,1356,51,1],[30,23,30,34,1]],"av1_create_primary_compressor":[[1016,7,1016,23,1]],"encoder_init":[[2715,49,2715,72,1],[2762,34,2762,50,1]],"get_stats_buf_size":[[4027,11,4027,29,1]],"av1_rc_update_framerate":[[31,38,31,39,0]],"set_rt_speed_feature_framesize_dependent":[[1326,7,1326,25,1],[1379,9,1379,19,1],[1384,9,1384,19,1],[1385,9,1385,19,0],[1386,9,1386,19,0],[1390,11,1390,41,0],[1391,11,1391,47,1],[1394,9,1394,19,0],[1397,9,1397,19,1],[1401,9,1401,19,1],[1402,9,1402,19,1],[1410,9,1410,20,1],[1419,7,1419,25,1],[1424,7,1424,25,1],[1429,9,1429,19,0],[1430,9,1430,19,0],[1431,9,1431,19,0],[1435,9,1435,19,1],[1440,9,1440,20,1],[1446,7,1446,25,0],[1447,9,1447,19,0],[1451,9,1451,19,1],[1459,7,1459,24,1],[1459,28,1459,66,1],[1522,7,1522,55,1],[1593,7,1593,47,1],[1599,7,1599,33,1],[30,23,30,34,1],[30,23,30,34,1],[30,23,30,34,1],[30,23,30,34,1],[30,23,30,34,1]],"av1_change_config":[[780,7,780,28,1],[821,7,821,30,0],[821,34,821,57,1],[823,14,823,41,1],[824,14,824,49,0],[947,7,947,42,1]],"av1_set_speed_features_framesize_dependent":[[2287,5,2287,14,1],[2293,5,2293,18,0]],"set_rt_speed_features_framesize_independent":[[1664,59,1664,66,0],[1671,37,1671,44,0],[1698,19,1698,31,0],[1698,19,1698,31,1],[1729,42,1729,65,0],[1747,49,1747,56,0],[1751,7,1751,36,1],[1752,7,1752,48,1],[1759,7,1759,41,0],[1759,45,1759,79,0],[1764,7,1764,50,0],[1770,7,1770,41,1],[1772,7,1772,17,0],[1785,9,1785,43,1],[1788,55,1788,62,0],[1791,7,1791,17,0],[1810,21,1810,33,0],[1810,21,1810,33,1],[1838,9,1838,45,1],[1844,21,1844,36,0],[1844,21,1844,36,1],[1845,11,1845,27,0],[1845,11,1845,27,1],[1859,7,1859,17,1],[1873,7,1873,17,1],[1888,7,1888,18,1],[1894,7,1894,18,1]],"av1_rc_set_gf_interval_range":[[2462,7,2462,32,0],[2462,36,2462,64,1],[2481,9,2481,30,1]]},"library_calls":["aom_codec_av1_cx","aom_codec_av1_dx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_set_frame_buffer_functions","aom_codec_destroy","aom_codec_destroy"]}
/*Here is the C++ program that implements the steps mentioned above:

*/


extern "C" int LLVMFuzzerTestOneInput_23(const uint8_t *data, size_t size) {
	if(size<=0) return 0;

	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);

    // Step 4: Create input and output file descriptors
    FILE *in_file = fmemopen((void *)data, size, "rb");
    FILE *out_file = fopen("output_file", "wb");

    // Step 5: Get file descriptors
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);

    // Step 6: File names
    const char *input_file = "input_file";
    const char *output_file = "output_file";

    // Step 1: Initialize libaom
    aom_codec_ctx_t encoder_ctx, decoder_ctx;
    aom_codec_iface_t *encoder_iface = aom_codec_av1_cx();
    aom_codec_iface_t *decoder_iface = aom_codec_av1_dx();

    // Step 2: Configure and initialize the encoder
    aom_codec_enc_cfg_t encoder_cfg;
    aom_codec_enc_config_default(encoder_iface, &encoder_cfg, 0);
    encoder_cfg.g_w = 1920;
    encoder_cfg.g_h = 1080;
    encoder_cfg.g_usage = AOM_USAGE_REALTIME;
    aom_codec_enc_init_ver(&encoder_ctx, encoder_iface, &encoder_cfg, 0, AOM_ENCODER_ABI_VERSION);

    // Step 3: Set frame buffer functions
    aom_get_frame_buffer_cb_fn_t cb_get = NULL;
    aom_release_frame_buffer_cb_fn_t cb_release = NULL;
    aom_codec_set_frame_buffer_functions(&encoder_ctx, cb_get, cb_release, NULL);

    // Step 7: Clean up and release resources
    aom_codec_destroy(&encoder_ctx);
    aom_codec_destroy(&decoder_ctx);
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