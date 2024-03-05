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
//<ID> 593
//<Prompt> ["curl_easy_perform","curl_multi_timeout","curl_mime_init","curl_mime_filename","curl_getenv"]
/*<Combination>: [CURLcode curl_easy_perform(CURL * curl),
    CURLMcode curl_multi_timeout(CURLM * multi_handle, long * milliseconds),
    curl_mime *curl_mime_init(CURL * easy),
    CURLcode curl_mime_filename(curl_mimepart * part, const char * filename),
    char *curl_getenv(const char * variable)
*/
//<score> 2.25, nr_unique_branch: 4, p_cov: 0.6
//<Quality> {"diversity":12,"density":9,"covered_api":["curl_easy_perform","curl_multi_timeout","curl_mime_init"],"uncovered_api":["curl_mime_filename","curl_getenv"],"unique_branches":{"Curl_vsetopt":[[556,3,556,35,0],[562,8,562,20,1],[565,8,565,41,0],[566,8,566,68,0]]},"library_calls":["curl_easy_init","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_perform","curl_multi_init","curl_multi_timeout","curl_mime_init","curl_mime_free","curl_easy_cleanup","curl_multi_cleanup"]}
/**/


extern "C" int LLVMFuzzerTestOneInput_31(const uint8_t* f_data, size_t f_size) {
	if(f_size<=0) return 0;

	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(f_data, sizeof(uint8_t), f_size, input_file_ptr);
	fclose(input_file_ptr);

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}





    // Step 4: Open input file using fmemopen
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Step 5: Get file descriptor for reading
    int in_fd = fuzz_fileno(in_file);

    // Step 6: Set input file name
    const char *input_file = "input_file";

    // Initialize curl easy handle
    CURL *curl = curl_easy_init();
    if (!curl) {
        assert_file_closed(&in_file);;
        assert_file_name_closed("input_file");
	assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
    }

    // Set input file handle
    curl_easy_setopt(curl, CURLOPT_READDATA, in_file);

    // Set input file descriptor
    curl_easy_setopt(curl, CURLOPT_READFUNCTION, NULL);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE_LARGE, (curl_off_t)size);
    curl_easy_setopt(curl, CURLOPT_READDATA, in_fd);

    // Step 2: Call curl_easy_perform
    CURLcode res = curl_easy_perform(curl);

    // Step 1: Call curl_multi_timeout
    CURLM *multi_handle = curl_multi_init();
    long timeout;
    curl_multi_timeout(multi_handle, &timeout);

    // Initialize curl mime handle
    curl_mime *mime = curl_mime_init(curl);

    // Step 7: Cleanup
    curl_mime_free(mime);
    curl_easy_cleanup(curl);
    curl_multi_cleanup(multi_handle);
    assert_file_closed(&in_file);;

    assert_file_name_closed("input_file");
	assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
}