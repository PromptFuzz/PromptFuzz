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
//<ID> 961
//<Prompt> ["aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_set_frame_buffer_functions","aom_codec_decode","aom_codec_destroy"]
/*<Combination>: [aom_codec_iface_t *aom_codec_av1_dx(),
    aom_codec_err_t aom_codec_dec_init_ver(aom_codec_ctx_t * ctx, aom_codec_iface_t * iface, const aom_codec_dec_cfg_t * cfg, aom_codec_flags_t flags, int ver),
    aom_codec_err_t aom_codec_set_frame_buffer_functions(aom_codec_ctx_t * ctx, aom_get_frame_buffer_cb_fn_t cb_get, aom_release_frame_buffer_cb_fn_t cb_release, void * cb_priv),
    aom_codec_err_t aom_codec_decode(aom_codec_ctx_t * ctx, const uint8_t * data, size_t data_sz, void * user_priv),
    aom_codec_err_t aom_codec_destroy(aom_codec_ctx_t * ctx)
*/
//<score> 16, nr_unique_branch: 15, p_cov: 1
//<Quality> {"diversity":6,"density":6,"covered_api":["aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_set_frame_buffer_functions","aom_codec_decode","aom_codec_destroy"],"uncovered_api":[],"unique_branches":{"get_last_nonzero_byte":[[757,10,757,16,1]],"read_bitdepth":[[3860,29,3860,39,1]],"alloc_read_metadata":[[585,7,585,21,0],[587,9,587,23,1],[594,7,594,16,1],[601,7,601,22,1]],"read_uncompressed_header":[[5001,7,5001,53,0]],"read_metadata":[[777,29,777,47,1],[780,9,780,73,0],[786,7,786,50,1],[790,14,790,56,0],[794,9,794,74,0]],"resize_context_buffers":[[1894,37,1894,65,0]],"av1_read_film_grain_params":[[3939,14,3939,78,1]],"read_metadata_hdr_cll":[[650,7,650,30,1]]},"library_calls":["aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_set_frame_buffer_functions","aom_codec_decode","aom_codec_get_frame","aom_codec_destroy"]}
/**/


extern "C" int LLVMFuzzerTestOneInput_26(const uint8_t *data, size_t size) {
	if(size<=0) return 0;

    // Create input file stream
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Create decoder configuration
    aom_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 1;
    dec_cfg.w = 0;
    dec_cfg.h = 0;
    dec_cfg.allow_lowbitdepth = 1;

    // Initialize decoder
    aom_codec_ctx_t decoder;
    aom_codec_iface_t *decoder_interface = aom_codec_av1_dx();
    aom_codec_dec_init_ver(&decoder, decoder_interface, &dec_cfg, 0, AOM_DECODER_ABI_VERSION);

    // Set frame buffer functions
    aom_codec_set_frame_buffer_functions(&decoder, NULL, NULL, NULL);

    // Read input data and decode frames
    uint8_t chunk[1024];
    while (size_t read_size = fread(chunk, 1, sizeof(chunk), in_file)) {
        aom_codec_decode(&decoder, chunk, read_size, NULL);

        // Get decoded frame
        const aom_image_t *frame;
        while ((frame = aom_codec_get_frame(&decoder, NULL)) != NULL) {
            // Write output frame to file
            fwrite(frame->planes[0], 1, frame->d_w * frame->d_h, out_file);
        }
    }

    // Destroy decoder
    aom_codec_destroy(&decoder);

    // Close input and output files
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;

    assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}