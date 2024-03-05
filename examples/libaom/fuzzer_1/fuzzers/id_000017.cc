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
//<ID> 762
//<Prompt> ["aom_codec_get_frame","aom_codec_get_caps","aom_codec_enc_config_set","aom_codec_set_cx_data_buf","aom_codec_enc_config_default"]
/*<Combination>: [aom_image_t *aom_codec_get_frame(aom_codec_ctx_t * ctx, aom_codec_iter_t * iter),
    aom_codec_caps_t aom_codec_get_caps(aom_codec_iface_t * iface),
    aom_codec_err_t aom_codec_enc_config_set(aom_codec_ctx_t * ctx, const aom_codec_enc_cfg_t * cfg),
    aom_codec_err_t aom_codec_set_cx_data_buf(aom_codec_ctx_t * ctx, const aom_fixed_buf_t * buf, unsigned int pad_before, unsigned int pad_after),
    aom_codec_err_t aom_codec_enc_config_default(aom_codec_iface_t * iface, aom_codec_enc_cfg_t * cfg, unsigned int usage)
*/
//<score> 1.2, nr_unique_branch: 2, p_cov: 0.8
//<Quality> {"diversity":14,"density":7,"covered_api":["aom_codec_get_frame","aom_codec_get_caps","aom_codec_enc_config_set","aom_codec_enc_config_default"],"uncovered_api":["aom_codec_set_cx_data_buf"],"unique_branches":{"decoder_decode":[[686,7,686,19,0],[686,23,686,35,0]]},"library_calls":["aom_codec_av1_cx","aom_codec_av1_dx","aom_codec_enc_config_default","aom_codec_enc_config_set","aom_codec_dec_init_ver","aom_codec_get_caps","aom_codec_encode","aom_codec_get_cx_data","aom_codec_peek_stream_info","aom_codec_get_stream_info","aom_codec_decode","aom_codec_get_frame","aom_codec_destroy","aom_codec_destroy"]}
/**/


extern "C" int LLVMFuzzerTestOneInput_17(const uint8_t* f_data, size_t f_size) {
	if(f_size<=36) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_1, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_2, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_3, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_4, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_5, fdp);
	//fuzzer shim end}




    // Create a file for reading input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    
    // Create a file for writing output data
    FILE *out_file = fopen("output_file", "wb");
    
    // Get the file descriptor for reading input data
    int in_fd = fuzz_fileno(in_file);
    
    // Get the file descriptor for writing output data
    int out_fd = fuzz_fileno(out_file);
    
    // Initialize the libaom encoder and decoder configurations
    aom_codec_ctx_t encoder, decoder;
    aom_codec_enc_cfg_t enc_cfg;
    aom_codec_dec_cfg_t dec_cfg;
    
    aom_codec_iface_t *encoder_iface = aom_codec_av1_cx();
    aom_codec_iface_t *decoder_iface = aom_codec_av1_dx();
    
    // Set the encoder and decoder configurations
    aom_codec_enc_config_default(encoder_iface, &enc_cfg, fuzz_uint32_t_1);
    aom_codec_enc_config_set(&encoder, &enc_cfg);
    
    aom_codec_dec_init_ver(&decoder, decoder_iface, &dec_cfg, fuzz_int64_t_2, AOM_DECODER_ABI_VERSION);
    
    // Get the capabilities of the encoder
    aom_codec_caps_t encoder_caps = aom_codec_get_caps(encoder_iface);
    
    // Encode the input data using the libaom encoder
    aom_codec_encode(&encoder, NULL, fuzz_int64_t_3, fuzz_uint64_t_4, fuzz_int64_t_5);
    
    // Iterate through the encoded frames and write them to the output file
    aom_codec_iter_t iter = NULL;
    const aom_codec_cx_pkt_t *pkt = NULL;
    
    while ((pkt = aom_codec_get_cx_data(&encoder, &iter)) != NULL) {
        if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
            fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
        }
    }
    
    // Decode the encoded data using the libaom decoder
    fseek(in_file, 0, SEEK_SET);
    aom_codec_stream_info_t si;
    aom_codec_peek_stream_info(decoder_iface, (uint8_t *)data, size, &si);
    aom_codec_get_stream_info(&decoder, &si);
    
    aom_codec_decode(&decoder, NULL, 0, NULL);
    
    // Iterate through the decoded frames and write them to the output file
    iter = NULL;
    const aom_image_t *img = NULL;
    
    while ((img = aom_codec_get_frame(&decoder, &iter)) != NULL) {
        fwrite(img->img_data, 1, img->sz, out_file);
    }
    
    // Cleanup
    aom_codec_destroy(&encoder);
    aom_codec_destroy(&decoder);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}