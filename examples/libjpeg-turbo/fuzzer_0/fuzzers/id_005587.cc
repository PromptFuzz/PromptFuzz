#include "FDSan.h"
#include <turbojpeg.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 5587
//<Prompt> ["tjGetErrorStr","tjPlaneHeight","tj3Compress16","tj3Set","tjPlaneWidth","tj3GetScalingFactors","tj3GetErrorCode","tjInitCompress"]
/*<Combination>: [char *tjGetErrorStr(),
    int tjPlaneHeight(int componentID, int height, int subsamp),
    int tj3Compress16(tjhandle handle, const unsigned short * srcBuf, int width, int pitch, int height, int pixelFormat, unsigned char ** jpegBuf, size_t * jpegSize),
    int tj3Set(tjhandle handle, int param, int value),
    int tjPlaneWidth(int componentID, int width, int subsamp),
    tjscalingfactor *tj3GetScalingFactors(int * numScalingFactors),
    int tj3GetErrorCode(tjhandle handle),
    tjhandle tjInitCompress()
*/
//<score> 2.4545455, nr_unique_branch: 2, p_cov: 1
//<Quality> {"diversity":11,"density":9,"covered_api":["tjGetErrorStr","tjPlaneHeight","tj3Compress16","tj3Set","tjPlaneWidth","tj3GetScalingFactors","tj3GetErrorCode","tjInitCompress"],"uncovered_api":[],"unique_branches":{"tj3Set":[[601,3,601,24,0],[579,20,579,29,0]]},"library_calls":["tjInitCompress","tjGetErrorStr","tjPlaneHeight","tj3Compress16","tjGetErrorStr2","tj3Set","tjPlaneWidth","tj3GetScalingFactors","tj3GetErrorCode","tjDestroy","tjFree"]}
/**/

extern "C" int LLVMFuzzerTestOneInput_94(const uint8_t *data, size_t size) {
	if(size<=0) return 0;

	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);

  	// Step 4: Create input and output file stream
	FILE *in_file = fmemopen((void *)data, size, "rb");
	FILE *out_file = fopen("output_file", "wb");
	if (in_file == NULL || out_file == NULL) {
		assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
	}

	// Step 5: Get file descriptors
	int in_fd = fuzz_fileno(in_file);
	int out_fd = fuzz_fileno(out_file);

	// Step 6: Create file name strings
	const char* in_filename = "input_file";
	const char* out_filename = "output_file";

	// Step 2: Use libjpeg-turbo APIs to achieve the desired event
	tjhandle compressHandle = tjInitCompress();
	if (compressHandle == NULL) {
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

	char *errorStr = tjGetErrorStr();
	
	int componentID = 0;
	int height = 100;
	int subsamp = 1;
	int planeHeight = tjPlaneHeight(componentID, height, subsamp);
	
	int width = 100;
	int pitch = width * tjPixelSize[TJPF_RGB];
	int pixelFormat = TJPF_RGB;
	unsigned short *srcBuf = new unsigned short[width * planeHeight * tjPixelSize[pixelFormat]];

	unsigned char *jpegBuf = nullptr;
	size_t jpegSize = 0;

	int result = tj3Compress16(compressHandle, srcBuf, width, pitch, planeHeight, pixelFormat, &jpegBuf, &jpegSize);
	if (result != 0) {
		const char *errorMsg = tjGetErrorStr2(compressHandle);
		fprintf(stderr, "Error compressing image: %s\n", errorMsg);
	}

	int param = 1;
	int value = 2;
	result = tj3Set(compressHandle, param, value);

	int planeWidth = tjPlaneWidth(componentID, width, subsamp);

	int numScalingFactors = 0;
	tjscalingfactor *scalingFactors = tj3GetScalingFactors(&numScalingFactors);

	int errorCode = tj3GetErrorCode(compressHandle);
	fprintf(stderr, "Error code: %d\n", errorCode);

	// Step 7: Clean up and release resources
	tjDestroy(compressHandle);
	assert_file_closed(&in_file);;
	assert_file_closed(&out_file);;
	delete[] srcBuf;
	tjFree(jpegBuf);

	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}