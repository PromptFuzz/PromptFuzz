#include "FDSan.h"
#include <turbojpeg.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 2224
//<Prompt> ["tjPlaneWidth","tjBufSizeYUV2","tj3Destroy","tj3LoadImage8","tj3DecodeYUV8","tj3GetErrorCode","tjInitDecompress","tj3SetScalingFactor"]
/*<Combination>: [int tjPlaneWidth(int componentID, int width, int subsamp),
    unsigned long tjBufSizeYUV2(int width, int align, int height, int subsamp),
    void tj3Destroy(tjhandle handle),
    unsigned char *tj3LoadImage8(tjhandle handle, const char * filename, int * width, int align, int * height, int * pixelFormat),
    int tj3DecodeYUV8(tjhandle handle, const unsigned char * srcBuf, int align, unsigned char * dstBuf, int width, int pitch, int height, int pixelFormat),
    int tj3GetErrorCode(tjhandle handle),
    tjhandle tjInitDecompress(),
    int tj3SetScalingFactor(tjhandle handle, tjscalingfactor scalingFactor)
*/
//<score> 3, nr_unique_branch: 2, p_cov: 1
//<Quality> {"diversity":10,"density":10,"covered_api":["tjPlaneWidth","tjBufSizeYUV2","tj3Destroy","tj3LoadImage8","tj3DecodeYUV8","tj3GetErrorCode","tjInitDecompress","tj3SetScalingFactor"],"uncovered_api":[],"unique_branches":{"tj3SetScalingFactor":[[1870,15,1870,24,1],[1874,7,1874,17,0]]},"library_calls":["tjPlaneWidth","tjBufSizeYUV2","tjInitDecompress","tj3LoadImage8","tj3DecodeYUV8","tj3GetErrorCode","tj3SetScalingFactor","tj3Destroy","tjFree","tjFree"]}
/**/



extern "C" int LLVMFuzzerTestOneInput_53(const uint8_t *data, size_t size)
{
	if(size<=0) return 0;

	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);

    // Step 4: Create a FILE pointer for reading the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    
    // Step 6: File name for input data
    const char *input_file = "input_file";
    
    // Step 7: Release resources
    assert_file_closed(&in_file);;
    
    // Step 4: Create a FILE pointer for writing the output data
    FILE *out_file = fopen("output_file", "wb");
    
    // Step 6: File name for output data
    const char *output_file = "output_file";
    
    // Step 7: Release resources
    assert_file_closed(&out_file);;
    
    // Step 1: Achieve the event
    int width = 0;
    int height = 0;
    int subsamp = 0;
    int componentID = 0;
    int align = 0;
    int pixelFormat = 0;
    
    int planeWidth = tjPlaneWidth(componentID, width, subsamp);
    unsigned long bufSizeYUV = tjBufSizeYUV2(width, align, height, subsamp);
    tjhandle handle = tjInitDecompress();
    unsigned char *srcBuf = tj3LoadImage8(handle, input_file, &width, align, &height, &pixelFormat);
    unsigned char *dstBuf = nullptr;
    int errorCode = tj3DecodeYUV8(handle, srcBuf, align, dstBuf, width, planeWidth, height, pixelFormat);
    int tjErrorCode = tj3GetErrorCode(handle);
    tjscalingfactor scalingFactor;
    tj3SetScalingFactor(handle, scalingFactor);
    tj3Destroy(handle);

    // Step 7: Release resources
    tjFree(srcBuf);
    tjFree(dstBuf);

    assert_file_name_closed("output_file");
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}