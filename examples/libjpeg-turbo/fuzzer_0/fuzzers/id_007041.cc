#include "FDSan.h"
#include <turbojpeg.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 7041
//<Prompt> ["tj3LoadImage16","tj3YUVPlaneHeight","tj3EncodeYUV8","tj3Transform","tj3DecompressHeader","tj3SetCroppingRegion"]
/*<Combination>: [unsigned short *tj3LoadImage16(tjhandle handle, const char * filename, int * width, int align, int * height, int * pixelFormat),
    int tj3YUVPlaneHeight(int componentID, int height, int subsamp),
    int tj3EncodeYUV8(tjhandle handle, const unsigned char * srcBuf, int width, int pitch, int height, int pixelFormat, unsigned char * dstBuf, int align),
    int tj3Transform(tjhandle handle, const unsigned char * jpegBuf, size_t jpegSize, int n, unsigned char ** dstBufs, size_t * dstSizes, const tjtransform * transforms),
    int tj3DecompressHeader(tjhandle handle, const unsigned char * jpegBuf, size_t jpegSize),
    int tj3SetCroppingRegion(tjhandle handle, tjregion croppingRegion)
*/
//<score> 2, nr_unique_branch: 1, p_cov: 1
//<Quality> {"diversity":11,"density":11,"covered_api":["tj3LoadImage16","tj3YUVPlaneHeight","tj3EncodeYUV8","tj3Transform","tj3DecompressHeader","tj3SetCroppingRegion"],"uncovered_api":[],"unique_branches":{"tj3LoadImage16":[[301,54,301,66,0]]},"library_calls":["tjInitTransform","tj3LoadImage16","tj3YUVPlaneHeight","tj3Alloc","tj3EncodeYUV8","tj3Transform","tj3DecompressHeader","tj3SetCroppingRegion","tjFree","tjFree","tjDestroy"]}
/*Here is the implementation of the C++ program:

*/


extern "C" int LLVMFuzzerTestOneInput_107(const uint8_t *data, size_t size) {
	if(size<=0) return 0;

    tjhandle handle = tjInitTransform();
    int width, height;
    unsigned char *jpegBuf = nullptr;
    size_t jpegSize = 0;
    
    // Load input image
    char *input_file = "input.jpg";
    FILE *in_file = fopen(input_file, "wb");
    fwrite(data, sizeof(uint8_t), size, in_file);
    assert_file_closed(&in_file);;

    // Load image using libjpeg-turbo
    tj3LoadImage16(handle, input_file, &width, 1, &height, nullptr);
    
    // YUV plane height of the image
    int componentID = 0;
    int subsamp = TJSAMP_420;
    int yuv_plane_height = tj3YUVPlaneHeight(componentID, height, subsamp);
    
    // Encode YUV from input image
    unsigned char *dstBuf = (unsigned char *)tj3Alloc(yuv_plane_height);
    int pixelFormat = TJPF_BGR;
    int align = 1;
    tj3EncodeYUV8(handle, data, width, width * tjPixelSize[pixelFormat], height, pixelFormat, dstBuf, align);
    
    // Transformation parameters
    tjtransform transform;
    transform.r.x = 0;
    transform.r.y = 0;
    transform.r.w = width;
    transform.r.h = height;
    transform.options = TJXOPT_TRIM;
    transform.customFilter = nullptr;
    
    // Transform the image
    unsigned char *dstBufs[1];
    size_t dstSizes[1];
    int n = 1;
    tj3Transform(handle, jpegBuf, jpegSize, n, dstBufs, dstSizes, &transform);
    
    // Extract header from transformed image
    tj3DecompressHeader(handle, dstBufs[0], dstSizes[0]);
    
    // Set cropping region
    tjregion croppingRegion;
    croppingRegion.x = width / 4;
    croppingRegion.y = height / 4;
    croppingRegion.w = width / 2;
    croppingRegion.h = height / 2;
    tj3SetCroppingRegion(handle, croppingRegion);
    
    tjFree(jpegBuf);
    tjFree(dstBuf);
    tjDestroy(handle); 
    assert_file_closed(&in_file);
	return 0;
}