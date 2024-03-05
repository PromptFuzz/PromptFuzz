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
//<ID> 2570
//<Prompt> ["tj3SaveImage16","tj3LoadImage8","tj3YUVBufSize","tj3DecompressToYUV8","tj3SaveImage12"]
/*<Combination>: [int tj3SaveImage16(tjhandle handle, const char * filename, const unsigned short * buffer, int width, int pitch, int height, int pixelFormat),
    unsigned char *tj3LoadImage8(tjhandle handle, const char * filename, int * width, int align, int * height, int * pixelFormat),
    size_t tj3YUVBufSize(int width, int align, int height, int subsamp),
    int tj3DecompressToYUV8(tjhandle handle, const unsigned char * jpegBuf, size_t jpegSize, unsigned char * dstBuf, int align),
    int tj3SaveImage12(tjhandle handle, const char * filename, const short * buffer, int width, int pitch, int height, int pixelFormat)
*/
//<score> 1.7142859, nr_unique_branch: 2, p_cov: 0.8
//<Quality> {"diversity":7,"density":5,"covered_api":["tj3LoadImage8","tj3YUVBufSize","tj3DecompressToYUV8","tj3SaveImage12"],"uncovered_api":["tj3SaveImage16"],"unique_branches":{"tj3DecompressToYUV8":[[290,7,290,12,0]],"tj3SaveImage12":[[301,7,301,12,0]]},"library_calls":["tj3LoadImage8","tj3Free","tj3YUVBufSize","tj3DecompressToYUV8","tj3Free","tj3SaveImage12","tj3Free"]}
/**/


extern "C" int LLVMFuzzerTestOneInput_63(const uint8_t* f_data, size_t f_size) {
	if(f_size<=12) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
	//fuzzer shim end}




  // Step 4: Create a FILE variable for reading the input data
  FILE *in_file = fmemopen((void *)data, size, "rb");
  
  // Step 5: Get the file descriptor for reading
  int in_fd = fuzz_fileno(in_file);
  
  // Step 7: Close the input file and release resources
  assert_file_closed(&in_file);;
  
  // Step 6: Specify the output file name
  const char *output_file = "output.jpg";
  
  // Step 4: Create a FILE variable for writing the output data
  FILE *out_file = fopen(output_file, "wb");
  
  // Step 5: Get the file descriptor for writing
  int out_fd = fuzz_fileno(out_file);
  
  // Step 7: Close the output file and release resources
  assert_file_closed(&out_file);;
  
  // Step 1: Call the tj3LoadImage8 API to load the input image
  int width, height, pixelFormat;
  unsigned char *input_image = tj3LoadImage8(NULL, "input.jpg", &width, fuzz_int32_t_1, &height, &pixelFormat);
  // Step 7: Free the memory allocated for the input image
  tj3Free(input_image);
  
  // Step 1: Call the tj3DecompressToYUV8 API to decompress the input image to YUV format
  unsigned char *yuv_image = nullptr;
  int align = 4;
  size_t yuv_size = tj3YUVBufSize(width, align, height, fuzz_int32_t_2);
  int result = tj3DecompressToYUV8(NULL, data, size, yuv_image, align);
  // Step 7: Free the memory allocated for the YUV image
  tj3Free(yuv_image);
  
  // Step 1: Call the tj3SaveImage12 API to save the output image
  short *output_image = nullptr;
  int output_width, output_height, output_pixelFormat;
  result = tj3SaveImage12(NULL, output_file, output_image, output_width, fuzz_int32_t_3, output_height, output_pixelFormat);
  // Step 7: Free the memory allocated for the output image
  tj3Free(output_image);
  
  // Step 7: Free any other resources acquired
  
  assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}