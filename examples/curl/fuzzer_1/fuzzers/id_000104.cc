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
//<ID> 2359
//<Prompt> ["curl_easy_option_by_name","curl_easy_pause","curl_ws_recv","curl_multi_socket_action","curl_slist_free_all"]
/*<Combination>: [const struct curl_easyoption *curl_easy_option_by_name(const char * name),
    CURLcode curl_easy_pause(CURL * handle, int bitmask),
    CURLcode curl_ws_recv(CURL * curl, void * buffer, size_t buflen, size_t * recv, const struct curl_ws_frame ** metap),
    CURLMcode curl_multi_socket_action(CURLM * multi_handle, curl_socket_t s, int ev_bitmask, int * running_handles),
    void curl_slist_free_all(struct curl_slist * list)
*/
//<score> 1.6, nr_unique_branch: 3, p_cov: 0.4
//<Quality> {"diversity":10,"density":10,"covered_api":["curl_easy_pause","curl_slist_free_all"],"uncovered_api":["curl_easy_option_by_name","curl_ws_recv","curl_multi_socket_action"],"unique_branches":{"Curl_vsetopt":[[296,3,296,26,0],[300,31,300,57,0],[301,8,301,31,0]]},"library_calls":["curl_easy_init","curl_easy_setopt","curl_slist_append","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_pause","curl_slist_free_all","curl_easy_cleanup"]}
/*Below is a C++ program that uses the curl library APIs to achieve the specified event:

*/


extern "C" int LLVMFuzzerTestOneInput_104(const uint8_t* f_data, size_t f_size) {
	if(f_size<=33) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
	//fuzzer shim end}




    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);

    CURL *curl = curl_easy_init();
    if (!curl) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }

    curl_easy_setopt(curl, CURLOPT_URL, "http://example.com");

    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, fuzz_str_1);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    curl_easy_setopt(curl, CURLOPT_WRITEDATA, out_file);
    curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");

    curl_easy_pause(curl, CURLPAUSE_ALL);

    char buffer[1024];
    size_t bytes_read = fread(buffer, 1, sizeof(buffer), in_file);
    while (bytes_read > 0) {
        size_t bytes_written = fwrite(buffer, 1, bytes_read, out_file);
        if (bytes_written < bytes_read) {
            break;
        }
        bytes_read = fread(buffer, 1, sizeof(buffer), in_file);
    }

    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);

    assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}