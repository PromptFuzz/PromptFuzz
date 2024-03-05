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
//<ID> 139
//<Prompt> ["curl_multi_socket","curl_mprintf","curl_easy_duphandle","curl_mime_data","curl_multi_strerror","curl_ws_send"]
/*<Combination>: [CURLMcode curl_multi_socket(CURLM * multi_handle, curl_socket_t s, int * running_handles),
    int curl_mprintf(const char * format),
    CURL *curl_easy_duphandle(CURL * curl),
    CURLcode curl_mime_data(curl_mimepart * part, const char * data, size_t datasize),
    const char *curl_multi_strerror(CURLMcode ),
    CURLcode curl_ws_send(CURL * curl, const void * buffer, size_t buflen, size_t * sent, curl_off_t fragsize, unsigned int flags)
*/
//<score> 10.733334, nr_unique_branch: 22, p_cov: 0.5
//<Quality> {"diversity":15,"density":14,"covered_api":["curl_easy_duphandle","curl_multi_strerror","curl_ws_send"],"uncovered_api":["curl_multi_socket","curl_mprintf","curl_mime_data"],"unique_branches":{"Curl_splayinsert":[[112,6,112,7,0],[114,8,114,31,1],[130,6,130,8,1],[133,11,133,33,1],[53,38,53,64,1],[54,38,54,64,1],[55,38,55,65,1],[56,38,56,65,0],[53,38,53,64,1],[54,38,54,64,1],[55,38,55,65,1],[56,38,56,65,0]],"unlink_easy":[[513,6,513,16,0]],"link_easy":[[489,6,489,18,0]],"Curl_splay":[[55,10,55,21,1],[57,10,57,41,0],[62,12,62,23,0],[69,13,69,21,0],[70,10,70,20,0],[56,38,56,65,0]],"multi_socket":[[3257,10,3257,16,1],[3262,15,3262,43,1]]},"library_calls":["curl_easy_init","curl_easy_duphandle","curl_multi_init","curl_multi_add_handle","curl_multi_add_handle","curl_ws_send","curl_multi_socket_action","curl_multi_strerror","curl_multi_remove_handle","curl_multi_remove_handle","curl_easy_cleanup","curl_easy_cleanup","curl_multi_cleanup"]}
/**/


extern "C" int LLVMFuzzerTestOneInput_6(const uint8_t* f_data, size_t f_size) {
	if(f_size<=0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Set up input and output files
    FILE *in_file = fmemopen((void *)data, size, "rb");
    FILE *out_file = fopen("output_file", "wb");
    if (!in_file || !out_file) {
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Set up CURL handles
    CURL *curl = curl_easy_init();
    CURL *curl_dup = curl_easy_duphandle(curl);
    if (!curl || !curl_dup) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Set up CURLM handle
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        curl_easy_cleanup(curl);
        curl_easy_cleanup(curl_dup);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Add the CURL handles to the CURLM handle
    curl_multi_add_handle(multi_handle, curl);
    curl_multi_add_handle(multi_handle, curl_dup);

    // Read input data from file and set it as CURLM websocket data
    char buf[1024];
    size_t bytesRead = fread(buf, 1, sizeof(buf), in_file);
    while (bytesRead > 0) {
        size_t sent;
        curl_ws_send(curl, buf, bytesRead, &sent, 0, 0);
        bytesRead = fread(buf, 1, sizeof(buf), in_file);
    }

    // Set up CURLM socket action
    int running_handles = 0;
    curl_multi_socket_action(multi_handle, CURL_SOCKET_TIMEOUT, 0, &running_handles);

    // Get CURLM error message
    const char *error = curl_multi_strerror(CURLM_OK);

    // Clean up and release resources
    curl_multi_remove_handle(multi_handle, curl);
    curl_multi_remove_handle(multi_handle, curl_dup);
    curl_easy_cleanup(curl);
    curl_easy_cleanup(curl_dup);
    curl_multi_cleanup(multi_handle);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;

    assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}