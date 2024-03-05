#include "FDSan.h"
#include <mprintf.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 1235
//<Prompt> ["curl_url_strerror","curl_share_init","curl_mime_type","curl_easy_option_by_id","curl_multi_remove_handle"]
/*<Combination>: [const char *curl_url_strerror(CURLUcode ),
    CURLSH *curl_share_init(),
    CURLcode curl_mime_type(curl_mimepart * part, const char * mimetype),
    const struct curl_easyoption *curl_easy_option_by_id(CURLoption id),
    CURLMcode curl_multi_remove_handle(CURLM * multi_handle, CURL * curl_handle)
*/
//<score> 0, nr_unique_branch: 5, p_cov: 0
//<Quality> {"diversity":11,"density":8,"covered_api":[],"uncovered_api":["curl_url_strerror","curl_share_init","curl_mime_type","curl_easy_option_by_id","curl_multi_remove_handle"],"unique_branches":{"Curl_failf":[[90,27,90,48,0],[97,8,97,29,0],[97,33,97,54,0]],"easy_perform":[[731,6,731,27,0]],"curl_multi_add_handle":[[560,6,560,27,0]]},"library_calls":["curl_global_init","curl_easy_init","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_perform","curl_easy_strerror","curl_easy_cleanup","curl_global_cleanup"]}
/**/


extern "C" int LLVMFuzzerTestOneInput_65(const uint8_t* data, size_t size) {
	if(size<=0) return 0;

    // Create a FILE pointer to read input data
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Create a FILE pointer to write output data
    FILE* out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Get the file descriptor for reading and writing
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);

    // Allocate memory for storing curl error messages
    char error_buffer[CURL_ERROR_SIZE];
    error_buffer[0] = '\0';

    // Initialize curl
    curl_global_init(CURL_GLOBAL_ALL);

    // Create a cURL easy handle
    CURL* curl = curl_easy_init();
    if (curl) {
        // Set the input file
        curl_easy_setopt(curl, CURLOPT_READDATA, in_file);
        curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
        curl_easy_setopt(curl, CURLOPT_INFILESIZE_LARGE, size);

        // Set the output file
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, out_file);

        // Set the error buffer
        curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, error_buffer);

        // Perform the request
        CURLcode res = curl_easy_perform(curl);

        // Check for errors
        if (res != CURLE_OK) {
            printf("curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
            printf("Error buffer: %s\n", error_buffer);
        }

        // Cleanup curl
        curl_easy_cleanup(curl);
    }

    // Cleanup global curl resources
    curl_global_cleanup();

    // Close input and output files
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;

    assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}