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
//<ID> 1888
//<Prompt> ["aom_codec_iface_name","aom_img_add_metadata","aom_uleb_decode","aom_img_get_metadata","aom_img_wrap","aom_uleb_encode","aom_codec_error_detail","aom_img_remove_metadata","aom_codec_get_preview_frame","aom_img_metadata_alloc"]
/*<Combination>: [const char *aom_codec_iface_name(aom_codec_iface_t * iface),
    int aom_img_add_metadata(aom_image_t * img, uint32_t type, const uint8_t * data, size_t sz, aom_metadata_insert_flags_t insert_flag),
    int aom_uleb_decode(const uint8_t * buffer, size_t available, uint64_t * value, size_t * length),
    const aom_metadata_t *aom_img_get_metadata(const aom_image_t * img, size_t index),
    aom_image_t *aom_img_wrap(aom_image_t * img, aom_img_fmt_t fmt, unsigned int d_w, unsigned int d_h, unsigned int align, unsigned char * img_data),
    int aom_uleb_encode(uint64_t value, size_t available, uint8_t * coded_value, size_t * coded_size),
    const char *aom_codec_error_detail(const aom_codec_ctx_t * ctx),
    void aom_img_remove_metadata(aom_image_t * img),
    const aom_image_t *aom_codec_get_preview_frame(aom_codec_ctx_t * ctx),
    aom_metadata_t *aom_img_metadata_alloc(uint32_t type, const uint8_t * data, size_t sz, aom_metadata_insert_flags_t insert_flag)
*/
//<score> 0.20000002, nr_unique_branch: 2, p_cov: 0.1
//<Quality> {"diversity":3,"density":2,"covered_api":["aom_codec_iface_name"],"uncovered_api":["aom_img_add_metadata","aom_uleb_decode","aom_img_get_metadata","aom_img_wrap","aom_uleb_encode","aom_codec_error_detail","aom_img_remove_metadata","aom_codec_get_preview_frame","aom_img_metadata_alloc"],"unique_branches":{"aom_codec_encode":[[172,7,172,11,0],[31,32,31,35,1]]},"library_calls":["aom_codec_iface_name","aom_codec_av1_dx","aom_codec_encode"]}
/**/

extern "C" int LLVMFuzzerTestOneInput_48(const uint8_t* f_data, size_t f_size) {
	if(f_size<=24) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int64_t, fuzz_int64_t_1, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_2, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_3, fdp);
	//fuzzer shim end}




    // Step 4: Create input file stream
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    // Step 5: Get file descriptor for reading
    int in_fd = fuzz_fileno(in_file);
  
    // Step 6: Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
    }
    // Step 7: Get file descriptor for writing
    int out_fd = fuzz_fileno(out_file);
  
    // Step 2: Call libaom APIs
    const char *iface_name = aom_codec_iface_name(aom_codec_av1_dx());
    int encode_result = aom_codec_encode(NULL, NULL, fuzz_int64_t_1, fuzz_uint64_t_2, fuzz_int64_t_3);
  
    // Step 3: Release resources
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;

    assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}