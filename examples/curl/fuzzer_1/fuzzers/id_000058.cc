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
//<ID> 1067
//<Prompt> ["curl_multi_add_handle","curl_mime_init","curl_mime_addpart","curl_mime_data","curl_formadd","curl_mime_data_cb"]
/*<Combination>: [CURLMcode curl_multi_add_handle(CURLM * multi_handle, CURL * curl_handle),
    curl_mime *curl_mime_init(CURL * easy),
    curl_mimepart *curl_mime_addpart(curl_mime * mime),
    CURLcode curl_mime_data(curl_mimepart * part, const char * data, size_t datasize),
    CURLFORMcode curl_formadd(struct curl_httppost ** httppost, struct curl_httppost ** last_post),
    CURLcode curl_mime_data_cb(curl_mimepart * part, curl_off_t datasize, curl_read_callback readfunc, curl_seek_callback seekfunc, curl_free_callback freefunc, void * arg)
*/
//<score> 0, nr_unique_branch: 3, p_cov: 0
//<Quality> {"diversity":7,"density":7,"covered_api":[],"uncovered_api":["curl_multi_add_handle","curl_mime_init","curl_mime_addpart","curl_mime_data","curl_formadd","curl_mime_data_cb"],"unique_branches":{"Curl_http":[[3333,20,3333,51,0],[3334,20,3334,52,0],[3335,20,3335,43,0]]},"library_calls":["curl_easy_init","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_perform","curl_easy_cleanup"]}
/**/

// Including the required headers

// Function to write received data to file
static size_t  id_000058_WriteToFile(void *buffer, size_t size, size_t nmemb, void *userdata) {
    FILE *file = (FILE *)userdata;
    return fwrite(buffer, size, nmemb, file);
}

// Fuzz driver function
extern "C" int LLVMFuzzerTestOneInput_58(const uint8_t* f_data, size_t f_size) {
	if(f_size<=0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Create a libcurl easy handle
    CURL *curl = curl_easy_init();
    
    // Set the URL to retrieve data from
    const char *url = "http://example.com";
    curl_easy_setopt(curl, CURLOPT_URL, url);
    
    // Set the CURLOPT_WRITEFUNCTION option to write received data to file
    FILE *file = fopen("output_file", "wb");
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION,  id_000058_WriteToFile);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, file);
    
    // Set the CURLOPT_USERAGENT option to specify the User-Agent header
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");
    
    // Perform the request
    curl_easy_perform(curl);
    
    // Clean up
    curl_easy_cleanup(curl);
    assert_file_closed(&file);;
    
    assert_file_name_closed("output_file");
	assert_file_closed(&file);
	return 0;
}