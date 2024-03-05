#include "FDSan.h"
#include <mprintf.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 2169
//<Prompt> ["curl_mvsprintf","curl_global_trace","curl_getdate","curl_easy_reset","curl_url_set"]
/*<Combination>: [int curl_mvsprintf(char * buffer, const char * format, __va_list_tag * args),
    CURLcode curl_global_trace(const char * config),
    time_t curl_getdate(const char * p, const time_t * unused),
    void curl_easy_reset(CURL * curl),
    CURLUcode curl_url_set(CURLU * handle, CURLUPart what, const char * part, unsigned int flags)
*/
//<score> 0.74285716, nr_unique_branch: 1, p_cov: 0.4
//<Quality> {"diversity":14,"density":13,"covered_api":["curl_global_trace","curl_easy_reset"],"uncovered_api":["curl_mvsprintf","curl_getdate","curl_url_set"],"unique_branches":{"Curl_vsetopt":[[1726,3,1726,29,0]]},"library_calls":["curl_easy_init","curl_global_trace","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_perform","curl_easy_reset","curl_easy_cleanup"]}
/**/


// Step 1: Define the event we want to achieve
void  id_002169_curl_output_event(const char* output_data) {
  // Process the output data as needed
  printf("Output data: %s\n", output_data);
}

extern "C" int LLVMFuzzerTestOneInput_101(const uint8_t* data, size_t size) {
	if(size<=0) return 0;

	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);

  // Step 4: Create a FILE pointer for reading the input data
  FILE* in_file = fmemopen((void*)data, size, "rb");
  if (!in_file) {
    assert_file_closed(&in_file);
	return 0;
  }

  // Step 5: Get the file descriptor for reading
  int in_fd = fuzz_fileno(in_file);

  // Step 6: Create a string for the input file name
  const char* input_file = "input_file";

  // Initialize curl
  CURL* curl = curl_easy_init();
  if (!curl) {
    assert_file_closed(&in_file);;
    assert_file_name_closed("input_file");
	assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
  }

  // Step 1: Call curl_global_trace to set trace configuration
  curl_global_trace("config");

  // Step 2: Define a callback function to handle the output data
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, [](void* ptr, size_t size, size_t nmemb, void* userdata) -> size_t {
    // Step 3: Call the event function with the output data
     id_002169_curl_output_event((const char*)ptr);
    return size * nmemb;
  });

  // Step 4: Set the input file for upload
  curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
  curl_easy_setopt(curl, CURLOPT_READDATA, in_file);

  // Step 5: Set the input file descriptor for upload
  curl_easy_setopt(curl, CURLOPT_READFUNCTION, nullptr);
  curl_easy_setopt(curl, CURLOPT_IOCTLFUNCTION, nullptr);
  curl_easy_setopt(curl, CURLOPT_READDATA, (void*)in_fd);

  // Step 6: Set the input file name for upload
  curl_easy_setopt(curl, CURLOPT_READFUNCTION, nullptr);
  curl_easy_setopt(curl, CURLOPT_IOCTLFUNCTION, nullptr);
  curl_easy_setopt(curl, CURLOPT_READDATA, (void*)input_file);

  // Perform the curl request
  CURLcode res = curl_easy_perform(curl);

  // Clean up
  assert_file_closed(&in_file);;
  curl_easy_reset(curl);
  curl_easy_cleanup(curl);

  assert_file_name_closed("input_file");
	assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
}