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
//<ID> 1041
//<Prompt> ["curl_multi_add_handle","curl_mime_init","curl_mime_addpart","curl_mime_data","curl_formadd","curl_mime_data_cb"]
/*<Combination>: [CURLMcode curl_multi_add_handle(CURLM * multi_handle, CURL * curl_handle),
    curl_mime *curl_mime_init(CURL * easy),
    curl_mimepart *curl_mime_addpart(curl_mime * mime),
    CURLcode curl_mime_data(curl_mimepart * part, const char * data, size_t datasize),
    CURLFORMcode curl_formadd(struct curl_httppost ** httppost, struct curl_httppost ** last_post),
    CURLcode curl_mime_data_cb(curl_mimepart * part, curl_off_t datasize, curl_read_callback readfunc, curl_seek_callback seekfunc, curl_free_callback freefunc, void * arg)
*/
//<score> 10, nr_unique_branch: 9, p_cov: 1
//<Quality> {"diversity":14,"density":14,"covered_api":["curl_multi_add_handle","curl_mime_init","curl_mime_addpart","curl_mime_data","curl_formadd","curl_mime_data_cb"],"uncovered_api":[],"unique_branches":{"FormAdd":[[553,27,553,39,0],[553,44,553,49,0],[649,8,649,39,0],[654,23,654,34,0],[654,23,654,34,1],[655,12,655,27,1],[659,12,659,28,1],[663,12,663,34,0],[667,12,667,35,1]]},"library_calls":["curl_multi_init","curl_easy_init","curl_multi_add_handle","curl_mime_init","curl_mime_addpart","curl_mime_data","curl_mime_data_cb","curl_formadd","curl_easy_setopt","curl_easy_perform","curl_formfree","curl_mime_free","curl_easy_cleanup","curl_multi_cleanup"]}
/**/


// Callback function for reading data from file stream
size_t  id_000055_read_callback(char *buffer, size_t size, size_t nmemb, void *userdata)
{
    FILE *file = (FILE *)userdata;
    return fread(buffer, size, nmemb, file);
}

extern "C" int LLVMFuzzerTestOneInput_55(const uint8_t* f_data, size_t f_size)
{
	if(f_size<=0) return 0;

	
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

    // Create a MIME part handle
    curl_mimepart *part_handle = curl_mime_addpart(mime_handle);

    // Set the data of the MIME part handle
    curl_mime_data(part_handle, (const char*)data, size);

    // Set the read callback for the MIME part handle
    curl_mime_data_cb(part_handle, size,  id_000055_read_callback, NULL, NULL, NULL);

    // Create a form handle
    struct curl_httppost *form_handle = NULL;
    struct curl_httppost *last_post = NULL;

    // Add the MIME part to the form handle
    curl_formadd(&form_handle, &last_post, CURLFORM_COPYNAME, "part", CURLFORM_CONTENTTYPE, "application/octet-stream", CURLFORM_END);

    // Set the form data for the CURL handle
    curl_easy_setopt(curl_handle, CURLOPT_HTTPPOST, form_handle);

    // Perform the CURL request
    curl_easy_perform(curl_handle);

    // Cleanup
    curl_formfree(form_handle);
    curl_mime_free(mime_handle);
    curl_easy_cleanup(curl_handle);
    curl_multi_cleanup(multi_handle);

    return 0;
}