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
//<ID> 387
//<Prompt> ["tjCompress","tj3DecompressToYUVPlanes8","tj3DecodeYUV8","tjGetScalingFactors","tjCompressFromYUV"]
/*<Combination>: [int tjCompress(tjhandle handle, unsigned char * srcBuf, int width, int pitch, int height, int pixelSize, unsigned char * dstBuf, unsigned long * compressedSize, int jpegSubsamp, int jpegQual, int flags),
    int tj3DecompressToYUVPlanes8(tjhandle handle, const unsigned char * jpegBuf, size_t jpegSize, unsigned char ** dstPlanes, int * strides),
    int tj3DecodeYUV8(tjhandle handle, const unsigned char * srcBuf, int align, unsigned char * dstBuf, int width, int pitch, int height, int pixelFormat),
    tjscalingfactor *tjGetScalingFactors(int * numscalingfactors),
    int tjCompressFromYUV(tjhandle handle, const unsigned char * srcBuf, int width, int align, int height, int subsamp, unsigned char ** jpegBuf, unsigned long * jpegSize, int jpegQual, int flags)
*/
//<score> 1.7142857, nr_unique_branch: 1, p_cov: 1
//<Quality> {"diversity":7,"density":6,"covered_api":["tjCompress","tj3DecompressToYUVPlanes8","tj3DecodeYUV8","tjGetScalingFactors","tjCompressFromYUV"],"uncovered_api":[],"unique_branches":{"tj3DecompressToYUVPlanes8":[[2303,7,2303,37,0]]},"library_calls":["tjInitCompress","tjCompress","tj3DecompressToYUVPlanes8","tj3DecodeYUV8","tjGetScalingFactors","tjCompressFromYUV","tjDestroy"]}
/**/


extern "C" int LLVMFuzzerTestOneInput_13(const uint8_t* f_data, size_t f_size) {
	if(f_size<=0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Step 1: Declare variables
    tjhandle handle = tjInitCompress();
    unsigned char *srcBuf = NULL;
    int width = 0;
    int pitch = 0;
    int height = 0;
    int pixelSize = 0;
    unsigned char *dstBuf = NULL;
    unsigned long compressedSize = 0;
    int jpegSubsamp = 0;
    int jpegQual = 100;
    int flags = 0;
    unsigned char *jpegBuf = NULL;
    unsigned long jpegSize = 0;
    unsigned char *yuvPlanes[3] = {NULL};
    int strides[3] = {0};
    unsigned char *decodingBuf = NULL;
    int align = 1;
    int pixelFormat = TJPF_UNKNOWN;
    int numScalingFactors = 0;
    tjscalingfactor *scalingFactors = NULL;
    
    // Step 2: Allocate memory
    srcBuf = (unsigned char *)malloc(size);
    memcpy(srcBuf, data, size);

    // Step 3: Initialize input parameters (width, pitch, height, pixelSize, jpegSubsamp)
    // You may need to define your own way of parsing the input data to obtain these parameters

    // Step 4: Compress input image to JPEG
    tjCompress(handle, srcBuf, width, pitch, height, pixelSize, dstBuf, &compressedSize, jpegSubsamp, jpegQual, flags);

    // Step 5: Compress YUV planes to JPEG directly
    tj3DecompressToYUVPlanes8(handle, jpegBuf, jpegSize, yuvPlanes, strides);

    // Step 6: Decode YUV planes to RGB
    tj3DecodeYUV8(handle, yuvPlanes[0], align, decodingBuf, width, pitch, height, pixelFormat);

    // Step 7: Get scaling factors
    scalingFactors = tjGetScalingFactors(&numScalingFactors);

    // Step 8: Compress YUV data to JPEG
    tjCompressFromYUV(handle, decodingBuf, width, align, height, jpegSubsamp, &jpegBuf, &jpegSize, jpegQual, flags);

    // Step 9: Clean up resources
    if(srcBuf) free(srcBuf);
    if(dstBuf) free(dstBuf);
    if(jpegBuf) free(jpegBuf);
    if(yuvPlanes[0]) free(yuvPlanes[0]);
    if(yuvPlanes[1]) free(yuvPlanes[1]);
    if(yuvPlanes[2]) free(yuvPlanes[2]);
    if(decodingBuf) free(decodingBuf);

    tjDestroy(handle);

    return 0;
}