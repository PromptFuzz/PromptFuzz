#include "FDSan.h"
#include <turbojpeg.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 5640
//<Prompt> ["tjGetErrorStr","tj3Compress12","tjEncodeYUV","tj3Set","tjPlaneWidth","tj3GetScalingFactors","tj3GetErrorCode","tjInitCompress"]
/*<Combination>: [char *tjGetErrorStr(),
    int tj3Compress12(tjhandle handle, const short * srcBuf, int width, int pitch, int height, int pixelFormat, unsigned char ** jpegBuf, size_t * jpegSize),
    int tjEncodeYUV(tjhandle handle, unsigned char * srcBuf, int width, int pitch, int height, int pixelSize, unsigned char * dstBuf, int subsamp, int flags),
    int tj3Set(tjhandle handle, int param, int value),
    int tjPlaneWidth(int componentID, int width, int subsamp),
    tjscalingfactor *tj3GetScalingFactors(int * numScalingFactors),
    int tj3GetErrorCode(tjhandle handle),
    tjhandle tjInitCompress()
*/
//<score> 0, nr_unique_branch: 6, p_cov: 0
//<Quality> {"diversity":0,"density":1,"covered_api":[],"uncovered_api":["tjGetErrorStr","tj3Compress12","tjEncodeYUV","tj3Set","tjPlaneWidth","tj3GetScalingFactors","tj3GetErrorCode","tjInitCompress"],"unique_branches":{"tj3Set":[[633,9,633,33,1],[573,7,573,23,1],[573,44,573,60,1]],"getPixelFormat":[[315,7,315,21,0],[316,9,316,30,1],[320,11,320,25,1]]},"library_calls":[]}
/**/

// Include the necessary headers

// Define the event that we want to achieve using the libjpeg-turbo APIs
void  id_005640_compressJPEG(const uint8_t *data, size_t size) {
    // Create a libjpeg-turbo compression instance
    tjhandle handle = tjInitCompress();
    if (handle == NULL) {
        fprintf(stderr, "Error initializing compression\n");
        return;
    }
   
    // Set the compression parameters
    int width = 640;
    int height = 480;
    int pixelFormat = TJPF_RGBA;
    int subsamp = TJSAMP_444;
    int flags = 0;
    int ret = tj3Set(handle, TJXOPT_GRAY, 0);
    if (ret != 0) {
        fprintf(stderr, "Error setting compression parameters: %s\n", tjGetErrorStr2(handle));
        tjDestroy(handle);
        return;
    }
  
    // Compress the input data
    unsigned char *jpegBuf = NULL;
    size_t jpegSize = 0;
    ret = tjEncodeYUV(handle, (unsigned char *)data, width, 0, height, 4, jpegBuf, subsamp, flags);
    if (ret != 0) {
        fprintf(stderr, "Error compressing image: %s\n", tjGetErrorStr2(handle));
        tjDestroy(handle);
        return;
    }
 
    // Save the compressed JPEG data to a file
    FILE *out_file = fopen("output.jpg", "wb");
    fwrite(jpegBuf, 1, jpegSize, out_file);
    assert_file_closed(&out_file);;
 
    // Clean up resources
    tjFree(jpegBuf);
    tjDestroy(handle);
}

extern "C" int LLVMFuzzerTestOneInput_95(const uint8_t *data, size_t size) {
	if(size<=0) return 0;

     id_005640_compressJPEG(data, size);
    return 0;
}