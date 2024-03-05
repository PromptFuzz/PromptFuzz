#include "FDSan.h"
#include <turbojpeg.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 2400
//<Prompt> ["tjPlaneWidth","tjBufSizeYUV2","tjSaveImage","tj3Destroy","tj3YUVPlaneWidth","tjEncodeYUV3","tj3SetScalingFactor"]
/*<Combination>: [int tjPlaneWidth(int componentID, int width, int subsamp),
    unsigned long tjBufSizeYUV2(int width, int align, int height, int subsamp),
    int tjSaveImage(const char * filename, unsigned char * buffer, int width, int pitch, int height, int pixelFormat, int flags),
    void tj3Destroy(tjhandle handle),
    int tj3YUVPlaneWidth(int componentID, int width, int subsamp),
    int tjEncodeYUV3(tjhandle handle, const unsigned char * srcBuf, int width, int pitch, int height, int pixelFormat, unsigned char * dstBuf, int align, int subsamp, int flags),
    int tj3SetScalingFactor(tjhandle handle, tjscalingfactor scalingFactor)
*/
//<score> 0.85714287, nr_unique_branch: 1, p_cov: 0.42857143
//<Quality> {"diversity":3,"density":3,"covered_api":["tjPlaneWidth","tjBufSizeYUV2","tjEncodeYUV3"],"uncovered_api":["tjSaveImage","tj3Destroy","tj3YUVPlaneWidth","tj3SetScalingFactor"],"unique_branches":{"tjEncodeYUV3":[[301,7,301,12,0]]},"library_calls":["tjPlaneWidth","tjBufSizeYUV2","tjEncodeYUV3"]}
/**/

extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_56(const uint8_t *data, size_t size) {
	if(size<=0) return 0;

	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);

  // Step 4: Create input and output file variables
  FILE *in_file = fmemopen((void *)data, size, "rb");
  FILE *out_file = fopen("output_file", "wb");
  
  // Step 5: Get file descriptors
  int in_fd = fuzz_fileno(in_file);
  int out_fd = fuzz_fileno(out_file);
  
  // Step 6: File names
  const char *input_file = "input_file";
  const char *output_file = "output_file";
  
  // Step 1: Usage of the APIs
  int componentID = 0;
  int width = 640;
  int subsamp = TJ_444;
  int planeWidth = tjPlaneWidth(componentID, width, subsamp);
  
  int align = 4;
  int height = 480;
  unsigned long bufSize = tjBufSizeYUV2(width, align, height, subsamp);

  unsigned char *buffer = new unsigned char[bufSize];
  int pitch = width * tjPixelSize[TJPF_RGB];
  int pixelFormat = TJPF_RGB;
  int flags = 0;
  
  int success = tjEncodeYUV3(NULL, buffer, width, pitch, height, pixelFormat,
                             buffer, align, subsamp, flags);
  
  // Step 2: Complete the implementation
  
  // Use the other APIs as needed
  
  // Step 3: Consume input and size
  fread(buffer, 1, bufSize, in_file);
  
  // Step 4: Write output data
  fwrite(buffer, 1, bufSize, out_file);
  
  // Step 7: Release resources
  assert_file_closed(&in_file);;
  assert_file_closed(&out_file);;
  delete[] buffer;
  
  assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}