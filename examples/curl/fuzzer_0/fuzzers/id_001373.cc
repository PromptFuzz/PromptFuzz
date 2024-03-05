#include "FDSan.h"
#include <mprintf.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 1373
//<Prompt> ["curl_multi_init","curl_multi_add_handle","curl_multi_perform","curl_multi_remove_handle","curl_url_strerror"]
/*<Combination>: [CURLM *curl_multi_init(),
    CURLMcode curl_multi_add_handle(CURLM * multi_handle, CURL * curl_handle),
    CURLMcode curl_multi_perform(CURLM * multi_handle, int * running_handles),
    CURLMcode curl_multi_remove_handle(CURLM * multi_handle, CURL * curl_handle),
    const char *curl_url_strerror(CURLUcode )
*/
//<score> 2.4, nr_unique_branch: 2, p_cov: 0.8
//<Quality> {"diversity":12,"density":12,"covered_api":["curl_multi_init","curl_multi_add_handle","curl_multi_perform","curl_multi_remove_handle"],"uncovered_api":["curl_url_strerror"],"unique_branches":{"create_hostcache_id":[[169,6,169,24,0]],"cf_socket_close":[[872,8,872,19,1]]},"library_calls":["curl_multi_init","curl_easy_init","curl_easy_setopt","curl_multi_add_handle","curl_multi_perform","curl_multi_remove_handle","curl_easy_cleanup","curl_multi_cleanup"]}
/**/


extern "C" int LLVMFuzzerTestOneInput_70(const uint8_t *data, size_t size) {
	if(size<=0) return 0;

    // Step 1: Initialize curl multi handle
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        return 0;
    }

    // Step 2: Create a file to write output data
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        curl_multi_cleanup(multi_handle);
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	return 0;
    }

    // Step 3: Set up curl easy handle and add it to multi handle
    CURL *curl_handle = curl_easy_init();
    if (!curl_handle) {
        assert_file_closed(&out_file);;
        curl_multi_cleanup(multi_handle);
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	return 0;
    }

    // Step 4: Set input data as the URL to request
    char *url = (char *)malloc(size + 1);
    if (!url) {
        assert_file_closed(&out_file);;
        curl_multi_cleanup(multi_handle);
        curl_easy_cleanup(curl_handle);
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	return 0;
    }
    memcpy(url, data, size);
    url[size] = '\0';

    curl_easy_setopt(curl_handle, CURLOPT_URL, url);

    curl_multi_add_handle(multi_handle, curl_handle);

    // Step 5: Perform the request
    int running_handles;
    curl_multi_perform(multi_handle, &running_handles);

    // Step 6: Remove handles from multi handle and cleanup
    curl_multi_remove_handle(multi_handle, curl_handle);
    curl_easy_cleanup(curl_handle);
    curl_multi_cleanup(multi_handle);

    // Step 7: Free resources
    free(url);
    assert_file_closed(&out_file);;

    assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	return 0;
}