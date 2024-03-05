#include "FDSan.h"
#include <turbojpeg.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 7262
//<Prompt> ["tjSaveImage","tjGetScalingFactors","tjPlaneHeight","tjEncodeYUV3","tjCompress2"]
/*<Combination>: [int tjSaveImage(const char * filename, unsigned char * buffer, int width, int pitch, int height, int pixelFormat, int flags),
    tjscalingfactor *tjGetScalingFactors(int * numscalingfactors),
    int tjPlaneHeight(int componentID, int height, int subsamp),
    int tjEncodeYUV3(tjhandle handle, const unsigned char * srcBuf, int width, int pitch, int height, int pixelFormat, unsigned char * dstBuf, int align, int subsamp, int flags),
    int tjCompress2(tjhandle handle, const unsigned char * srcBuf, int width, int pitch, int height, int pixelFormat, unsigned char ** jpegBuf, unsigned long * jpegSize, int jpegSubsamp, int jpegQual, int flags)
*/
//<score> 1.5555556, nr_unique_branch: 1, p_cov: 1
//<Quality> {"diversity":9,"density":7,"covered_api":["tjSaveImage","tjGetScalingFactors","tjPlaneHeight","tjEncodeYUV3","tjCompress2"],"uncovered_api":[],"unique_branches":{"tj3EncodeYUVPlanes8":[[1226,7,1226,22,0]]},"library_calls":["tjSaveImage","tjGetScalingFactors","tjPlaneHeight","tjInitTransform","tjEncodeYUV3","tjInitCompress","tjCompress2","tjDestroy","tjDestroy"]}
/**/


extern "C" int LLVMFuzzerTestOneInput_111(const uint8_t *data, size_t size) {
	if(size<=0) return 0;

	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);

    // Step 4: Create input file from input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Step 5: Get file descriptor for input file
    int in_fd = fuzz_fileno(in_file);
    
    // Step 6: Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Step 7: Get file descriptor for output file
    int out_fd = fuzz_fileno(out_file);
    
    // Step 1: Save input data to output file
    tjSaveImage("input_file", (unsigned char *)data, size, 0, 0, TJPF_UNKNOWN, 0);
    
    // Step 2: Get scaling factors
    int numScalingFactors;
    tjscalingfactor *scalingFactors = tjGetScalingFactors(&numScalingFactors);
    
    // Step 3: Get plane height
    int componentID = 0; // Assuming Y component
    int height = 100; // Example height value
    int subsamp = 0; // Example subsampling value
    int planeHeight = tjPlaneHeight(componentID, height, subsamp);
    
    // Step 4: Encode YUV data
    tjhandle handle = tjInitTransform();
    unsigned char *srcBuf = (unsigned char *)malloc(size);
    unsigned char *dstBuf = (unsigned char *)malloc(size);
    int width = 100; // Example width value
    int pitch = 0; // Example pitch value
    int pixelFormat = TJPF_UNKNOWN; // Example pixel format
    int align = 1; // Example alignment value
    int flags = 0; // Example flags value
    int result = tjEncodeYUV3(handle, srcBuf, width, pitch, height, pixelFormat, dstBuf, align, subsamp, flags);
    
    // Step 5: Compress image
    unsigned char *jpegBuf = nullptr;
    unsigned long jpegSize = 0;
    tjhandle compressHandle = tjInitCompress();
    result = tjCompress2(compressHandle, srcBuf, width, pitch, height, pixelFormat, &jpegBuf, &jpegSize, TJ_420, 90, 0);
    
    // Step 7: Free allocated buffers and destroy handles
    free(srcBuf);
    free(dstBuf);
    free(jpegBuf);
    tjDestroy(handle);
    tjDestroy(compressHandle);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_name_closed("input_file");
	assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}