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
//<ID> 5240
//<Prompt> ["tjPlaneWidth","tj3LoadImage12","tjDestroy","tjCompressFromYUV","tj3DecodeYUVPlanes8"]
/*<Combination>: [int tjPlaneWidth(int componentID, int width, int subsamp),
    short *tj3LoadImage12(tjhandle handle, const char * filename, int * width, int align, int * height, int * pixelFormat),
    int tjDestroy(tjhandle handle),
    int tjCompressFromYUV(tjhandle handle, const unsigned char * srcBuf, int width, int align, int height, int subsamp, unsigned char ** jpegBuf, unsigned long * jpegSize, int jpegQual, int flags),
    int tj3DecodeYUVPlanes8(tjhandle handle, const unsigned char *const * srcPlanes, const int * strides, unsigned char * dstBuf, int width, int pitch, int height, int pixelFormat)
*/
//<score> 3, nr_unique_branch: 2, p_cov: 1
//<Quality> {"diversity":7,"density":7,"covered_api":["tjPlaneWidth","tj3LoadImage12","tjDestroy","tjCompressFromYUV","tj3DecodeYUVPlanes8"],"uncovered_api":[],"unique_branches":{"tj3CompressFromYUV8":[[1649,7,1649,38,1],[1656,7,1656,35,1]]},"library_calls":["tjInitDecompress","tjPlaneWidth","tj3LoadImage12","tjCompressFromYUV","tj3DecodeYUVPlanes8","tjDestroy","tjFree"]}
/**/


extern "C" int LLVMFuzzerTestOneInput_88(const uint8_t* f_data, size_t f_size) {
	if(f_size<=0) return 0;

	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(f_data, sizeof(uint8_t), f_size, input_file_ptr);
	fclose(input_file_ptr);

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




  FILE *in_file = fmemopen((void *)data, size, "rb");
  if (!in_file) {
    assert_file_closed(&in_file);
	return 0;
  }

  int width = 0;
  int height = 0;
  int pixelFormat = 0;
  tjhandle handle = tjInitDecompress();
  if (!handle) {
    assert_file_closed(&in_file);;
    assert_file_closed(&in_file);
	return 0;
  }

  // Step 1: Get plane width
  int subsamp = TJSAMP_444;
  int componentID = 0;
  int planeWidth = tjPlaneWidth(componentID, width, subsamp);
  if (planeWidth != 0) {
    // Success
  }

  // Step 2: Load image
  int align = 4;
  short *srcBuf = tj3LoadImage12(handle, "input_file", &width, align, &height,
                                 &pixelFormat);
  if (srcBuf) {
    // Success
  }

  // Step 3: Compress from YUV
  unsigned char *jpegBuf = nullptr;
  unsigned long jpegSize = 0;
  int jpegQual = 90;
  int flags = 0;
  int compressResult = tjCompressFromYUV(handle, (const unsigned char *)srcBuf,
                                         width, align, height, subsamp,
                                         &jpegBuf, &jpegSize, jpegQual, flags);
  if (compressResult == 0) {
    // Success
  }

  // Step 4: Decode YUV planes
  const unsigned char *srcPlanes[3] = {jpegBuf, nullptr, nullptr};
  int strides[3] = {0, 0, 0};
  unsigned char *dstBuf = nullptr;
  int decodeResult = tj3DecodeYUVPlanes8(handle, srcPlanes, strides, dstBuf,
                                         width, planeWidth, height,
                                         pixelFormat);
  if (decodeResult == 0) {
    // Success
  }

  // Cleanup
  tjDestroy(handle);
  assert_file_closed(&in_file);;
  free(srcBuf);
  tjFree(jpegBuf);
  free(dstBuf);

  assert_file_name_closed("input_file");
	assert_file_closed(&in_file);
	return 0;
}