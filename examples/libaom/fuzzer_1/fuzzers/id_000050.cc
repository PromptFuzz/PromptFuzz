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
//<ID> 1980
//<Prompt> ["aom_img_add_metadata","aom_uleb_encode","aom_img_wrap","aom_codec_error_detail","aom_codec_get_preview_frame","aom_codec_set_frame_buffer_functions","aom_uleb_encode_fixed_size","aom_img_metadata_alloc"]
/*<Combination>: [int aom_img_add_metadata(aom_image_t * img, uint32_t type, const uint8_t * data, size_t sz, aom_metadata_insert_flags_t insert_flag),
    int aom_uleb_encode(uint64_t value, size_t available, uint8_t * coded_value, size_t * coded_size),
    aom_image_t *aom_img_wrap(aom_image_t * img, aom_img_fmt_t fmt, unsigned int d_w, unsigned int d_h, unsigned int align, unsigned char * img_data),
    const char *aom_codec_error_detail(const aom_codec_ctx_t * ctx),
    const aom_image_t *aom_codec_get_preview_frame(aom_codec_ctx_t * ctx),
    aom_codec_err_t aom_codec_set_frame_buffer_functions(aom_codec_ctx_t * ctx, aom_get_frame_buffer_cb_fn_t cb_get, aom_release_frame_buffer_cb_fn_t cb_release, void * cb_priv),
    int aom_uleb_encode_fixed_size(uint64_t value, size_t available, size_t pad_to_size, uint8_t * coded_value, size_t * coded_size),
    aom_metadata_t *aom_img_metadata_alloc(uint32_t type, const uint8_t * data, size_t sz, aom_metadata_insert_flags_t insert_flag)
*/
//<score> 1.6875, nr_unique_branch: 5, p_cov: 0.5
//<Quality> {"diversity":16,"density":9,"covered_api":["aom_img_add_metadata","aom_img_wrap","aom_codec_get_preview_frame","aom_codec_set_frame_buffer_functions"],"uncovered_api":["aom_uleb_encode","aom_codec_error_detail","aom_uleb_encode_fixed_size","aom_img_metadata_alloc"],"unique_branches":{"img_alloc_helper":[[75,5,75,12,0],[91,5,91,12,0],[102,5,102,12,0],[109,7,109,33,1],[126,9,126,35,1]]},"library_calls":["aom_img_wrap","aom_img_add_metadata","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_set_frame_buffer_functions","aom_codec_encode","aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_peek_stream_info","aom_codec_get_stream_info","aom_codec_decode","aom_codec_get_preview_frame","aom_codec_destroy","aom_codec_destroy","aom_img_free"]}
/**/


extern "C" int LLVMFuzzerTestOneInput_50(const uint8_t* f_data, size_t f_size) {
	if(f_size<=44) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_1, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_2, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_3, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_4, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_5, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_6, fdp);
	//fuzzer shim end}




    // Wrap the input data into an aom_image_t struct
    aom_image_t img;
    aom_img_fmt_t fmt = AOM_IMG_FMT_NONE;
    unsigned int d_w = 0;
    unsigned int d_h = 0;
    unsigned int align = 16;
    unsigned char* img_data = nullptr;
    aom_img_wrap(&img, fmt, d_w, d_h, align, img_data);

    // Add metadata to the image
    uint32_t type = 0;
    const uint8_t* metadata_data = nullptr;
    size_t metadata_sz = 0;
    aom_metadata_insert_flags_t insert_flag = AOM_MIF_NON_KEY_FRAME;
    aom_img_add_metadata(&img, type, metadata_data, metadata_sz, insert_flag);

    // Encode the image
    aom_codec_ctx_t encoder;
    aom_codec_iface_t* encoder_iface = aom_codec_av1_cx();
    aom_codec_enc_cfg_t encoder_cfg;
    aom_codec_enc_config_default(encoder_iface, &encoder_cfg, fuzz_uint32_t_1);
    aom_codec_enc_init_ver(&encoder, encoder_iface, &encoder_cfg, fuzz_int64_t_2, AOM_CODEC_ABI_VERSION);
    
    aom_codec_set_frame_buffer_functions(&encoder, nullptr, nullptr, nullptr);
    aom_codec_encode(&encoder, &img, fuzz_int64_t_3, fuzz_uint64_t_4, fuzz_int64_t_5);

    // Decode the encoded data
    aom_codec_ctx_t decoder;
    aom_codec_iface_t* decoder_iface = aom_codec_av1_dx();
    aom_codec_dec_cfg_t decoder_cfg;
    aom_codec_dec_init_ver(&decoder, decoder_iface, &decoder_cfg, fuzz_int64_t_6, AOM_CODEC_ABI_VERSION);

    aom_codec_stream_info_t si;
    aom_codec_peek_stream_info(decoder_iface, data, size, &si);
    aom_codec_get_stream_info(&decoder, &si);
    aom_codec_decode(&decoder, data, size, nullptr);

    // Get preview frame from decoder
    const aom_image_t* preview_frame = aom_codec_get_preview_frame(&decoder);

    // Close encoder and decoder
    aom_codec_destroy(&encoder);
    aom_codec_destroy(&decoder);

    // Free allocated resources
    aom_img_free(&img);

    return 0;
}