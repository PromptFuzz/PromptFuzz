#include "FDSan.h"
#include <turbojpeg.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 6852
//<Prompt> ["tj3SetCroppingRegion","tj3CompressFromYUVPlanes8","tj3DecompressHeader","tj3Set","tj3Compress16"]
/*<Combination>: [int tj3SetCroppingRegion(tjhandle handle, tjregion croppingRegion),
    int tj3CompressFromYUVPlanes8(tjhandle handle, const unsigned char *const * srcPlanes, int width, const int * strides, int height, unsigned char ** jpegBuf, size_t * jpegSize),
    int tj3DecompressHeader(tjhandle handle, const unsigned char * jpegBuf, size_t jpegSize),
    int tj3Set(tjhandle handle, int param, int value),
    int tj3Compress16(tjhandle handle, const unsigned short * srcBuf, int width, int pitch, int height, int pixelFormat, unsigned char ** jpegBuf, size_t * jpegSize)
*/
//<score> 1.3333334, nr_unique_branch: 1, p_cov: 1
//<Quality> {"diversity":15,"density":10,"covered_api":["tj3SetCroppingRegion","tj3CompressFromYUVPlanes8","tj3DecompressHeader","tj3Set","tj3Compress16"],"uncovered_api":[],"unique_branches":{"tj3SetCroppingRegion":[[1891,7,1891,37,0]]},"library_calls":["tjInitCompress","tjInitDecompress","tjInitTransform","tj3SetCroppingRegion","tjPlaneSizeYUV","tjPlaneWidth","tjPlaneWidth","tj3CompressFromYUVPlanes8","tj3DecompressHeader","tj3Set","tj3Compress16","tjDestroy","tjDestroy","tjDestroy","tjFree"]}
/**/


extern "C" int LLVMFuzzerTestOneInput_104(const uint8_t *data, size_t size) {
	if(size<=0) return 0;

  tjhandle compressHandle = tjInitCompress();
  tjhandle decompressHandle = tjInitDecompress();
  tjhandle transformHandle = tjInitTransform();
  
  tjregion croppingRegion;
  croppingRegion.x = 0;
  croppingRegion.y = 0;
  croppingRegion.w = 100;
  croppingRegion.h = 100;
  
  // Set cropping region
  tj3SetCroppingRegion(compressHandle, croppingRegion);
  
  // Compress from YUV planes
  unsigned char** srcPlanes = new unsigned char*[3];
  int strides[3];
  int width = 100;
  int height = 100;
  int subsamp = TJSAMP_420;
  size_t jpegSize;
  unsigned char* jpegBuf;
  
  // Allocate memory for source planes
  for (int i = 0; i < 3; i++) {
    srcPlanes[i] = new unsigned char[tjPlaneSizeYUV(i, width, tjPlaneWidth(i, width, subsamp), height, subsamp)];
    strides[i] = tjPlaneWidth(i, width, subsamp);
  }
  
  // Compress from YUV planes
  tj3CompressFromYUVPlanes8(compressHandle, (const unsigned char**)srcPlanes, width, strides, height, &jpegBuf, &jpegSize);
  
  // Decompress header
  tj3DecompressHeader(decompressHandle, jpegBuf, jpegSize);
  
  // Set parameter
  tj3Set(transformHandle, TJXOP_ROT180, 1);
  
  // Compress 16-bit image
  unsigned short* srcBuf = new unsigned short[width * height];
  int pitch = width * 2;  // Assuming 16-bit image
  int pixelFormat = TJPF_RGB;
  tj3Compress16(compressHandle, srcBuf, width, pitch, height, pixelFormat, &jpegBuf, &jpegSize);
  
  // Clean up
  tjDestroy(compressHandle);
  tjDestroy(decompressHandle);
  tjDestroy(transformHandle);
  
  for (int i = 0; i < 3; i++) {
    delete[] srcPlanes[i];
  }
  delete[] srcPlanes;
  delete[] srcBuf;
  tjFree(jpegBuf);
  
  return 0;
}