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
//<ID> 1057
//<Prompt> ["curl_multi_add_handle","curl_mime_init","curl_mime_addpart","curl_formget","curl_mime_data","curl_formadd","curl_mime_data_cb"]
/*<Combination>: [CURLMcode curl_multi_add_handle(CURLM * multi_handle, CURL * curl_handle),
    curl_mime *curl_mime_init(CURL * easy),
    curl_mimepart *curl_mime_addpart(curl_mime * mime),
    int curl_formget(struct curl_httppost * form, void * arg, curl_formget_callback append),
    CURLcode curl_mime_data(curl_mimepart * part, const char * data, size_t datasize),
    CURLFORMcode curl_formadd(struct curl_httppost ** httppost, struct curl_httppost ** last_post),
    CURLcode curl_mime_data_cb(curl_mimepart * part, curl_off_t datasize, curl_read_callback readfunc, curl_seek_callback seekfunc, curl_free_callback freefunc, void * arg)
*/
//<score> 1.4285715, nr_unique_branch: 1, p_cov: 0.71428573
//<Quality> {"diversity":24,"density":24,"covered_api":["curl_multi_add_handle","curl_mime_init","curl_mime_addpart","curl_mime_data","curl_formadd"],"uncovered_api":["curl_formget","curl_mime_data_cb"],"unique_branches":{"FormAdd":[[571,10,571,28,1]]},"library_calls":["curl_easy_init","curl_multi_init","curl_mime_init","curl_mime_addpart","curl_mime_data","curl_formadd","curl_multi_add_handle","curl_multi_perform","curl_easy_cleanup","curl_multi_cleanup","curl_formfree","curl_mime_free"]}
/**/


// Event: Perform a multi-part form upload using libcurl
extern "C" int LLVMFuzzerTestOneInput_57(const uint8_t* f_data, size_t f_size) {
	if(f_size<=0) return 0;

	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(f_data, sizeof(uint8_t), f_size, input_file_ptr);
	fclose(input_file_ptr);

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    CURL *curl = curl_easy_init();
    if (!curl) {
        return 0;
    }

    // Create a multi handle
    CURLM *multi_handle = curl_multi_init();

    // Create a MIME handle
    curl_mime *mime = curl_mime_init(curl);

    // Create a part for the form data
    curl_mimepart *part = curl_mime_addpart(mime);

    // Set the form data
    curl_mime_data(part, (const char *)data, size);

    // Create a form handle
    struct curl_httppost *form = NULL;
    struct curl_httppost *last_post = NULL;

    // Convert the MIME handle to form
    CURLFORMcode form_code = curl_formadd(&form, &last_post,
                                          CURLFORM_COPYNAME, "data",
                                          CURLFORM_CONTENTSLENGTH, size,
                                          CURLFORM_CONTENTTYPE, "application/octet-stream",
                                          CURLFORM_BUFFER, "input_file",
                                          CURLFORM_BUFFERPTR, data,
                                          CURLFORM_BUFFERLENGTH, size,
                                          CURLFORM_END);
    if (form_code != CURL_FORMADD_OK) {
        curl_easy_cleanup(curl);
        curl_multi_cleanup(multi_handle);
        curl_formfree(form);
        curl_mime_free(mime);
        assert_file_name_closed("input_file");
	return 0;
    }

    // Add the form handle to the multi handle
    CURLMcode multi_code = curl_multi_add_handle(multi_handle, curl);
    if (multi_code != CURLM_OK) {
        curl_easy_cleanup(curl);
        curl_multi_cleanup(multi_handle);
        curl_formfree(form);
        curl_mime_free(mime);
        assert_file_name_closed("input_file");
	return 0;
    }

    // Perform the multi-part form upload
    int running_handles = 0;
    multi_code = curl_multi_perform(multi_handle, &running_handles);
    if (multi_code != CURLM_OK) {
        curl_easy_cleanup(curl);
        curl_multi_cleanup(multi_handle);
        curl_formfree(form);
        curl_mime_free(mime);
        assert_file_name_closed("input_file");
	return 0;
    }

    // Cleanup
    curl_easy_cleanup(curl);
    curl_multi_cleanup(multi_handle);
    curl_formfree(form);
    curl_mime_free(mime);

    assert_file_name_closed("input_file");
	return 0;
}