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
//<ID> 26
//<Prompt> ["curl_multi_socket_action","curl_mime_headers","curl_easy_recv","curl_global_init_mem","curl_mime_free"]
/*<Combination>: [CURLMcode curl_multi_socket_action(CURLM * multi_handle, curl_socket_t s, int ev_bitmask, int * running_handles),
    CURLcode curl_mime_headers(curl_mimepart * part, struct curl_slist * headers, int take_ownership),
    CURLcode curl_easy_recv(CURL * curl, void * buffer, size_t buflen, size_t * n),
    CURLcode curl_global_init_mem(long flags, curl_malloc_callback m, curl_free_callback f, curl_realloc_callback r, curl_strdup_callback s, curl_calloc_callback c),
    void curl_mime_free(curl_mime * mime)
*/
//<score> 0.32000002, nr_unique_branch: 1, p_cov: 0.2
//<Quality> {"diversity":15,"density":12,"covered_api":["curl_global_init_mem"],"uncovered_api":["curl_multi_socket_action","curl_mime_headers","curl_easy_recv","curl_mime_free"],"unique_branches":{"Curl_vsetopt":[[1697,8,1697,30,0]]},"library_calls":["curl_multi_init","curl_global_init_mem","curl_easy_init","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_multi_add_handle","curl_multi_perform","curl_multi_remove_handle","curl_multi_cleanup","curl_easy_cleanup","curl_global_cleanup"]}
/**/

extern "C" int LLVMFuzzerTestOneInput_1(const uint8_t* f_data, size_t f_size) {
	if(f_size<=0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




  // Create a FILE pointer for reading the input data
  FILE* in_file = fmemopen((void*)data, size, "rb");
  if (!in_file) {
    assert_file_closed(&in_file);
	return 0;
  }

  // Create a FILE pointer for writing the output data
  FILE* out_file = fopen("output_file", "wb");
  if (!out_file) {
    assert_file_closed(&in_file);;
    assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
  }

  // Create a CURLM handle for performing multiple transfers
  CURLM* multi_handle = curl_multi_init();
  if (!multi_handle) {
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
  }

  // Initialize libcurl globally with memory callbacks
  curl_global_init_mem(
      CURL_GLOBAL_ALL, nullptr, nullptr, nullptr, nullptr, nullptr);

  // Create a CURL handle for a single transfer
  CURL* curl = curl_easy_init();
  if (!curl) {
    curl_multi_cleanup(multi_handle);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    curl_global_cleanup();
    assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
  }

  // Set the read callback function for CURLOPT_READFUNCTION
  curl_easy_setopt(curl, CURLOPT_READFUNCTION, nullptr);

  // Set the write callback function for CURLOPT_WRITEFUNCTION
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, nullptr);

  // Set the input file for CURLOPT_READDATA
  curl_easy_setopt(curl, CURLOPT_READDATA, in_file);

  // Set the output file for CURLOPT_WRITEDATA
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, out_file);

  // Add the CURL handle to the CURLM handle for performing multi transfers
  curl_multi_add_handle(multi_handle, curl);

  // Perform the multi transfers until completed
  int running_handles = 0;
  while (curl_multi_perform(multi_handle, &running_handles) ==
         CURLM_CALL_MULTI_PERFORM) {
    // Keep performing multi transfers
  }

  // Cleanup and release all allocated resources
  curl_multi_remove_handle(multi_handle, curl);
  curl_multi_cleanup(multi_handle);
  curl_easy_cleanup(curl);
  assert_file_closed(&in_file);;
  assert_file_closed(&out_file);;
  curl_global_cleanup();

  assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}