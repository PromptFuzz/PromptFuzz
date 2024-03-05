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
//<ID> 57
//<Prompt> ["tjDecompress","tjDecompressToYUV","tj3Decompress8","tjGetErrorCode","tjLoadImage"]
/*<Combination>: [int tjDecompress(tjhandle handle, unsigned char * jpegBuf, unsigned long jpegSize, unsigned char * dstBuf, int width, int pitch, int height, int pixelSize, int flags),
    int tjDecompressToYUV(tjhandle handle, unsigned char * jpegBuf, unsigned long jpegSize, unsigned char * dstBuf, int flags),
    int tj3Decompress8(tjhandle handle, const unsigned char * jpegBuf, size_t jpegSize, unsigned char * dstBuf, int pitch, int pixelFormat),
    int tjGetErrorCode(tjhandle handle),
    unsigned char *tjLoadImage(const char * filename, int * width, int align, int * height, int * pixelFormat, int flags)
*/
//<score> 0, nr_unique_branch: 2, p_cov: 0
//<Quality> {"diversity":22,"density":22,"covered_api":[],"uncovered_api":["tjDecompress","tjDecompressToYUV","tj3Decompress8","tjGetErrorCode","tjLoadImage"],"unique_branches":{"tj3SaveImage8":[[470,7,470,10,0],[470,14,470,38,1]]},"library_calls":["tjInitDecompress","tjAlloc","tjDecompressHeader","tjAlloc","tjDecompress2","tjSaveImage","tjFree","tjFree","tjDestroy"]}
/**/

#define INPUT_FILE "input.jpg"
#define OUTPUT_FILE "output.jpg"

extern "C" int LLVMFuzzerTestOneInput_2(const uint8_t* f_data, size_t f_size) {
	if(f_size<=8) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
	//fuzzer shim end}




  // Create a TJ handle for decompression
  tjhandle decompressor = tjInitDecompress();

  // Create variables to store image information
  int width, height, pixelFormat;

  // Load input data from the fuzzer
  FILE *input_file = fmemopen((void *)data, size, "rb");
  if (input_file == nullptr) {
    tjDestroy(decompressor);
    assert_file_closed(&input_file);
	return 0;
  }

  // Get the size of the input file
  fseek(input_file, 0, SEEK_END);
  size_t jpegSize = ftell(input_file);
  fseek(input_file, 0, SEEK_SET);

  // Allocate memory for the input file data
  unsigned char *jpegBuf = (unsigned char *)tjAlloc(jpegSize);
  if (jpegBuf == nullptr) {
    assert_file_closed(&input_file);;
    tjDestroy(decompressor);
    assert_file_closed(&input_file);
	return 0;
  }

  // Read the input file data into the buffer
  fread(jpegBuf, jpegSize, 1, input_file);
  assert_file_closed(&input_file);;

  // Decompress the input JPEG data to an RGB buffer
  tjDecompressHeader(decompressor, jpegBuf, jpegSize, &width, &height);
  unsigned char *dstBuf = (unsigned char *)tjAlloc(width * height * tjPixelSize[TJPF_RGB]);
  if (dstBuf == nullptr) {
    tjFree(jpegBuf);
    tjDestroy(decompressor);
    assert_file_closed(&input_file);
	return 0;
  }

  int result = tjDecompress2(decompressor, jpegBuf, jpegSize, dstBuf, width, 0, height, TJPF_RGB, fuzz_int32_t_1);
  if (result != 0) {
    tjFree(jpegBuf);
    tjFree(dstBuf);
    tjDestroy(decompressor);
    assert_file_closed(&input_file);
	return 0;
  }

  // Save the decompressed data to an output JPEG file
  FILE *output_file = fopen(OUTPUT_FILE, "wb");
  if (output_file == nullptr) {
    tjFree(jpegBuf);
    tjFree(dstBuf);
    tjDestroy(decompressor);
    assert_file_closed(&output_file);
	assert_file_closed(&input_file);
	return 0;
  }

  result = tjSaveImage(OUTPUT_FILE, dstBuf, width, 0, height, TJPF_RGB, fuzz_int32_t_2);
  if (result != 0) {
    assert_file_closed(&output_file);;
    tjFree(jpegBuf);
    tjFree(dstBuf);
    tjDestroy(decompressor);
    assert_file_closed(&output_file);
	assert_file_closed(&input_file);
	return 0;
  }

  assert_file_closed(&output_file);;
  tjFree(jpegBuf);
  tjFree(dstBuf);
  tjDestroy(decompressor);

  assert_file_closed(&output_file);
	assert_file_closed(&input_file);
	return 0;
}