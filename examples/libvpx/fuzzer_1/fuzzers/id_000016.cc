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
//<ID> 876
//<Prompt> ["vpx_codec_build_config","vpx_codec_version_str","vpx_read_tpl_gop_stats","vpx_codec_enc_config_set","vpx_codec_vp8_cx","vpx_codec_vp8_dx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_get_stream_info","vpx_codec_dec_init_ver","vpx_codec_get_global_headers","vpx_codec_destroy"]
/*<Combination>: [const char *vpx_codec_build_config(),
    const char *vpx_codec_version_str(),
    vpx_codec_err_t vpx_read_tpl_gop_stats(FILE * tpl_file, VpxTplGopStats * tpl_gop_stats),
    vpx_codec_err_t vpx_codec_enc_config_set(vpx_codec_ctx_t * ctx, const vpx_codec_enc_cfg_t * cfg),
    vpx_codec_iface_t *vpx_codec_vp8_cx(),
    vpx_codec_iface_t *vpx_codec_vp8_dx(),
    vpx_codec_err_t vpx_codec_enc_config_default(vpx_codec_iface_t * iface, vpx_codec_enc_cfg_t * cfg, unsigned int usage),
    vpx_codec_err_t vpx_codec_enc_init_ver(vpx_codec_ctx_t * ctx, vpx_codec_iface_t * iface, const vpx_codec_enc_cfg_t * cfg, vpx_codec_flags_t flags, int ver),
    vpx_codec_err_t vpx_codec_get_stream_info(vpx_codec_ctx_t * ctx, vpx_codec_stream_info_t * si),
    vpx_codec_err_t vpx_codec_dec_init_ver(vpx_codec_ctx_t * ctx, vpx_codec_iface_t * iface, const vpx_codec_dec_cfg_t * cfg, vpx_codec_flags_t flags, int ver),
    vpx_fixed_buf_t *vpx_codec_get_global_headers(vpx_codec_ctx_t * ctx),
    vpx_codec_err_t vpx_codec_destroy(vpx_codec_ctx_t * ctx)
*/
//<score> 7, nr_unique_branch: 2
//<Quality> {"density":7,"unique_branches":{"vpx_codec_enc_config_set":[[346,7,346,11,0,0,4,0],[23,32,23,37,2,0,4,1]]},"library_calls":["vpx_codec_vp8_cx","vpx_codec_vp8_dx","vpx_codec_build_config","vpx_codec_version_str","vpx_codec_enc_config_default","vpx_codec_enc_config_set","vpx_codec_enc_init_ver","vpx_codec_destroy","vpx_codec_get_global_headers","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp8_cx","vpx_codec_vp8_dx","vpx_codec_build_config","vpx_codec_version_str","vpx_codec_enc_config_default","vpx_codec_enc_config_set","vpx_codec_enc_init_ver","vpx_codec_get_global_headers","vpx_codec_destroy"],"visited":1}
/**/

extern "C" int LLVMFuzzerTestOneInput_16(const uint8_t* f_data, size_t f_size) {
	if(f_size<12) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_1, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_2, fdp);
	//fuzzer shim end}




    vpx_codec_iface_t *encoder_iface = vpx_codec_vp8_cx();
    vpx_codec_iface_t *decoder_iface = vpx_codec_vp8_dx();

    const char *vpx_build_config = vpx_codec_build_config();
    const char *vpx_version_str = vpx_codec_version_str();
    vpx_codec_stream_info_t si;
    vpx_codec_err_t codec_err;

    vpx_codec_enc_cfg_t enc_cfg;
    vpx_codec_enc_config_default(encoder_iface, &enc_cfg, fuzz_uint32_t_1);
    vpx_codec_enc_config_set(nullptr, &enc_cfg);

    vpx_codec_ctx_t encoder_ctx;
    codec_err = vpx_codec_enc_init_ver(&encoder_ctx, encoder_iface, &enc_cfg, fuzz_int64_t_2, VPX_ENCODER_ABI_VERSION);
    if (codec_err != VPX_CODEC_OK) {
        vpx_codec_destroy(&encoder_ctx);
        return 0;
    }

    vpx_fixed_buf_t *global_headers = vpx_codec_get_global_headers(&encoder_ctx);

    FILE *in_file = fmemopen((void *)data, size, "rb");
    FILE *out_file = fopen("output_file", "wb");
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);

    // Use the file descriptors or file names as needed

    // Release allocated resources
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;

    vpx_codec_destroy(&encoder_ctx);

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}