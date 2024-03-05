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
//<ID> 897
//<Prompt> ["TJBUFSIZE","tj3Compress8","tjCompress2","TJBUFSIZEYUV","tjEncodeYUV3"]
/*<Combination>: [unsigned long TJBUFSIZE(int width, int height),
    int tj3Compress8(tjhandle handle, const unsigned char * srcBuf, int width, int pitch, int height, int pixelFormat, unsigned char ** jpegBuf, size_t * jpegSize),
    int tjCompress2(tjhandle handle, const unsigned char * srcBuf, int width, int pitch, int height, int pixelFormat, unsigned char ** jpegBuf, unsigned long * jpegSize, int jpegSubsamp, int jpegQual, int flags),
    unsigned long TJBUFSIZEYUV(int width, int height, int jpegSubsamp),
    int tjEncodeYUV3(tjhandle handle, const unsigned char * srcBuf, int width, int pitch, int height, int pixelFormat, unsigned char * dstBuf, int align, int subsamp, int flags)
*/
//<score> 0.8, nr_unique_branch: 1, p_cov: 0.4
//<Quality> {"diversity":7,"density":7,"covered_api":["TJBUFSIZE","tjCompress2"],"uncovered_api":["tj3Compress8","TJBUFSIZEYUV","tjEncodeYUV3"],"unique_branches":{"TJBUFSIZE":[[971,7,971,16,0]]},"library_calls":["TJBUFSIZE","tjInitCompress","tj3Alloc","tjCompress2","tjFree","tjFree","tjDestroy"]}
/*Here is the C++ program that implements the steps mentioned above:

*/


extern "C" int LLVMFuzzerTestOneInput_32(const uint8_t* f_data, size_t f_size) {
	if(f_size<=8) return 0;

	FILE *input_file_ptr = fopen("input_file.jpg", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(f_data, sizeof(uint8_t), f_size, input_file_ptr);
	fclose(input_file_ptr);

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
	//fuzzer shim end}




  // Step 4: Create input and output file streams
  FILE *in_file = fmemopen((void *)data, size, "rb");
  FILE *out_file = fopen("output_file.jpg", "wb");

  // Step 5: Get file descriptors
  int in_fd = fuzz_fileno(in_file);
  int out_fd = fuzz_fileno(out_file);

  // Step 6: Create file names
  const char *in_file_name = "input_file.jpg";
  const char *out_file_name = "output_file.jpg";

  // Step 1: Determine the output buffer size
  int width = 0; // Set your desired width
  int height = 0; // Set your desired height
  unsigned long output_size = TJBUFSIZE(width, height);

  // Step 2: Compress the input image
  tjhandle compressor = tjInitCompress();
  unsigned char *input_image = nullptr;
  unsigned long input_size = 0;

  fseek(in_file, 0, SEEK_END);
  input_size = ftell(in_file);
  fseek(in_file, 0, SEEK_SET);

  input_image = (unsigned char *)tj3Alloc(input_size);
  fread(input_image, 1, input_size, in_file);

  unsigned char *output_image = nullptr;

  int result = tjCompress2(compressor, input_image, width, 0, height, TJPF_RGB,
                           &output_image, &output_size, TJSAMP_444, fuzz_int32_t_1, fuzz_int32_t_2);

  // Step 3: Write the compressed image to the output file
  fwrite(output_image, 1, output_size, out_file);

  // Clean up
  tjFree(input_image);
  tjFree(output_image);
  tjDestroy(compressor);
  assert_file_closed(&in_file);;
  assert_file_closed(&out_file);;

  assert_file_name_closed("output_file.jpg");
	assert_file_name_closed("input_file.jpg");
	assert_file_name_closed("output_file.jpg");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}