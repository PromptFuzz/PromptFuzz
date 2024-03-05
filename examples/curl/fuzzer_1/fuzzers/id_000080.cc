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
//<ID> 1610
//<Prompt> ["curl_easy_option_by_id","curl_mime_encoder","curl_strnequal","curl_mime_filename","curl_strequal","curl_mvfprintf","curl_easy_nextheader"]
/*<Combination>: [const struct curl_easyoption *curl_easy_option_by_id(CURLoption id),
    CURLcode curl_mime_encoder(curl_mimepart * part, const char * encoding),
    int curl_strnequal(const char * s1, const char * s2, size_t n),
    CURLcode curl_mime_filename(curl_mimepart * part, const char * filename),
    int curl_strequal(const char * s1, const char * s2),
    int curl_mvfprintf(FILE * fd, const char * format, __va_list_tag * args),
    struct curl_header *curl_easy_nextheader(CURL * easy, unsigned int origin, int request, struct curl_header * prev)
*/
//<score> 12.952382, nr_unique_branch: 50, p_cov: 0.2857143
//<Quality> {"diversity":18,"density":16,"covered_api":["curl_mime_encoder","curl_mime_filename"],"uncovered_api":["curl_easy_option_by_id","curl_strnequal","curl_strequal","curl_mvfprintf","curl_easy_nextheader"],"unique_branches":{"Curl_http_header":[[3603,11,3604,73,0]],"Curl_add_custom_headers":[[1961,11,1961,18,0],[1964,10,1964,14,1],[1997,10,1997,13,0],[1997,17,1997,39,0],[2001,15,2001,19,0],[2004,12,2004,16,0],[2007,27,2007,37,1],[2009,14,2009,35,0],[2014,19,2014,59,1],[2018,19,2018,59,1],[2022,19,2022,37,1],[2027,19,2027,38,1],[2032,19,2032,44,1],[2049,14,2049,24,1],[2051,14,2051,20,1],[45,37,45,49,0],[45,53,45,66,1],[46,29,46,59,1],[46,29,46,59,1],[46,29,46,59,1],[44,22,44,34,0],[44,22,44,34,1],[44,38,44,51,1]],"Curl_http_readwrite_headers":[[4126,14,4126,42,1],[4286,37,4286,54,1]],"Curl_compareheader":[[1531,6,1531,22,1],[1538,9,1538,15,0],[1543,6,1543,10,1],[1555,9,1555,20,0],[45,37,45,49,0],[45,53,45,66,1],[39,32,39,53,0],[44,22,44,34,0],[44,22,44,34,1],[44,38,44,51,1]],"Curl_readwrite":[[1153,10,1153,44,0],[106,39,106,45,0],[106,49,106,68,1]],"Curl_checkheaders":[[109,33,109,37,0],[39,32,39,53,0],[39,32,39,53,1],[27,29,27,39,0]],"Curl_http_bodysend":[[2681,8,2681,30,1],[2692,8,2692,58,1],[2708,54,2708,72,0],[2710,10,2710,16,1],[2791,10,2791,33,0],[2791,37,2791,55,1]],"multi_wait":[[1216,33,1216,70,1]]},"library_calls":["curl_global_init","curl_easy_init","curl_easy_setopt","curl_easy_setopt","curl_mime_init","curl_mime_addpart","curl_mime_name","curl_mime_filename","curl_mime_encoder","curl_mime_filedata","curl_slist_append","curl_easy_setopt","curl_easy_setopt","curl_easy_perform","curl_easy_cleanup","curl_mime_free","curl_slist_free_all","curl_global_cleanup"]}
/**/


extern "C" int LLVMFuzzerTestOneInput_80(const uint8_t* f_data, size_t f_size) {
	if(f_size<=82) return 0;

	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(f_data, sizeof(uint8_t), f_size, input_file_ptr);
	fclose(input_file_ptr);

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_5, fuzz_str_sz_5, fdp);
	//fuzzer shim end}




    // Step 4: Create a FILE pointer for reading the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");

    // Step 5: Get the file descriptor for reading
    int in_fd = fuzz_fileno(in_file);

    // Step 6: Create a string for the input file name
    const char *in_file_name = "input_file";

    // Initialize curl
    curl_global_init(CURL_GLOBAL_ALL);

    // Create a CURL handle
    CURL *curl = curl_easy_init();

    // Step 4: Create a FILE pointer for writing the output data
    FILE *out_file = fopen("output_file", "wb");

    // Step 5: Get the file descriptor for writing
    int out_fd = fuzz_fileno(out_file);

    // Step 6: Create a string for the output file name
    const char *out_file_name = "output_file";

    // Step 1: An event that can be achieved using the curl_easy_option_by_id, curl_mime_encoder, curl_strnequal, curl_mime_filename, curl_strequal,
    //         curl_mvfprintf, and curl_easy_nextheader APIs is to perform a HTTP POST request with a file attachment.

    // Step 2: Here is an example of how these APIs can be used to achieve the above event:
    // Set the URL to perform the request to
    curl_easy_setopt(curl, CURLOPT_URL, "http://example.com");

    // Set the HTTP method to POST
    curl_easy_setopt(curl, CURLOPT_POST, 1L);

    // Set the file attachment
    curl_mime *mime = curl_mime_init(curl);
    curl_mimepart *part = curl_mime_addpart(mime);
    curl_mime_name(part, fuzz_str_1);
    curl_mime_filename(part, fuzz_str_2);
    curl_mime_encoder(part, fuzz_str_3);
    curl_mime_filedata(part, fuzz_str_4);

    // Set the headers for the request
    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, fuzz_str_5);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    // Set the output file
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, out_file);

    // Perform the request
    curl_easy_perform(curl);

    // Cleanup
    curl_easy_cleanup(curl);
    curl_mime_free(mime);
    curl_slist_free_all(headers);

    // Step 7: Release all allocated resources
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    curl_global_cleanup();

    assert_file_name_closed("output_file");
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}