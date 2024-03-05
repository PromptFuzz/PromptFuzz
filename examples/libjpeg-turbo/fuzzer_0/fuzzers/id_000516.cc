#include "FDSan.h"
#include <turbojpeg.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 516
//<Prompt> ["tjTransform","tjBufSize","tjInitCompress","tjAlloc","tjBufSizeYUV","tj3Alloc"]
/*<Combination>: [int tjTransform(tjhandle handle, const unsigned char * jpegBuf, unsigned long jpegSize, int n, unsigned char ** dstBufs, unsigned long * dstSizes, tjtransform * transforms, int flags),
    unsigned long tjBufSize(int width, int height, int jpegSubsamp),
    tjhandle tjInitCompress(),
    unsigned char *tjAlloc(int bytes),
    unsigned long tjBufSizeYUV(int width, int height, int subsamp),
    void *tj3Alloc(size_t bytes)
*/
//<score> 1, nr_unique_branch: 1, p_cov: 0.5
//<Quality> {"diversity":12,"density":12,"covered_api":["tjTransform","tjBufSize","tj3Alloc"],"uncovered_api":["tjInitCompress","tjAlloc","tjBufSizeYUV"],"unique_branches":{"tj3Transform":[[2657,21,2657,49,1]]},"library_calls":["tjInitTransform","tjTransform","tjBufSize","tj3Alloc","tjDecompress","tjFree","tjFree","tjDestroy"]}
/**/


extern "C" int LLVMFuzzerTestOneInput_21(const uint8_t *data, size_t size) {
	if(size<=0) return 0;

    tjhandle handle = tjInitTransform();
    if (handle == NULL) {
        return 0;
    }
    
    unsigned char *jpegBuf = NULL;
    unsigned long jpegSize = 0;
    tjtransform transform;
    
    // Compress the input data using libjpeg-turbo
    if (tjTransform(handle, data, size, 1, &jpegBuf, &jpegSize, &transform, 0) == -1) {
        tjFree(jpegBuf);
        tjDestroy(handle);
        return 0;
    }
    
    // Allocate memory for output buffer
    unsigned long bufSize = tjBufSize(100, 100, TJSAMP_444);
    unsigned char *buf = (unsigned char*) tj3Alloc(bufSize);
    if (buf == NULL) {
        tjFree(jpegBuf);
        tjDestroy(handle);
        return 0;
    }
    
    // Decompress the compressed data
    tjDecompress(handle, jpegBuf, jpegSize, buf, 100, 0, 100, TJPF_RGB, 0);
    
    // Release resources
    tjFree(buf);
    tjFree(jpegBuf);
    tjDestroy(handle);
    
    return 0;
}