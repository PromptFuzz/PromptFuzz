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
//<ID> 1178
//<Prompt> ["aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_set_frame_buffer_functions","aom_img_plane_width","aom_codec_decode","aom_codec_get_frame"]
/*<Combination>: [aom_codec_iface_t *aom_codec_av1_dx(),
    aom_codec_err_t aom_codec_dec_init_ver(aom_codec_ctx_t * ctx, aom_codec_iface_t * iface, const aom_codec_dec_cfg_t * cfg, aom_codec_flags_t flags, int ver),
    aom_codec_err_t aom_codec_set_frame_buffer_functions(aom_codec_ctx_t * ctx, aom_get_frame_buffer_cb_fn_t cb_get, aom_release_frame_buffer_cb_fn_t cb_release, void * cb_priv),
    int aom_img_plane_width(const aom_image_t * img, int plane),
    aom_codec_err_t aom_codec_decode(aom_codec_ctx_t * ctx, const uint8_t * data, size_t data_sz, void * user_priv),
    aom_image_t *aom_codec_get_frame(aom_codec_ctx_t * ctx, aom_codec_iter_t * iter)
*/
//<score> 4.1666665, nr_unique_branch: 4, p_cov: 0.8333333
//<Quality> {"diversity":6,"density":6,"covered_api":["aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_set_frame_buffer_functions","aom_codec_decode","aom_codec_get_frame"],"uncovered_api":["aom_img_plane_width"],"unique_branches":{"release_pending_output_frames":[[571,24,571,50,0]],"decrease_ref_count":[[406,7,406,18,0],[415,9,415,28,1]],"decoder_decode":[[690,23,690,35,0]]},"library_calls":["aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_set_frame_buffer_functions","aom_codec_decode","aom_codec_get_frame","aom_codec_destroy"]}
/*Here is the implementation of the C++ program using libaom library APIs:

*/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_31(const uint8_t* f_data, size_t f_size) {
	if(f_size<=8) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int64_t, fuzz_int64_t_1, fdp);
	//fuzzer shim end}




    // Step 1: Initialize the decoder
    aom_codec_iface_t *codec = aom_codec_av1_dx();
    aom_codec_ctx_t decoder;
    aom_codec_dec_cfg_t cfg = {0};
    cfg.allow_lowbitdepth = 1; // Allow low bit depth
    aom_codec_dec_init_ver(&decoder, codec, &cfg, fuzz_int64_t_1, AOM_DECODER_ABI_VERSION);

    // Step 2: Set frame buffer functions for the decoder
    aom_codec_set_frame_buffer_functions(&decoder, NULL, NULL, NULL);

    // Step 3: Create input and output file pointers
    FILE *in_file = fmemopen((void *)data, size, "rb");
    FILE *out_file = fopen("output_file", "wb");

    if (in_file == NULL || out_file == NULL) {
        // Error in opening files
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Step 4: Get file descriptors
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);

    // Step 5: Perform decoding
    uint8_t buf[4096];
    size_t nbytes;
    do {
        nbytes = fread(buf, 1, sizeof(buf), in_file);
        aom_codec_decode(&decoder, buf, nbytes, NULL);
        
        // Step 6: Get the decoded frame
        aom_codec_iter_t iter = NULL;
        const aom_image_t *frame = aom_codec_get_frame(&decoder, &iter);
        
        // Step 6: Write the decoded frame to output file
        if (frame) {
            fwrite(frame, frame->sz, 1, out_file);
        }
    } while (nbytes > 0);

    // Step 7: Release resources
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    aom_codec_destroy(&decoder);

    assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}