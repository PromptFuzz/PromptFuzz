#include "FDSan.h"
#include "FuzzedDataProvider.h"
#include <turbojpeg.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 3093
//<Prompt> ["tj3Decompress16","tj3LoadImage12","tj3Get","tj3Free","tjEncodeYUVPlanes"]
/*<Combination>: [int tj3Decompress16(tjhandle handle, const unsigned char * jpegBuf, size_t jpegSize, unsigned short * dstBuf, int pitch, int pixelFormat),
    short *tj3LoadImage12(tjhandle handle, const char * filename, int * width, int align, int * height, int * pixelFormat),
    int tj3Get(tjhandle handle, int param),
    void tj3Free(void * buffer),
    int tjEncodeYUVPlanes(tjhandle handle, const unsigned char * srcBuf, int width, int pitch, int height, int pixelFormat, unsigned char ** dstPlanes, int * strides, int subsamp, int flags)
*/
//<score> 1.8000001, nr_unique_branch: 2, p_cov: 0.6
//<Quality> {"diversity":14,"density":14,"covered_api":["tj3Decompress16","tj3Get","tj3Free"],"uncovered_api":["tj3LoadImage12","tjEncodeYUVPlanes"],"unique_branches":{"tj3Decompress16":[[158,43,158,57,0]],"tj3Get":[[729,3,729,23,0]]},"library_calls":["tjInitDecompress","tj3Get","tj3Alloc","tj3Decompress16","tjDestroy","tj3Free"]}
/**/

extern "C" int LLVMFuzzerTestOneInput_71(const uint8_t* f_data, size_t f_size) {
	if(f_size<=0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Step 4: Create a FILE* variable to read the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Step 5: Get the file descriptor for reading input data
    int in_fd = fuzz_fileno(in_file);

    // Step 2: Initialize libjpeg-turbo handle
    tjhandle handle = tjInitDecompress();
    if (!handle) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
    }

    // Step 3: Get libjpeg-turbo parameter
    int pixelFormat = tj3Get(handle, TJPF_BGRX);

    // Step 6: Set the output file name
    const char *output_filename = "output_file";

    // Step 7: Create a FILE* variable to write the output data
    FILE *out_file = fopen(output_filename, "wb");
    if (!out_file) {
        tjDestroy(handle);
        assert_file_closed(&in_file);;
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
    }

    // Step 5: Get the file descriptor for writing output data
    int out_fd = fuzz_fileno(out_file);

    // Step 1: Decompress the input data
    unsigned char *jpegBuf = nullptr;
    unsigned short *dstBuf = nullptr;
    int width, height;
    size_t jpegSize = 0;

    // Read the input file and get the size of the input data
    fseek(in_file, 0, SEEK_END);
    jpegSize = ftell(in_file);
    fseek(in_file, 0, SEEK_SET);

    // Allocate memory for the input data
    jpegBuf = (unsigned char*)tj3Alloc(jpegSize);
    if (!jpegBuf) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        tjDestroy(handle);
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }

    // Read the input data from the file
    size_t bytesRead = fread(jpegBuf, 1, jpegSize, in_file);
    if (bytesRead != jpegSize) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        tjDestroy(handle);
        tj3Free(jpegBuf);
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }

    // Decompress the input data
    int pitch = 0;
    int result = tj3Decompress16(handle, jpegBuf, jpegSize, dstBuf, pitch, pixelFormat);
    if (result != 0) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        tjDestroy(handle);
        tj3Free(jpegBuf);
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }

    // Step 7: Write the output data to the file
    size_t bytesWritten = fwrite(dstBuf, 1, width * height, out_file);
    if (bytesWritten != width * height) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        tjDestroy(handle);
        tj3Free(jpegBuf);
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }

    // Step 7: Close the output file
    assert_file_closed(&out_file);;
  
    // Step 7: Free allocated memory
    tjDestroy(handle);
    tj3Free(jpegBuf);
    
    assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}