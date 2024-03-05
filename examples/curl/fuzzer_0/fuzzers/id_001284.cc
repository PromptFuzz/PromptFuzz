#include "FDSan.h"
#include <mprintf.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 1284
//<Prompt> ["curl_formget","curl_url_strerror","curl_share_init","curl_mime_type","curl_easy_option_by_id","curl_multi_remove_handle"]
/*<Combination>: [int curl_formget(struct curl_httppost * form, void * arg, curl_formget_callback append),
    const char *curl_url_strerror(CURLUcode ),
    CURLSH *curl_share_init(),
    CURLcode curl_mime_type(curl_mimepart * part, const char * mimetype),
    const struct curl_easyoption *curl_easy_option_by_id(CURLoption id),
    CURLMcode curl_multi_remove_handle(CURLM * multi_handle, CURL * curl_handle)
*/
//<score> 15, nr_unique_branch: 44, p_cov: 0.33333334
//<Quality> {"diversity":13,"density":13,"covered_api":["curl_formget","curl_multi_remove_handle"],"uncovered_api":["curl_url_strerror","curl_share_init","curl_mime_type","curl_easy_option_by_id"],"unique_branches":{"curl_dbg_fopen":[[421,6,421,12,0]],"escape_string":[[315,7,315,11,1]],"mime_open_file":[[710,6,710,14,0],[710,6,710,14,1],[713,10,713,18,0]],"curl_formget":[[720,6,720,13,0],[724,9,724,16,0],[728,8,728,14,0],[728,8,728,14,1],[731,8,731,30,1],[731,34,731,69,1]],"add_content_type":[[1703,31,1703,39,1],[1704,31,1704,39,1]],"curl_dbg_fclose":[[444,6,444,12,0]],"mime_file_read":[[721,6,721,13,1],[724,6,724,26,1]],"Curl_getformdata":[[839,20,839,24,1],[858,33,858,37,1],[896,14,896,21,0],[896,25,896,58,1],[921,10,921,17,0],[921,21,921,39,1],[928,6,928,12,1]],"Curl_mime_prepare_headers":[[1783,6,1783,15,1],[1785,6,1785,14,0],[1812,11,1812,22,0],[1812,26,1812,35,1],[1835,18,1835,32,0],[1837,12,1837,21,1],[1847,36,1847,44,0],[1848,36,1848,44,0],[1849,36,1849,44,0]],"read_part_content":[[805,6,805,39,1],[818,5,818,23,0],[819,10,819,18,0],[819,10,819,18,1],[819,22,819,36,0],[819,22,819,36,1],[824,12,824,43,0],[825,14,825,22,1]],"readback_part":[[970,12,970,39,0],[970,43,970,51,0],[970,43,970,51,1],[979,16,979,23,1]]},"library_calls":["curl_easy_init","curl_easy_setopt","curl_easy_setopt","curl_formadd","curl_formget","curl_multi_init","curl_multi_add_handle","curl_multi_perform","curl_multi_remove_handle","curl_multi_cleanup","curl_easy_cleanup","curl_formfree"]}
/**/

// Callback function for curl_formget
static size_t  id_001284_append_callback(void* ptr, const char* data, size_t size) {
    FILE* file = (FILE*)ptr;
    return fwrite(data, 1, size, file);
}

extern "C" int LLVMFuzzerTestOneInput_66(const uint8_t* data, size_t size) {
	if(size<=0) return 0;

	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);

    // Create input file stream
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Create output file
    FILE* out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Get file descriptor for reading and writing
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);

    // Initialize libcurl
    CURL* curl = curl_easy_init();
    if (!curl) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }

    // Set curl options
    curl_easy_setopt(curl, CURLOPT_READDATA, in_file);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, out_file);

    // Create HTTP form
    struct curl_httppost* form = nullptr;
    struct curl_httppost* last_post = nullptr;
    curl_formadd(&form, &last_post, CURLFORM_COPYNAME, "file", CURLFORM_FILE, "input_file", CURLFORM_END);

    // Callback function for curl_formget
    curl_formget(form, out_file,  id_001284_append_callback);

    // Create CURLM handle for multi handle operations
    CURLM* multi_handle = curl_multi_init();
    if (!multi_handle) {
        curl_easy_cleanup(curl);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_name_closed("input_file");
	assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }

    // Add curl handle to multi handle
    curl_multi_add_handle(multi_handle, curl);

    // Perform the multi handle operation
    int running_handles;
    curl_multi_perform(multi_handle, &running_handles);

    // Remove curl handle from multi handle
    curl_multi_remove_handle(multi_handle, curl);

    // Cleanup multi handle and curl handle
    curl_multi_cleanup(multi_handle);
    curl_easy_cleanup(curl);

    // Clean up HTTP form
    curl_formfree(form);

    // Close input and output file streams
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;

    assert_file_name_closed("input_file");
	assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}