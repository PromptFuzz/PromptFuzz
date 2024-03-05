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
//<ID> 7881
//<Prompt> ["tj3DecompressToYUV8","tj3LoadImage16","tj3Compress16","tj3SaveImage8","tj3Set","tj3YUVPlaneHeight"]
/*<Combination>: [int tj3DecompressToYUV8(tjhandle handle, const unsigned char * jpegBuf, size_t jpegSize, unsigned char * dstBuf, int align),
    unsigned short *tj3LoadImage16(tjhandle handle, const char * filename, int * width, int align, int * height, int * pixelFormat),
    int tj3Compress16(tjhandle handle, const unsigned short * srcBuf, int width, int pitch, int height, int pixelFormat, unsigned char ** jpegBuf, size_t * jpegSize),
    int tj3SaveImage8(tjhandle handle, const char * filename, const unsigned char * buffer, int width, int pitch, int height, int pixelFormat),
    int tj3Set(tjhandle handle, int param, int value),
    int tj3YUVPlaneHeight(int componentID, int height, int subsamp)
*/
//<score> 1.3333334, nr_unique_branch: 1, p_cov: 0.6666667
//<Quality> {"diversity":9,"density":9,"covered_api":["tj3DecompressToYUV8","tj3LoadImage16","tj3Compress16","tj3Set"],"uncovered_api":["tj3SaveImage8","tj3YUVPlaneHeight"],"unique_branches":{"tj3LoadImage16":[[317,7,317,45,0]]},"library_calls":["tjInitDecompress","tj3LoadImage16","tj3Set","tjAlloc","tj3DecompressToYUV8","tj3Compress16","tjFree","tjFree","tjDestroy"]}
/**/

extern "C" int LLVMFuzzerTestOneInput_120(const uint8_t* f_data, size_t f_size) {
	if(f_size<=8) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
	//fuzzer shim end}




  tjhandle handle = tjInitDecompress();
  
  // Read the input data from memory
  FILE *in_file = fmemopen((void *)data, size, "rb");
  
  // Load the input image
  int width = 0, height = 0, pixelFormat = 0;
  unsigned short *srcBuf = tj3LoadImage16(handle, "", &width, fuzz_int32_t_1, &height, &pixelFormat);
  
  // Set decompression parameters
  tj3Set(handle, TJXOP_ROT180, fuzz_int32_t_2);
  
  // Decompress the input image to YUV8 format
  unsigned char *dstBuf = (unsigned char*) tjAlloc(width * height * tjPixelSize[TJPF_GRAY]);
  int align = 1;
  tj3DecompressToYUV8(handle, (unsigned char *)srcBuf, width * tjPixelSize[TJPF_RGB], dstBuf, align);
  
  // Compress the YUV8 image
  unsigned char *jpegBuf = NULL;
  size_t jpegSize = 0;
  int jpegPixelFormat = TJPF_GRAY;
  tj3Compress16(handle, (const unsigned short *)dstBuf, width, tjPixelSize[TJPF_GRAY] * width, height, jpegPixelFormat, &jpegBuf, &jpegSize);
  
  // Save the compressed image to file
  FILE *out_file = fopen("output_file", "wb");
  fwrite(jpegBuf, jpegSize, 1, out_file);
  assert_file_closed(&out_file);;
  
  // Clean up
  tjFree(dstBuf);
  tjFree(jpegBuf);
  tjDestroy(handle);
  assert_file_closed(&in_file);;
  
  assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}