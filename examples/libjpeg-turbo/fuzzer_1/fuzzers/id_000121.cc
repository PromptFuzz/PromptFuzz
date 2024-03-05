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
//<ID> 8018
//<Prompt> ["tjDestroy","tj3SetScalingFactor","tj3CompressFromYUVPlanes8","tj3Compress16","tjGetErrorStr"]
/*<Combination>: [int tjDestroy(tjhandle handle),
    int tj3SetScalingFactor(tjhandle handle, tjscalingfactor scalingFactor),
    int tj3CompressFromYUVPlanes8(tjhandle handle, const unsigned char *const * srcPlanes, int width, const int * strides, int height, unsigned char ** jpegBuf, size_t * jpegSize),
    int tj3Compress16(tjhandle handle, const unsigned short * srcBuf, int width, int pitch, int height, int pixelFormat, unsigned char ** jpegBuf, size_t * jpegSize),
    char *tjGetErrorStr()
*/
//<score> 1.2, nr_unique_branch: 1, p_cov: 0.6
//<Quality> {"diversity":7,"density":7,"covered_api":["tjDestroy","tj3SetScalingFactor","tj3CompressFromYUVPlanes8"],"uncovered_api":["tj3Compress16","tjGetErrorStr"],"unique_branches":{"tj3CompressFromYUVPlanes8":[[1489,52,1489,63,0]]},"library_calls":["tjInitCompress","tj3SetScalingFactor","tj3CompressFromYUVPlanes8","tjFree","tjDestroy"]}
/*Here is a step-by-step implementation of a fuzz driver using the libjpeg-turbo library APIs:

*/


extern "C" int LLVMFuzzerTestOneInput_121(const uint8_t* f_data, size_t f_size) {
	if(f_size<=0) return 0;

	FILE *input_file_ptr = fopen("input_file.jpg", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(f_data, sizeof(uint8_t), f_size, input_file_ptr);
	fclose(input_file_ptr);

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




  // Step 1: Create a tjhandle for compression
  tjhandle handle = tjInitCompress();
  if (handle == nullptr) {
    return 0;
  }

  // Step 2: Set scaling factor
  tjscalingfactor scalingFactor = {1, 2};  // Example scaling factor
  tj3SetScalingFactor(handle, scalingFactor);

  // Step 3: Prepare input data
  FILE *in_file = fmemopen((void *)data, size, "rb");
  if (in_file == nullptr) {
    tjDestroy(handle);
    assert_file_closed(&in_file);
	return 0;
  }

  // Step 4: Prepare output data
  FILE *out_file = fopen("output_file.jpg", "wb");
  if (out_file == nullptr) {
    assert_file_closed(&in_file);;
    tjDestroy(handle);
    assert_file_name_closed("output_file.jpg");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
  }

  // Step 5: Get file descriptors
  int in_fd = fuzz_fileno(in_file);
  int out_fd = fuzz_fileno(out_file);

  // Step 6: Get file names
  const char *in_filename = "input_file.jpg";
  const char *out_filename = "output_file.jpg";

  // Step 7: Read input data
  fseek(in_file, 0, SEEK_END);
  long in_file_size = ftell(in_file);
  fseek(in_file, 0, SEEK_SET);

  unsigned char *srcBuf = new unsigned char[in_file_size];
  fread(srcBuf, sizeof(unsigned char), in_file_size, in_file);

  // Step 8: Compress input data and write to output file
  unsigned char *jpegBuf = nullptr;
  size_t jpegSize = 0;
  tj3CompressFromYUVPlanes8(handle, &srcBuf, in_file_size, nullptr, 0,
                            &jpegBuf, &jpegSize);
  fwrite(jpegBuf, sizeof(unsigned char), jpegSize, out_file);

  // Step 9: Release resources
  delete[] srcBuf;
  tjFree(jpegBuf);
  assert_file_closed(&in_file);;
  assert_file_closed(&out_file);;
  tjDestroy(handle);

  assert_file_name_closed("output_file.jpg");
	assert_file_name_closed("input_file.jpg");
	assert_file_name_closed("output_file.jpg");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}