#include "FDSan.h"
#include <turbojpeg.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 90
//<Prompt> ["tjDecompress","tjDecompressToYUV","tjBufSizeYUV","tj3Decompress8","tjGetErrorCode"]
/*<Combination>: [int tjDecompress(tjhandle  id_000090_handle, unsigned char * jpegBuf, unsigned long jpegSize, unsigned char * dstBuf, int width, int pitch, int height, int pixelSize, int flags),
    int tjDecompressToYUV(tjhandle  id_000090_handle, unsigned char * jpegBuf, unsigned long jpegSize, unsigned char * dstBuf, int flags),
    unsigned long tjBufSizeYUV(int width, int height, int subsamp),
    int tj3Decompress8(tjhandle  id_000090_handle, const unsigned char * jpegBuf, size_t jpegSize, unsigned char * dstBuf, int pitch, int pixelFormat),
    int tjGetErrorCode(tjhandle  id_000090_handle)
*/
//<score> 1.6, nr_unique_branch: 3, p_cov: 0.4
//<Quality> {"diversity":5,"density":5,"covered_api":["tjDecompress","tjGetErrorCode"],"uncovered_api":["tjDecompressToYUV","tjBufSizeYUV","tj3Decompress8"],"unique_branches":{"tj3Decompress8":[[210,7,210,17,1]],"getPixelFormat":[[311,7,311,21,0],[312,9,312,23,1]]},"library_calls":["tjInitDecompress","tjDecompressHeader","tjDecompress","tjGetErrorCode","tjDestroy"]}
/**/


// Handle for libjpeg-turbo
tjhandle  id_000090_handle;

extern "C" int LLVMFuzzerTestOneInput_5(const uint8_t *data, size_t size) {
	if(size<=0) return 0;

  // Step 1: Create temporary input and output files
  FILE *in_file = fmemopen((void *)data, size, "rb");
  FILE *out_file = fopen("output_file", "wb");

  // Step 2: Read input data from the file
  unsigned char *jpegBuf = NULL;
  unsigned long jpegSize = 0;
  fseek(in_file, 0, SEEK_END);
  jpegSize = ftell(in_file);
  fseek(in_file, 0, SEEK_SET);
  jpegBuf = (unsigned char *)malloc(jpegSize);
  fread(jpegBuf, 1, jpegSize, in_file);

  // Step 3: Initialize libjpeg-turbo
   id_000090_handle = tjInitDecompress();

  // Step 4: Decompress JPEG data to RGB
  int width, height, pixelSize, pitch;
  unsigned char *dstBuf = NULL;
  tjDecompressHeader( id_000090_handle, jpegBuf, jpegSize, &width, &height);
  pixelSize = tjPixelSize[TJPF_RGB];
  pitch = tjPixelSize[TJPF_RGB] * width;
  dstBuf = (unsigned char *)malloc(pitch * height);
  tjDecompress( id_000090_handle, jpegBuf, jpegSize, dstBuf, width, pitch, height,
               pixelSize, 0);

  // Step 5: Write output data to the file
  fwrite(dstBuf, 1, pitch * height, out_file);

  // Step 6: Error checking
  int errorCode = tjGetErrorCode( id_000090_handle);
  if (errorCode != 0) {
    // Handle error
  }

  // Step 7: Clean up resources
  tjDestroy( id_000090_handle);
  assert_file_closed(&in_file);;
  assert_file_closed(&out_file);;
  free(jpegBuf);
  free(dstBuf);

  assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}