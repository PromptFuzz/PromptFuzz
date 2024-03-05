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
//<ID> 2127
//<Prompt> ["curl_global_trace","curl_getdate","curl_easy_reset","curl_url_set","curl_formadd"]
/*<Combination>: [CURLcode curl_global_trace(const char * config),
    time_t curl_getdate(const char * p, const time_t * unused),
    void curl_easy_reset(CURL * curl),
    CURLUcode curl_url_set(CURLU * handle, CURLUPart what, const char * part, unsigned int flags),
    CURLFORMcode curl_formadd(struct curl_httppost ** httppost, struct curl_httppost ** last_post)
*/
//<score> 1.6923077, nr_unique_branch: 1, p_cov: 1
//<Quality> {"diversity":13,"density":11,"covered_api":["curl_global_trace","curl_getdate","curl_easy_reset","curl_url_set","curl_formadd"],"uncovered_api":[],"unique_branches":{"Curl_pretransfer":[[1325,8,1325,10,0]]},"library_calls":["curl_easy_init","curl_global_trace","curl_getdate","curl_easy_reset","curl_url","curl_url_set","curl_easy_setopt","curl_formadd","curl_easy_setopt","curl_easy_perform","curl_formfree","curl_url_cleanup","curl_easy_cleanup"]}
/*Here is the implementation of the LLVMFuzzerTestOneInput_96 function using curl library APIs:

*/


extern "C" int LLVMFuzzerTestOneInput_96(const uint8_t* f_data, size_t f_size) {
	if(f_size<=32) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
	//fuzzer shim end}




    // Create a FILE variable to read the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");

    // Initialize curl
    CURL *curl = curl_easy_init();
    if (!curl) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // Set verbose mode for curl
    curl_global_trace(fuzz_str_1);

    // Set the date for curl
    time_t now = time(NULL);
    char date[256];
    strftime(date, sizeof(date), "%a, %d %b %Y %H:%M:%S GMT", gmtime(&now));
    curl_getdate(date, NULL);

    // Reset curl easy handle
    curl_easy_reset(curl);

    // Set the URL for curl
    CURLU *url = curl_url();
    curl_url_set(url, CURLUPART_URL, fuzz_str_2, 0);
    curl_easy_setopt(curl, CURLOPT_CURLU, url);

    // Add a form field to curl
    struct curl_httppost *formpost = NULL;
    struct curl_httppost *lastptr = NULL;
    curl_formadd(&formpost, &lastptr, CURLFORM_COPYNAME, "name",
                 CURLFORM_COPYCONTENTS, "john", CURLFORM_END);

    // Set the form data for curl
    curl_easy_setopt(curl, CURLOPT_HTTPPOST, formpost);

    // Perform the curl request
    CURLcode res = curl_easy_perform(curl);

    // Clean up the form data
    curl_formfree(formpost);

    // Clean up the URL handle
    curl_url_cleanup(url);

    // Clean up curl
    curl_easy_cleanup(curl);

    // Close the input file
    assert_file_closed(&in_file);;

    assert_file_closed(&in_file);
	return 0;
}