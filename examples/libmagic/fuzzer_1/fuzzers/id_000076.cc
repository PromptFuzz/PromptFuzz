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
//<ID> 2846
//<Prompt> ["vpx_codec_vp9_dx","vpx_codec_enc_init_multi_ver","vpx_read_tpl_gop_stats","vpx_codec_decode","vpx_codec_control_","vpx_codec_destroy"]
/*<Combination>: [vpx_codec_iface_t *vpx_codec_vp9_dx(),
    vpx_codec_err_t vpx_codec_enc_init_multi_ver(vpx_codec_ctx_t * ctx, vpx_codec_iface_t * iface, vpx_codec_enc_cfg_t * cfg, int num_enc, vpx_codec_flags_t flags, vpx_rational_t * dsf, int ver),
    vpx_codec_err_t vpx_read_tpl_gop_stats(FILE * tpl_file, VpxTplGopStats * tpl_gop_stats),
    vpx_codec_err_t vpx_codec_decode(vpx_codec_ctx_t * ctx, const uint8_t * data, unsigned int data_sz, void * user_priv, long deadline),
    vpx_codec_err_t vpx_codec_control_(vpx_codec_ctx_t * ctx, int ctrl_id),
    vpx_codec_err_t vpx_codec_destroy(vpx_codec_ctx_t * ctx)
*/
//<score> 5, nr_unique_branch: 0
//<Quality> {"density":5,"unique_branches":{},"library_calls":["vpx_codec_vp9_dx","vpx_codec_dec_init_ver","vpx_codec_decode","vpx_codec_get_frame","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp9_dx","vpx_codec_dec_init_ver","vpx_codec_decode","vpx_codec_get_frame","vpx_codec_destroy"],"visited":0}
/**/

// Function to read input data from a byte stream and decode it using libvpx
extern "C" int LLVMFuzzerTestOneInput_76(const uint8_t* f_data, size_t f_size) {
	if(f_size<16) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int64_t, fuzz_int64_t_1, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_2, fdp);
	//fuzzer shim end}




    vpx_codec_ctx_t decoder_ctx;
    vpx_codec_iface_t *decoder_iface = vpx_codec_vp9_dx();
    vpx_codec_dec_cfg_t decoder_cfg;
    vpx_codec_err_t decoder_err;

    // Create a FILE object to read the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file == nullptr) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Initialize the decoder with default configuration
    decoder_err = vpx_codec_dec_init_ver(&decoder_ctx, decoder_iface, &decoder_cfg, fuzz_int64_t_1, VPX_DECODER_ABI_VERSION);
    if (decoder_err != VPX_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // Decode the input data until all frames are processed
    uint8_t *buffer = new uint8_t[size];
    while (!feof(in_file)) {
        size_t bytes_read = fread(buffer, sizeof(uint8_t), size, in_file);

        // Decode the frame
        decoder_err = vpx_codec_decode(&decoder_ctx, buffer, bytes_read, nullptr, fuzz_int64_t_2);
        if (decoder_err != VPX_CODEC_OK) {
            break;
        }

        // Get the decoded frame
        const vpx_image_t *decoded_frame = nullptr;
        while ((decoded_frame = vpx_codec_get_frame(&decoder_ctx, nullptr)) != nullptr) {
            // Process the decoded frame
        }
    }

    // Release resources
    delete[] buffer;
    assert_file_closed(&in_file);;
    vpx_codec_destroy(&decoder_ctx);

    assert_file_closed(&in_file);
	return 0;
}