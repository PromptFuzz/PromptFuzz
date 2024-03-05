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
//<ID> 5473
//<Prompt> ["tjCompressFromYUV","tj3Init","tj3Compress8","tj3CompressFromYUVPlanes8","tj3SaveImage8","tjDestroy"]
/*<Combination>: [int tjCompressFromYUV(tjhandle handle, const unsigned char * srcBuf, int width, int align, int height, int subsamp, unsigned char ** jpegBuf, unsigned long * jpegSize, int jpegQual, int flags),
    tjhandle tj3Init(int initType),
    int tj3Compress8(tjhandle handle, const unsigned char * srcBuf, int width, int pitch, int height, int pixelFormat, unsigned char ** jpegBuf, size_t * jpegSize),
    int tj3CompressFromYUVPlanes8(tjhandle handle, const unsigned char *const * srcPlanes, int width, const int * strides, int height, unsigned char ** jpegBuf, size_t * jpegSize),
    int tj3SaveImage8(tjhandle handle, const char * filename, const unsigned char * buffer, int width, int pitch, int height, int pixelFormat),
    int tjDestroy(tjhandle handle)
*/
//<score> 1.5, nr_unique_branch: 2, p_cov: 0.5
//<Quality> {"diversity":4,"density":4,"covered_api":["tjCompressFromYUV","tj3Init","tjDestroy"],"uncovered_api":["tj3Compress8","tj3CompressFromYUVPlanes8","tj3SaveImage8"],"unique_branches":{"tjCompressFromYUV":[[301,7,301,12,0]],"tj3Init":[[538,23,538,45,0]]},"library_calls":["tj3Init","tjCompressFromYUV","tjDestroy","tjFree"]}
/*Here is the implementation of the fuzz driver following the instructions provided:

*/


extern "C" int LLVMFuzzerTestOneInput_91(const uint8_t* f_data, size_t f_size) {
	if(f_size<=4) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
	//fuzzer shim end}




  // Create a handle for libjpeg-turbo
  tjhandle handle = tj3Init(fuzz_int32_t_1);

  // Create variables for the output data
  unsigned char *jpegBuf = nullptr;
  size_t jpegSize = 0;

  // Create variables for the input data
  const unsigned char *srcBuf = data;
  int width = 0;
  int align = 1;
  int height = 0;
  int subsamp = TJSAMP_420;
  int jpegQual = 90;
  int flags = 0;

  // Compress the input data to JPEG format
  int compressResult = tjCompressFromYUV(handle, srcBuf, width, align, height,
                                         subsamp, &jpegBuf, &jpegSize,
                                         jpegQual, flags);
  if (compressResult == 0) {
    // Write the JPEG data to a file
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
      fwrite(jpegBuf, 1, jpegSize, out_file);
      assert_file_closed(&out_file);;
    }
  }

  // Free the allocated memory
  tjDestroy(handle);
  tjFree(jpegBuf);

  return 0;
}