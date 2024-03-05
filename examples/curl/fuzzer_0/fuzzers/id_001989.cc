#include "FDSan.h"
#include <mprintf.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 1989
//<Prompt> ["curl_multi_add_handle","curl_mime_init","curl_mime_addpart","curl_mfprintf","curl_mime_data","curl_formadd","curl_mime_data_cb"]
/*<Combination>: [CURLMcode curl_multi_add_handle(CURLM * multi_handle, CURL * curl_handle),
    curl_mime *curl_mime_init(CURL * easy),
    curl_mimepart *curl_mime_addpart(curl_mime * mime),
    int curl_mfprintf(FILE * fd, const char * format),
    CURLcode curl_mime_data(curl_mimepart * part, const char * data, size_t datasize),
    CURLFORMcode curl_formadd(struct curl_httppost ** httppost, struct curl_httppost ** last_post),
    CURLcode curl_mime_data_cb(curl_mimepart * part, curl_off_t datasize, curl_read_callback readfunc, curl_seek_callback seekfunc, curl_free_callback freefunc, void * arg)
*/
//<score> 1.4285715, nr_unique_branch: 1, p_cov: 0.71428573
//<Quality> {"diversity":12,"density":12,"covered_api":["curl_multi_add_handle","curl_mime_init","curl_mime_addpart","curl_mfprintf","curl_mime_data"],"uncovered_api":["curl_formadd","curl_mime_data_cb"],"unique_branches":{"curl_mime_data":[[1391,8,1391,16,1]]},"library_calls":["curl_multi_init","curl_easy_init","curl_multi_add_handle","curl_mime_init","curl_mime_addpart","curl_mfprintf","curl_mime_data","curl_easy_setopt","curl_easy_perform","curl_mime_free","curl_easy_cleanup","curl_multi_cleanup"]}
/**/



// API 1: Add a CURL* handle to a CURLM* multi_handle
extern "C" CURLMcode curl_multi_add_handle(CURLM *multi_handle, CURL *curl_handle);

// API 2: Initialize a curl_mime structure
extern "C" curl_mime *curl_mime_init(CURL *easy);

// API 3: Add a new part to a mime structure
extern "C" curl_mimepart *curl_mime_addpart(curl_mime *mime);

// API 4: Write formatted data to a file stream
extern "C" int curl_mfprintf(FILE *fd, const char *format, ...);

// API 5: Add data to a mime part
extern "C" CURLcode curl_mime_data(curl_mimepart *part, const char *data, size_t datasize);

// API 6: Add a new formdata part
extern "C" CURLFORMcode curl_formadd(struct curl_httppost **httppost, struct curl_httppost **last_post, ...);

// API 7: Add a data stream to a mime part
extern "C" CURLcode curl_mime_data_cb(curl_mimepart *part, curl_off_t datasize,
                                      curl_read_callback readfunc, curl_seek_callback seekfunc,
                                      curl_free_callback freefunc, void *arg);


extern "C" int LLVMFuzzerTestOneInput_90(const uint8_t *data, size_t size) {
	if(size<=0) return 0;

  // Create a CURLM* handle
  CURLM *multi_handle = curl_multi_init();

  // Create a CURL* handle
  CURL *curl_handle = curl_easy_init();

  // Add the CURL* handle to the CURLM* multi_handle
  curl_multi_add_handle(multi_handle, curl_handle);

  // Create a curl_mime structure
  curl_mime *mime = curl_mime_init(curl_handle);

  // Add a new part to the mime structure
  curl_mimepart *part = curl_mime_addpart(mime);

  // Create an input file stream from the input data
  FILE *in_file = fmemopen((void *)data, size, "rb");

  // Process the input file
  if (in_file) {
    char buffer[1024];
    while (fgets(buffer, sizeof(buffer), in_file) != nullptr) {
      // Write formatted data to the input file
      curl_mfprintf(in_file, "%s", buffer);

      // Add data to the mime part
      curl_mime_data(part, buffer, strlen(buffer));
    }

    // Close the input file stream
    assert_file_closed(&in_file);;
  }

  // Create an output file stream
  FILE *out_file = fopen("output_file", "wb");

  // Set the output file as the stdout for curl
  curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, out_file);

  // Perform the curl operation
  curl_easy_perform(curl_handle);

  // Close the output file stream
  assert_file_closed(&out_file);;

  // Release resources
  curl_mime_free(mime);
  curl_easy_cleanup(curl_handle);
  curl_multi_cleanup(multi_handle);

  assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}