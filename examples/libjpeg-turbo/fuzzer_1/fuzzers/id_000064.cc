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
//<ID> 2686
//<Prompt> ["tjBufSize","tjEncodeYUVPlanes","tj3SaveImage16","tjLoadImage","tj3JPEGBufSize"]
/*<Combination>: [unsigned long tjBufSize(int width, int height, int jpegSubsamp),
    int tjEncodeYUVPlanes(tjhandle handle, const unsigned char * srcBuf, int width, int pitch, int height, int pixelFormat, unsigned char ** dstPlanes, int * strides, int subsamp, int flags),
    int tj3SaveImage16(tjhandle handle, const char * filename, const unsigned short * buffer, int width, int pitch, int height, int pixelFormat),
    unsigned char *tjLoadImage(const char * filename, int * width, int align, int * height, int * pixelFormat, int flags),
    size_t tj3JPEGBufSize(int width, int height, int jpegSubsamp)
*/
//<score> 2.4, nr_unique_branch: 2, p_cov: 0.8
//<Quality> {"diversity":8,"density":8,"covered_api":["tjEncodeYUVPlanes","tj3SaveImage16","tjLoadImage","tj3JPEGBufSize"],"uncovered_api":["tjBufSize"],"unique_branches":{"tj3SaveImage16":[[439,20,439,27,1],[439,31,439,40,0]]},"library_calls":["tjInitDecompress","tjLoadImage","tj3JPEGBufSize","tjEncodeYUVPlanes","tj3SaveImage16","tjFree","tjDestroy"]}
/*Here is the C++ program that fulfills the given requirements:

*/


extern "C" int LLVMFuzzerTestOneInput_64(const uint8_t* f_data, size_t f_size) {
	if(f_size<=8) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
	//fuzzer shim end}




  // Step 1: Declare the variables and initialize them
  int width = 0, height = 0, pixelFormat = 0;
  unsigned char *jpegBuf = nullptr;
  size_t jpegSize = 0;

  // Step 2: Load the input image using tjLoadImage
  int flags = TJFLAG_FASTDCT;  // Set the desired flags
  tjhandle handle = tjInitDecompress();
  tjLoadImage(reinterpret_cast<const char *>(data), &width, fuzz_int32_t_1, &height,
              &pixelFormat, flags);

  // Step 3: Calculate the buffer size required for the compressed image
  size_t bufSize = tj3JPEGBufSize(width, height, TJSAMP_420);

  // Step 4: Allocate memory for the compressed image buffer
  jpegBuf = static_cast<unsigned char *>(std::malloc(bufSize));
  if (!jpegBuf) {
    tjDestroy(handle);
    return 0;
  }

  // Step 5: Compress the input image using tjEncodeYUVPlanes
  int subsamp = TJSAMP_420;
  int strides[3] = {width, width / 2, width / 2};  // YUV strides
  unsigned char *dstPlanes[3] = {nullptr, nullptr, nullptr};  // Output planes
  tjEncodeYUVPlanes(handle, const_cast<unsigned char *>(data), width, width,
                    height, pixelFormat, dstPlanes, strides, subsamp, flags);

  // Step 6: Save the compressed image using tj3SaveImage16
  tj3SaveImage16(handle, "output_file", reinterpret_cast<unsigned short *>(jpegBuf),
                 width, fuzz_int32_t_2, height, pixelFormat);

  // Step 7: Cleanup and release resources
  tjFree(jpegBuf);
  tjDestroy(handle);

  assert_file_name_closed("output_file");
	return 0;
}