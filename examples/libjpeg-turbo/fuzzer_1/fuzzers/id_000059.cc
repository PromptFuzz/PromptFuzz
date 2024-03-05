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
//<ID> 2455
//<Prompt> ["tjPlaneWidth","tjBufSizeYUV2","tjSaveImage","tjEncodeYUV3","tj3YUVBufSize"]
/*<Combination>: [int tjPlaneWidth(int componentID, int width, int subsamp),
    unsigned long tjBufSizeYUV2(int width, int align, int height, int subsamp),
    int tjSaveImage(const char * filename, unsigned char * buffer, int width, int pitch, int height, int pixelFormat, int flags),
    int tjEncodeYUV3(tjhandle handle, const unsigned char * srcBuf, int width, int pitch, int height, int pixelFormat, unsigned char * dstBuf, int align, int subsamp, int flags),
    size_t tj3YUVBufSize(int width, int align, int height, int subsamp)
*/
//<score> 3, nr_unique_branch: 4, p_cov: 0.6
//<Quality> {"diversity":3,"density":3,"covered_api":["tjPlaneWidth","tjBufSizeYUV2","tjSaveImage"],"uncovered_api":["tjEncodeYUV3","tj3YUVBufSize"],"unique_branches":{"calc_buffer_dimensions_ppm":[[295,7,295,46,0]],"jinit_write_ppm":[[350,16,350,50,1],[78,22,78,39,1]],"start_output_ppm":[[243,3,243,21,0]]},"library_calls":["tjPlaneWidth","tjBufSizeYUV2","tjSaveImage"]}
/**/


extern "C" int LLVMFuzzerTestOneInput_59(const uint8_t* f_data, size_t f_size) {
	if(f_size<=4) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
	//fuzzer shim end}




  // Step 4: Read input data from the memory buffer
  FILE *in_file = fmemopen((void *)data, size, "rb");
  if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	

  // Step 5: Get the file descriptor for reading
  int in_fd = fuzz_fileno(in_file);
  if (in_fd < 0) {
    assert_file_closed(&in_file);;
    assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
  }

  // Step 6: Set the output file name
  const char *output_file = "output_file";

  // Step 1: Calculate the width of a plane
  int componentID = 0;
  int width = 640;
  int subsamp = TJSAMP_420;
  int planeWidth = tjPlaneWidth(componentID, width, subsamp);

  // Step 2: Calculate the buffer size
  int align = 1;
  int height = 480;
  unsigned long bufSize = tjBufSizeYUV2(width, align, height, subsamp);

  // Step 3: Save the YUV data as an image
  unsigned char *yuvBuffer = (unsigned char *)malloc(bufSize);
  if (!yuvBuffer) {
    assert_file_closed(&in_file);;
    assert_file_name_closed("output_file");
	assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
  }
  if (fread(yuvBuffer, bufSize, 1, in_file) != 1) {
    free(yuvBuffer);
    assert_file_closed(&in_file);;
    assert_file_name_closed("output_file");
	assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
  }

  if (tjSaveImage(output_file, yuvBuffer, planeWidth, 0, height, TJPF_GRAY,
                  fuzz_int32_t_1) != 0) {
    free(yuvBuffer);
    assert_file_closed(&in_file);;
    assert_file_name_closed("output_file");
	assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
  }

  // Step 7: Release resources
  free(yuvBuffer);
  assert_file_closed(&in_file);;

  assert_file_name_closed("output_file");
	assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
}