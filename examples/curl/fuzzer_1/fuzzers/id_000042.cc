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
//<ID> 773
//<Prompt> ["curl_mime_type","curl_mime_free","curl_multi_wait","curl_multi_remove_handle","curl_easy_strerror","curl_multi_assign","curl_multi_cleanup"]
/*<Combination>: [CURLcode curl_mime_type(curl_mimepart * part, const char * mimetype),
    void curl_mime_free(curl_mime * mime),
    CURLMcode curl_multi_wait(CURLM * multi_handle, struct curl_waitfd * extra_fds, unsigned int extra_nfds, int timeout_ms, int * ret),
    CURLMcode curl_multi_remove_handle(CURLM * multi_handle, CURL * curl_handle),
    const char *curl_easy_strerror(CURLcode ),
    CURLMcode curl_multi_assign(CURLM * multi_handle, curl_socket_t sockfd, void * sockp),
    CURLMcode curl_multi_cleanup(CURLM * multi_handle)
*/
//<score> 0.74285716, nr_unique_branch: 1, p_cov: 0.42857143
//<Quality> {"diversity":15,"density":13,"covered_api":["curl_multi_wait","curl_multi_remove_handle","curl_multi_cleanup"],"uncovered_api":["curl_mime_type","curl_mime_free","curl_easy_strerror","curl_multi_assign"],"unique_branches":{"Curl_http_bodysend":[[2791,10,2791,33,1]]},"library_calls":["curl_global_init","curl_multi_init","curl_easy_init","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_multi_add_handle","curl_multi_wait","curl_multi_perform","curl_multi_remove_handle","curl_multi_cleanup","curl_easy_cleanup","curl_global_cleanup"]}
/*Here is the completed C++ program that uses the curl library APIs to achieve the event described in step 1:

*/


extern "C" int LLVMFuzzerTestOneInput_42(const uint8_t* f_data, size_t f_size) {
	if(f_size<=0) return 0;

	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(f_data, sizeof(uint8_t), f_size, input_file_ptr);
	fclose(input_file_ptr);

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Step 4: Open input file
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Step 5: Get file descriptor for input file
    int in_fd = fuzz_fileno(in_file);

    // Step 6: Set file name for input file
    const char *in_file_name = "input_file";

    // Initialize curl
    curl_global_init(CURL_GLOBAL_ALL);

    // Create a multi handle
    CURLM *multi_handle = curl_multi_init();

    // Create a curl handle
    CURL *curl_handle = curl_easy_init();

    // Set the input file as the POST data
    curl_easy_setopt(curl_handle, CURLOPT_URL, "http://example.com");
    curl_easy_setopt(curl_handle, CURLOPT_POST, 1);
    curl_easy_setopt(curl_handle, CURLOPT_READDATA, in_file);
    curl_easy_setopt(curl_handle, CURLOPT_READFUNCTION, fread);
    curl_easy_setopt(curl_handle, CURLOPT_POSTFIELDSIZE, size);

    // Add the curl handle to the multi handle
    curl_multi_add_handle(multi_handle, curl_handle);

    // Perform the request
    int running_handles;
    do {
        curl_multi_wait(multi_handle, nullptr, 0, 1000, nullptr);
        curl_multi_perform(multi_handle, &running_handles);
    } while (running_handles > 0);

    // Remove the curl handle from the multi handle and cleanup
    curl_multi_remove_handle(multi_handle, curl_handle);
    curl_multi_cleanup(multi_handle);
    curl_easy_cleanup(curl_handle);

    // Cleanup curl
    curl_global_cleanup();

    // Release resources
    assert_file_closed(&in_file);;

    assert_file_name_closed("input_file");
	assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
}