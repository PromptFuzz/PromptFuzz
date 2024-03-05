#include "FDSan.h"
#include <mprintf.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
//<ID> 250
//<Prompt> ["curl_easy_unescape","curl_multi_fdset","curl_ws_meta","curl_ws_recv","curl_multi_info_read"]
/*<Combination>: [char *curl_easy_unescape(CURL * handle, const char * string, int length, int * outlength),
    CURLMcode curl_multi_fdset(CURLM * multi_handle, fd_set * read_fd_set, fd_set * write_fd_set, fd_set * exc_fd_set, int * max_fd),
    const struct curl_ws_frame *curl_ws_meta(CURL * curl),
    CURLcode curl_ws_recv(CURL * curl, void * buffer, size_t buflen, size_t * recv, const struct curl_ws_frame ** metap),
    CURLMsg *curl_multi_info_read(CURLM * multi_handle, int * msgs_in_queue)
*/
//<score> 1.846154, nr_unique_branch: 2, p_cov: 0.8
//<Quality> {"diversity":26,"density":20,"covered_api":["curl_easy_unescape","curl_ws_meta","curl_ws_recv","curl_multi_info_read"],"uncovered_api":["curl_multi_fdset"],"unique_branches":{"curl_ws_recv":[[853,6,853,11,0],[855,8,855,31,0]]},"library_calls":["curl_global_init","curl_easy_init","curl_easy_setopt","curl_easy_unescape","curl_multi_init","curl_multi_add_handle","curl_ws_meta","curl_ws_recv","curl_multi_info_read","curl_free","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup"]}
/*Here is a step by step C++ program that uses curl library APIs to achieve the desired event:

*/


extern "C" int LLVMFuzzerTestOneInput_15(const uint8_t *data, size_t size) {
	if(size<=0) return 0;

    // Initialize libcurl
    curl_global_init(CURL_GLOBAL_ALL);

    // Create and configure the curl handle
    CURL *curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

    // Unescape the input data
    int outlength = 0;
    const char *inputData = reinterpret_cast<const char *>(data);
    char *unescapedData = curl_easy_unescape(curl, inputData, size, &outlength);

    // Create a file to write the unescaped data
    FILE *outfile = fopen("output_file", "wb");
    if (outfile == NULL) {
        curl_free(unescapedData);
        curl_easy_cleanup(curl);
        curl_global_cleanup();
        assert_file_name_closed("output_file");
	assert_file_closed(&outfile);
	return 0;
    }

    // Write the unescaped data to the file
    size_t bytesWritten = fwrite(unescapedData, 1, outlength, outfile);
    if (bytesWritten != outlength) {
        assert_file_closed(&outfile);;
        curl_free(unescapedData);
        curl_easy_cleanup(curl);
        curl_global_cleanup();
        assert_file_name_closed("output_file");
	assert_file_closed(&outfile);
	return 0;
    }

    // Close the file
    assert_file_closed(&outfile);;

    // Create a file to read the unescaped data
    FILE *infile = fmemopen(reinterpret_cast<void *>(unescapedData), outlength, "rb");
    if (infile == NULL) {
        curl_free(unescapedData);
        curl_easy_cleanup(curl);
        curl_global_cleanup();
        assert_file_name_closed("output_file");
	assert_file_closed(&infile);
	assert_file_closed(&outfile);
	return 0;
    }

    // Get the file descriptor for reading
    int inFd = fuzz_fileno(infile);

    // Create a multi handle
    CURLM *multiHandle = curl_multi_init();

    // Add the curl easy handle to the multi handle
    curl_multi_add_handle(multiHandle, curl);

    // Wait for the file descriptor to be ready for reading
    fd_set readFdSet;
    FD_ZERO(&readFdSet);
    FD_SET(inFd, &readFdSet);
    int maxFd = inFd + 1;
    struct timeval timeout;
    timeout.tv_sec = 1;
    timeout.tv_usec = 0;

    // Check if the file descriptor is ready for reading
    int readyFdCount = select(maxFd, &readFdSet, NULL, NULL, &timeout);
    if (readyFdCount <= 0) {
        assert_file_closed(&infile);;
        curl_free(unescapedData);
        curl_easy_cleanup(curl);
        curl_multi_cleanup(multiHandle);
        curl_global_cleanup();
        assert_file_name_closed("output_file");
	assert_file_closed(&infile);
	assert_file_closed(&outfile);
	assert_fd_closed(inFd);
	return 0;
    }

    // Read from the file descriptor
    char buffer[1024];
    size_t bytesRead = fread(buffer, 1, sizeof(buffer), infile);
    assert_file_closed(&infile);;

    // Create a websocket frame pointer
    const struct curl_ws_frame *wsFrame = curl_ws_meta(curl);

    // Receive websocket data
    size_t receivedCount = 0;
    CURLcode recvResult = curl_ws_recv(curl, buffer, sizeof(buffer), &receivedCount, &wsFrame);

    // Read multi info
    int msgsInQueue = 0;
    CURLMsg *msg = NULL;
    while ((msg = curl_multi_info_read(multiHandle, &msgsInQueue)) != NULL) {
        // Process the message
    }

    // Free allocated resources
    curl_free(unescapedData);
    curl_easy_cleanup(curl);
    curl_multi_cleanup(multiHandle);
    curl_global_cleanup();

    assert_file_name_closed("output_file");
	assert_file_closed(&infile);
	assert_file_closed(&outfile);
	assert_fd_closed(inFd);
	return 0;
}