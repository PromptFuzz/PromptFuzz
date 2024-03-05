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
//<ID> 2116
//<Prompt> ["curl_global_trace","curl_global_init_mem","curl_getdate","curl_url_set","curl_formadd"]
/*<Combination>: [CURLcode curl_global_trace(const char * config),
    CURLcode curl_global_init_mem(long flags, curl_malloc_callback m, curl_free_callback f, curl_realloc_callback r, curl_strdup_callback s, curl_calloc_callback c),
    time_t curl_getdate(const char * p, const time_t * unused),
    CURLUcode curl_url_set(CURLU * handle, CURLUPart what, const char * part, unsigned int flags),
    CURLFORMcode curl_formadd(struct curl_httppost ** httppost, struct curl_httppost ** last_post)
*/
//<score> 13.671429, nr_unique_branch: 28, p_cov: 0.6
//<Quality> {"diversity":14,"density":11,"covered_api":["curl_global_trace","curl_url_set","curl_formadd"],"uncovered_api":["curl_global_init_mem","curl_getdate"],"unique_branches":{"curl_url_strerror":[[532,3,532,26,0]],"Curl_resolv":[[756,10,756,54,0]],"multi_runsingle":[[2202,12,2202,19,1],[2237,17,2237,45,1],[2288,14,2288,24,0]],"zonefrom_url":[[1700,6,1700,9,1]],"Curl_ip2addr":[[399,3,399,15,1],[405,3,405,16,0]],"Curl_http_body":[[2417,3,2417,25,0],[2420,8,2420,26,0],[2422,10,2422,28,1],[2427,10,2427,16,0]],"multi_done":[[692,3,692,24,0]],"curl_url_get":[[1439,3,1439,24,0],[1526,10,1526,27,0],[1527,12,1527,21,1]],"Curl_ipv6_scope":[[84,15,84,28,0]],"Curl_he2ai":[[279,8,279,34,0],[316,5,316,17,1],[325,5,325,18,0]],"Curl_getformdata":[[839,9,839,16,1],[858,22,858,29,1],[896,14,896,21,1],[921,10,921,17,1],[928,6,928,12,0]],"parseurlandfillconn":[[1814,18,1814,36,0]],"Curl_http_method":[[2169,7,2169,29,0]],"Curl_http":[[3267,6,3267,12,0]]},"library_calls":["curl_global_init","curl_global_trace","curl_url","curl_url_set","curl_formadd","curl_easy_init","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_perform","curl_easy_cleanup","curl_formfree","curl_url_cleanup","curl_global_cleanup"]}
/**/


extern "C" int LLVMFuzzerTestOneInput_95(const uint8_t* f_data, size_t f_size) {
	if(f_size<=12) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
	//fuzzer shim end}




    FILE *in_file = fmemopen((void *)data, size, "rb");
    FILE *out_file = fopen("output_file", "wb");

    // Initialize curl
    curl_global_init(CURL_GLOBAL_ALL);

    // Set trace configuration
    curl_global_trace(fuzz_str_1);

    // Initialize CURLU handle
    CURLU *handle = curl_url();

    // Set URL
    char url[256];
    fgets(url, sizeof(url), in_file);
    url[strcspn(url, "\n")] = '\0';
    curl_url_set(handle, CURLUPART_URL, url, 0);

    // Set form data
    struct curl_httppost *formpost = NULL;
    struct curl_httppost *lastpost = NULL;
    curl_formadd(&formpost, &lastpost, CURLFORM_COPYNAME, "fileupload",
                 CURLFORM_FILE, "test.png", CURLFORM_END);

    // Perform the request and write response to output file
    CURL *curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_HTTPPOST, formpost);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, out_file);
    curl_easy_perform(curl);

    // Clean up
    curl_easy_cleanup(curl);
    curl_formfree(formpost);
    assert_file_closed(&out_file);;
    assert_file_closed(&in_file);;
    curl_url_cleanup(handle);
    curl_global_cleanup();

    assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}