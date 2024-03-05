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
//<ID> 655
//<Prompt> ["curl_multi_socket_all","curl_share_setopt","curl_easy_pause","curl_easy_init","curl_easy_cleanup"]
/*<Combination>: [CURLMcode curl_multi_socket_all(CURLM * multi_handle, int * running_handles),
    CURLSHcode curl_share_setopt(CURLSH * share, CURLSHoption option),
    CURLcode curl_easy_pause(CURL * handle, int bitmask),
    CURL *curl_easy_init(),
    void curl_easy_cleanup(CURL * curl)
*/
//<score> 4.4, nr_unique_branch: 10, p_cov: 1
//<Quality> {"diversity":15,"density":6,"covered_api":["curl_multi_socket_all","curl_share_setopt","curl_easy_pause","curl_easy_init","curl_easy_cleanup"],"uncovered_api":[],"unique_branches":{"Curl_cookie_freelist":[[1516,9,1516,11,1]],"Curl_cookie_init":[[1207,6,1207,10,0],[1210,8,1210,10,1],[1224,6,1224,10,1]],"Curl_cookie_cleanup":[[1571,6,1571,7,0],[1573,16,1573,36,0],[1573,16,1573,36,1]],"curl_share_setopt":[[82,5,82,31,0],[84,10,84,25,0],[86,12,86,27,1]]},"library_calls":["curl_multi_init","curl_multi_socket_all","curl_share_init","curl_share_setopt","curl_easy_init","curl_easy_pause","curl_multi_cleanup","curl_share_cleanup","curl_easy_cleanup"]}
/**/

extern "C" int LLVMFuzzerTestOneInput_37(const uint8_t* f_data, size_t f_size) {
	if(f_size<=0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Step 4: open input file
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Step 5: get file descriptor for reading
    int in_fd = fuzz_fileno(in_file);
    if (in_fd < 0) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Step 6: create output file
    FILE* out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Step 5: get file descriptor for writing
    int out_fd = fuzz_fileno(out_file);
    if (out_fd < 0) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }

    // Step 1: CURLMcode curl_multi_socket_all(CURLM * multi_handle, int * running_handles)
    CURLM *multi_handle = curl_multi_init();
    int running_handles;
    CURLMcode res = curl_multi_socket_all(multi_handle, &running_handles);
    if (res != CURLM_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        curl_multi_cleanup(multi_handle);
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Step 2: CURLSHcode curl_share_setopt(CURLSH * share, CURLSHoption option)
    CURLSH *share = curl_share_init();
    CURLSHcode share_res = curl_share_setopt(share, CURLSHOPT_SHARE, CURL_LOCK_DATA_COOKIE);
    if (share_res != CURLSHE_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        curl_multi_cleanup(multi_handle);
        curl_share_cleanup(share);
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }

    // Step 3: CURLcode curl_easy_pause(CURL * handle, int bitmask)
    CURL *curl_handle = curl_easy_init();
    CURLcode pause_res = curl_easy_pause(curl_handle, CURLPAUSE_ALL);
    if (pause_res != CURLE_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        curl_multi_cleanup(multi_handle);
        curl_share_cleanup(share);
        curl_easy_cleanup(curl_handle);
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Step 7: Release resources
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    curl_multi_cleanup(multi_handle);
    curl_share_cleanup(share);
    curl_easy_cleanup(curl_handle);
    
    assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}