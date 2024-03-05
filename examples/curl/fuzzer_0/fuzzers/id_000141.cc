#include "FDSan.h"
#include <mprintf.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 141
//<Prompt> ["curl_multi_socket","curl_mprintf","curl_easy_duphandle","curl_mime_data","curl_multi_strerror"]
/*<Combination>: [CURLMcode curl_multi_socket(CURLM * multi_handle, curl_socket_t s, int * running_handles),
    int curl_mprintf(const char * format),
    CURL *curl_easy_duphandle(CURL * curl),
    CURLcode curl_mime_data(curl_mimepart * part, const char * data, size_t datasize),
    const char *curl_multi_strerror(CURLMcode )
*/
//<score> 3.2, nr_unique_branch: 3, p_cov: 0.8
//<Quality> {"diversity":18,"density":18,"covered_api":["curl_mprintf","curl_easy_duphandle","curl_mime_data","curl_multi_strerror"],"uncovered_api":["curl_multi_socket"],"unique_branches":{"dprintf_IsQualifierNoDollar":[[207,33,207,41,0]],"dprintf_Pass1":[[351,9,351,17,0],[534,43,534,74,0]]},"library_calls":["curl_multi_init","curl_easy_init","curl_easy_duphandle","curl_easy_setopt","curl_easy_setopt","curl_multi_add_handle","curl_multi_socket_action","curl_mprintf","curl_mime_init","curl_mime_addpart","curl_mime_data","curl_multi_strerror","curl_multi_socket_action","curl_mime_free","curl_multi_remove_handle","curl_multi_cleanup","curl_easy_cleanup","curl_easy_cleanup"]}
/**/


extern "C" int LLVMFuzzerTestOneInput_8(const uint8_t *data, size_t size) {
	if(size<=0) return 0;

    CURLM *multi_handle = curl_multi_init();
    curl_socket_t socket = 0;
    int running_handles = 0;
    
    FILE *in_file = fmemopen((void *)data, size, "rb");
    FILE *out_file = fopen("output_file", "wb");
    
    CURL *curl_handle = curl_easy_init();
    CURL *dup_curl_handle = curl_easy_duphandle(curl_handle);
    
    // Set the input file descriptor for reading
    curl_easy_setopt(curl_handle, CURLOPT_READDATA, in_file);
    
    // Set the output file descriptor for writing
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, out_file);
    
    // Add the handle to the multi handle
    curl_multi_add_handle(multi_handle, curl_handle);
    
    // Associate the socket with the multi handle
    curl_multi_socket(multi_handle, socket, &running_handles);
    
    // Print debug message
    curl_mprintf("Input data size: %zu\n", size);
    
    // Create a mime part and set its data
    curl_mime *mime = curl_mime_init(curl_handle);
    curl_mimepart *part = curl_mime_addpart(mime);
    curl_mime_data(part, (const char *)data, size);
    
    // Get the error message for the multi handle
    const char *error_message = curl_multi_strerror(curl_multi_socket(multi_handle, socket, &running_handles));
    
    // Cleanup
    curl_mime_free(mime);
    curl_multi_remove_handle(multi_handle, curl_handle);
    curl_multi_cleanup(multi_handle);
    curl_easy_cleanup(curl_handle);
    curl_easy_cleanup(dup_curl_handle);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}