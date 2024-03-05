#include "FDSan.h"
#include <mprintf.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 639
//<Prompt> ["curl_multi_socket_all","curl_easy_init","curl_easy_duphandle","curl_easy_cleanup","curl_mime_init"]
/*<Combination>: [CURLMcode curl_multi_socket_all(CURLM * multi_handle, int * running_handles),
    CURL *curl_easy_init(),
    CURL *curl_easy_duphandle(CURL * curl),
    void curl_easy_cleanup(CURL * curl),
    curl_mime *curl_mime_init(CURL * easy)
*/
//<score> 2, nr_unique_branch: 4, p_cov: 0.4
//<Quality> {"diversity":6,"density":6,"covered_api":["curl_easy_init","curl_easy_cleanup"],"uncovered_api":["curl_multi_socket_all","curl_easy_duphandle","curl_mime_init"],"unique_branches":{"curl_multi_cleanup":[[2803,11,2803,15,0],[2805,10,2805,27,0],[2805,31,2805,41,1],[2808,10,2808,49,0]]},"library_calls":["curl_multi_init","curl_easy_init","curl_easy_setopt","curl_multi_add_handle","curl_multi_cleanup","curl_easy_cleanup"]}
/**/

extern "C" int LLVMFuzzerTestOneInput_36(const uint8_t *data, size_t size) {
	if(size<=0) return 0;

	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);

    // Step 4: Create a FILE * variable to read the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    
    // Step 6: Create a string with the file name for input file
    const char* input_file = "input_file";
    
    // Step 5: Obtain the file descriptor for reading
    int in_fd = fuzz_fileno(in_file);
    
    // Step 6: Create a string with the file name for output file
    const char* output_file = "output_file";
    
    // Step 1: Initialize CURL multi handle
    CURLM *multi_handle = curl_multi_init();
    
    // Step 2: Initialize CURL easy handle
    CURL *easy_handle = curl_easy_init();
    
    // Step 3: Set the input file for the easy handle
    // This step might require additional processing of data from the input file
    curl_easy_setopt(easy_handle, CURLOPT_READDATA, in_file);
    
    // Step 1: Add the easy handle to multi handle
    curl_multi_add_handle(multi_handle, easy_handle);
    
    // Step 7: Release the allocated resources
    assert_file_closed(&in_file);;
    curl_multi_cleanup(multi_handle);
    curl_easy_cleanup(easy_handle);
    
    assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
}