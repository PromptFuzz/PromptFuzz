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
//<ID> 66
//<Prompt> ["tjDecompress","tjDecompressToYUV","tjBufSizeYUV","tj3Decompress8","tjGetErrorCode","tjLoadImage"]
/*<Combination>: [int tjDecompress(tjhandle handle, unsigned char * jpegBuf, unsigned long jpegSize, unsigned char * dstBuf, int width, int pitch, int height, int pixelSize, int flags),
    int tjDecompressToYUV(tjhandle handle, unsigned char * jpegBuf, unsigned long jpegSize, unsigned char * dstBuf, int flags),
    unsigned long tjBufSizeYUV(int width, int height, int subsamp),
    int tj3Decompress8(tjhandle handle, const unsigned char * jpegBuf, size_t jpegSize, unsigned char * dstBuf, int pitch, int pixelFormat),
    int tjGetErrorCode(tjhandle handle),
    unsigned char *tjLoadImage(const char * filename, int * width, int align, int * height, int * pixelFormat, int flags)
*/
//<score> 1.6666666, nr_unique_branch: 1, p_cov: 0.8333333
//<Quality> {"diversity":9,"density":9,"covered_api":["tjDecompress","tjDecompressToYUV","tjBufSizeYUV","tj3Decompress8","tjGetErrorCode"],"uncovered_api":["tjLoadImage"],"unique_branches":{"tj3Decompress8":[[158,43,158,57,0]]},"library_calls":["tjInitDecompress","tjAlloc","tjDecompress","tjDecompressToYUV","tjBufSizeYUV","tj3Decompress8","tjGetErrorCode","tjFree","tjDestroy"]}
/**/


extern "C" int LLVMFuzzerTestOneInput_4(const uint8_t* f_data, size_t f_size) {
	if(f_size<=0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Step 4: Create a FILE pointer to read the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
  
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Step 5: Get the file descriptor for reading
    int in_fd = fuzz_fileno(in_file);

    // Step 6: Specify the output file name
    const char *output_file = "output.jpg";

    // Step 1: Declare variables and initialize
    tjhandle handle = tjInitDecompress();
    unsigned char *jpegBuf = nullptr;
    unsigned long jpegSize = 0;
    unsigned char *dstBuf = nullptr;
    int width = 0;
    int height = 0;
    int pixelFormat = 0;
    int flags = 0;

    // Step 7: Read the input file
    int result = 1;
    int file_size = 0;
    fseek(in_file, 0, SEEK_END);
    file_size = ftell(in_file);
    fseek(in_file, 0, SEEK_SET);
    jpegBuf = (unsigned char *)tjAlloc(file_size);
    fread(jpegBuf, sizeof(uint8_t), file_size, in_file);
    assert_file_closed(&in_file);;

    // Step 2: Call the libjpeg-turbo APIs
    result = tjDecompress(handle, jpegBuf, file_size, dstBuf, width, 0, height, pixelFormat, flags);
    result = tjDecompressToYUV(handle, jpegBuf, file_size, dstBuf, flags);
    unsigned long bufSize = tjBufSizeYUV(width, height, TJSAMP_420);
    result = tj3Decompress8(handle, jpegBuf, file_size, dstBuf, width, pixelFormat);
    int errorCode = tjGetErrorCode(handle);

    // Step 3: Use the output data
    if (result != 0) {
        // Output the result to a file
        FILE *out_file = fopen(output_file, "wb");
        fwrite(dstBuf, sizeof(uint8_t), bufSize, out_file);
        assert_file_closed(&out_file);;
    }

    // Step 7: Cleanup
    tjFree(jpegBuf);
    tjDestroy(handle);

    assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
}