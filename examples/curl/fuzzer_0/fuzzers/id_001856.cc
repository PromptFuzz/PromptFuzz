#include "FDSan.h"
#include <mprintf.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 1856
//<Prompt> ["curl_formadd","curl_pushheader_byname","curl_ws_send","curl_strnequal","curl_multi_assign"]
/*<Combination>: [CURLFORMcode curl_formadd(struct curl_httppost ** httppost, struct curl_httppost ** last_post),
    char *curl_pushheader_byname(struct curl_pushheaders * h, const char * name),
    CURLcode curl_ws_send(CURL * curl, const void * buffer, size_t buflen, size_t * sent, curl_off_t fragsize, unsigned int flags),
    int curl_strnequal(const char * s1, const char * s2, size_t n),
    CURLMcode curl_multi_assign(CURLM * multi_handle, curl_socket_t sockfd, void * sockp)
*/
//<score> 0.51428574, nr_unique_branch: 1, p_cov: 0.6
//<Quality> {"diversity":7,"density":3,"covered_api":["curl_formadd","curl_pushheader_byname","curl_ws_send"],"uncovered_api":["curl_strnequal","curl_multi_assign"],"unique_branches":{"curl_easy_strerror":[[205,3,205,24,0]]},"library_calls":["curl_formadd","curl_pushheader_byname","curl_easy_init","curl_ws_send","curl_easy_strerror","curl_easy_cleanup","curl_formfree"]}
/*Here is the C++ program using curl library APIs to achieve the event described:

*/


extern "C" int LLVMFuzzerTestOneInput_85(const uint8_t *data, size_t size) {
	if(size<=0) return 0;

	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);

    // Step 4: Create a FILE * variable to read input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Step 6: Use the input file name as needed
    const char *input_file = "input_file";

    // Step 1: Use curl_formadd API to add a form data
    struct curl_httppost *httppost = NULL;
    struct curl_httppost *last_post = NULL;
    curl_formadd(&httppost, &last_post, CURLFORM_COPYNAME, "file", CURLFORM_FILE, input_file, CURLFORM_END);

    // Step 2: Use curl_pushheader_byname API to get a specific header by name
    struct curl_pushheaders *headers = NULL;
    char *header = curl_pushheader_byname(headers, "Content-Type");

    // Step 3: Use curl_ws_send API to send data through WebSocket
    CURL *curl = curl_easy_init();
    if (curl) {
        size_t sent = 0;
        curl_off_t fragsize = 0;
        unsigned int flags = 0;
        CURLcode res = curl_ws_send(curl, (void *)data, size, &sent, fragsize, flags);
        if (res != CURLE_OK) {
            printf("curl_ws_send failed: %s\n", curl_easy_strerror(res));
        }
        curl_easy_cleanup(curl);
    }

    // Step 5: Use fuzz_fileno to get a file descriptor
    int in_file_descriptor = fuzz_fileno(in_file);
    if (in_file_descriptor == -1) {
        assert_file_closed(&in_file);;
        assert_file_name_closed("input_file");
	assert_file_closed(&in_file);
	assert_fd_closed(in_file_descriptor);
	return 0;
    }

    // Step 7: Release all allocated resources
    curl_formfree(httppost);
    assert_file_closed(&in_file);;

    assert_file_name_closed("input_file");
	assert_file_closed(&in_file);
	assert_fd_closed(in_file_descriptor);
	return 0;
}