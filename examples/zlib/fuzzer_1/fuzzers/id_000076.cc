#include "FDSan.h"
#include "FuzzedDataProvider.h"
#include <zlib.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 1434
//<Prompt> ["inflateGetDictionary","deflate","inflateBackEnd","inflateResetKeep","gzclose_r"]
/*<Combination>: [int inflateGetDictionary(z_streamp strm, Bytef * dictionary, uInt * dictLength),
    int deflate(z_streamp strm, int flush),
    int inflateBackEnd(z_streamp strm),
    int inflateResetKeep(z_streamp ),
    int gzclose_r(gzFile file)
*/
//<score> 4, nr_unique_branch: 0
//<Quality> {"density":4,"unique_branches":{},"library_calls":["inflateGetDictionary","deflate","inflateBackEnd","inflateResetKeep","gzdopen","gzclose_r"],"critical_calls":["inflateGetDictionary","deflate","inflateBackEnd","inflateResetKeep","gzdopen","gzclose_r"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_76(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    // Step 4: Create a FILE * variable for reading input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    
    // Step 5: Get the file descriptor for reading input data
    int in_fd = fuzz_fileno(in_file);
    
    // Step 6: Set the input file name
    const char *in_filename = "input_file";
    
    // Step 4: Create a FILE * variable for writing output data
    FILE *out_file = fopen("output_file", "wb");
    
    // Step 5: Get the file descriptor for writing output data
    int out_fd = fuzz_fileno(out_file);
    
    // Step 6: Set the output file name
    const char *out_filename = "output_file";
    
    // Step 1: Initialize zlib variables
    z_stream strm;
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    
    // Step 2: Call inflateGetDictionary API
    uInt dictLength = 0;
    Bytef *dictionary = nullptr;
    inflateGetDictionary(&strm, dictionary, &dictLength);
    
    // Step 2: Call deflate API
    deflate(&strm, Z_FINISH);
    
    // Step 2: Call inflateBackEnd API
    inflateBackEnd(&strm);
    
    // Step 2: Call inflateResetKeep API
    inflateResetKeep(&strm);
    
    // Step 2: Call gzclose_r API
    gzFile file = gzdopen(in_fd, "rb");
    gzclose_r(file);
    
    // Step 7: Release all allocated resources
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_name_closed("output_file");
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}