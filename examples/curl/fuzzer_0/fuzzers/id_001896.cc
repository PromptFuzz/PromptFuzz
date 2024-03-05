#include "FDSan.h"
#include <mprintf.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 1896
//<Prompt> ["curl_multi_get_handles","curl_url_get","curl_url","curl_msnprintf","curl_strnequal"]
/*<Combination>: [CURL **curl_multi_get_handles(CURLM * multi_handle),
    CURLUcode curl_url_get(const CURLU * handle, CURLUPart what, char ** part, unsigned int flags),
    CURLU *curl_url(),
    int curl_msnprintf(char * buffer, size_t maxlength, const char * format),
    int curl_strnequal(const char * s1, const char * s2, size_t n)
*/
//<score> 1.6666667, nr_unique_branch: 2, p_cov: 1
//<Quality> {"diversity":9,"density":5,"covered_api":["curl_multi_get_handles","curl_url_get","curl_url","curl_msnprintf","curl_strnequal"],"uncovered_api":[],"unique_branches":{"curl_multi_get_handles":[[3796,6,3796,7,0],[3799,11,3799,12,1]]},"library_calls":["curl_multi_init","curl_multi_get_handles","curl_url","curl_url_get","curl_msnprintf","curl_strnequal","curl_multi_cleanup","curl_free","curl_url_cleanup"]}
/**/


extern "C" int LLVMFuzzerTestOneInput_89(const uint8_t *data, size_t size) {
	if(size<=0) return 0;

    // Step 4: Create a FILE * variable to read the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file == NULL) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Step 5: Get the file descriptor for reading
    int in_fd = fuzz_fileno(in_file);

    // Step 6: Create a string of file name for output
    const char *out_filename = "output_file";

    // Step 1: Perform desired event using curl APIs
    CURLM *multi_handle = curl_multi_init();
    CURL **easy_handles = curl_multi_get_handles(multi_handle);
    CURLU *url_handle = curl_url();
    char *part;
    CURLUcode url_code = curl_url_get(url_handle, CURLUPART_URL, &part, 0);
    char buffer[128];
    int snprintf_result = curl_msnprintf(buffer, sizeof(buffer), "%s", part);
    int strnequal_result = curl_strnequal(buffer, "test", 4);

    // Step 7: Release allocated resources
    curl_multi_cleanup(multi_handle);
    curl_free(easy_handles);
    curl_url_cleanup(url_handle);
    assert_file_closed(&in_file);;

    assert_file_name_closed("output_file");
	assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
}