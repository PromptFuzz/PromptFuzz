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
//<ID> 7596
//<Prompt> ["tjEncodeYUV2","tjDecompressHeader3","tj3LoadImage8","tjDecompressToYUVPlanes","tjCompressFromYUV"]
/*<Combination>: [int tjEncodeYUV2(tjhandle handle, unsigned char * srcBuf, int width, int pitch, int height, int pixelFormat, unsigned char * dstBuf, int subsamp, int flags),
    int tjDecompressHeader3(tjhandle handle, const unsigned char * jpegBuf, unsigned long jpegSize, int * width, int * height, int * jpegSubsamp, int * jpegColorspace),
    unsigned char *tj3LoadImage8(tjhandle handle, const char * filename, int * width, int align, int * height, int * pixelFormat),
    int tjDecompressToYUVPlanes(tjhandle handle, const unsigned char * jpegBuf, unsigned long jpegSize, unsigned char ** dstPlanes, int width, int * strides, int height, int flags),
    int tjCompressFromYUV(tjhandle handle, const unsigned char * srcBuf, int width, int align, int height, int subsamp, unsigned char ** jpegBuf, unsigned long * jpegSize, int jpegQual, int flags)
*/
//<score> 1.6, nr_unique_branch: 1, p_cov: 0.8
//<Quality> {"diversity":12,"density":12,"covered_api":["tjDecompressHeader3","tj3LoadImage8","tjDecompressToYUVPlanes","tjCompressFromYUV"],"uncovered_api":["tjEncodeYUV2"],"unique_branches":{"tjDecompressToYUVPlanes":[[2450,7,2450,37,0]]},"library_calls":["tjInitCompress","tj3LoadImage8","tjCompressFromYUV","tjDecompressHeader3","tjDecompressToYUVPlanes","tjFree","tjFree","tjFree","tjFree","tjFree","tjFree","tjDestroy"]}
/**/


extern "C" int LLVMFuzzerTestOneInput_116(const uint8_t* f_data, size_t f_size) {
	if(f_size<=20) return 0;

	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(f_data, sizeof(uint8_t), f_size, input_file_ptr);
	fclose(input_file_ptr);

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
	//fuzzer shim end}




    int width, height, pixelFormat;
    unsigned char *srcBuf, *dstBuf, *jpegBuf;
    unsigned long jpegSize;
    tjhandle handle = tjInitCompress();

    // Load input image
    srcBuf = tj3LoadImage8(handle, "input_file", &width, fuzz_int32_t_1, &height, &pixelFormat);
  
    // Compress image to JPEG
    tjCompressFromYUV(handle, srcBuf, width, fuzz_int32_t_2, height, TJ_GRAYSCALE, &jpegBuf, &jpegSize, fuzz_int32_t_3, fuzz_int32_t_4);
  
    // Write output data to file
    FILE *out_file = fopen("output_file", "wb");
    fwrite(jpegBuf, 1, jpegSize, out_file);
    assert_file_closed(&out_file);;
  
    // Read compressed data from file
    FILE *in_file = fopen("output_file", "rb");
    fseek(in_file, 0, SEEK_END);
    size_t file_size = ftell(in_file);
    rewind(in_file);
    unsigned char *jpegData = (unsigned char *)malloc(file_size);
    fread(jpegData, 1, file_size, in_file);
    assert_file_closed(&in_file);;

    // Decompress JPEG to YUV planes
    int strides[3] = {width, width / 2, width / 2};
    unsigned char *dstPlanes[3];
    tjDecompressHeader3(handle, jpegData, file_size, &width, &height, NULL, NULL);
    dstPlanes[0] = (unsigned char *)malloc(width * height);
    dstPlanes[1] = (unsigned char *)malloc(width * height / 4);
    dstPlanes[2] = (unsigned char *)malloc(width * height / 4);
    tjDecompressToYUVPlanes(handle, jpegData, file_size, dstPlanes, width, strides, height, fuzz_int32_t_5);

    // Clean up
    tjFree(jpegData);
    tjFree(dstPlanes[0]);
    tjFree(dstPlanes[1]);
    tjFree(dstPlanes[2]);
    tjFree(srcBuf);
    tjFree(jpegBuf);
    tjDestroy(handle);

    assert_file_name_closed("output_file");
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_closed(&in_file);
	assert_file_closed(&out_file);
	return 0;
}