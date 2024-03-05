#include "FDSan.h"
#include <turbojpeg.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 1970
//<Prompt> ["tjDecompressToYUVPlanes","TJBUFSIZEYUV","tjBufSizeYUV","tj3Compress12","tjPlaneWidth"]
/*<Combination>: [int tjDecompressToYUVPlanes(tjhandle handle, const unsigned char * jpegBuf, unsigned long jpegSize, unsigned char ** dstPlanes, int width, int * strides, int height, int flags),
    unsigned long TJBUFSIZEYUV(int width, int height, int jpegSubsamp),
    unsigned long tjBufSizeYUV(int width, int height, int subsamp),
    int tj3Compress12(tjhandle handle, const short * srcBuf, int width, int pitch, int height, int pixelFormat, unsigned char ** jpegBuf, size_t * jpegSize),
    int tjPlaneWidth(int componentID, int width, int subsamp)
*/
//<score> 3, nr_unique_branch: 4, p_cov: 0.6
//<Quality> {"diversity":11,"density":11,"covered_api":["tjDecompressToYUVPlanes","tjBufSizeYUV","tjPlaneWidth"],"uncovered_api":["TJBUFSIZEYUV","tj3Compress12"],"unique_branches":{"tj3DecompressToYUVPlanes8":[[2327,7,2327,38,0]],"tjDecompressToYUVPlanes":[[2466,15,2466,24,1],[2469,29,2469,46,1],[2472,7,2472,17,0]]},"library_calls":["tjInitDecompress","tjAlloc","tjBufSizeYUV","tjAlloc","tjPlaneWidth","tjDecompressToYUVPlanes","tjFree","tjFree","tjDestroy"]}
/**/


extern "C" int LLVMFuzzerTestOneInput_50(const uint8_t *data, size_t size) {
	if(size<=0) return 0;

  // Create handle for decompression
  tjhandle decompressHandle = tjInitDecompress();
  if (!decompressHandle) {
    return 0;
  }

  // Read JPEG data from input file
  FILE *in_file = fmemopen((void *)data, size, "rb");
  if (!in_file) {
    tjDestroy(decompressHandle);
    assert_file_closed(&in_file);
	return 0;
  }

  // Get JPEG file size
  fseek(in_file, 0, SEEK_END);
  long jpegSize = ftell(in_file);
  fseek(in_file, 0, SEEK_SET);

  // Allocate memory for JPEG data
  unsigned char *jpegBuf = (unsigned char *)tjAlloc(jpegSize);
  if (!jpegBuf) {
    assert_file_closed(&in_file);;
    tjDestroy(decompressHandle);
    assert_file_closed(&in_file);
	return 0;
  }

  // Read JPEG data into buffer
  fread(jpegBuf, jpegSize, 1, in_file);
  assert_file_closed(&in_file);;

  // Decompress JPEG to YUV planes
  const int width = 1920; // Example width
  const int height = 1080; // Example height
  const int subsamp = TJSAMP_420; // Example subsampling
  const int numPlanes = 3; // Number of planes (Y, U, V)
  unsigned char *dstPlanes[numPlanes];
  int strides[numPlanes];
  unsigned long yuvSize = tjBufSizeYUV(width, height, subsamp);

  for (int i = 0; i < numPlanes; i++) {
    dstPlanes[i] = (unsigned char *)tjAlloc(yuvSize);
    strides[i] = tjPlaneWidth(i, width, subsamp);
  }

  tjDecompressToYUVPlanes(decompressHandle, jpegBuf, jpegSize, dstPlanes,
                          width, strides, height, 0);

  // Cleanup
  for (int i = 0; i < numPlanes; i++) {
    tjFree(dstPlanes[i]);
  }
  tjFree(jpegBuf);
  tjDestroy(decompressHandle);

  assert_file_closed(&in_file);
	return 0;
}