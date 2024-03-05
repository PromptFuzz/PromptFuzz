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
//<ID> 179
//<Prompt> ["curl_mime_name","curl_easy_send","curl_pushheader_bynum","curl_multi_socket_action","curl_mprintf"]
/*<Combination>: [CURLcode curl_mime_name(curl_mimepart * part, const char * name),
    CURLcode curl_easy_send(CURL * curl, const void * buffer, size_t buflen, size_t * n),
    char *curl_pushheader_bynum(struct curl_pushheaders * h, size_t num),
    CURLMcode curl_multi_socket_action(CURLM * multi_handle, curl_socket_t s, int ev_bitmask, int * running_handles),
    int curl_mprintf(const char * format)
*/
//<score> 12.8, nr_unique_branch: 63, p_cov: 0.2
//<Quality> {"diversity":11,"density":11,"covered_api":["curl_mime_name"],"uncovered_api":["curl_easy_send","curl_pushheader_bynum","curl_multi_socket_action","curl_mprintf"],"unique_branches":{"read_part_content":[[822,5,822,12,0],[824,12,824,43,1]],"mime_part_rewind":[[1071,6,1071,34,0],[1074,6,1074,37,1],[1093,6,1093,29,0]],"Curl_http_body":[[2413,3,2413,25,0],[2438,6,2438,26,0],[2446,8,2446,13,1],[2449,13,2449,61,0],[2456,8,2456,15,0],[2458,8,2458,14,1],[2473,11,2473,39,0],[2474,10,2474,28,1]],"Curl_mime_prepare_headers":[[1789,6,1789,18,0],[1791,5,1791,28,1],[1794,5,1794,23,1],[1801,5,1801,12,0],[1812,11,1812,22,1],[1835,18,1835,32,1],[1847,36,1847,44,1],[1848,36,1848,44,1],[1849,36,1849,44,1],[1858,6,1858,17,1],[1869,13,1869,24,1]],"mime_mem_read":[[666,6,666,13,1],[669,6,669,17,0],[669,6,669,17,1],[672,6,672,8,0]],"multipart_size":[[1634,6,1634,11,1],[1640,31,1640,35,0],[1640,31,1640,35,1],[1643,8,1643,14,1],[1646,8,1646,17,0]],"Curl_mime_contenttype":[[1730,6,1730,14,1]],"Curl_mime_rewind":[[1611,10,1611,52,0]],"Curl_http_bodysend":[[2576,3,2576,25,0],[2578,8,2578,26,1],[2600,8,2600,28,0],[2600,32,2600,56,0],[2601,8,2601,62,0],[2607,10,2607,16,1],[2616,52,2616,55,0],[2616,52,2616,55,1],[2618,12,2618,18,1],[2629,8,2629,11,1],[2633,13,2633,50,1],[2633,54,2633,72,1],[2643,8,2643,14,1],[2658,8,2658,14,1],[2663,27,2663,41,0],[2664,8,2664,14,1]],"slist_size":[[1621,9,1621,10,0],[1621,9,1621,10,1],[1622,8,1622,13,0]],"Curl_http_method":[[2170,7,2170,29,0]],"Curl_mime_size":[[1658,6,1658,38,0],[1658,6,1658,38,1],[1663,6,1663,19,1],[1666,6,1666,15,0],[1666,19,1666,50,0],[1666,19,1666,50,1]],"escape_string":[[315,7,315,11,0],[315,15,315,64,1]]},"library_calls":["curl_mime_init","curl_mime_addpart","curl_mime_name","curl_mime_data","curl_easy_init","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_perform","curl_easy_cleanup","curl_mime_free"]}
/**/

extern "C" int LLVMFuzzerTestOneInput_11(const uint8_t* f_data, size_t f_size) {
	if(f_size<=9) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
	//fuzzer shim end}




    // Create a multipart form data
    curl_mime *mime = curl_mime_init(NULL);

    // Create a new part
    curl_mimepart *part = curl_mime_addpart(mime);

    // Set the name of the part
    curl_mime_name(part, fuzz_str_1);

    // Set the data of the part from the input data
    curl_mime_data(part, (const char*)data, size);

    // Create a CURL handle
    CURL *curl = curl_easy_init();

    // Set the URL to send the data
    curl_easy_setopt(curl, CURLOPT_URL, "http://example.com");

    // Set the POST data to the multipart form data
    curl_easy_setopt(curl, CURLOPT_MIMEPOST, mime);

    // Open a file for writing the response
    FILE *out_file = fopen("output_file", "wb");
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, out_file);

    // Perform the request
    curl_easy_perform(curl);

    // Cleanup
    curl_easy_cleanup(curl);
    curl_mime_free(mime);
    assert_file_closed(&out_file);;

    assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	return 0;
}