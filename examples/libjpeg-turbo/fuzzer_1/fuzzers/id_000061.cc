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
//<ID> 2514
//<Prompt> ["tj3Decompress12","tj3CompressFromYUV8","tj3Compress12","tj3EncodeYUV8","tjDecompressHeader"]
/*<Combination>: [int tj3Decompress12(tjhandle handle, const unsigned char * jpegBuf, size_t jpegSize, short * dstBuf, int pitch, int pixelFormat),
    int tj3CompressFromYUV8(tjhandle handle, const unsigned char * srcBuf, int width, int align, int height, unsigned char ** jpegBuf, size_t * jpegSize),
    int tj3Compress12(tjhandle handle, const short * srcBuf, int width, int pitch, int height, int pixelFormat, unsigned char ** jpegBuf, size_t * jpegSize),
    int tj3EncodeYUV8(tjhandle handle, const unsigned char * srcBuf, int width, int pitch, int height, int pixelFormat, unsigned char * dstBuf, int align),
    int tjDecompressHeader(tjhandle handle, unsigned char * jpegBuf, unsigned long jpegSize, int * width, int * height)
*/
//<score> 0.6, nr_unique_branch: 2, p_cov: 0.2
//<Quality> {"diversity":12,"density":12,"covered_api":["tjDecompressHeader"],"uncovered_api":["tj3Decompress12","tj3CompressFromYUV8","tj3Compress12","tj3EncodeYUV8"],"unique_branches":{"pre_process_data":[[147,10,147,37,1],[180,34,180,75,1]]},"library_calls":["tjInitDecompress","tjDecompressHeader","tjDecompress2","tjDestroy","tjInitCompress","tjCompress2","tjDestroy","tjFree"]}
/**/


// Include the libjpeg-turbo header

extern "C" int LLVMFuzzerTestOneInput_61(const uint8_t* f_data, size_t f_size) {
	if(f_size<=12) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
	//fuzzer shim end}




  // Step 1: Decompress JPEG
  tjhandle handle = tjInitDecompress();
  int width, height;
  unsigned char *jpegBuf = (unsigned char *)data;
  unsigned long jpegSize = size;

  if (tjDecompressHeader(handle, jpegBuf, jpegSize, &width, &height) != 0) {
    tjDestroy(handle);
    return 0;
  }

  // Allocate buffer for decompressed image
  unsigned char *dstBuf = (unsigned char *)malloc(width * height * 3);

  if (tjDecompress2(handle, jpegBuf, jpegSize, dstBuf, width, 0, height, TJPF_RGB, fuzz_int32_t_1) != 0) {
    tjDestroy(handle);
    free(dstBuf);
    return 0;
  }

  tjDestroy(handle);

  // Step 2: Compress to JPEG
  tjhandle handle2 = tjInitCompress();
  unsigned char *jpegBuf2;
  unsigned long jpegSize2;

  if (tjCompress2(handle2, dstBuf, width, 0, height, TJPF_RGB, &jpegBuf2, &jpegSize2, TJSAMP_444, fuzz_int32_t_2, fuzz_int32_t_3) != 0) {
    tjDestroy(handle2);
    free(dstBuf);
    tjFree(jpegBuf2);
    return 0;
  }

  tjDestroy(handle2);
  free(dstBuf);
  tjFree(jpegBuf2);

  return 0;
}