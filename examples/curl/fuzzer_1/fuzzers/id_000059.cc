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
//<ID> 1069
//<Prompt> ["curl_multi_add_handle","curl_mime_init","curl_mime_addpart","curl_mime_data","curl_formadd","curl_mime_data_cb"]
/*<Combination>: [CURLMcode curl_multi_add_handle(CURLM * multi_handle, CURL * curl_handle),
    curl_mime *curl_mime_init(CURL * easy),
    curl_mimepart *curl_mime_addpart(curl_mime * mime),
    CURLcode curl_mime_data(curl_mimepart * part, const char * data, size_t datasize),
    CURLFORMcode curl_formadd(struct curl_httppost ** httppost, struct curl_httppost ** last_post),
    CURLcode curl_mime_data_cb(curl_mimepart * part, curl_off_t datasize, curl_read_callback readfunc, curl_seek_callback seekfunc, curl_free_callback freefunc, void * arg)
*/
//<score> 10, nr_unique_branch: 10, p_cov: 1
//<Quality> {"diversity":11,"density":10,"covered_api":["curl_multi_add_handle","curl_mime_init","curl_mime_addpart","curl_mime_data","curl_formadd","curl_mime_data_cb"],"uncovered_api":[],"unique_branches":{"FormAdd":[[236,9,236,40,1],[515,5,515,12,0],[521,6,521,37,0],[525,27,525,38,0],[525,27,525,38,1],[526,10,526,25,1],[530,10,530,26,1],[534,10,534,32,1],[538,10,538,33,1],[545,6,545,37,1]]},"library_calls":["curl_multi_init","curl_easy_init","curl_mime_init","curl_mime_addpart","curl_multi_add_handle","curl_mime_data","curl_mime_data_cb","curl_formadd","curl_multi_cleanup","curl_easy_cleanup","curl_mime_free"]}
/**/

extern "C" int LLVMFuzzerTestOneInput_59(const uint8_t* f_data, size_t f_size) {
	if(f_size<=7) return 0;

	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(f_data, sizeof(uint8_t), f_size, input_file_ptr);
	fclose(input_file_ptr);

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
	//fuzzer shim end}




  CURLM *multi_handle = curl_multi_init();
  CURL *curl_handle = curl_easy_init();
  curl_mime *mime = curl_mime_init(curl_handle);
  curl_mimepart *part = curl_mime_addpart(mime);

  FILE *in_file = fmemopen((void *)data, size, "rb");
  FILE *out_file = fopen("output_file", "wb");

  int in_fd = fuzz_fileno(in_file);
  int out_fd = fuzz_fileno(out_file);

  char *input_file = "input_file";
  char *output_file = "output_file";

  CURLMcode multi_add_handle_result = curl_multi_add_handle(multi_handle, curl_handle);
  curl_mime_data(part, fuzz_str_1, static_cast<size_t>(fuzz_str_sz_1));
  CURLcode mime_data_cb_result = curl_mime_data_cb(part, 10, NULL, NULL, NULL, NULL);
  CURLFORMcode formadd_result = curl_formadd(NULL, NULL);

  assert_file_closed(&in_file);;
  assert_file_closed(&out_file);;
  curl_multi_cleanup(multi_handle);
  curl_easy_cleanup(curl_handle);
  curl_mime_free(mime);

  assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}