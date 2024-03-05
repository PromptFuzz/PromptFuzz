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
//<ID> 9012
//<Prompt> ["tj3DecompressToYUVPlanes8","tjDecodeYUV","tj3GetErrorCode","tj3CompressFromYUVPlanes8","tj3Compress12"]
/*<Combination>: [int tj3DecompressToYUVPlanes8(tjhandle handle, const unsigned char * jpegBuf, size_t jpegSize, unsigned char ** dstPlanes, int * strides),
    int tjDecodeYUV(tjhandle handle, const unsigned char * srcBuf, int align, int subsamp, unsigned char * dstBuf, int width, int pitch, int height, int pixelFormat, int flags),
    int tj3GetErrorCode(tjhandle handle),
    int tj3CompressFromYUVPlanes8(tjhandle handle, const unsigned char *const * srcPlanes, int width, const int * strides, int height, unsigned char ** jpegBuf, size_t * jpegSize),
    int tj3Compress12(tjhandle handle, const short * srcBuf, int width, int pitch, int height, int pixelFormat, unsigned char ** jpegBuf, size_t * jpegSize)
*/
//<score> 2, nr_unique_branch: 1, p_cov: 1
//<Quality> {"diversity":16,"density":16,"covered_api":["tj3DecompressToYUVPlanes8","tjDecodeYUV","tj3GetErrorCode","tj3CompressFromYUVPlanes8","tj3Compress12"],"uncovered_api":[],"unique_branches":{"tj3Compress12":[[81,7,81,35,0]]},"library_calls":["tjInitDecompress","tj3DecompressToYUVPlanes8","tj3GetErrorCode","tjDecodeYUV","tj3GetErrorCode","tj3CompressFromYUVPlanes8","tj3GetErrorCode","tj3Compress12","tj3GetErrorCode","tjFree","tjFree","tjFree","tjFree","tjFree","tjFree","tjDestroy"]}
/*Here is a C++ program that achieves the event using the libjpeg-turbo APIs:

*/


extern "C" int LLVMFuzzerTestOneInput_126(const uint8_t* f_data, size_t f_size) {
	if(f_size<=8) return 0;

	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(f_data, sizeof(uint8_t), f_size, input_file_ptr);
	fclose(input_file_ptr);

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
	//fuzzer shim end}




    // Step 4: Create input file stream
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Step 6: Set input file name
    const char* input_file = "input_file";
    
    // Step 2: Decompress JPEG to YUV planes
    tjhandle handle = tjInitDecompress();
    unsigned char* jpegBuf = NULL;
    unsigned char* yuvPlanes[3] = {NULL};
    int strides[3] = {0};
    int width, height, subsamp, pixelFormat;
    size_t jpegSize;
    
    // Step 1: tj3DecompressToYUVPlanes8
    int decompressCode = tj3DecompressToYUVPlanes8(handle, jpegBuf, jpegSize, yuvPlanes, strides);
    if (decompressCode != 0) {
        // Step 3: Get error code
        int errorCode = tj3GetErrorCode(handle);
        printf("Decompression failed with error code %d\n", errorCode);
    }
    
    // Step 5: Get file descriptor for input file stream
    int in_fd = fuzz_fileno(in_file);
    
    // Step 4: Create output file stream
    FILE* out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Step 6: Set output file name
    const char* output_file = "output_file";
    
    // Step 2: Decode YUV
    unsigned char* dstBuf = NULL;
    int decodeCode = tjDecodeYUV(handle, dstBuf, fuzz_int32_t_1, subsamp, yuvPlanes[0], width, strides[0], height, pixelFormat, fuzz_int32_t_2);
    if (decodeCode != 0) {
        // Step 3: Get error code
        int errorCode = tj3GetErrorCode(handle);
        printf("Decoding failed with error code %d\n", errorCode);
    }
    
    // Step 2: Compress from YUV planes
    unsigned char* compressedBuf = NULL;
    size_t compressedSize;
    int compressCode = tj3CompressFromYUVPlanes8(handle, yuvPlanes, width, strides, height, &compressedBuf, &compressedSize);
    if (compressCode != 0) {
        // Step 3: Get error code
        int errorCode = tj3GetErrorCode(handle);
        printf("Compression failed with error code %d\n", errorCode);
    }
    
    // Step 2: Compress using 12-bit source buffer
    short* srcBuf = NULL;
    int compressCode12 = tj3Compress12(handle, srcBuf, width, strides[0], height, pixelFormat, &compressedBuf, &compressedSize);
    if (compressCode12 != 0) {
        // Step 3: Get error code
        int errorCode = tj3GetErrorCode(handle);
        printf("Compression (12-bit) failed with error code %d\n", errorCode);
    }
    
    // Step 5: Get file descriptor for output file stream
    int out_fd = fuzz_fileno(out_file);
    
    // Step 2: Write output data to file
    fwrite(compressedBuf, 1, compressedSize, out_file);
    
    // Step 7: Free resources and close file streams
    tjFree(compressedBuf);
    tjFree(dstBuf);
    tjFree(jpegBuf);
    tjFree(yuvPlanes[0]);
    tjFree(yuvPlanes[1]);
    tjFree(yuvPlanes[2]);
    tjDestroy(handle);
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