#include "FDSan.h"
#include <turbojpeg.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 59
//<Prompt> ["tjDecompress","tjDecompressToYUV","tj3Decompress8","tjGetErrorCode","tjLoadImage"]
/*<Combination>: [int tjDecompress(tjhandle handle, unsigned char * jpegBuf, unsigned long jpegSize, unsigned char * dstBuf, int width, int pitch, int height, int pixelSize, int flags),
    int tjDecompressToYUV(tjhandle handle, unsigned char * jpegBuf, unsigned long jpegSize, unsigned char * dstBuf, int flags),
    int tj3Decompress8(tjhandle handle, const unsigned char * jpegBuf, size_t jpegSize, unsigned char * dstBuf, int pitch, int pixelFormat),
    int tjGetErrorCode(tjhandle handle),
    unsigned char *tjLoadImage(const char * filename, int * width, int align, int * height, int * pixelFormat, int flags)
*/
//<score> 0, nr_unique_branch: 7, p_cov: 0
//<Quality> {"diversity":6,"density":5,"covered_api":[],"uncovered_api":["tjDecompress","tjDecompressToYUV","tj3Decompress8","tjGetErrorCode","tjLoadImage"],"unique_branches":{"jsimd_can_idct_islow":[[946,7,946,34,0],[32,30,32,50,0]],"jsimd_idct_islow":[[1008,7,1008,26,1],[1011,7,1011,32,0]],"start_pass":[[158,7,158,22,0],[160,13,160,35,0],[169,7,169,22,1]]},"library_calls":["tjInitDecompress","tjDecompressHeader3","tjDecompress2","tjDestroy"]}
/**/


extern "C" int LLVMFuzzerTestOneInput_3(const uint8_t* data, size_t size) {
	if(size<=0) return 0;

	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);

  // Step 4: Create input file
  FILE* in_file = fmemopen((void*)data, size, "rb");
  
  // Step 5: Get input file descriptor
  int in_fd = fuzz_fileno(in_file);
  
  // Step 6: Specify input file name
  const char* input_file = "input_file";
  
  // Step 7: Create output file
  FILE* out_file = fopen("output_file", "wb");
  
  // Step 8: Get output file descriptor
  int out_fd = fuzz_fileno(out_file);
  
  // Step 9: Specify output file name
  const char* output_file = "output_file";
  
  // Step 10: Initialize libjpeg-turbo
  tjhandle handle = tjInitDecompress();
  
  // Step 11: Read the input file and get its size
  fseek(in_file, 0, SEEK_END);
  size_t input_size = ftell(in_file);
  fseek(in_file, 0, SEEK_SET);
  unsigned char* jpeg_buf = new unsigned char[input_size];
  fread(jpeg_buf, 1, input_size, in_file);
  
  // Step 12: Decompress JPEG to RGB
  int width, height, subsamp, colorspace;
  if (tjDecompressHeader3(handle, jpeg_buf, input_size, &width, &height, &subsamp, &colorspace) != 0) {
    // Handle error
    fprintf(stderr, "Error decompressing JPEG header: %s\n", tjGetErrorStr2(handle));
    delete[] jpeg_buf;
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    tjDestroy(handle);
    assert_file_name_closed("output_file");
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
  }
  
  int pixel_size = tjPixelSize[TJPF_RGB];
  unsigned char* rgb_buf = new unsigned char[width * height * pixel_size];
  tjDecompress2(handle, jpeg_buf, input_size, rgb_buf, width, 0, height, TJPF_RGB, 0);
  
  // Step 13: Write RGB data to output file
  fwrite(rgb_buf, 1, width * height * pixel_size, out_file);
  
  // Step 14: Clean up and release resources
  delete[] jpeg_buf;
  delete[] rgb_buf;
  assert_file_closed(&in_file);;
  assert_file_closed(&out_file);;
  tjDestroy(handle);
  
  assert_file_name_closed("output_file");
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}