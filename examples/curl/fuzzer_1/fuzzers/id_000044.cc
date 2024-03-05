#include "FDSan.h"
#include "FuzzedDataProvider.h"
#include <mprintf.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 809
//<Prompt> ["curl_mime_subparts","curl_mime_type","curl_easy_upkeep","curl_url_dup","curl_multi_wait","curl_multi_remove_handle","curl_easy_strerror","curl_multi_cleanup"]
/*<Combination>: [CURLcode curl_mime_subparts(curl_mimepart * part, curl_mime * subparts),
    CURLcode curl_mime_type(curl_mimepart * part, const char * mimetype),
    CURLcode curl_easy_upkeep(CURL * curl),
    CURLU *curl_url_dup(const CURLU * in),
    CURLMcode curl_multi_wait(CURLM * multi_handle, struct curl_waitfd * extra_fds, unsigned int extra_nfds, int timeout_ms, int * ret),
    CURLMcode curl_multi_remove_handle(CURLM * multi_handle, CURL * curl_handle),
    const char *curl_easy_strerror(CURLcode ),
    CURLMcode curl_multi_cleanup(CURLM * multi_handle)
*/
//<score> 0, nr_unique_branch: 2, p_cov: 0
//<Quality> {"diversity":11,"density":11,"covered_api":[],"uncovered_api":["curl_mime_subparts","curl_mime_type","curl_easy_upkeep","curl_url_dup","curl_multi_wait","curl_multi_remove_handle","curl_easy_strerror","curl_multi_cleanup"],"unique_branches":{"Curl_vsetopt":[[1574,3,1574,32,0],[1579,8,1579,27,1]]},"library_calls":["curl_easy_init","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_perform","curl_easy_cleanup"]}
/**/


extern "C" int LLVMFuzzerTestOneInput_44(const uint8_t* f_data, size_t f_size) {
	if(f_size<=0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




  CURL *curl = curl_easy_init();
  if (!curl) {
    return 0;
  }

  FILE *in_file = fmemopen((void *)data, size, "rb");
  if (!in_file) {
    curl_easy_cleanup(curl);
    assert_file_closed(&in_file);
	return 0;
  }

  FILE *out_file = fopen("output_file", "wb");
  if (!out_file) {
    assert_file_closed(&in_file);;
    curl_easy_cleanup(curl);
    assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
  }

  // Set CURLOPT_URL option
  curl_easy_setopt(curl, CURLOPT_URL, "https://example.com");

  // Set CURLOPT_WRITEDATA option
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, out_file);

  // Set CURLOPT_READDATA option
  curl_easy_setopt(curl, CURLOPT_READDATA, in_file);

  // Set CURLOPT_UPLOAD option
  curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);

  // Set CURLOPT_PROGRESSFUNCTION option
  curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, NULL);

  // Set CURLOPT_VERBOSE option
  curl_easy_setopt(curl, CURLOPT_VERBOSE, 0L);

  // Perform the request
  CURLcode res = curl_easy_perform(curl);

  // Cleanup
  assert_file_closed(&in_file);;
  assert_file_closed(&out_file);;
  curl_easy_cleanup(curl);

  assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}