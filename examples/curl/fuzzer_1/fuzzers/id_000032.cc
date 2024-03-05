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
//<ID> 601
//<Prompt> ["curl_easy_perform","curl_multi_timeout","curl_mime_init","curl_mime_data_cb","curl_mime_filename"]
/*<Combination>: [CURLcode curl_easy_perform(CURL * curl),
    CURLMcode curl_multi_timeout(CURLM * multi_handle, long * milliseconds),
    curl_mime *curl_mime_init(CURL * easy),
    CURLcode curl_mime_data_cb(curl_mimepart * part, curl_off_t datasize, curl_read_callback readfunc, curl_seek_callback seekfunc, curl_free_callback freefunc, void * arg),
    CURLcode curl_mime_filename(curl_mimepart * part, const char * filename)
*/
//<score> 0.7714286, nr_unique_branch: 1, p_cov: 0.6
//<Quality> {"diversity":14,"density":9,"covered_api":["curl_easy_perform","curl_multi_timeout","curl_mime_init"],"uncovered_api":["curl_mime_data_cb","curl_mime_filename"],"unique_branches":{"randit":[[106,49,106,68,0]]},"library_calls":["curl_global_init","curl_easy_init","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_perform","curl_multi_init","curl_multi_timeout","curl_mime_init","curl_mime_addpart","curl_mime_free","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup"]}
/**/


extern "C" int LLVMFuzzerTestOneInput_32(const uint8_t* f_data, size_t f_size) {
	if(f_size<=0) return 0;

	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(f_data, sizeof(uint8_t), f_size, input_file_ptr);
	fclose(input_file_ptr);

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Step 4: Create a FILE variable to read input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    
    // Step 5: Get the file descriptor for reading
    int in_fd = fuzz_fileno(in_file);
    
    // Step 6: Create a string for the input file name
    const char *input_file = "input_file";
    
    // Initialize curl
    curl_global_init(CURL_GLOBAL_ALL);
    
    // Step 1: Create a CURL handle and perform easy transfer
    CURL *curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_URL, "https://example.com");
    curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
    curl_easy_setopt(curl, CURLOPT_READDATA, in_file);
    curl_easy_perform(curl);
    
    // Step 2: Create a CURLM handle and get multi timeout
    CURLM *multi_handle = curl_multi_init();
    long timeout;
    curl_multi_timeout(multi_handle, &timeout);
    
    // Step 3: Create a MIME handle and add MIME part
    curl_mime *mime = curl_mime_init(curl);
    curl_mimepart *part = curl_mime_addpart(mime);
    
    // Step 7: Cleanup and release resources
    curl_mime_free(mime);
    curl_easy_cleanup(curl);
    curl_multi_cleanup(multi_handle);
    curl_global_cleanup();
    assert_file_closed(&in_file);;
    
    assert_file_name_closed("input_file");
	assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
}