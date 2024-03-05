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
//<ID> 1118
//<Prompt> ["curl_mime_init","curl_mime_addpart","curl_mfprintf","curl_mime_data","curl_strequal","curl_formadd","curl_mime_data_cb"]
/*<Combination>: [curl_mime *curl_mime_init(CURL * easy),
    curl_mimepart *curl_mime_addpart(curl_mime * mime),
    int curl_mfprintf(FILE * fd, const char * format),
    CURLcode curl_mime_data(curl_mimepart * part, const char * data, size_t datasize),
    int curl_strequal(const char * s1, const char * s2),
    CURLFORMcode curl_formadd(struct curl_httppost ** httppost, struct curl_httppost ** last_post),
    CURLcode curl_mime_data_cb(curl_mimepart * part, curl_off_t datasize, curl_read_callback readfunc, curl_seek_callback seekfunc, curl_free_callback freefunc, void * arg)
*/
//<score> 0.72527474, nr_unique_branch: 1, p_cov: 0.42857143
//<Quality> {"diversity":13,"density":11,"covered_api":["curl_mime_init","curl_mime_addpart","curl_mime_data"],"uncovered_api":["curl_mfprintf","curl_strequal","curl_formadd","curl_mime_data_cb"],"unique_branches":{"curl_mime_data":[[1382,8,1382,40,0]]},"library_calls":["curl_global_init","curl_mime_init","curl_mime_addpart","curl_mime_type","curl_mime_data","curl_easy_init","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_perform","curl_easy_cleanup","curl_mime_free","curl_global_cleanup"]}
/**/


extern "C" int LLVMFuzzerTestOneInput_61(const uint8_t* f_data, size_t f_size) {
	if(f_size<=26) return 0;

	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(f_data, sizeof(uint8_t), f_size, input_file_ptr);
	fclose(input_file_ptr);

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
	//fuzzer shim end}




    // Step 4: Create a file pointer for reading input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    
    // Step 5: Get the file descriptor for reading
    int in_fd = fuzz_fileno(in_file);
    
    // Step 6: Set the input file name
    const char *input_file = "input_file";
    
    // Step 7: Create a file pointer for writing output data
    FILE *out_file = fopen("output_file", "wb");
    
    // Step 8: Get the file descriptor for writing
    int out_fd = fuzz_fileno(out_file);
    
    // Initialize curl
    curl_global_init(CURL_GLOBAL_DEFAULT);
    
    // Create a MIME object
    curl_mime *mime = curl_mime_init(NULL);
    
    // Add a part to the MIME object
    curl_mimepart *part = curl_mime_addpart(mime);
    
    // Set the content type of the part
    curl_mime_type(part, fuzz_str_1);
    
    // Set the data of the part from the input file
    curl_mime_data(part, fuzz_str_2, static_cast<size_t>(fuzz_str_sz_2));
    
    // Create a new handle
    CURL *handle = curl_easy_init();
    
    // Set the handle options
    curl_easy_setopt(handle, CURLOPT_MIMEPOST, mime);
    curl_easy_setopt(handle, CURLOPT_URL, "https://example.com");
    curl_easy_setopt(handle, CURLOPT_WRITEDATA, out_file);
    
    // Perform the request
    curl_easy_perform(handle);
    
    // Cleanup and release resources
    curl_easy_cleanup(handle);
    curl_mime_free(mime);
    curl_global_cleanup();
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}