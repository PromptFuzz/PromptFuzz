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
//<ID> 2088
//<Prompt> ["aom_codec_iface_name","aom_uleb_encode","aom_codec_enc_init_ver","aom_codec_av1_dx","aom_codec_error"]
/*<Combination>: [const char *aom_codec_iface_name(aom_codec_iface_t * iface),
    int aom_uleb_encode(uint64_t value, size_t available, uint8_t * coded_value, size_t * coded_size),
    aom_codec_err_t aom_codec_enc_init_ver(aom_codec_ctx_t * ctx, aom_codec_iface_t * iface, const aom_codec_enc_cfg_t * cfg, aom_codec_flags_t flags, int ver),
    aom_codec_iface_t *aom_codec_av1_dx(),
    const char *aom_codec_error(const aom_codec_ctx_t * ctx)
*/
//<score> 1, nr_unique_branch: 1, p_cov: 1
//<Quality> {"diversity":6,"density":3,"covered_api":["aom_codec_iface_name","aom_uleb_encode","aom_codec_enc_init_ver","aom_codec_av1_dx","aom_codec_error"],"uncovered_api":[],"unique_branches":{"aom_codec_err_to_string":[[40,5,40,32,0]]},"library_calls":["aom_codec_iface_name","aom_codec_av1_dx","aom_uleb_encode","aom_codec_enc_init_ver","aom_codec_av1_dx","aom_codec_error"]}
/**/


extern "C" int LLVMFuzzerTestOneInput_54(const uint8_t* f_data, size_t f_size) {
	if(f_size<=8) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_1, fdp);
	//fuzzer shim end}




  FILE *in_file = fmemopen((void *)data, size, "rb");
  FILE *out_file = fopen("output_file", "wb");

  // Read input data from in_file
  // create and initialize variables using libaom APIs

  // Call libaom APIs to achieve the desired event
  const char *iface_name = aom_codec_iface_name(aom_codec_av1_dx());
  uint8_t coded_value[64];
  size_t coded_size;
  aom_uleb_encode(fuzz_uint64_t_1, sizeof(coded_value), coded_value, &coded_size);
  aom_codec_ctx_t ctx;
  aom_codec_enc_cfg_t cfg;
  aom_codec_flags_t flags;
  int ver;
  aom_codec_err_t err = aom_codec_enc_init_ver(&ctx, aom_codec_av1_dx(), &cfg, flags, ver);
  const char *error = aom_codec_error(&ctx);

  // Write output data to out_file

  // Release resources
  assert_file_closed(&in_file);;
  assert_file_closed(&out_file);;

  assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}