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
//<ID> 7759
//<Prompt> ["tj3DecompressToYUV8","tj3LoadImage16","tj3Compress16","tj3YUVBufSize","tj3Set","tj3YUVPlaneHeight"]
/*<Combination>: [int tj3DecompressToYUV8(tjhandle handle, const unsigned char * jpegBuf, size_t jpegSize, unsigned char * dstBuf, int align),
    unsigned short *tj3LoadImage16(tjhandle handle, const char * filename, int * width, int align, int * height, int * pixelFormat),
    int tj3Compress16(tjhandle handle, const unsigned short * srcBuf, int width, int pitch, int height, int pixelFormat, unsigned char ** jpegBuf, size_t * jpegSize),
    size_t tj3YUVBufSize(int width, int align, int height, int subsamp),
    int tj3Set(tjhandle handle, int param, int value),
    int tj3YUVPlaneHeight(int componentID, int height, int subsamp)
*/
//<score> 2, nr_unique_branch: 1, p_cov: 1
//<Quality> {"diversity":10,"density":10,"covered_api":["tj3DecompressToYUV8","tj3LoadImage16","tj3Compress16","tj3YUVBufSize","tj3Set","tj3YUVPlaneHeight"],"uncovered_api":[],"unique_branches":{"tj3LoadImage16":[[301,30,301,39,0]]},"library_calls":["tjInitDecompress","tj3DecompressToYUV8","tj3LoadImage16","tj3Compress16","tj3YUVBufSize","tj3Set","tj3YUVPlaneHeight","tjFree","tjFree","tjDestroy"]}
/*Here is the C++ code that implements the steps mentioned above:

*/


extern "C" int LLVMFuzzerTestOneInput_118(const uint8_t* f_data, size_t f_size) {
	if(f_size<=24) return 0;

	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(f_data, sizeof(uint8_t), f_size, input_file_ptr);
	fclose(input_file_ptr);

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
	//fuzzer shim end}




    // Step 4: Create a FILE * variable for reading input data
    FILE *in_file = fmemopen((void *) data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Step 5: Get the file descriptor for reading
    int in_fd = fuzz_fileno(in_file);
    if (in_fd < 0) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
    }

    // Step 6: Use the file name directly
    const char *input_file = "input_file";
    const char *output_file = "output_file";

    // Step 1: Define the event we want to achieve using libjpeg-turbo APIs
    // Decompress JPEG image to YUV format
    tjhandle handle = tjInitDecompress();
    int width, height, pixelFormat;
    unsigned char *jpegBuf = nullptr;
    unsigned long jpegSize;
    unsigned char *dstBuf = nullptr;

    // Step 2: Call the libjpeg-turbo APIs to achieve the event
    tj3DecompressToYUV8(handle, jpegBuf, jpegSize, dstBuf, fuzz_int32_t_1);
    tj3LoadImage16(handle, input_file, &width, fuzz_int32_t_2, &height, &pixelFormat);
    tj3Compress16(handle, (const unsigned short *) dstBuf, width, width * 2, height, pixelFormat, &jpegBuf, &jpegSize);
    size_t yuvBufSize = tj3YUVBufSize(width, fuzz_int32_t_3, height, TJSAMP_444);
    tj3Set(handle, fuzz_int32_t_4, fuzz_int32_t_5);
    tj3YUVPlaneHeight(fuzz_int32_t_6, height, TJSAMP_444);

    // Clean up allocated resources
    tjFree(jpegBuf);
    tjFree(dstBuf);
    tjDestroy(handle);
    assert_file_closed(&in_file);;

    assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
}