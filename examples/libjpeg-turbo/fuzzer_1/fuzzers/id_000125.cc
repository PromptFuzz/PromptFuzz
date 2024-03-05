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
//<ID> 8820
//<Prompt> ["tjCompress2","tjAlloc","tjEncodeYUV","tj3CompressFromYUV8","tjDecompressHeader2"]
/*<Combination>: [int tjCompress2(tjhandle handle, const unsigned char * srcBuf, int width, int pitch, int height, int pixelFormat, unsigned char ** jpegBuf, unsigned long * jpegSize, int jpegSubsamp, int jpegQual, int flags),
    unsigned char *tjAlloc(int bytes),
    int tjEncodeYUV(tjhandle handle, unsigned char * srcBuf, int width, int pitch, int height, int pixelSize, unsigned char * dstBuf, int subsamp, int flags),
    int tj3CompressFromYUV8(tjhandle handle, const unsigned char * srcBuf, int width, int align, int height, unsigned char ** jpegBuf, size_t * jpegSize),
    int tjDecompressHeader2(tjhandle handle, unsigned char * jpegBuf, unsigned long jpegSize, int * width, int * height, int * jpegSubsamp)
*/
//<score> 1.8000001, nr_unique_branch: 2, p_cov: 0.6
//<Quality> {"diversity":10,"density":10,"covered_api":["tjCompress2","tjAlloc","tjDecompressHeader2"],"uncovered_api":["tjEncodeYUV","tj3CompressFromYUV8"],"unique_branches":{"tjCompress2":[[1159,27,1159,42,0]],"tjDecompress2":[[1947,7,1947,37,0]]},"library_calls":["tjInitCompress","tjAlloc","tjDecompressHeader2","tjAlloc","tjDecompress2","tjCompress2","tjFree","tjFree","tjFree","tjDestroy"]}
/**/

extern "C" int LLVMFuzzerTestOneInput_125(const uint8_t* f_data, size_t f_size) {
	if(f_size<=0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    tjhandle handle = tjInitCompress();
    unsigned char *jpegBuf = NULL;
    unsigned long jpegSize;

    // Read the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    int in_fd = fuzz_fileno(in_file);
    fseek(in_file, 0, SEEK_END);
    long in_size = ftell(in_file);
    fseek(in_file, 0, SEEK_SET);
    unsigned char *in_buffer = (unsigned char *)tjAlloc(in_size);
    fread(in_buffer, 1, in_size, in_file);
    assert_file_closed(&in_file);;

    // Decompress the JPEG header to get image information
    int width, height, jpegSubsamp;
    tjDecompressHeader2(handle, in_buffer, in_size, &width, &height, &jpegSubsamp);

    // Allocate buffer for the decompressed image
    int pixelSize = 4; // Assuming 32-bit RGBA image
    unsigned char *dstBuf = (unsigned char *)tjAlloc(width * height * pixelSize);

    // Decompress the JPEG image
    int flags = TJFLAG_FASTDCT; // Use fast DCT algorithm
    tjDecompress2(handle, in_buffer, in_size, dstBuf, width, 0, height, pixelSize, flags);

    // Compress the decompressed image
    int jpegQual = 90; // JPEG quality
    unsigned char *outJpegBuf = NULL;
    tjCompress2(handle, dstBuf, width, 0, height, pixelSize, &outJpegBuf, &jpegSize, jpegSubsamp, jpegQual, flags);

    // Write the output JPEG data to a file
    FILE *out_file = fopen("output.jpg", "wb");
    int out_fd = fuzz_fileno(out_file);
    fwrite(outJpegBuf, 1, jpegSize, out_file);
    assert_file_closed(&out_file);;

    // Clean up
    tjFree(outJpegBuf);
    tjFree(dstBuf);
    tjFree(in_buffer);
    tjDestroy(handle);

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}