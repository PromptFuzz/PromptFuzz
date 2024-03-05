#include "FDSan.h"
#include <mprintf.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 638
//<Prompt> ["curl_multi_socket_all","curl_easy_init","curl_easy_duphandle","curl_easy_cleanup","curl_mime_init"]
/*<Combination>: [CURLMcode curl_multi_socket_all(CURLM * multi_handle, int * running_handles),
    CURL *curl_easy_init(),
    CURL *curl_easy_duphandle(CURL * curl),
    void curl_easy_cleanup(CURL * curl),
    curl_mime *curl_mime_init(CURL * easy)
*/
//<score> 34.800003, nr_unique_branch: 57, p_cov: 0.6
//<Quality> {"diversity":13,"density":13,"covered_api":["curl_multi_socket_all","curl_easy_init","curl_easy_cleanup"],"uncovered_api":["curl_easy_duphandle","curl_mime_init"],"unique_branches":{"Curl_hash_add":[[113,6,113,15,1]],"singlesocket":[[2934,8,2934,37,1],[2936,8,2936,38,0],[2958,23,2958,45,0],[2960,10,2960,35,0],[2960,10,2960,35,1],[2962,10,2962,36,0],[2962,10,2962,36,1],[2964,10,2964,31,0],[2966,10,2966,32,0],[2966,10,2966,32,1],[2972,10,2972,31,1],[2974,10,2974,32,0],[2985,20,2985,34,0],[2986,21,2986,35,1],[2989,23,2989,58,1],[3028,8,3028,13,0],[3032,10,3032,36,1],[3034,10,3034,35,0],[3036,10,3036,23,0],[3037,12,3037,28,1]],"Curl_http_resume":[[2986,34,2986,56,0]],"Curl_http_body":[[2475,11,2475,29,0]],"dprintf_formatf":[[657,8,657,14,1],[754,10,754,20,1],[780,20,780,47,0],[783,10,783,34,1],[802,10,802,34,1]],"Curl_http_readwrite_headers":[[4286,37,4286,54,0],[4287,12,4287,41,0],[4302,11,4302,27,0],[4303,11,4303,28,1],[4304,11,4304,33,1],[4305,11,4305,33,1],[4311,16,4311,31,1],[4352,11,4352,18,1]],"Curl_retry_request":[[1781,6,1781,24,0],[1782,6,1782,67,1]],"multi_socket":[[3192,13,3192,17,0],[3192,21,3192,28,0]],"Curl_fillreadbuffer":[[325,10,325,29,1],[349,10,349,31,1]],"dprintf_Pass1":[[399,10,399,17,0],[399,21,399,42,0]],"Curl_http_method":[[2156,6,2156,24,0],[2173,7,2173,23,0]],"Curl_http_bodysend":[[2521,3,2521,19,0],[2523,8,2523,26,1],[2528,8,2528,30,1],[2543,8,2543,11,1],[2547,13,2547,50,1],[2547,54,2547,72,0],[2549,10,2549,16,1],[2555,8,2555,14,1],[2565,8,2565,14,1],[2570,27,2570,41,0],[2571,8,2571,14,1]]},"library_calls":["curl_multi_init","curl_easy_init","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_multi_add_handle","curl_multi_socket_all","curl_multi_perform","curl_multi_remove_handle","curl_multi_cleanup","curl_easy_cleanup"]}
/**/


extern "C" int LLVMFuzzerTestOneInput_35(const uint8_t *data, size_t size) {
	if(size<=0) return 0;

    FILE *in_file = fmemopen((void *)data, size, "rb");
    FILE *out_file = fopen("output_file", "wb");

    if (in_file == nullptr || out_file == nullptr) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    CURLM *multi_handle = curl_multi_init();
    if (multi_handle == nullptr) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    CURL *handle = curl_easy_init();
    if (handle == nullptr) {
        curl_multi_cleanup(multi_handle);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Set the input file as the source of the request
    curl_easy_setopt(handle, CURLOPT_READDATA, in_file);
    curl_easy_setopt(handle, CURLOPT_READFUNCTION, fread);
    curl_easy_setopt(handle, CURLOPT_UPLOAD, 1L);
    curl_easy_setopt(handle, CURLOPT_URL, "http://example.com/upload");

    // Add handle to the multi handle
    curl_multi_add_handle(multi_handle, handle);

    int running_handles;
    CURLMcode res;
    do {
        // Wait for activity on the handles
        res = curl_multi_socket_all(multi_handle, &running_handles);

        // Perform the transfer on each handle
        res = curl_multi_perform(multi_handle, &running_handles);

    } while (running_handles > 0 && res == CURLM_OK);

    // Cleanup
    curl_multi_remove_handle(multi_handle, handle);
    curl_multi_cleanup(multi_handle);
    curl_easy_cleanup(handle);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;

    assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}