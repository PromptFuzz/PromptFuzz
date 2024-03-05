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
//<ID> 1480
//<Prompt> ["curl_strnequal","curl_global_init_mem","curl_multi_perform","curl_easy_nextheader","curl_easy_init","curl_easy_setopt"]
/*<Combination>: [int curl_strnequal(const char * s1, const char * s2, size_t n),
    CURLcode curl_global_init_mem(long flags, curl_malloc_callback m, curl_free_callback f, curl_realloc_callback r, curl_strdup_callback s, curl_calloc_callback c),
    CURLMcode curl_multi_perform(CURLM * multi_handle, int * running_handles),
    struct curl_header *curl_easy_nextheader(CURL * easy, unsigned int origin, int request, struct curl_header * prev),
    CURL *curl_easy_init(),
    CURLcode curl_easy_setopt(CURL * curl, CURLoption option)
*/
//<score> 4.1666665, nr_unique_branch: 5, p_cov: 0.8333333
//<Quality> {"diversity":12,"density":10,"covered_api":["curl_global_init_mem","curl_multi_perform","curl_easy_nextheader","curl_easy_init","curl_easy_setopt"],"uncovered_api":["curl_strnequal"],"unique_branches":{"curl_easy_nextheader":[[139,6,139,36,1],[141,6,141,19,1],[144,6,144,10,1],[154,6,154,10,1],[164,6,164,11,0]]},"library_calls":["curl_global_init_mem","curl_multi_init","curl_easy_init","curl_easy_setopt","curl_easy_setopt","curl_multi_add_handle","curl_multi_perform","curl_easy_nextheader","curl_multi_remove_handle","curl_multi_cleanup","curl_easy_cleanup","curl_global_cleanup"]}
/**/

extern "C" int LLVMFuzzerTestOneInput_76(const uint8_t* f_data, size_t f_size) {
	if(f_size<=8) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
	//fuzzer shim end}




    FILE *in_file = fmemopen((void *)data, size, "rb");
    FILE *out_file = fopen("output_file", "wb");

    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);

    // Initialize curl
    curl_global_init_mem(CURL_GLOBAL_DEFAULT, malloc, free, realloc, strdup, calloc);

    // Create multi-handle
    CURLM *multi_handle = curl_multi_init();

    // Create easy handle
    CURL *easy_handle = curl_easy_init();

    // Set input file descriptor for easy handle
    curl_easy_setopt(easy_handle, CURLOPT_READDATA, &in_fd);

    // Set output file descriptor for easy handle
    curl_easy_setopt(easy_handle, CURLOPT_WRITEDATA, &out_fd);

    // Add easy handle to multi-handle
    curl_multi_add_handle(multi_handle, easy_handle);

    // Perform the transfer
    int running_handles;
    curl_multi_perform(multi_handle, &running_handles);

    // Get next header from easy handle
    struct curl_header *header = curl_easy_nextheader(easy_handle, fuzz_uint32_t_1, fuzz_int32_t_2, NULL);

    // Process the header
    if (header != NULL) {
        // Do something with header
        printf("Header Name: %s\n", header->name);
        printf("Header Value: %s\n", header->value);
    }

    // Cleanup
    curl_multi_remove_handle(multi_handle, easy_handle);
    curl_multi_cleanup(multi_handle);
    curl_easy_cleanup(easy_handle);
    curl_global_cleanup();

    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;

    assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}