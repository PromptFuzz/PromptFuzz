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
//<ID> 291
//<Prompt> ["curl_multi_get_handles","curl_mime_data","curl_share_setopt","curl_url_get","curl_multi_perform"]
/*<Combination>: [CURL **curl_multi_get_handles(CURLM * multi_handle),
    CURLcode curl_mime_data(curl_mimepart * part, const char * data, size_t datasize),
    CURLSHcode curl_share_setopt(CURLSH * share, CURLSHoption option),
    CURLUcode curl_url_get(const CURLU * handle, CURLUPart what, char ** part, unsigned int flags),
    CURLMcode curl_multi_perform(CURLM * multi_handle, int * running_handles)
*/
//<score> 0.8, nr_unique_branch: 1, p_cov: 0.4
//<Quality> {"diversity":28,"density":28,"covered_api":["curl_url_get","curl_multi_perform"],"uncovered_api":["curl_multi_get_handles","curl_mime_data","curl_share_setopt"],"unique_branches":{"curl_dbg_free":[[300,6,300,12,1]]},"library_calls":["curl_multi_init","curl_easy_init","curl_multi_add_handle","curl_url","curl_easy_setopt","curl_easy_setopt","curl_url_set","curl_url_get","curl_easy_setopt","curl_free","curl_easy_setopt","curl_multi_perform","curl_url_cleanup","curl_multi_remove_handle","curl_easy_cleanup","curl_multi_cleanup"]}
/**/


extern "C" int LLVMFuzzerTestOneInput_16(const uint8_t* f_data, size_t f_size) {
	if(f_size<=25) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
	//fuzzer shim end}




    // Create a CURLM handle
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        return 0; // Error handling
    }
  
    // Create a CURL handle
    CURL *curl_handle = curl_easy_init();
    if (!curl_handle) {
        curl_multi_cleanup(multi_handle);
        return 0; // Error handling
    }
  
    // Set the CURL handle options
    // ...

    // Add the CURL handle to the multi handle
    curl_multi_add_handle(multi_handle, curl_handle);

    // Create a CURLU handle
    CURLU *url_handle = curl_url();
    if (!url_handle) {
        curl_multi_remove_handle(multi_handle, curl_handle);
        curl_easy_cleanup(curl_handle);
        curl_multi_cleanup(multi_handle);
        return 0; // Error handling
    }

    // Get the data from the input stream
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        curl_url_cleanup(url_handle);
        curl_multi_remove_handle(multi_handle, curl_handle);
        curl_easy_cleanup(curl_handle);
        curl_multi_cleanup(multi_handle);
        assert_file_closed(&in_file);
	return 0; // Error handling
    }

    // Set the input stream for the CURL handle
    int in_fd = fuzz_fileno(in_file);
    curl_easy_setopt(curl_handle, CURLOPT_READDATA, &in_fd);
    curl_easy_setopt(curl_handle, CURLOPT_UPLOAD, 1L);

    // Set the URL for the CURL handle
    curl_url_set(url_handle, CURLUPART_URL, fuzz_str_1, 0);
    char *url;
    curl_url_get(url_handle, CURLUPART_URL, &url, fuzz_uint32_t_2);
    curl_easy_setopt(curl_handle, CURLOPT_URL, url);
    curl_free(url);

    // Set the output file for the CURL handle
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        curl_url_cleanup(url_handle);
        curl_multi_remove_handle(multi_handle, curl_handle);
        curl_easy_cleanup(curl_handle);
        curl_multi_cleanup(multi_handle);
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0; // Error handling
    }
    int out_fd = fuzz_fileno(out_file);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, &out_fd);

    // Perform the multi handle action
    int running_handles;
    curl_multi_perform(multi_handle, &running_handles);

    // Clean up
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    curl_url_cleanup(url_handle);
    curl_multi_remove_handle(multi_handle, curl_handle);
    curl_easy_cleanup(curl_handle);
    curl_multi_cleanup(multi_handle);

    assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}