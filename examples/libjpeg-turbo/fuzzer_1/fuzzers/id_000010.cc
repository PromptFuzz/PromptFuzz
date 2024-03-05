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
//<ID> 344
//<Prompt> ["tjDecodeYUV","tj3YUVPlaneWidth","tj3YUVPlaneSize","tjDecompress","tj3EncodeYUVPlanes8"]
/*<Combination>: [int tjDecodeYUV(tjhandle handle, const unsigned char * srcBuf, int align, int subsamp, unsigned char * dstBuf, int width, int pitch, int height, int pixelFormat, int flags),
    int tj3YUVPlaneWidth(int componentID, int width, int subsamp),
    size_t tj3YUVPlaneSize(int componentID, int width, int stride, int height, int subsamp),
    int tjDecompress(tjhandle handle, unsigned char * jpegBuf, unsigned long jpegSize, unsigned char * dstBuf, int width, int pitch, int height, int pixelSize, int flags),
    int tj3EncodeYUVPlanes8(tjhandle handle, const unsigned char * srcBuf, int width, int pitch, int height, int pixelFormat, unsigned char ** dstPlanes, int * strides)
*/
//<score> 1.0400001, nr_unique_branch: 1, p_cov: 0.6
//<Quality> {"diversity":15,"density":13,"covered_api":["tjDecodeYUV","tj3YUVPlaneSize","tj3EncodeYUVPlanes8"],"uncovered_api":["tj3YUVPlaneWidth","tjDecompress"],"unique_branches":{"tj3DecodeYUV8":[[2226,25,2226,34,0]]},"library_calls":["tjInitDecompress","tjDecompressHeader","tjPlaneWidth","tjPlaneHeight","tj3YUVPlaneSize","tjAlloc","tjDecodeYUV","tjPlaneWidth","tjPlaneHeight","tj3YUVPlaneSize","tjAlloc","tj3EncodeYUVPlanes8","tjFree","tjFree","tjDestroy"]}
/**/

extern "C" int LLVMFuzzerTestOneInput_10(const uint8_t* f_data, size_t f_size) {
	if(f_size<=16) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
	//fuzzer shim end}




    tjhandle tjInstance = tjInitDecompress();
    unsigned char *jpegBuf = const_cast<unsigned char *>(data);
    unsigned long jpegSize = size;

    // Determine the width and height of the JPEG image
    int width, height;
    tjDecompressHeader(tjInstance, jpegBuf, jpegSize, &width, &height);

    // Calculate the size of the YUV420p buffer
    int yuvStride = tjPlaneWidth(0, width, TJ_420);
    int yuvHeight = tjPlaneHeight(0, height, TJ_420);
    size_t yuvSize = tj3YUVPlaneSize(fuzz_int32_t_1, width, yuvStride, yuvHeight, TJ_420);

    // Allocate memory for the YUV420p buffer
    unsigned char *yuvBuffer = static_cast<unsigned char *>(tjAlloc(yuvSize));

    // Decompress the JPEG image to YUV420p format
    tjDecodeYUV(tjInstance, jpegBuf, fuzz_int32_t_2, TJ_420, yuvBuffer, width, yuvStride, yuvHeight, TJPF_GRAY, fuzz_int32_t_3);

    // Calculate the size of the RGB buffer
    int rgbStride = tjPlaneWidth(0, width, TJ_420);
    int rgbHeight = tjPlaneHeight(0, height, TJ_420);
    size_t rgbSize = tj3YUVPlaneSize(fuzz_int32_t_4, width, rgbStride, rgbHeight, TJPF_RGB);

    // Allocate memory for the RGB buffer
    unsigned char *rgbBuffer = static_cast<unsigned char *>(tjAlloc(rgbSize));

    // Convert the YUV420p image to RGB format
    tj3EncodeYUVPlanes8(tjInstance, yuvBuffer, width, rgbStride, rgbHeight, TJPF_RGB, &rgbBuffer, &rgbStride);

    // Clean up
    tjFree(yuvBuffer);
    tjFree(rgbBuffer);
    tjDestroy(tjInstance);

    return 0;
}