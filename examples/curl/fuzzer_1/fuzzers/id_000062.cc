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
//<ID> 1150
//<Prompt> ["curl_mime_init","curl_mime_addpart","curl_mfprintf","curl_mime_filedata","curl_formadd","curl_easy_header","curl_mime_data_cb"]
/*<Combination>: [curl_mime *curl_mime_init(CURL * easy),
    curl_mimepart *curl_mime_addpart(curl_mime * mime),
    int curl_mfprintf(FILE * fd, const char * format),
    CURLcode curl_mime_filedata(curl_mimepart * part, const char * filename),
    CURLFORMcode curl_formadd(struct curl_httppost ** httppost, struct curl_httppost ** last_post),
    CURLHcode curl_easy_header(CURL * easy, const char * name, size_t index, unsigned int origin, int request, struct curl_header ** hout),
    CURLcode curl_mime_data_cb(curl_mimepart * part, curl_off_t datasize, curl_read_callback readfunc, curl_seek_callback seekfunc, curl_free_callback freefunc, void * arg)
*/
//<score> 1.1904762, nr_unique_branch: 1, p_cov: 0.71428573
//<Quality> {"diversity":6,"density":5,"covered_api":["curl_mime_init","curl_mime_addpart","curl_mime_filedata","curl_easy_header","curl_mime_data_cb"],"uncovered_api":["curl_mfprintf","curl_formadd"],"unique_branches":{"curl_easy_header":[[75,15,75,20,0]]},"library_calls":["curl_mime_init","curl_mime_addpart","curl_mime_filedata","curl_easy_header","curl_mime_data_cb","curl_mime_free"]}
/**/


extern "C" int LLVMFuzzerTestOneInput_62(const uint8_t* f_data, size_t f_size) {
	if(f_size<=44) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_3, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
	//fuzzer shim end}




    // Step 4: Create FILE variable for reading the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    
    // Step 5: Get file descriptor for reading
    int in_fd = fuzz_fileno(in_file);
    
    // Step 6: Set output file name
    const char *output_file = "output_file";
    
    // Step 1: Initialize curl_mime structure
    curl_mime *mime = curl_mime_init(NULL);
    
    // Step 1: Add a new part to mime structure
    curl_mimepart *part = curl_mime_addpart(mime);
    
    // Step 1: Set the filename for the part
    curl_mime_filedata(part, fuzz_str_1);
    
    // Step 1: Set the header for the part
    curl_easy_header(NULL, fuzz_str_2, fuzz_uint64_t_3, fuzz_uint32_t_4, fuzz_int32_t_5, NULL);
    
    // Step 1: Set the data for the part using callback
    curl_mime_data_cb(part, size, NULL, NULL, NULL, NULL);
    
    // Step 2: Write input data to output file
    FILE *out_file = fopen(output_file, "wb");
    if (out_file) {
        // Write input data
        fwrite(data, 1, size, out_file);
        assert_file_closed(&out_file);;
    }
    
    // Step 7: Release allocated resources
    curl_mime_free(mime);
    assert_file_closed(&in_file);;
    
    assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
}