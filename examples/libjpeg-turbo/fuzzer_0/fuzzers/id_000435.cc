#include "FDSan.h"
#include <turbojpeg.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 435
//<Prompt> ["tjCompress","tj3DecompressToYUVPlanes8","tjGetScalingFactors","tjEncodeYUV","tjCompressFromYUV"]
/*<Combination>: [int tjCompress(tjhandle handle, unsigned char * srcBuf, int width, int pitch, int height, int pixelSize, unsigned char * dstBuf, unsigned long * compressedSize, int jpegSubsamp, int jpegQual, int flags),
    int tj3DecompressToYUVPlanes8(tjhandle handle, const unsigned char * jpegBuf, size_t jpegSize, unsigned char ** dstPlanes, int * strides),
    tjscalingfactor *tjGetScalingFactors(int * numscalingfactors),
    int tjEncodeYUV(tjhandle handle, unsigned char * srcBuf, int width, int pitch, int height, int pixelSize, unsigned char * dstBuf, int subsamp, int flags),
    int tjCompressFromYUV(tjhandle handle, const unsigned char * srcBuf, int width, int align, int height, int subsamp, unsigned char ** jpegBuf, unsigned long * jpegSize, int jpegQual, int flags)
*/
//<score> 0.4, nr_unique_branch: 1, p_cov: 0.2
//<Quality> {"diversity":6,"density":6,"covered_api":["tjCompress"],"uncovered_api":["tj3DecompressToYUVPlanes8","tjGetScalingFactors","tjEncodeYUV","tjCompressFromYUV"],"unique_branches":{"tj3Compress8":[[84,7,84,21,0]]},"library_calls":["tjInitCompress","tjLoadImage","tjCompress","tjFree","tjFree","tjDestroy"]}
/*Here is a step-by-step implementation of the C++ program using libjpeg-turbo library APIs:

*/


extern "C" int LLVMFuzzerTestOneInput_16(const uint8_t *data, size_t size) {
	if(size<=0) return 0;

	FILE *input_file_ptr = fopen("input_file.jpg", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);

    tjhandle handle = tjInitCompress();
    unsigned char *jpegBuf = nullptr;
    unsigned long jpegSize = 0;
    FILE *in_file = fmemopen((void *)data, size, "rb");
    FILE *out_file = fopen("output_file.jpg", "wb");

    // Read input data from file
    int width, height, pixelSize;
    unsigned char *srcBuf = tjLoadImage("input_file.jpg", &width, 0, &height, &pixelSize, 0);

    // Compress image
    tjCompress(handle, srcBuf, width, 0, height, pixelSize, jpegBuf, &jpegSize, TJSAMP_444, 75, 0);

    // Write output to file
    fwrite(jpegBuf, 1, jpegSize, out_file);

    // Clean up
    tjFree(srcBuf);
    tjFree(jpegBuf);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    tjDestroy(handle);

    assert_file_name_closed("input_file.jpg");
	assert_file_name_closed("output_file.jpg");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}