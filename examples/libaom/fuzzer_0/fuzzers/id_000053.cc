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
//<ID> 2062
//<Prompt> ["aom_img_add_metadata","aom_uleb_encode","aom_uleb_decode","aom_codec_error_detail","aom_codec_get_stream_info"]
/*<Combination>: [int aom_img_add_metadata(aom_image_t * img, uint32_t type, const uint8_t * data, size_t sz, aom_metadata_insert_flags_t insert_flag),
    int aom_uleb_encode(uint64_t value, size_t available, uint8_t * coded_value, size_t * coded_size),
    int aom_uleb_decode(const uint8_t * buffer, size_t available, uint64_t * value, size_t * length),
    const char *aom_codec_error_detail(const aom_codec_ctx_t * ctx),
    aom_codec_err_t aom_codec_get_stream_info(aom_codec_ctx_t * ctx, aom_codec_stream_info_t * si)
*/
//<score> 1.8000001, nr_unique_branch: 8, p_cov: 0.2
//<Quality> {"diversity":10,"density":10,"covered_api":["aom_codec_get_stream_info"],"uncovered_api":["aom_img_add_metadata","aom_uleb_encode","aom_uleb_decode","aom_codec_error_detail"],"unique_branches":{"aom_smooth_h_predictor_32x8_ssse3":[[2593,33,2593,52,0],[2593,33,2593,52,1]],"aom_paeth_predictor_16x64_avx2":[[833,19,833,24,0],[833,19,833,24,1],[836,21,836,27,0],[836,21,836,27,1]],"aom_paeth_predictor_32x16_avx2":[[872,15,872,21,0],[872,15,872,21,1]]},"library_calls":["aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_get_stream_info","aom_img_alloc","aom_codec_decode","aom_codec_get_frame","aom_img_free","aom_codec_destroy"]}
/**/

extern "C" int LLVMFuzzerTestOneInput_53(const uint8_t *data, size_t size) {
	if(size<=0) return 0;

    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file == NULL) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Create an aom_codec_ctx_t
    aom_codec_ctx_t codec;
    aom_codec_iface_t *iface = aom_codec_av1_dx();
    if (aom_codec_dec_init_ver(&codec, iface, NULL, 0, AOM_DECODER_ABI_VERSION) != AOM_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Get stream info
    aom_codec_stream_info_t si;
    aom_codec_get_stream_info(&codec, &si);
    
    // Read encoded data from input file
    uint8_t *buf = (uint8_t *)malloc(size);
    size_t read_bytes = fread(buf, 1, size, in_file);
    if (read_bytes != size) {
        assert_file_closed(&in_file);;
        free(buf);
        aom_codec_destroy(&codec);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Create aom image
    aom_image_t *img = aom_img_alloc(NULL, AOM_IMG_FMT_I420, si.w, si.h, 16);
    if (img == NULL) {
        assert_file_closed(&in_file);;
        free(buf);
        aom_codec_destroy(&codec);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Decode the frame
    aom_codec_decode(&codec, buf, size, NULL);
    
    // Get decoded frame
    aom_codec_iter_t iter = NULL;
    const aom_image_t *frame = aom_codec_get_frame(&codec, &iter);
    if (frame != NULL) {
        // Manipulate the frame or extract metadata
        // ...
    }
    
    // Free resources
    assert_file_closed(&in_file);;
    free(buf);
    aom_img_free(img);
    aom_codec_destroy(&codec);
    
    assert_file_closed(&in_file);
	return 0;
}