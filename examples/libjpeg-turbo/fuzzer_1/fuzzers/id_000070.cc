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
//<ID> 3085
//<Prompt> ["tj3Decompress16","tj3LoadImage12","tjPlaneSizeYUV","tj3Get","tj3Free","tjEncodeYUVPlanes"]
/*<Combination>: [int tj3Decompress16(tjhandle handle, const unsigned char * jpegBuf, size_t jpegSize, unsigned short * dstBuf, int pitch, int pixelFormat),
    short *tj3LoadImage12(tjhandle handle, const char * filename, int * width, int align, int * height, int * pixelFormat),
    unsigned long tjPlaneSizeYUV(int componentID, int width, int stride, int height, int subsamp),
    int tj3Get(tjhandle handle, int param),
    void tj3Free(void * buffer),
    int tjEncodeYUVPlanes(tjhandle handle, const unsigned char * srcBuf, int width, int pitch, int height, int pixelFormat, unsigned char ** dstPlanes, int * strides, int subsamp, int flags)
*/
//<score> 0.33333334, nr_unique_branch: 1, p_cov: 0.16666667
//<Quality> {"diversity":8,"density":8,"covered_api":["tjEncodeYUVPlanes"],"uncovered_api":["tj3Decompress16","tj3LoadImage12","tjPlaneSizeYUV","tj3Get","tj3Free"],"unique_branches":{"tjEncodeYUVPlanes":[[1366,7,1366,18,0]]},"library_calls":["tjInitDecompress","tjDecompressHeader3","tjEncodeYUVPlanes","tjFree","tjDestroy"]}
/**/


// LLVM Fuzzer Test One Input function
extern "C" int LLVMFuzzerTestOneInput_70(const uint8_t* f_data, size_t f_size) {
	if(f_size<=0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    tjhandle handle = tjInitDecompress();
    if (!handle) {
        return 0; // Return 0 to indicate failure
    }

    // Create a FILE pointer for reading input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        tjDestroy(handle);
        assert_file_closed(&in_file);
	return 0; // Return 0 to indicate failure
    }

    // Read the input data using libjpeg-turbo
    int width, height, subsamp, pixelFormat;
    unsigned char *jpegBuf = nullptr;
    unsigned long jpegSize;
    tjDecompressHeader3(handle, jpegBuf, jpegSize, &width, &height, &subsamp, &pixelFormat);

    // Create a FILE pointer for writing output data
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        tjFree(jpegBuf);
        assert_file_closed(&in_file);;
        tjDestroy(handle);
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0; // Return 0 to indicate failure
    }

    // Write the output data using libjpeg-turbo
    unsigned char **dstPlanes = nullptr;
    int *strides = nullptr;
    int flags = 0;
    tjEncodeYUVPlanes(handle, jpegBuf, width, width * tjPixelSize[pixelFormat], height, pixelFormat, dstPlanes, strides, subsamp, flags);

    // Cleanup
    tjFree(jpegBuf);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    tjDestroy(handle);

    assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0; // Return 0 to indicate success
}