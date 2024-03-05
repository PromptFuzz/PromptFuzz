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
//<ID> 2432
//<Prompt> ["tjPlaneWidth","tjBufSizeYUV2","tjSaveImage","tjEncodeYUV3","tj3YUVBufSize","tj3SetScalingFactor"]
/*<Combination>: [int tjPlaneWidth(int componentID, int width, int subsamp),
    unsigned long tjBufSizeYUV2(int width, int align, int height, int subsamp),
    int tjSaveImage(const char * filename, unsigned char * buffer, int width, int pitch, int height, int pixelFormat, int flags),
    int tjEncodeYUV3(tjhandle handle, const unsigned char * srcBuf, int width, int pitch, int height, int pixelFormat, unsigned char * dstBuf, int align, int subsamp, int flags),
    size_t tj3YUVBufSize(int width, int align, int height, int subsamp),
    int tj3SetScalingFactor(tjhandle handle, tjscalingfactor scalingFactor)
*/
//<score> 1.3333334, nr_unique_branch: 1, p_cov: 0.6666667
//<Quality> {"diversity":4,"density":4,"covered_api":["tjPlaneWidth","tjBufSizeYUV2","tjSaveImage","tj3YUVBufSize"],"uncovered_api":["tjEncodeYUV3","tj3SetScalingFactor"],"unique_branches":{"tj3SaveImage8":[[439,31,439,40,0]]},"library_calls":["tjPlaneWidth","tjBufSizeYUV2","tj3YUVBufSize","tjSaveImage"]}
/**/


extern "C" int LLVMFuzzerTestOneInput_58(const uint8_t* f_data, size_t f_size) {
	if(f_size<=0) return 0;

	FILE *input_file_ptr = fopen("input_file.jpg", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(f_data, sizeof(uint8_t), f_size, input_file_ptr);
	fclose(input_file_ptr);

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Step 4: Create input and output file pointers
    FILE *in_file = fmemopen((void *)data, size, "rb");
    FILE *out_file = fopen("output_file.jpg", "wb");
    
    // Step 5: Create file descriptors
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);
    
    // Step 6: Create file names
    const char *input_file = "input_file.jpg";
    const char *output_file = "output_file.jpg";
    
    // Step 1: Retrieve image information using tjPlaneWidth
    int width = 0;
    int subsamp = 0;
    int componentID = 0;
    int planeWidth = tjPlaneWidth(componentID, width, subsamp);
    
    // Step 2: Calculate buffer sizes using tjBufSizeYUV2 and tj3YUVBufSize
    int align = 4;
    int height = 0;
    unsigned long yuvBufSize = tjBufSizeYUV2(width, align, height, subsamp);
    size_t yuv3BufSize = tj3YUVBufSize(width, align, height, subsamp);
    
    // Step 3: Save image using tjSaveImage
    unsigned char *buffer = (unsigned char *)malloc(size);
    int pitch = width * tjPixelSize[TJPF_RGB];
    int pixelFormat = TJPF_RGB;
    int flags = 0;
    int result = tjSaveImage(output_file, buffer, width, pitch, height, pixelFormat, flags);
    
    // Step 7: Release resources
    free(buffer);
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