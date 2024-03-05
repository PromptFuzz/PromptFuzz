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
//<ID> 2211
//<Prompt> ["curl_escape","curl_share_setopt","curl_easy_escape","curl_version","curl_multi_strerror"]
/*<Combination>: [char *curl_escape(const char * string, int length),
    CURLSHcode curl_share_setopt(CURLSH * share, CURLSHoption option),
    char *curl_easy_escape(CURL * handle, const char * string, int length),
    char *curl_version(),
    const char *curl_multi_strerror(CURLMcode )
*/
//<score> 1.1076924, nr_unique_branch: 2, p_cov: 0.8
//<Quality> {"diversity":13,"density":6,"covered_api":["curl_share_setopt","curl_easy_escape","curl_version","curl_multi_strerror"],"uncovered_api":["curl_escape"],"unique_branches":{"curl_multi_strerror":[[370,3,370,16,1],[385,3,385,24,0]]},"library_calls":["curl_easy_init","curl_easy_setopt","curl_easy_escape","curl_free","curl_version","curl_multi_init","curl_share_init","curl_share_setopt","curl_multi_strerror","curl_share_cleanup","curl_multi_cleanup","curl_easy_cleanup"]}
/**/


extern "C" int LLVMFuzzerTestOneInput_102(const uint8_t* f_data, size_t f_size) {
	if(f_size<=15) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
	//fuzzer shim end}




    // Create a FILE pointer to read input data
    FILE* in_file = fmemopen((void*)data, size, "rb");
  
    // Create a CURL handle
    CURL* curl = curl_easy_init();
    
    // Check if CURL handle creation succeeded
    if(curl == NULL) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
  
    // Set the input file as the request body
    curl_easy_setopt(curl, CURLOPT_READDATA, in_file);
    
    // Escape a string using curl_easy_escape
    const char* input_str = "Hello World!";
    char* escaped_str = curl_easy_escape(curl, fuzz_str_1, static_cast<int>(fuzz_str_sz_1));
    curl_free(escaped_str);

    // Get the version of libcurl
    const char* version = curl_version();
  
    // Create a CURLM handle for multi handle usage
    CURLM* multi_handle = curl_multi_init();
    
    // Check if CURLM handle creation succeeded
    if(multi_handle == NULL) {
        assert_file_closed(&in_file);;
        curl_easy_cleanup(curl);
        assert_file_closed(&in_file);
	return 0;
    }
  
    // Set options for the CURLM handle
    CURLSH* share_handle = curl_share_init();
    curl_share_setopt(share_handle, CURLSHOPT_SHARE, CURL_LOCK_DATA_DNS);
  
    // Get the error string for a CURLM error code
    const char* error_str = curl_multi_strerror(CURLM_BAD_SOCKET);
    
    // Cleanup resources
    curl_share_cleanup(share_handle);
    curl_multi_cleanup(multi_handle);
    curl_easy_cleanup(curl);
    assert_file_closed(&in_file);;
  
    assert_file_closed(&in_file);
	return 0;
}