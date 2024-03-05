#include "FDSan.h"
#include <mprintf.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 1684
//<Prompt> ["curl_easy_option_by_id","curl_easy_nextheader","curl_ws_recv","curl_share_setopt","curl_mvsprintf","curl_easy_setopt"]
/*<Combination>: [const struct curl_easyoption *curl_easy_option_by_id(CURLoption id),
    struct curl_header *curl_easy_nextheader(CURL * easy, unsigned int origin, int request, struct curl_header * prev),
    CURLcode curl_ws_recv(CURL * curl, void * buffer, size_t buflen, size_t * recv, const struct curl_ws_frame ** metap),
    CURLSHcode curl_share_setopt(CURLSH * share, CURLSHoption option),
    int curl_mvsprintf(char * buffer, const char * format, __va_list_tag * args),
    CURLcode curl_easy_setopt(CURL * curl, CURLoption option)
*/
//<score> 0.8333334, nr_unique_branch: 4, p_cov: 0.16666667
//<Quality> {"diversity":14,"density":14,"covered_api":["curl_easy_setopt"],"uncovered_api":["curl_easy_option_by_id","curl_easy_nextheader","curl_ws_recv","curl_share_setopt","curl_mvsprintf"],"unique_branches":{"Curl_vsetopt":[[1597,3,1597,28,0],[1925,3,1925,30,0],[1929,40,1929,66,1],[1933,8,1933,18,1]]},"library_calls":["curl_easy_init","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_perform","curl_easy_cleanup"]}
/**/


extern "C" int LLVMFuzzerTestOneInput_82(const uint8_t *data, size_t size) {
	if(size<=0) return 0;

    // Create a FILE * variable to read the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
  
    // Create a CURL * variable
    CURL *curl = curl_easy_init();
    if (!curl) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
  
    // Set CURLOPT_URL option
    curl_easy_setopt(curl, CURLOPT_URL, "https://example.com");
  
    // Set CURLOPT_WRITEDATA option to write the output data to a file
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        curl_easy_cleanup(curl);
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, out_file);
  
    // Set CURLOPT_HEADERFUNCTION and CURLOPT_HEADERDATA options
    // to receive the response headers
    struct curl_header *header = NULL;
    curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, curl_easy_nextheader);
    curl_easy_setopt(curl, CURLOPT_HEADERDATA, header);
  
    // Set CURLOPT_WRITEFUNCTION and CURLOPT_WRITEDATA options
    // to receive the response body
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, fwrite);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, out_file);
  
    // Set CURLOPT_PROGRESSFUNCTION and CURLOPT_PROGRESSDATA options
    // to receive progress updates
    curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, NULL);
    curl_easy_setopt(curl, CURLOPT_PROGRESSDATA, NULL);
  
    // Set other options as needed
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
  
    // Perform the request
    curl_easy_perform(curl);
  
    // Cleanup and release resources
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    curl_easy_cleanup(curl);
  
    assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}