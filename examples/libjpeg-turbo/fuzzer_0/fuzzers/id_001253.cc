#include "FDSan.h"
#include <turbojpeg.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 1253
//<Prompt> ["tj3SetCroppingRegion","tj3YUVPlaneHeight","tjDecodeYUVPlanes","tjGetErrorStr","tjDecompressHeader2","tjDecompressHeader3"]
/*<Combination>: [int tj3SetCroppingRegion(tjhandle handle, tjregion croppingRegion),
    int tj3YUVPlaneHeight(int componentID, int height, int subsamp),
    int tjDecodeYUVPlanes(tjhandle handle, const unsigned char ** srcPlanes, const int * strides, int subsamp, unsigned char * dstBuf, int width, int pitch, int height, int pixelFormat, int flags),
    char *tjGetErrorStr(),
    int tjDecompressHeader2(tjhandle handle, unsigned char * jpegBuf, unsigned long jpegSize, int * width, int * height, int * jpegSubsamp),
    int tjDecompressHeader3(tjhandle handle, const unsigned char * jpegBuf, unsigned long jpegSize, int * width, int * height, int * jpegSubsamp, int * jpegColorspace)
*/
//<score> 1.6666666, nr_unique_branch: 1, p_cov: 0.8333333
//<Quality> {"diversity":9,"density":9,"covered_api":["tj3SetCroppingRegion","tj3YUVPlaneHeight","tjDecodeYUVPlanes","tjDecompressHeader2","tjDecompressHeader3"],"uncovered_api":["tjGetErrorStr"],"unique_branches":{"tj3DecodeYUVPlanes8":[[2074,56,2074,66,0]]},"library_calls":["tjInitDecompress","tj3SetCroppingRegion","tj3YUVPlaneHeight","tj3YUVPlaneHeight","tj3YUVPlaneHeight","tjDecodeYUVPlanes","tjDecompressHeader2","tjDecompressHeader3","tjDestroy"]}
/**/

extern "C" int LLVMFuzzerTestOneInput_40(const uint8_t *data, size_t size) {
	if(size<=0) return 0;

    // Create a buffer to store the input data
    unsigned char *jpegBuf = new unsigned char[size];
    memcpy(jpegBuf, data, size);

    // Initialize libjpeg-turbo variables
    tjhandle handle = tjInitDecompress();
    int width, height, subsamp;
    int jpegColorspace;
    tjregion croppingRegion;
    const unsigned char *srcPlanes[3];
    int strides[3];
    unsigned char *dstBuf;
    int pixelFormat;

    // Set the cropping region
    croppingRegion.x = 0;
    croppingRegion.y = 0;
    croppingRegion.w = width;   // Set the width and height according to your needs
    croppingRegion.h = height;

    tj3SetCroppingRegion(handle, croppingRegion);

    // Allocate memory for destination buffers
    tj3YUVPlaneHeight(0, height, subsamp);
    tj3YUVPlaneHeight(1, height, subsamp);
    tj3YUVPlaneHeight(2, height, subsamp);
    dstBuf = new unsigned char[width * height * 3 / 2];  // Assuming 4:2:0 subsampling

    // Set up source planes and strides
    srcPlanes[0] = jpegBuf;
    srcPlanes[1] = jpegBuf + width * height;
    srcPlanes[2] = jpegBuf + width * height * 5 / 4;  // Assuming 4:2:0 subsampling
    strides[0] = width;
    strides[1] = width / 2;  // Assuming 4:2:0 subsampling
    strides[2] = width / 2;  // Assuming 4:2:0 subsampling

    // Perform YUV plane decompression
    tjDecodeYUVPlanes(handle, srcPlanes, strides, subsamp, dstBuf, width, width, height, pixelFormat, 0);

    // Get the decompressed image dimensions and subsampling type
    tjDecompressHeader2(handle, jpegBuf, size, &width, &height, &subsamp);

    // Get the color space of the decompressed image
    tjDecompressHeader3(handle, jpegBuf, size, &width, &height, &subsamp, &jpegColorspace);

    delete[] jpegBuf;
    delete[] dstBuf;

    // Clean up libjpeg-turbo variables
    tjDestroy(handle);

    return 0;
}