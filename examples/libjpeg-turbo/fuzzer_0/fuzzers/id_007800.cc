#include "FDSan.h"
#include <turbojpeg.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 7800
//<Prompt> ["tj3DecompressToYUV8","tj3LoadImage16","tj3Compress16","tj3SaveImage8","tj3Compress12","tj3Set","tj3YUVPlaneHeight"]
/*<Combination>: [int tj3DecompressToYUV8(tjhandle handle, const unsigned char * jpegBuf, size_t jpegSize, unsigned char * dstBuf, int align),
    unsigned short *tj3LoadImage16(tjhandle handle, const char * filename, int * width, int align, int * height, int * pixelFormat),
    int tj3Compress16(tjhandle handle, const unsigned short * srcBuf, int width, int pitch, int height, int pixelFormat, unsigned char ** jpegBuf, size_t * jpegSize),
    int tj3SaveImage8(tjhandle handle, const char * filename, const unsigned char * buffer, int width, int pitch, int height, int pixelFormat),
    int tj3Compress12(tjhandle handle, const short * srcBuf, int width, int pitch, int height, int pixelFormat, unsigned char ** jpegBuf, size_t * jpegSize),
    int tj3Set(tjhandle handle, int param, int value),
    int tj3YUVPlaneHeight(int componentID, int height, int subsamp)
*/
//<score> 1.7142857, nr_unique_branch: 1, p_cov: 0.85714287
//<Quality> {"diversity":10,"density":10,"covered_api":["tj3DecompressToYUV8","tj3LoadImage16","tj3Compress16","tj3SaveImage8","tj3Set","tj3YUVPlaneHeight"],"uncovered_api":["tj3Compress12"],"unique_branches":{"tj3Set":[[633,9,633,33,0]]},"library_calls":["tjInitDecompress","tj3DecompressToYUV8","tj3LoadImage16","tj3Compress16","tj3SaveImage8","tj3Set","tj3YUVPlaneHeight","tjFree","tjFree","tjDestroy"]}
/*Here is the C++ program that achieves the event using the libjpeg-turbo APIs:

*/


extern "C" int LLVMFuzzerTestOneInput_119(const uint8_t *data, size_t size) {
	if(size<=0) return 0;

	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);

    // Step 1: Define variables
    tjhandle handle;
    unsigned char *jpegBuf = NULL;
    size_t jpegSize;
    unsigned char *dstBuf = NULL;
    int width, height, pixelFormat;

    // Step 2: Create the TurboJPEG decompressor instance
    handle = tjInitDecompress();

    // Step 3: Read the input data from memory
    FILE *in_file = fmemopen((void *)data, size, "rb");
    fseek(in_file, 0, SEEK_END);
    long file_size = ftell(in_file);
    rewind(in_file);
    unsigned char *jpegData = (unsigned char *)malloc(file_size);
    fread(jpegData, 1, file_size, in_file);
    assert_file_closed(&in_file);;

    // Step 4: Decompress the JPEG image to YUV8 format
    tj3DecompressToYUV8(handle, jpegData, file_size, dstBuf, TJ_420);

    // Step 5: Load a 16-bit grayscale image
    tj3LoadImage16(handle, "input_file", &width, 0, &height, &pixelFormat);

    // Step 6: Compress the loaded image to JPEG format
    tj3Compress16(handle, (const unsigned short *)dstBuf, width, width * 2, height, pixelFormat, &jpegBuf, &jpegSize);

    // Step 7: Save the compressed image to a file
    tj3SaveImage8(handle, "output_file", jpegBuf, width, width, height, pixelFormat);

    // Step 8: Set TurboJPEG options
    tj3Set(handle, TJXOPT_GRAY, 1);

    // Step 9: Get the YUV plane height
    tj3YUVPlaneHeight(0, height, TJ_420);

    // Step 10: Cleanup and release resources
    tjFree(jpegBuf);
    tjFree(dstBuf);
    free(jpegData);
    tjDestroy(handle);

    assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_closed(&in_file);
	return 0;
}