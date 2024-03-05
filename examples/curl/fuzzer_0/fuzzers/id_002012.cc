#include "FDSan.h"
#include <mprintf.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 2012
//<Prompt> ["curl_multi_add_handle","curl_mime_init","curl_mime_addpart","curl_mfprintf","curl_mime_data","curl_formadd"]
/*<Combination>: [CURLMcode curl_multi_add_handle(CURLM * multi_handle, CURL * curl_handle),
    curl_mime *curl_mime_init(CURL * easy),
    curl_mimepart *curl_mime_addpart(curl_mime * mime),
    int curl_mfprintf(FILE * fd, const char * format),
    CURLcode curl_mime_data(curl_mimepart * part, const char * data, size_t datasize),
    CURLFORMcode curl_formadd(struct curl_httppost ** httppost, struct curl_httppost ** last_post)
*/
//<score> 1.2280701, nr_unique_branch: 1, p_cov: 0.8333333
//<Quality> {"diversity":19,"density":14,"covered_api":["curl_multi_add_handle","curl_mime_init","curl_mime_addpart","curl_mfprintf","curl_mime_data"],"uncovered_api":["curl_formadd"],"unique_branches":{"curl_multi_add_handle":[[536,6,536,17,0]]},"library_calls":["curl_global_init","curl_multi_init","curl_easy_init","curl_mime_init","curl_mime_addpart","curl_mime_name","curl_mime_filedata","curl_mime_addpart","curl_mime_name","curl_mime_data","curl_mfprintf","curl_mfprintf","curl_mfprintf","curl_multi_add_handle","curl_multi_perform","curl_mime_free","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup"]}
/**/

// Event: Upload a file using multipart/form-data

// Helper function to write data to a file
size_t  id_002012_write_data(void *ptr, size_t size, size_t nmemb, FILE *stream) {
    size_t written = fwrite(ptr, size, nmemb, stream);
    return written;
}

extern "C" int LLVMFuzzerTestOneInput_91(const uint8_t *data, size_t size) {
	if(size<=0) return 0;

	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);

    // Initialize libcurl
    curl_global_init(CURL_GLOBAL_DEFAULT);
    
    // Create a CURLM handle for performing multiple transfers
    CURLM *multi_handle = curl_multi_init();
    
    // Create a CURL handle for performing a single transfer
    CURL *curl_handle = curl_easy_init();
    
    // Create a MIME handle for creating the multipart/form-data request
    curl_mime *mime = curl_mime_init(curl_handle);
    
    // Set the form data file
    curl_mimepart *part = curl_mime_addpart(mime);
    curl_mime_name(part, "file");
    curl_mime_filedata(part, "input_file");
    
    // Set the form data field
    curl_mimepart *field = curl_mime_addpart(mime);
    curl_mime_name(field, "field");
    curl_mime_data(field, "value", strlen("value"));
    
    // Create a temporary file to store the request body
    FILE *request_file = tmpfile();
    
    // Write the request body to the temporary file
    curl_mfprintf(request_file, "Content-Type: multipart/form-data");
    curl_mfprintf(request_file, "Content-Length: %ld", size);
    curl_mfprintf(request_file, "\r\n");
     id_002012_write_data((void *)data, size, 1, request_file);
    fseek(request_file, 0, SEEK_SET);
    
    // Read the request body from the temporary file
    char buffer[1024];
    while (!feof(request_file)) {
        size_t bytesRead = fread(buffer, 1, sizeof(buffer), request_file);
        
        // Perform the transfer
        CURLMcode res = curl_multi_add_handle(multi_handle, curl_handle);
        if (res != CURLM_OK) {
            // Handle error
            break;
        }
        
        // Wait for the transfer to complete
        int running_handles = 0;
        curl_multi_perform(multi_handle, &running_handles);
    }
    
    // Cleanup
    assert_file_closed(&request_file);;
    curl_mime_free(mime);
    curl_easy_cleanup(curl_handle);
    curl_multi_cleanup(multi_handle);
    curl_global_cleanup();
    
    assert_file_name_closed("input_file");
	return 0;
}