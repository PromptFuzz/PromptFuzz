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
//<ID> 2807
//<Prompt> ["vpx_codec_vp8_cx","vpx_codec_vp8_dx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_get_preview_frame","vpx_codec_control_","vpx_codec_register_put_frame_cb","vpx_codec_encode","vpx_codec_decode","vpx_read_tpl_gop_stats","vpx_img_free","vpx_codec_destroy"]
/*<Combination>: [vpx_codec_iface_t *vpx_codec_vp8_cx(),
    vpx_codec_iface_t *vpx_codec_vp8_dx(),
    vpx_codec_err_t vpx_codec_enc_config_default(vpx_codec_iface_t * iface, vpx_codec_enc_cfg_t * cfg, unsigned int usage),
    vpx_codec_err_t vpx_codec_enc_init_ver(vpx_codec_ctx_t * ctx, vpx_codec_iface_t * iface, const vpx_codec_enc_cfg_t * cfg, vpx_codec_flags_t flags, int ver),
    const vpx_image_t *vpx_codec_get_preview_frame(vpx_codec_ctx_t * ctx),
    vpx_codec_err_t vpx_codec_control_(vpx_codec_ctx_t * ctx, int ctrl_id),
    vpx_codec_err_t vpx_codec_register_put_frame_cb(vpx_codec_ctx_t * ctx, vpx_codec_put_frame_cb_fn_t cb, void * user_priv),
    vpx_codec_err_t vpx_codec_encode(vpx_codec_ctx_t * ctx, const vpx_image_t * img, vpx_codec_pts_t pts, unsigned long duration, vpx_enc_frame_flags_t flags, unsigned long deadline),
    vpx_codec_err_t vpx_codec_decode(vpx_codec_ctx_t * ctx, const uint8_t * data, unsigned int data_sz, void * user_priv, long deadline),
    vpx_codec_err_t vpx_read_tpl_gop_stats(FILE * tpl_file, VpxTplGopStats * tpl_gop_stats),
    void vpx_img_free(vpx_image_t * img),
    vpx_codec_err_t vpx_codec_destroy(vpx_codec_ctx_t * ctx)
*/
//<score> 20, nr_unique_branch: 10
//<Quality> {"density":5,"unique_branches":{"vp8_setup_version":[[150,5,150,11,0,0,4,0]],"init_frame":[[876,7,876,21,0,0,4,0]],"vp8dx_receive_compressed_data":[[324,7,324,18,0,0,4,0],[325,9,325,47,0,0,4,0],[331,9,331,43,0,0,4,1],[346,7,346,21,0,0,4,0]],"vp8_get_frame":[[541,9,542,71,0,0,4,0]],"vp8_decode_frame":[[972,35,972,62,0,0,4,0]],"vp8_decode":[[510,9,510,53,0,0,4,0]],"vp8dx_get_raw_frame":[[385,7,385,34,0,0,4,1]]},"library_calls":["vpx_codec_vp8_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_vp8_dx","vpx_codec_dec_init_ver","vpx_codec_encode","vpx_codec_decode","vpx_codec_get_frame","vpx_codec_destroy","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp8_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_vp8_dx","vpx_codec_dec_init_ver","vpx_codec_encode","vpx_codec_decode","vpx_codec_get_frame","vpx_codec_destroy","vpx_codec_destroy"],"visited":1}
/*Here is a sample implementation of the LLVMFuzzerTestOneInput_74 function that utilizes the libvpx library APIs to achieve the desired event:

*/


extern "C" int LLVMFuzzerTestOneInput_74(const uint8_t* f_data, size_t f_size) {
	if(f_size<60) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_1, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_2, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_3, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_4, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_5, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_6, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_7, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_8, fdp);
	//fuzzer shim end}




    // Step 4: Create input file
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file == nullptr) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Step 5: Get file descriptor for reading
    int in_fd = fuzz_fileno(in_file);

    // Step 6: Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (out_file == nullptr) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
    }

    // Step 7: Get file descriptor for writing
    int out_fd = fuzz_fileno(out_file);

    // Step 1: Create encoder
    vpx_codec_iface_t *encoder_iface = vpx_codec_vp8_cx();
    vpx_codec_ctx_t encoder_ctx;
    vpx_codec_enc_cfg_t encoder_cfg;
    vpx_codec_enc_config_default(encoder_iface, &encoder_cfg, fuzz_uint32_t_1);
    vpx_codec_enc_init_ver(&encoder_ctx, encoder_iface, &encoder_cfg, fuzz_int64_t_2, VPX_ENCODER_ABI_VERSION);

    // Step 2: Create decoder
    vpx_codec_iface_t *decoder_iface = vpx_codec_vp8_dx();
    vpx_codec_ctx_t decoder_ctx;
    vpx_codec_dec_cfg_t decoder_cfg;
    vpx_codec_dec_init_ver(&decoder_ctx, decoder_iface, &decoder_cfg, fuzz_int64_t_3, VPX_DECODER_ABI_VERSION);

    // Step 3: Encode and decode
    uint8_t encoded_data[1024];
    vpx_image_t raw_frame;
    vpx_codec_iter_t iter = NULL;
    int frame_count = 0;

    while (true) {
        // Read a frame from input file
        if (fread(encoded_data, 1, sizeof(encoded_data), in_file) <= 0) {
            break;
        }

        // Encode frame
        vpx_codec_encode(&encoder_ctx, &raw_frame, fuzz_int64_t_4, fuzz_uint64_t_5, fuzz_int64_t_6, fuzz_uint64_t_7);

        // Decode frame
        vpx_codec_decode(&decoder_ctx, encoded_data, sizeof(encoded_data), NULL, fuzz_int64_t_8);

        // Get decoded frame
        const vpx_image_t *decoded_frame = vpx_codec_get_frame(&decoder_ctx, &iter);
        
        // Write decoded frame to output file
        if (decoded_frame != NULL) {
            fwrite(decoded_frame->img_data, 1, decoded_frame->h * decoded_frame->stride[0], out_file);
        }

        frame_count++;
    }

    // Step 7: Release resources
    vpx_codec_destroy(&encoder_ctx);
    vpx_codec_destroy(&decoder_ctx);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}