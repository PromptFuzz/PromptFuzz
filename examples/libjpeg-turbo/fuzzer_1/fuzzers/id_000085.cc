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
//<ID> 4619
//<Prompt> ["tjDecompressToYUVPlanes","tj3LoadImage12","tj3GetErrorStr","tjDecompressToYUV","tj3DecompressToYUVPlanes8"]
/*<Combination>: [int tjDecompressToYUVPlanes(tjhandle handle, const unsigned char * jpegBuf, unsigned long jpegSize, unsigned char ** dstPlanes, int width, int * strides, int height, int flags),
    short *tj3LoadImage12(tjhandle handle, const char * filename, int * width, int align, int * height, int * pixelFormat),
    char *tj3GetErrorStr(tjhandle handle),
    int tjDecompressToYUV(tjhandle handle, unsigned char * jpegBuf, unsigned long jpegSize, unsigned char * dstBuf, int flags),
    int tj3DecompressToYUVPlanes8(tjhandle handle, const unsigned char * jpegBuf, size_t jpegSize, unsigned char ** dstPlanes, int * strides)
*/
//<score> 3, nr_unique_branch: 2, p_cov: 1
//<Quality> {"diversity":6,"density":6,"covered_api":["tjDecompressToYUVPlanes","tj3LoadImage12","tj3GetErrorStr","tjDecompressToYUV","tj3DecompressToYUVPlanes8"],"uncovered_api":[],"unique_branches":{"tjDecompressToYUVPlanes":[[290,7,290,12,0]],"tjDecompressToYUV2":[[290,7,290,12,0]]},"library_calls":["tjDecompressToYUVPlanes","tj3LoadImage12","tj3GetErrorStr","tjDecompressToYUV","tj3DecompressToYUVPlanes8","tjDestroy"]}
/**/

extern "C" int LLVMFuzzerTestOneInput_85(const uint8_t* f_data, size_t f_size) {
	if(f_size<=4) return 0;

	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(f_data, sizeof(uint8_t), f_size, input_file_ptr);
	fclose(input_file_ptr);

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
	//fuzzer shim end}




    tjhandle handle = NULL;
    unsigned char *jpegBuf = NULL;
    unsigned long jpegSize = 0;
    unsigned char *dstBuf = NULL;
    unsigned char *dstPlanes[3] = { NULL, NULL, NULL };
    int width = 0;
    int height = 0;
    int pixelFormat = 0;
    int strides[3] = { 0, 0, 0 };
    int flags = 0;
  
    // Create a FILE pointer for reading
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file == NULL) {
        // Handle error
        assert_file_closed(&in_file);
	return 0;
    }
  
    // Read the input file and decompress to YUV planes
    if (tjDecompressToYUVPlanes(handle, jpegBuf, jpegSize, dstPlanes, width, strides,
        height, flags) != 0) {
        // Handle error
    }
  
    // Load JPEG image from file
    if (tj3LoadImage12(handle, "input_file", &width, fuzz_int32_t_1, &height, &pixelFormat) == NULL) {
        // Handle error
    }
  
    // Get error message
    char *errorMsg = tj3GetErrorStr(handle);
    // Handle error message
  
    // Decompress JPEG to YUV buffer
    if (tjDecompressToYUV(handle, jpegBuf, jpegSize, dstBuf, flags) != 0) {
        // Handle error
    }
  
    // Decompress JPEG to YUV planes (8-bit)
    if (tj3DecompressToYUVPlanes8(handle, jpegBuf, jpegSize, dstPlanes, strides) != 0) {
        // Handle error
    }
  
    // Release all allocated resources
    tjDestroy(handle);
    assert_file_closed(&in_file);;
    free(jpegBuf);
    free(dstBuf);
  
    assert_file_name_closed("input_file");
	assert_file_closed(&in_file);
	return 0;
}