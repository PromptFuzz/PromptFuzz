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
//<ID> 79
//<Prompt> ["curl_global_init","curl_mime_headers","curl_easy_unescape","curl_easy_option_by_name","curl_mime_free"]
/*<Combination>: [CURLcode curl_global_init(long flags),
    CURLcode curl_mime_headers(curl_mimepart * part, struct curl_slist * headers, int take_ownership),
    char *curl_easy_unescape(CURL * handle, const char * string, int length, int * outlength),
    const struct curl_easyoption *curl_easy_option_by_name(const char * name),
    void curl_mime_free(curl_mime * mime)
*/
//<score> 1.1428572, nr_unique_branch: 1, p_cov: 0.8
//<Quality> {"diversity":21,"density":15,"covered_api":["curl_global_init","curl_easy_unescape","curl_easy_option_by_name","curl_mime_free"],"uncovered_api":["curl_mime_headers"],"unique_branches":{"curl_easy_unescape":[[191,8,191,12,1]]},"library_calls":["curl_easy_init","curl_global_init","curl_mime_init","curl_easy_unescape","curl_easy_option_by_name","curl_free","curl_mime_free","curl_global_cleanup","curl_easy_cleanup"]}
/**/


extern "C" int LLVMFuzzerTestOneInput_3(const uint8_t* f_data, size_t f_size) {
	if(f_size<=14) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
	//fuzzer shim end}




    CURL *curl = curl_easy_init();
    if (!curl) {
        return 0;
    }

    // Step 4: create input file
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        curl_easy_cleanup(curl);
        assert_file_closed(&in_file);
	return 0;
    }

    // Step 5: get file descriptor for reading
    int in_fd = fuzz_fileno(in_file);

    // Step 6: create output file
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        curl_easy_cleanup(curl);
        assert_file_closed(&in_file);;
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
    }

    // Step 7: get file descriptor for writing
    int out_fd = fuzz_fileno(out_file);

    // Step 1: initialize curl
    CURLcode res = curl_global_init(CURL_GLOBAL_ALL);
    if (res != CURLE_OK) {
        curl_easy_cleanup(curl);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }

    // Step 2: create a curl_mime struct
    curl_mime *mime = curl_mime_init(curl);
    if (!mime) {
        curl_global_cleanup();
        curl_easy_cleanup(curl);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }

    // Step 3: parse input data
    char *unescaped_data = curl_easy_unescape(curl, (char *)data, size, NULL);
    if (!unescaped_data) {
        curl_mime_free(mime);
        curl_global_cleanup();
        curl_easy_cleanup(curl);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }

    // Step 8: get option by name
    const struct curl_easyoption *option = curl_easy_option_by_name(fuzz_str_1);
    if (!option) {
        curl_free(unescaped_data);
        curl_mime_free(mime);
        curl_global_cleanup();
        curl_easy_cleanup(curl);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }

    // Clean up allocated resources
    curl_free(unescaped_data);
    curl_mime_free(mime);
    curl_global_cleanup();
    curl_easy_cleanup(curl);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;

    assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}