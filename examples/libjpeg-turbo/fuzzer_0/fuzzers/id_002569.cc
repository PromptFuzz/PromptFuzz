#include "FDSan.h"
#include <turbojpeg.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 2569
//<Prompt> ["tjCompressFromYUVPlanes","tj3SaveImage8","tjSaveImage","tj3LoadImage12","tjEncodeYUV"]
/*<Combination>: [int tjCompressFromYUVPlanes(tjhandle handle, const unsigned char ** srcPlanes, int width, const int * strides, int height, int subsamp, unsigned char ** jpegBuf, unsigned long * jpegSize, int jpegQual, int flags),
    int tj3SaveImage8(tjhandle handle, const char * filename, const unsigned char * buffer, int width, int pitch, int height, int pixelFormat),
    int tjSaveImage(const char * filename, unsigned char * buffer, int width, int pitch, int height, int pixelFormat, int flags),
    short *tj3LoadImage12(tjhandle handle, const char * filename, int * width, int align, int * height, int * pixelFormat),
    int tjEncodeYUV(tjhandle handle, unsigned char * srcBuf, int width, int pitch, int height, int pixelSize, unsigned char * dstBuf, int subsamp, int flags)
*/
//<score> 1.2, nr_unique_branch: 1, p_cov: 0.6
//<Quality> {"diversity":5,"density":5,"covered_api":["tjCompressFromYUVPlanes","tj3SaveImage8","tj3LoadImage12"],"uncovered_api":["tjSaveImage","tjEncodeYUV"],"unique_branches":{"tj3LoadImage12":[[301,7,301,12,0]]},"library_calls":["tj3LoadImage12","tjInitCompress","tjCompressFromYUVPlanes","tj3SaveImage8","tjDestroy"]}
/**/


extern "C" int LLVMFuzzerTestOneInput_62(const uint8_t *data, size_t size) {
	if(size<=0) return 0;

	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);

  // Create input and output file pointers
  FILE *in_file = fmemopen((void *)data, size, "rb");
  FILE *out_file = fopen("output_file", "wb");

  // Check if file pointers are valid
  if (!in_file || !out_file) {
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
  }

  // Get the file descriptor
  int in_fd = fuzz_fileno(in_file);
  int out_fd = fuzz_fileno(out_file);

  // Read input data using libjpeg-turbo APIs
  int width, height, pixelFormat;
  short *image_data = tj3LoadImage12(nullptr, "input_file", &width, 0, &height, &pixelFormat);

  // Compress the image using libjpeg-turbo APIs
  unsigned char *jpegBuf = nullptr;
  unsigned long jpegSize = 0;
  tjhandle handle = tjInitCompress();
  tjCompressFromYUVPlanes(handle, (const unsigned char **)&image_data, width, nullptr, height, TJSAMP_420, &jpegBuf, &jpegSize, 90, 0);
  tj3SaveImage8(nullptr, "output_file", jpegBuf, width, 0, height, TJPF_RGB);

  // Free the allocated memory and close the file pointers
  free(image_data);
  tjDestroy(handle);
  assert_file_closed(&in_file);;
  assert_file_closed(&out_file);;

  assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}