#include "FDSan.h"
#include <turbojpeg.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 4784
//<Prompt> ["tj3SaveImage12","tjBufSizeYUV","tjDestroy","tj3GetScalingFactors","tjGetScalingFactors"]
/*<Combination>: [int tj3SaveImage12(tjhandle handle, const char * filename, const short * buffer, int width, int pitch, int height, int pixelFormat),
    unsigned long tjBufSizeYUV(int width, int height, int subsamp),
    int tjDestroy(tjhandle handle),
    tjscalingfactor *tj3GetScalingFactors(int * numScalingFactors),
    tjscalingfactor *tjGetScalingFactors(int * numscalingfactors)
*/
//<score> 0.8, nr_unique_branch: 1, p_cov: 0.4
//<Quality> {"diversity":5,"density":5,"covered_api":["tjBufSizeYUV","tjDestroy"],"uncovered_api":["tj3SaveImage12","tj3GetScalingFactors","tjGetScalingFactors"],"unique_branches":{"tj3CompressFromYUV8":[[1645,39,1645,48,0]]},"library_calls":["tjInitCompress","tjBufSizeYUV","tjCompressFromYUV","tjFree","tjDestroy"]}
/**/


extern "C" int LLVMFuzzerTestOneInput_86(const uint8_t *data, size_t size) {
	if(size<=0) return 0;

  // Step 1: Define the event to achieve
  const char *filename = "output_file.jpg";
  const int width = 640;
  const int height = 480;
  const int subsamp = TJSAMP_444;
  tjhandle handle = tjInitCompress();

  // Step 2: Use the libjpeg-turbo APIs to achieve the event
  unsigned long bufSize = tjBufSizeYUV(width, height, subsamp);
  unsigned char *yuvBuf = static_cast<unsigned char *>(malloc(bufSize));
  int jpegQual = 90;
  unsigned char *jpegBuf = nullptr;
  unsigned long jpegSize = 0;

  // Create a FILE pointer to read the input data
  FILE *in_file = fmemopen((void *)data, size, "rb");

  // Read the YUV data from the input file
  fread(yuvBuf, 1, bufSize, in_file);

  // Compress the YUV data to JPEG
  tjCompressFromYUV(handle, yuvBuf, width, 0, height, subsamp, &jpegBuf, &jpegSize, jpegQual, 0);

  // Write the JPEG data to the output file
  FILE *out_file = fopen(filename, "wb");
  fwrite(jpegBuf, jpegSize, 1, out_file);
  assert_file_closed(&out_file);;

  // Release allocated resources
  free(yuvBuf);
  tjFree(jpegBuf);
  tjDestroy(handle);

  assert_file_name_closed("output_file.jpg");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}