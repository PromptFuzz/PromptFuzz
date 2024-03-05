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
//<ID> 995
//<Prompt> ["tj3Decompress8","tjInitDecompress","tj3Decompress12","tjCompressFromYUVPlanes","tjCompress2"]
/*<Combination>: [int tj3Decompress8(tjhandle handle, const unsigned char * jpegBuf, size_t jpegSize, unsigned char * dstBuf, int pitch, int pixelFormat),
    tjhandle tjInitDecompress(),
    int tj3Decompress12(tjhandle handle, const unsigned char * jpegBuf, size_t jpegSize, short * dstBuf, int pitch, int pixelFormat),
    int tjCompressFromYUVPlanes(tjhandle handle, const unsigned char ** srcPlanes, int width, const int * strides, int height, int subsamp, unsigned char ** jpegBuf, unsigned long * jpegSize, int jpegQual, int flags),
    int tjCompress2(tjhandle handle, const unsigned char * srcBuf, int width, int pitch, int height, int pixelFormat, unsigned char ** jpegBuf, unsigned long * jpegSize, int jpegSubsamp, int jpegQual, int flags)
*/
//<score> 0.8, nr_unique_branch: 1, p_cov: 0.4
//<Quality> {"diversity":8,"density":8,"covered_api":["tjInitDecompress","tjCompressFromYUVPlanes"],"uncovered_api":["tj3Decompress8","tj3Decompress12","tjCompress2"],"unique_branches":{"tj3DecompressToYUV8":[[2503,7,2503,15,0]]},"library_calls":["tjInitDecompress","tjDecompressHeader3","tjDecompressToYUV2","tjInitCompress","tjCompressFromYUVPlanes","tjDestroy","tjDestroy"]}
/**/


extern "C" int LLVMFuzzerTestOneInput_36(const uint8_t* f_data, size_t f_size) {
	if(f_size<=8) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
	//fuzzer shim end}




  // Create a decompressor instance
  tjhandle decompressor = tjInitDecompress();
  if (decompressor == nullptr) {
    return 0;
  }

  // Allocate buffers for input and output data
  unsigned char *jpegBuf = new unsigned char[size];
  memcpy(jpegBuf, data, size);
  unsigned char *dstBuf = nullptr;
  int width = 0, height = 0, subsamp = 0, pixelFormat = 0;
  
  // Decompress the input data
  int res = tjDecompressHeader3(decompressor, jpegBuf, size, &width, &height, &subsamp, &pixelFormat);
  if (res == 0) {
    int pitch = tjPixelSize[pixelFormat] * width;
    dstBuf = new unsigned char[pitch * height];
    res = tjDecompressToYUV2(decompressor, jpegBuf, size, dstBuf, width, pitch, height, fuzz_int32_t_1);
    if (res != 0) {
      delete[] dstBuf;
      dstBuf = nullptr;
    }
  }

  // Create a compressor instance
  tjhandle compressor = tjInitCompress();
  if (compressor == nullptr) {
    delete[] jpegBuf;
    delete[] dstBuf;
    tjDestroy(decompressor);
    return 0;
  }

  // Allocate buffers for YUV planes
  const unsigned char *srcPlanes[3] = {nullptr, nullptr, nullptr};
  int strides[3] = {0, 0, 0};
  int jpegQual = 90;
  unsigned long jpegSize = 0;

  // Compress the YUV planes
  res = tjCompressFromYUVPlanes(compressor, srcPlanes, width, strides, height, subsamp, &jpegBuf, &jpegSize, jpegQual, fuzz_int32_t_2);

  // Cleanup
  delete[] jpegBuf;
  delete[] dstBuf;
  tjDestroy(decompressor);
  tjDestroy(compressor);

  return 0;
}