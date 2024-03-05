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
//<ID> 7290
//<Prompt> ["tjSaveImage","tjGetScalingFactors","tjPlaneHeight","tj3Compress12","tjEncodeYUV3","tjCompress2"]
/*<Combination>: [int tjSaveImage(const char * filename, unsigned char * buffer, int width, int pitch, int height, int pixelFormat, int flags),
    tjscalingfactor *tjGetScalingFactors(int * numscalingfactors),
    int tjPlaneHeight(int componentID, int height, int subsamp),
    int tj3Compress12(tjhandle handle, const short * srcBuf, int width, int pitch, int height, int pixelFormat, unsigned char ** jpegBuf, size_t * jpegSize),
    int tjEncodeYUV3(tjhandle handle, const unsigned char * srcBuf, int width, int pitch, int height, int pixelFormat, unsigned char * dstBuf, int align, int subsamp, int flags),
    int tjCompress2(tjhandle handle, const unsigned char * srcBuf, int width, int pitch, int height, int pixelFormat, unsigned char ** jpegBuf, unsigned long * jpegSize, int jpegSubsamp, int jpegQual, int flags)
*/
//<score> 3.7142859, nr_unique_branch: 5, p_cov: 0.6666667
//<Quality> {"diversity":14,"density":13,"covered_api":["tjGetScalingFactors","tjPlaneHeight","tjEncodeYUV3","tjCompress2"],"uncovered_api":["tjSaveImage","tj3Compress12"],"unique_branches":{"jpeg_set_defaults":[[196,7,196,31,1]],"flush_bits":[[496,11,496,37,0],[497,13,497,32,1],[444,26,444,35,1]],"add_huff_table":[[26,7,26,23,1]]},"library_calls":["tjInitTransform","tjGetScalingFactors","tjPlaneHeight","tjCompress2","tjEncodeYUV3","tjFree","tjDestroy"]}
/**/


extern "C" int LLVMFuzzerTestOneInput_112(const uint8_t* f_data, size_t f_size) {
	if(f_size<=0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




  // Step 4: Create input file stream with data
  FILE *in_file = fmemopen((void *)data, size, "rb");
  if (!in_file) {
    assert_file_closed(&in_file);
	return 0;
  }

  // Step 5: Get file descriptor for reading
  int in_fd = fuzz_fileno(in_file);
  if (in_fd < 0) {
    assert_file_closed(&in_file);;
    assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
  }

  // Step 6: Define output file name
  const char *out_file_name = "output_file";

  // Step 1: Initialize libjpeg-turbo
  tjhandle tjh = tjInitTransform();
  if (!tjh) {
    assert_file_closed(&in_file);;
    assert_file_name_closed("output_file");
	assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
  }

  // Step 2: Get scaling factors
  tjscalingfactor *scalingFactors;
  int numScalingFactors;
  scalingFactors = tjGetScalingFactors(&numScalingFactors);
  if (!scalingFactors) {
    tjDestroy(tjh);
    assert_file_closed(&in_file);;
    assert_file_name_closed("output_file");
	assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
  }

  // Step 3: Get plane height
  int componentID = 0; // choose a component ID
  int height = 100;    // choose a height
  int subsamp = 0;     // choose a subsampling value
  int planeHeight = tjPlaneHeight(componentID, height, subsamp);

  // Step 4: Compress image to JPEG
  // Read image from input file
  int width = 100;      // choose a width
  int pitch = width * 3;
  int pixelFormat = TJPF_RGB;
  unsigned char *srcBuf = (unsigned char *)malloc(height * pitch);
  fread(srcBuf, 1, height * pitch, in_file);

  // Compress image to a JPEG buffer
  unsigned char *jpegBuf = NULL;
  size_t jpegSize = 0;
  int jpegSubsamp = TJSAMP_420;
  int jpegQual = 90;
  int flags = 0;
  int compressResult = tjCompress2(tjh, srcBuf, width, pitch, height, pixelFormat,
                                  &jpegBuf, &jpegSize, jpegSubsamp, jpegQual, flags);
  free(srcBuf);
  assert_file_closed(&in_file);;
  if (compressResult != 0) {
    tjFree(jpegBuf);
    tjDestroy(tjh);
    assert_file_name_closed("output_file");
	assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
  }

  // Step 5: Encode YUV image from RGB
  unsigned char *dstBuf = (unsigned char *)malloc(height * pitch);
  int align = 4;
  int encodeResult = tjEncodeYUV3(tjh, srcBuf, width, pitch, height, pixelFormat,
                                  dstBuf, align, jpegSubsamp, flags);
  free(dstBuf);
  if (encodeResult != 0) {
    tjFree(jpegBuf);
    tjDestroy(tjh);
    assert_file_name_closed("output_file");
	assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
  }

  // Step 6: Save the compressed image to output file
  FILE *out_file = fopen(out_file_name, "wb");
  if (!out_file) {
    tjFree(jpegBuf);
    tjDestroy(tjh);
    assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
  }
  fwrite(jpegBuf, 1, jpegSize, out_file);
  assert_file_closed(&out_file);;
  tjFree(jpegBuf);

  // Step 7: Cleanup and return
  tjDestroy(tjh);
  assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
}