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
//<ID> 4806
//<Prompt> ["tj3YUVPlaneSize","tjDecompressHeader3","tjDecodeYUV","tjInitDecompress","tjDecompressHeader"]
/*<Combination>: [size_t tj3YUVPlaneSize(int componentID, int width, int stride, int height, int subsamp),
    int tjDecompressHeader3(tjhandle handle, const unsigned char * jpegBuf, unsigned long jpegSize, int * width, int * height, int * jpegSubsamp, int * jpegColorspace),
    int tjDecodeYUV(tjhandle handle, const unsigned char * srcBuf, int align, int subsamp, unsigned char * dstBuf, int width, int pitch, int height, int pixelFormat, int flags),
    tjhandle tjInitDecompress(),
    int tjDecompressHeader(tjhandle handle, unsigned char * jpegBuf, unsigned long jpegSize, int * width, int * height)
*/
//<score> 1.8000001, nr_unique_branch: 2, p_cov: 0.8
//<Quality> {"diversity":24,"density":18,"covered_api":["tj3YUVPlaneSize","tjDecompressHeader3","tjDecodeYUV","tjInitDecompress"],"uncovered_api":["tjDecompressHeader"],"unique_branches":{"tj3YUVPlaneSize":[[1114,7,1114,14,0]],"tj3YUVPlaneWidth":[[1043,26,1043,43,0]]},"library_calls":["tjAlloc","tjInitDecompress","tjDecompressHeader3","tjPlaneWidth","tjPlaneWidth","tjPlaneWidth","tj3YUVPlaneSize","tj3YUVPlaneSize","tj3YUVPlaneSize","tjAlloc","tjPlaneSizeYUV","tjPlaneSizeYUV","tjDecodeYUV","tjFree","tjFree","tjDestroy"]}
/*Here is the C++ program that achieves the event using the libjpeg-turbo library APIs:

*/


extern "C" int LLVMFuzzerTestOneInput_87(const uint8_t* f_data, size_t f_size) {
	if(f_size<=8) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
	//fuzzer shim end}




    // Declarations
    tjhandle handle = NULL;
    unsigned char* jpegBuf = NULL;
    unsigned long jpegSize = 0;
    int width = 0, height = 0, subsamp = 0, colorspace = 0;

    // Create input file stream
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Read the file into jpegBuf
    fseek(in_file, 0, SEEK_END);
    jpegSize = ftell(in_file);
    rewind(in_file);
    jpegBuf = (unsigned char*)tjAlloc(jpegSize);
    if (!jpegBuf) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    fread(jpegBuf, 1, jpegSize, in_file);
    assert_file_closed(&in_file);;

    // Initialize decompressor
    handle = tjInitDecompress();
    if (!handle) {
        tjFree(jpegBuf);
        assert_file_closed(&in_file);
	return 0;
    }

    // Get the image width, height, subsampling, and colorspace from the JPEG header
    if (tjDecompressHeader3(handle, jpegBuf, jpegSize, &width, &height, &subsamp, &colorspace) < 0) {
        tjFree(jpegBuf);
        tjDestroy(handle);
        assert_file_closed(&in_file);
	return 0;
    }

    // Allocate memory for the decompressed YUV image
    int planes[3] = {0};  // Number of planes in YUV image
    planes[0] = tjPlaneWidth(0, width, subsamp);
    planes[1] = tjPlaneWidth(1, width, subsamp);
    planes[2] = tjPlaneWidth(2, width, subsamp);
    size_t yuvSize = 0;
    unsigned char* yuvBuf[3] = {NULL};
    yuvSize += tj3YUVPlaneSize(TJPF_GRAY, planes[0], planes[0], height, subsamp);
    yuvSize += tj3YUVPlaneSize(TJPF_GRAY, planes[1], planes[1], height, subsamp);
    yuvSize += tj3YUVPlaneSize(TJPF_GRAY, planes[2], planes[2], height, subsamp);
    yuvBuf[0] = (unsigned char*)tjAlloc(yuvSize);
    yuvBuf[1] = yuvBuf[0] + tjPlaneSizeYUV(1, planes[0], planes[0], height, subsamp);
    yuvBuf[2] = yuvBuf[1] + tjPlaneSizeYUV(2, planes[1], planes[1], height, subsamp);
    if (!yuvBuf[0]) {
        tjFree(jpegBuf);
        tjDestroy(handle);
        assert_file_closed(&in_file);
	return 0;
    }

    // Decode the JPEG into the YUV image
    if (tjDecodeYUV(handle, jpegBuf, fuzz_int32_t_1, subsamp, yuvBuf[0], width, planes[0], height, TJPF_GRAY, fuzz_int32_t_2) < 0) {
        tjFree(yuvBuf[0]);
        tjFree(jpegBuf);
        tjDestroy(handle);
        assert_file_closed(&in_file);
	return 0;
    }

    // Cleanup
    tjFree(yuvBuf[0]);
    tjFree(jpegBuf);
    tjDestroy(handle);

    assert_file_closed(&in_file);
	return 0;
}