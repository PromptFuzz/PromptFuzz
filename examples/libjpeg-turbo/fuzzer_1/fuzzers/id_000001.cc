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
//<ID> 52
//<Prompt> ["tjDecompress","tjDecompressToYUV","tj3Decompress8","tjGetErrorCode","tjLoadImage"]
/*<Combination>: [int tjDecompress(tjhandle handle, unsigned char * jpegBuf, unsigned long jpegSize, unsigned char * dstBuf, int width, int pitch, int height, int pixelSize, int flags),
    int tjDecompressToYUV(tjhandle handle, unsigned char * jpegBuf, unsigned long jpegSize, unsigned char * dstBuf, int flags),
    int tj3Decompress8(tjhandle handle, const unsigned char * jpegBuf, size_t jpegSize, unsigned char * dstBuf, int pitch, int pixelFormat),
    int tjGetErrorCode(tjhandle handle),
    unsigned char *tjLoadImage(const char * filename, int * width, int align, int * height, int * pixelFormat, int flags)
*/
//<score> 3, nr_unique_branch: 2, p_cov: 1
//<Quality> {"diversity":8,"density":8,"covered_api":["tjDecompress","tjDecompressToYUV","tj3Decompress8","tjGetErrorCode","tjLoadImage"],"uncovered_api":[],"unique_branches":{"tjDecompressToYUV2":[[2559,7,2559,22,0],[2590,7,2590,41,1]]},"library_calls":["tjInitDecompress","tjLoadImage","tjDecompress","tjDecompressToYUV","tj3Decompress8","tjGetErrorCode","tjFree","tjDestroy"]}
/**/



extern "C" int LLVMFuzzerTestOneInput_1(const uint8_t* f_data, size_t f_size) {
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




  tjhandle handle = tjInitDecompress();
  unsigned char* dstBuf = nullptr;
  int width, height, pixelFormat;
  int flags = 0;

  FILE* in_file = fmemopen((void*)data, size, "rb");
  int in_fd = fuzz_fileno(in_file);

  // Read the input image using tjLoadImage
  tjLoadImage("input_file", &width, fuzz_int32_t_1, &height, &pixelFormat, flags);

  // Allocate memory for the destination buffer
  size_t dstSize = width * height * tjPixelSize[pixelFormat];
  dstBuf = new unsigned char[dstSize];

  // Decompress the input JPEG image
  tjDecompress(handle, nullptr, 0, dstBuf, width, 0, height, pixelFormat, flags);
  tjDecompressToYUV(handle, nullptr, 0, dstBuf, flags);
  tj3Decompress8(handle, nullptr, 0, dstBuf, 0, pixelFormat);

  // Check for any error code
  int errorCode = tjGetErrorCode(handle);

  // Cleanup
  tjFree(dstBuf);
  tjDestroy(handle);
  assert_file_closed(&in_file);;

  assert_file_name_closed("input_file");
	assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
}