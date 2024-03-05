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
//<ID> 1042
//<Prompt> ["curl_multi_add_handle","curl_mime_init","curl_mime_addpart","curl_mime_data","curl_formadd","curl_mime_data_cb"]
/*<Combination>: [CURLMcode curl_multi_add_handle(CURLM * multi_handle, CURL * curl_handle),
    curl_mime *curl_mime_init(CURL * easy),
    curl_mimepart *curl_mime_addpart(curl_mime * mime),
    CURLcode curl_mime_data(curl_mimepart * part, const char * data, size_t datasize),
    CURLFORMcode curl_formadd(struct curl_httppost ** httppost, struct curl_httppost ** last_post),
    CURLcode curl_mime_data_cb(curl_mimepart * part, curl_off_t datasize, curl_read_callback readfunc, curl_seek_callback seekfunc, curl_free_callback freefunc, void * arg)
*/
//<score> 8, nr_unique_branch: 7, p_cov: 1
//<Quality> {"diversity":14,"density":14,"covered_api":["curl_multi_add_handle","curl_mime_init","curl_mime_addpart","curl_mime_data","curl_formadd","curl_mime_data_cb"],"uncovered_api":[],"unique_branches":{"FormAdd":[[331,5,331,30,0],[332,10,332,72,1],[335,32,335,43,1],[337,12,337,20,0],[339,14,339,34,1],[563,12,563,45,0],[564,12,564,48,1]]},"library_calls":["curl_multi_init","curl_easy_init","curl_multi_add_handle","curl_mime_init","curl_mime_addpart","curl_mime_data","curl_formadd","curl_mime_data_cb","curl_multi_perform","curl_formfree","curl_multi_remove_handle","curl_multi_cleanup","curl_mime_free","curl_easy_cleanup"]}
/**/

extern "C" int LLVMFuzzerTestOneInput_56(const uint8_t* f_data, size_t f_size) {
	if(f_size<=0) return 0;

	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(f_data, sizeof(uint8_t), f_size, input_file_ptr);
	fclose(input_file_ptr);

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




  // Create a CURLM handle
  CURLM *multi_handle = curl_multi_init();
  
  // Create a CURL handle
  CURL *curl_handle = curl_easy_init();
  
  // Add the CURL handle to the CURLM handle
  curl_multi_add_handle(multi_handle, curl_handle);
  
  // Create a MIME handle
  curl_mime *mime_handle = curl_mime_init(curl_handle);
  
  // Create a MIME part
  curl_mimepart *part = curl_mime_addpart(mime_handle);
  
  // Set the data of the MIME part
  curl_mime_data(part, reinterpret_cast<const char *>(data), size);
  
  // Create a form handle
  struct curl_httppost *form_handle = nullptr;
  struct curl_httppost *last_post = nullptr;
  
  // Add the MIME data to the form handle
  curl_formadd(&form_handle, &last_post, CURLFORM_COPYNAME, "data", CURLFORM_FILECONTENT, "input_file", CURLFORM_END);
  
  // Set the form data of the MIME part
  curl_mime_data_cb(part, size, nullptr, nullptr, nullptr, reinterpret_cast<void *>(form_handle));
  
  // Perform the multi request
  int running_handles;
  curl_multi_perform(multi_handle, &running_handles);
  
  // Cleanup
  curl_formfree(form_handle);
  curl_multi_remove_handle(multi_handle, curl_handle);
  curl_multi_cleanup(multi_handle);
  curl_mime_free(mime_handle);
  curl_easy_cleanup(curl_handle);
  
  assert_file_name_closed("input_file");
	return 0;
}