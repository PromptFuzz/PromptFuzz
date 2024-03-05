#include "FDSan.h"
#include <mprintf.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 1382
//<Prompt> ["curl_multi_init","curl_multi_add_handle","curl_global_init_mem","curl_multi_perform","curl_multi_remove_handle"]
/*<Combination>: [CURLM *curl_multi_init(),
    CURLMcode curl_multi_add_handle(CURLM * multi_handle, CURL * curl_handle),
    CURLcode curl_global_init_mem(long flags, curl_malloc_callback m, curl_free_callback f, curl_realloc_callback r, curl_strdup_callback s, curl_calloc_callback c),
    CURLMcode curl_multi_perform(CURLM * multi_handle, int * running_handles),
    CURLMcode curl_multi_remove_handle(CURLM * multi_handle, CURL * curl_handle)
*/
//<score> 2.5, nr_unique_branch: 2, p_cov: 1
//<Quality> {"diversity":12,"density":10,"covered_api":["curl_multi_init","curl_multi_add_handle","curl_global_init_mem","curl_multi_perform","curl_multi_remove_handle"],"uncovered_api":[],"unique_branches":{"global_init":[[149,6,149,17,1]],"curl_global_init_mem":[[252,6,252,17,1]]},"library_calls":["curl_global_init_mem","curl_multi_init","curl_easy_init","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_multi_add_handle","curl_multi_perform","curl_multi_remove_handle","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup"]}
/*Here is a sample C++ program that utilizes the curl library APIs to achieve the given event:

*/


// Callback function to write the received data to file
static size_t  id_001382_write_data_callback(void* ptr, size_t size, size_t nmemb, FILE* stream) {
    size_t written = fwrite(ptr, size, nmemb, stream);
    return written;
}

extern "C" int LLVMFuzzerTestOneInput_71(const uint8_t* data, size_t size) {
	if(size<=0) return 0;

    // Initialize curl
    curl_global_init_mem(CURL_GLOBAL_DEFAULT, malloc, free, realloc, strdup, calloc);
    
    // Create curl multi handle
    CURLM* multi_handle = curl_multi_init();
    
    // Create easy handle
    CURL* curl_handle = curl_easy_init();
    curl_easy_setopt(curl_handle, CURLOPT_URL, "http://example.com");
    
    // Set the output file
    FILE* output_file = fopen("output_file", "wb");
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, output_file);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION,  id_001382_write_data_callback);
    
    // Add the easy handle to the multi handle
    curl_multi_add_handle(multi_handle, curl_handle);
    
    // Perform the request
    int running_handles;
    curl_multi_perform(multi_handle, &running_handles);
    
    // Remove the easy handle from the multi handle
    curl_multi_remove_handle(multi_handle, curl_handle);
    
    // Cleanup
    curl_easy_cleanup(curl_handle);
    curl_multi_cleanup(multi_handle);
    curl_global_cleanup();
    
    // Close the output file
    assert_file_closed(&output_file);;
    
    assert_file_name_closed("output_file");
	assert_file_closed(&output_file);
	return 0;
}