#include "FDSan.h"
#include <turbojpeg.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 663
//<Prompt> ["tj3GetScalingFactors","tj3Compress16","tj3YUVPlaneSize","tj3YUVPlaneWidth","tj3LoadImage12","tjDecompressHeader3"]
/*<Combination>: [tjscalingfactor *tj3GetScalingFactors(int * numScalingFactors),
    int tj3Compress16(tjhandle handle, const unsigned short * srcBuf, int width, int pitch, int height, int pixelFormat, unsigned char ** jpegBuf, size_t * jpegSize),
    size_t tj3YUVPlaneSize(int componentID, int width, int stride, int height, int subsamp),
    int tj3YUVPlaneWidth(int componentID, int width, int subsamp),
    short *tj3LoadImage12(tjhandle handle, const char * filename, int * width, int align, int * height, int * pixelFormat),
    int tjDecompressHeader3(tjhandle handle, const unsigned char * jpegBuf, unsigned long jpegSize, int * width, int * height, int * jpegSubsamp, int * jpegColorspace)
*/
//<score> 0.45833334, nr_unique_branch: 1, p_cov: 0.33333334
//<Quality> {"diversity":16,"density":11,"covered_api":["tj3Compress16","tjDecompressHeader3"],"uncovered_api":["tj3GetScalingFactors","tj3YUVPlaneSize","tj3YUVPlaneWidth","tj3LoadImage12"],"unique_branches":{"tj3Compress16":[[81,7,81,35,0]]},"library_calls":["tjInitDecompress","tjDecompressHeader3","tjGetScalingFactors","tj3Compress16","tjFree","tjDestroy"]}
/**/


extern "C" int LLVMFuzzerTestOneInput_25(const uint8_t *data, size_t size) {
	if(size<=0) return 0;

    // Step 1: Declare variables
    tjhandle handle = NULL;
    int width, height, subsamp, colorspace;
    unsigned char *jpegBuf = NULL;
    unsigned long jpegSize = 0;
    const char *input_file = "input.jpg";
    
    // Step 2: Read input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Step 3: Initialize libjpeg-turbo
    handle = tjInitDecompress();
    if (!handle) {
        assert_file_closed(&in_file);;
        tjFree(jpegBuf);
        tjDestroy(handle);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Step 4: Write input data to a file
    FILE *jpeg_file = fopen(input_file, "wb");
    if (!jpeg_file) {
        assert_file_closed(&jpeg_file);;
        assert_file_closed(&in_file);;
        tjFree(jpegBuf);
        tjDestroy(handle);
        assert_file_closed(&jpeg_file);
	assert_file_closed(&in_file);
	return 0;
    }
    fwrite(data, sizeof(uint8_t), size, jpeg_file);
    assert_file_closed(&jpeg_file);;
    
    // Step 5: Read header from input file
    FILE *input = fopen(input_file, "rb");
    if (!input) {
        assert_file_closed(&input);;
        tjFree(jpegBuf);
        tjDestroy(handle);
        assert_file_closed(&input);
	assert_file_closed(&jpeg_file);
	assert_file_closed(&in_file);
	return 0;
    }
    fseek(input, 0, SEEK_END);
    long input_size = ftell(input);
    uint8_t *input_data = new uint8_t[input_size];
    fseek(input, 0, SEEK_SET);
    fread(input_data, sizeof(uint8_t), input_size, input);
    assert_file_closed(&input);;
    
    // Step 6: Decompress header and get image information
    int ret;
    ret = tjDecompressHeader3(handle, input_data, input_size, &width, &height, &subsamp, &colorspace);
    if (ret != 0) {
        delete[] input_data;
        tjFree(jpegBuf);
        tjDestroy(handle);
        assert_file_closed(&input);
	assert_file_closed(&jpeg_file);
	assert_file_closed(&in_file);
	return 0;
    }
    delete[] input_data;
    
    // Step 7: Compress image
    int numScalingFactors;
    tjscalingfactor *scalingFactors = tjGetScalingFactors(&numScalingFactors);
    ret = tj3Compress16(handle, NULL, width, 0, height, TJPF_RGBA, &jpegBuf, &jpegSize);
    if (ret != 0) {
        tjFree(jpegBuf);
        tjDestroy(handle);
        assert_file_closed(&input);
	assert_file_closed(&jpeg_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Step 8: Free resources
    tjFree(jpegBuf);
    tjDestroy(handle);
    assert_file_closed(&input);
	assert_file_closed(&jpeg_file);
	assert_file_closed(&in_file);
	return 0;
}