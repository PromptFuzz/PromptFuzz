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
//<ID> 71
//<Prompt> ["curl_global_init","curl_mime_headers","curl_easy_unescape","curl_easy_option_by_name","curl_mime_free"]
/*<Combination>: [CURLcode curl_global_init(long flags),
    CURLcode curl_mime_headers(curl_mimepart * part, struct curl_slist * headers, int take_ownership),
    char *curl_easy_unescape(CURL * handle, const char * string, int length, int * outlength),
    const struct curl_easyoption *curl_easy_option_by_name(const char * name),
    void curl_mime_free(curl_mime * mime)
*/
//<score> 2.64, nr_unique_branch: 10, p_cov: 0.6
//<Quality> {"diversity":20,"density":8,"covered_api":["curl_global_init","curl_mime_headers","curl_mime_free"],"uncovered_api":["curl_easy_unescape","curl_easy_option_by_name"],"unique_branches":{"cf_socket_data_pending":[[1268,11,1268,23,1]],"readwrite_data":[[758,29,758,47,1]],"Curl_ipv6_scope":[[81,5,81,16,1],[87,5,87,12,0]],"socket_connect_result":[[729,3,729,19,1],[742,3,742,10,0],[106,39,106,45,0],[106,49,106,68,1]],"data_pending":[[377,5,377,46,1]],"cf_tcp_connect":[[1208,8,1208,18,1]]},"library_calls":["curl_global_init","curl_mime_init","curl_mime_addpart","curl_slist_append","curl_mime_headers","curl_mime_filedata","curl_easy_init","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_perform","curl_mime_free","curl_easy_cleanup","curl_global_cleanup"]}
/**/


extern "C" int LLVMFuzzerTestOneInput_2(const uint8_t* f_data, size_t f_size) {
	if(f_size<=54) return 0;

	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(f_data, sizeof(uint8_t), f_size, input_file_ptr);
	fclose(input_file_ptr);

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
	//fuzzer shim end}




    // Step 4: Create a FILE pointer for reading the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Step 5: Get the file descriptor for reading
    int input_fd = fuzz_fileno(in_file);

    // Step 6: Create a string for the input file name
    const char *input_file = "input_file";

    // Initialize curl
    CURLcode res = curl_global_init(CURL_GLOBAL_ALL);
    if (res != CURLE_OK) {
        assert_file_closed(&in_file);;
        assert_file_name_closed("input_file");
	assert_file_closed(&in_file);
	assert_fd_closed(input_fd);
	return 0;
    }

    // Create mime structure
    curl_mime *mime = curl_mime_init(NULL);
    if (!mime) {
        assert_file_closed(&in_file);;
        curl_global_cleanup();
        assert_file_name_closed("input_file");
	assert_file_closed(&in_file);
	assert_fd_closed(input_fd);
	return 0;
    }

    // Step 7: Create a string for the output file name
    const char *output_file = "output_file";

    // Create mime part and add headers
    curl_mimepart *part = curl_mime_addpart(mime);
    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, fuzz_str_1);
    curl_mime_headers(part, headers, 1);

    // Set mime data from input file
    curl_mime_filedata(part, fuzz_str_2);

    // Create easy handle
    CURL *curl = curl_easy_init();
    if (!curl) {
        assert_file_closed(&in_file);;
        curl_mime_free(mime);
        curl_global_cleanup();
        assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_closed(&in_file);
	assert_fd_closed(input_fd);
	return 0;
    }

    // Set the output file for writing
    FILE *out_file = fopen(output_file, "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        curl_mime_free(mime);
        curl_easy_cleanup(curl);
        curl_global_cleanup();
        assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(input_fd);
	return 0;
    }

    // Set curl options
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, out_file);
    curl_easy_setopt(curl, CURLOPT_URL, "http://example.com");
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

    // Perform the request
    curl_easy_perform(curl);

    // Cleanup
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    curl_mime_free(mime);
    curl_easy_cleanup(curl);
    curl_global_cleanup();

    assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(input_fd);
	return 0;
}