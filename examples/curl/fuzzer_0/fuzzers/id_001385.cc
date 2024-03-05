#include "FDSan.h"
#include <mprintf.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 1385
//<Prompt> ["curl_multi_init","curl_multi_add_handle","curl_global_init_mem","curl_multi_perform","curl_multi_remove_handle"]
/*<Combination>: [CURLM *curl_multi_init(),
    CURLMcode curl_multi_add_handle(CURLM * multi_handle, CURL * curl_handle),
    CURLcode curl_global_init_mem(long flags, curl_malloc_callback m, curl_free_callback f, curl_realloc_callback r, curl_strdup_callback s, curl_calloc_callback c),
    CURLMcode curl_multi_perform(CURLM * multi_handle, int * running_handles),
    CURLMcode curl_multi_remove_handle(CURLM * multi_handle, CURL * curl_handle)
*/
//<score> 1.6, nr_unique_branch: 1, p_cov: 1
//<Quality> {"diversity":10,"density":8,"covered_api":["curl_multi_init","curl_multi_add_handle","curl_global_init_mem","curl_multi_perform","curl_multi_remove_handle"],"uncovered_api":[],"unique_branches":{"curl_global_init_mem":[[252,6,252,17,0]]},"library_calls":["curl_multi_init","curl_global_init_mem","curl_easy_init","curl_easy_setopt","curl_multi_add_handle","curl_multi_perform","curl_multi_remove_handle","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup"]}
/**/


extern "C" int LLVMFuzzerTestOneInput_72(const uint8_t *data, size_t size) {
	if(size<=0) return 0;

    // Step 4: Create input and output FILE pointers
    FILE *in_file = fmemopen((void *)data, size, "rb");
    FILE *out_file = fopen("output_file", "wb");
    
    // Step 5: Get file descriptors
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);
    
    // Step 1: Initialize curl
    CURLM *multi_handle = curl_multi_init();
    curl_global_init_mem(CURL_GLOBAL_DEFAULT, malloc, free, realloc, strdup, calloc);
    
    // Step 2: Add a curl handle to the multi_handle
    CURL *curl_handle = curl_easy_init();
    curl_easy_setopt(curl_handle, CURLOPT_URL, "https://example.com");
    curl_multi_add_handle(multi_handle, curl_handle);
    
    // Step 3: Perform the curl requests
    int running_handles;
    curl_multi_perform(multi_handle, &running_handles);
    
    // Step 6: Release resources
    curl_multi_remove_handle(multi_handle, curl_handle);
    curl_easy_cleanup(curl_handle);
    curl_multi_cleanup(multi_handle);
    curl_global_cleanup();
    
    assert_file_closed(&out_file);;
    assert_file_closed(&in_file);;
    
    assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}