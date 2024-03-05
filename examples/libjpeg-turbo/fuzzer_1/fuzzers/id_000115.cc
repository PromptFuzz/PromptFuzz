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
//<ID> 7543
//<Prompt> ["tj3SetScalingFactor","tj3SaveImage16","TJBUFSIZEYUV","tj3Compress16","tjDecompressToYUV2","tj3GetErrorStr"]
/*<Combination>: [int tj3SetScalingFactor(tjhandle handle, tjscalingfactor scalingFactor),
    int tj3SaveImage16(tjhandle handle, const char * filename, const unsigned short * buffer, int width, int pitch, int height, int pixelFormat),
    unsigned long TJBUFSIZEYUV(int width, int height, int jpegSubsamp),
    int tj3Compress16(tjhandle handle, const unsigned short * srcBuf, int width, int pitch, int height, int pixelFormat, unsigned char ** jpegBuf, size_t * jpegSize),
    int tjDecompressToYUV2(tjhandle handle, const unsigned char * jpegBuf, unsigned long jpegSize, unsigned char * dstBuf, int width, int align, int height, int flags),
    char *tj3GetErrorStr(tjhandle handle)
*/
//<score> 1.6666666, nr_unique_branch: 1, p_cov: 0.8333333
//<Quality> {"diversity":13,"density":13,"covered_api":["tj3SetScalingFactor","TJBUFSIZEYUV","tj3Compress16","tjDecompressToYUV2","tj3GetErrorStr"],"uncovered_api":["tj3SaveImage16"],"unique_branches":{"decode_mcu_fast":[[732,15,732,22,1]]},"library_calls":["tjInitTransform","TJBUFSIZEYUV","tj3SetScalingFactor","tjDecompressToYUV2","tj3Compress16","tj3GetErrorStr","tjDestroy"]}
/**/


extern "C" int LLVMFuzzerTestOneInput_115(const uint8_t* f_data, size_t f_size) {
	if(f_size<=8) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
	//fuzzer shim end}




    tjhandle handle = tjInitTransform();
    if (handle == NULL) {
        return 0;
    }
    tjscalingfactor scalingFactor;
    scalingFactor.num = 1;
    scalingFactor.denom = 2;
    
    int width = 640;
    int height = 480;
    int jpegSubsamp = TJSAMP_444;
    unsigned long jpegBufSize = TJBUFSIZEYUV(width, height, jpegSubsamp);
    unsigned char *jpegBuf = (unsigned char *)malloc(jpegBufSize);
    unsigned char *dstBuf = (unsigned char *)malloc(width * height * tjPixelSize[TJPF_RGB] * sizeof(unsigned char));
    
    int ret = tj3SetScalingFactor(handle, scalingFactor);
    if (ret != 0) {
        char *error = tj3GetErrorStr(handle);
        // Handle the error
        free(jpegBuf);
        free(dstBuf);
        tjDestroy(handle);
        return 0;
    }
    
    ret = tjDecompressToYUV2(handle, data, size, dstBuf, width, 1, height, fuzz_int32_t_1);
    if (ret != 0) {
        char *error = tj3GetErrorStr(handle);
        // Handle the error
        free(jpegBuf);
        free(dstBuf);
        tjDestroy(handle);
        return 0;
    }
    
    ret = tj3Compress16(handle, (unsigned short*)dstBuf, width, fuzz_int32_t_2, height, TJPF_RGB, &jpegBuf, &jpegBufSize);
    if (ret != 0) {
        char *error = tj3GetErrorStr(handle);
        // Handle the error
        free(jpegBuf);
        free(dstBuf);
        tjDestroy(handle);
        return 0;
    }
    
    FILE *out_file = fopen("output_file", "wb");
    if (out_file == NULL) {
        // Handle the error
        free(jpegBuf);
        free(dstBuf);
        tjDestroy(handle);
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	return 0;
    }
    
    fwrite(jpegBuf, 1, jpegBufSize, out_file);
    assert_file_closed(&out_file);;
    
    free(jpegBuf);
    free(dstBuf);
    tjDestroy(handle);
    assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	return 0;
}