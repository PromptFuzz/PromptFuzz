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
//<ID> 1230
//<Prompt> ["curl_url_strerror","curl_share_init","curl_mime_type","curl_easy_option_by_id","curl_multi_remove_handle"]
/*<Combination>: [const char *curl_url_strerror(CURLUcode ),
    CURLSH *curl_share_init(),
    CURLcode curl_mime_type(curl_mimepart * part, const char * mimetype),
    const struct curl_easyoption *curl_easy_option_by_id(CURLoption id),
    CURLMcode curl_multi_remove_handle(CURLM * multi_handle, CURL * curl_handle)
*/
//<score> 0.7058824, nr_unique_branch: 1, p_cov: 1
//<Quality> {"diversity":17,"density":6,"covered_api":["curl_url_strerror","curl_share_init","curl_mime_type","curl_easy_option_by_id","curl_multi_remove_handle"],"uncovered_api":[],"unique_branches":{"curl_url_strerror":[[469,3,469,30,0]]},"library_calls":["curl_url","curl_url_get","curl_url_strerror","curl_share_init","curl_multi_init","curl_mime_init","curl_mime_addpart","curl_mime_type","curl_easy_option_by_id","curl_easy_init","curl_easy_setopt","curl_multi_remove_handle","curl_url_cleanup","curl_share_cleanup","curl_multi_cleanup","curl_mime_free","curl_easy_cleanup"]}
/*Here is the complete code for the C++ program using curl library APIs:

*/


extern "C" int LLVMFuzzerTestOneInput_64(const uint8_t* f_data, size_t f_size) {
	if(f_size<=17) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
	//fuzzer shim end}




    // Create a FILE * variable to read the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file == NULL) {
        assert_file_closed(&in_file);
	return 0; // Return if unable to create the file stream
    }
    
    // Create a CURLU * handle
    CURLU *url_handle = curl_url();
    
    // Call curl_url_get to get the CURLUcode
    CURLUcode url_code = curl_url_get(url_handle, CURLUPART_HOST, NULL, fuzz_uint32_t_1);
    const char *url_error = curl_url_strerror(url_code);
    
    // Create a CURLSH * handle
    CURLSH *share_handle = curl_share_init();
    
    // Create a CURLM * handle
    CURLM *multi_handle = curl_multi_init();
    
    // Create a curl_mimepart * variable
    curl_mime *mime_handle = curl_mime_init(NULL);
    curl_mimepart *mime_part = curl_mime_addpart(mime_handle);
    
    // Call curl_mime_type to set the MIME type
    CURLcode mime_code = curl_mime_type(mime_part, fuzz_str_2);
    
    // Call curl_easy_option_by_id to get the curl_easyoption by id
    const struct curl_easyoption *option = curl_easy_option_by_id(CURLOPT_USERAGENT);
    
    // Create a CURL * handle
    CURL *easy_handle = curl_easy_init();
    
    // Call curl_easy_setopt to set an option
    CURLcode easy_code = curl_easy_setopt(easy_handle, CURLOPT_URL, "https://example.com");
    
    // Call curl_multi_remove_handle to remove the handle from multi handle
    CURLMcode multi_code = curl_multi_remove_handle(multi_handle, easy_handle);
    
    // Close the input file stream
    assert_file_closed(&in_file);;
    
    // Cleanup all the resources
    curl_url_cleanup(url_handle);
    curl_share_cleanup(share_handle);
    curl_multi_cleanup(multi_handle);
    curl_mime_free(mime_handle);
    curl_easy_cleanup(easy_handle);
    
    assert_file_closed(&in_file);
	return 0;
}