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
//<ID> 1405
//<Prompt> ["curl_multi_init","curl_multi_add_handle","curl_slist_append","curl_pushheader_bynum","curl_global_init_mem","curl_multi_perform","curl_multi_remove_handle"]
/*<Combination>: [CURLM *curl_multi_init(),
    CURLMcode curl_multi_add_handle(CURLM * multi_handle, CURL * curl_handle),
    struct curl_slist *curl_slist_append(struct curl_slist * list, const char * data),
    char *curl_pushheader_bynum(struct curl_pushheaders * h, size_t num),
    CURLcode curl_global_init_mem(long flags, curl_malloc_callback m, curl_free_callback f, curl_realloc_callback r, curl_strdup_callback s, curl_calloc_callback c),
    CURLMcode curl_multi_perform(CURLM * multi_handle, int * running_handles),
    CURLMcode curl_multi_remove_handle(CURLM * multi_handle, CURL * curl_handle)
*/
//<score> 1.1428572, nr_unique_branch: 1, p_cov: 0.5714286
//<Quality> {"diversity":13,"density":13,"covered_api":["curl_multi_init","curl_multi_add_handle","curl_slist_append","curl_multi_remove_handle"],"uncovered_api":["curl_pushheader_bynum","curl_global_init_mem","curl_multi_perform"],"unique_branches":{"slist_get_last":[[46,9,46,19,0]]},"library_calls":["curl_multi_init","curl_easy_init","curl_easy_setopt","curl_multi_add_handle","curl_slist_append","curl_slist_append","curl_slist_append","curl_easy_setopt","curl_easy_setopt","curl_multi_remove_handle","curl_easy_cleanup","curl_slist_free_all","curl_multi_cleanup"]}
/**/

extern "C" int LLVMFuzzerTestOneInput_73(const uint8_t* f_data, size_t f_size) {
	if(f_size<=96) return 0;

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
	//fuzzer shim end}




    // Step 4: Create a file stream for input data
    FILE* in_file = fmemopen((void*)data, size, "rb");
    
    // Step 6: Define file names
    const char* input_file = "input_file";
    const char* output_file = "output_file";
    
    // Step 1: Initialize curl multi handle
    CURLM* multi_handle = curl_multi_init();
    
    // Step 1: Check if multi handle was successfully initialized
    if (!multi_handle) {
        assert_file_closed(&in_file);;
        assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Step 5: Get the file descriptor of the input file
    int in_fd = fuzz_fileno(in_file);
    
    // Step 5: Create a file stream for output data
    FILE* out_file = fopen(output_file, "wb");
    
    // Step 5: Get the file descriptor of the output file
    int out_fd = fuzz_fileno(out_file);
    
    // Step 1: Add the input file to the multi handle
    CURL* curl_handle = curl_easy_init();
    curl_easy_setopt(curl_handle, CURLOPT_READDATA, in_file);
    curl_multi_add_handle(multi_handle, curl_handle);
    
    // Step 2: Create a curl header list and append headers
    struct curl_slist* header_list = NULL;
    header_list = curl_slist_append(header_list, fuzz_str_1);
    header_list = curl_slist_append(header_list, fuzz_str_2);
    header_list = curl_slist_append(header_list, fuzz_str_3);
    
    // Step 3: Set the headers for the curl handle
    curl_easy_setopt(curl_handle, CURLOPT_HTTPHEADER, header_list);
    
    // Step 1: Set the output file for the curl handle
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, out_file);
    
    // Step 7: Release the file streams and header list
    curl_multi_remove_handle(multi_handle, curl_handle);
    curl_easy_cleanup(curl_handle);
    curl_slist_free_all(header_list);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Step 7: Cleanup the curl multi handle
    curl_multi_cleanup(multi_handle);
    
    assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}