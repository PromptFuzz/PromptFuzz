#include "FDSan.h"
#include <mprintf.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 1101
//<Prompt> ["curl_multi_add_handle","curl_mime_init","curl_mime_addpart","curl_mfprintf","curl_mime_data","curl_formadd","curl_mime_data_cb"]
/*<Combination>: [CURLMcode curl_multi_add_handle(CURLM * multi_handle, CURL * curl_handle),
    curl_mime *curl_mime_init(CURL * easy),
    curl_mimepart *curl_mime_addpart(curl_mime * mime),
    int curl_mfprintf(FILE * fd, const char * format),
    CURLcode curl_mime_data(curl_mimepart * part, const char * data, size_t datasize),
    CURLFORMcode curl_formadd(struct curl_httppost ** httppost, struct curl_httppost ** last_post),
    CURLcode curl_mime_data_cb(curl_mimepart * part, curl_off_t datasize, curl_read_callback readfunc, curl_seek_callback seekfunc, curl_free_callback freefunc, void * arg)
*/
//<score> 1.2244898, nr_unique_branch: 1, p_cov: 0.85714287
//<Quality> {"diversity":14,"density":10,"covered_api":["curl_mime_init","curl_mime_addpart","curl_mfprintf","curl_mime_data","curl_formadd","curl_mime_data_cb"],"uncovered_api":["curl_multi_add_handle"],"unique_branches":{"FormAdd":[[572,19,572,50,0]]},"library_calls":["curl_global_init","curl_multi_init","curl_easy_init","curl_mime_init","curl_mime_addpart","curl_mfprintf","curl_mime_data","curl_formadd","curl_mime_data_cb","curl_formfree","curl_mime_free","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup"]}
/**/


extern "C" int LLVMFuzzerTestOneInput_60(const uint8_t *data, size_t size) {
	if(size<=0) return 0;

    // Step 4: Create the input and output files
    FILE *in_file = fmemopen((void *)data, size, "rb");
    FILE *out_file = fopen("output_file", "wb");

    // Step 5: Get file descriptors
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);

    // Step 1: Initialize curl and multi handle
    curl_global_init(CURL_GLOBAL_ALL);
    CURLM *multi_handle = curl_multi_init();

    // Step 2: Create mime and mime part
    CURL *curl_handle = curl_easy_init();
    curl_mime *mime = curl_mime_init(curl_handle);
    curl_mimepart *part = curl_mime_addpart(mime);

    // Step 3: Read and write data using curl APIs
    char buffer[4096];
    size_t bytes_read;
    while ((bytes_read = fread(buffer, 1, sizeof(buffer), in_file)) > 0) {
        curl_mfprintf(out_file, "%.*s", bytes_read, buffer);

        // Use curl_mime_data to set data for the mime part
        curl_mime_data(part, buffer, bytes_read);
    }

    // Step 6: Add mime part to form
    struct curl_httppost *form = NULL;
    struct curl_httppost *last_post = NULL;
    curl_formadd(&form, &last_post,
                 CURLFORM_COPYNAME, "data",
                 CURLFORM_BUFFER, "data.txt",
                 CURLFORM_BUFFERPTR, buffer,
                 CURLFORM_BUFFERLENGTH, bytes_read,
                 CURLFORM_END);

    // Step 7: Set mime part data callback
    curl_mime_data_cb(part, bytes_read, [](char *buffer, size_t size, size_t n, void *arg) -> size_t {
        FILE *file = (FILE *)arg;
        size_t bytes_read = fread(buffer, size, n, file);
        return bytes_read;
    }, nullptr, nullptr, in_file);

    // Clean up and release resources
    curl_formfree(form);
    curl_mime_free(mime);
    curl_easy_cleanup(curl_handle);
    curl_multi_cleanup(multi_handle);
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