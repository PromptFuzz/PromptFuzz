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
//<ID> 186
//<Prompt> ["tj3Compress8","tj3Free","tjDecodeYUV","tjPlaneWidth","tj3SaveImage12"]
/*<Combination>: [int tj3Compress8(tjhandle handle, const unsigned char * srcBuf, int width, int pitch, int height, int pixelFormat, unsigned char ** jpegBuf, size_t * jpegSize),
    void tj3Free(void * buffer),
    int tjDecodeYUV(tjhandle handle, const unsigned char * srcBuf, int align, int subsamp, unsigned char * dstBuf, int width, int pitch, int height, int pixelFormat, int flags),
    int tjPlaneWidth(int componentID, int width, int subsamp),
    int tj3SaveImage12(tjhandle handle, const char * filename, const short * buffer, int width, int pitch, int height, int pixelFormat)
*/
//<score> 0.96000004, nr_unique_branch: 2, p_cov: 0.4
//<Quality> {"diversity":10,"density":8,"covered_api":["tjPlaneWidth","tj3SaveImage12"],"uncovered_api":["tj3Compress8","tj3Free","tjDecodeYUV"],"unique_branches":{"tj3SaveImage12":[[439,20,439,27,1],[439,31,439,40,0]]},"library_calls":["tjInitDecompress","tjDecompressHeader3","tjPlaneWidth","tjAlloc","tjDecompress2","tj3SaveImage12","tjFree","tjFree","tjDestroy"]}
/**/


// Define the maximum size for output data
#define MAX_OUT_SIZE 1000000

extern "C" int LLVMFuzzerTestOneInput_7(const uint8_t* f_data, size_t f_size) {
	if(f_size<=4) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
	//fuzzer shim end}




    tjhandle handle = tjInitDecompress(); // Initialize the turbojpeg decompressor
    int width, height, subsamp;
    unsigned char *jpegBuf = NULL; // Pointer to the compressed JPEG data
    unsigned long jpegSize = 0; // Size of the compressed JPEG data

    // Create a FILE pointer to read input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file == NULL) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Get the width and height from the jpeg header
    tjDecompressHeader3(handle, (unsigned char*)data, size, &width, &height, &subsamp, NULL);

    // Allocate memory for the output image data
    int pitch = tjPlaneWidth(0, width, subsamp) * tjPixelSize[TJPF_RGB];
    unsigned char* imgBuf = (unsigned char*)tjAlloc(pitch * height);

    // Decompress the JPEG data into the output image buffer
    tjDecompress2(handle, (unsigned char*)data, size, imgBuf, width, pitch, height, TJPF_RGB, fuzz_int32_t_1);

    // Create a FILE pointer to write output data
    FILE *out_file = fopen("output_file", "wb");
    if (out_file == NULL) {
        tjFree(imgBuf);
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Save the output image buffer as a JPEG file
    tj3SaveImage12(handle, "output_file", (short*)imgBuf, width, pitch, height, TJPF_RGB);

    // Clean up allocated resources
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    tjFree(imgBuf);
    tjFree(jpegBuf);
    tjDestroy(handle);

    assert_file_name_closed("output_file");
	assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}