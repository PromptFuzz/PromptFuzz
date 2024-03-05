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
//<ID> 365
//<Prompt> ["tjDecodeYUV","tjTransform","tj3DecompressHeader","tjDecompressToYUV","tjGetErrorStr2"]
/*<Combination>: [int tjDecodeYUV(tjhandle handle, const unsigned char * srcBuf, int align, int subsamp, unsigned char * dstBuf, int width, int pitch, int height, int pixelFormat, int flags),
    int tjTransform(tjhandle handle, const unsigned char * jpegBuf, unsigned long jpegSize, int n, unsigned char ** dstBufs, unsigned long * dstSizes, tjtransform * transforms, int flags),
    int tj3DecompressHeader(tjhandle handle, const unsigned char * jpegBuf, size_t jpegSize),
    int tjDecompressToYUV(tjhandle handle, unsigned char * jpegBuf, unsigned long jpegSize, unsigned char * dstBuf, int flags),
    char *tjGetErrorStr2(tjhandle handle)
*/
//<score> 0, nr_unique_branch: 1, p_cov: 0
//<Quality> {"diversity":9,"density":9,"covered_api":[],"uncovered_api":["tjDecodeYUV","tjTransform","tj3DecompressHeader","tjDecompressToYUV","tjGetErrorStr2"],"unique_branches":{"tjDecompressToYUV2":[[2559,43,2559,52,0]]},"library_calls":["tjInitTransform","tjDecompressHeader3","tjGetErrorCode","tjDecompressToYUV2","tjGetErrorCode","tjDestroy"]}
/*Here is the C++ program that achieves the event mentioned above:

*/


extern "C" int LLVMFuzzerTestOneInput_12(const uint8_t* f_data, size_t f_size) {
	if(f_size<=4) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
	//fuzzer shim end}




    tjhandle handle = tjInitTransform();
    if (handle == NULL) {
        return 0;
    }

    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file == NULL) {
        tjDestroy(handle);
        assert_file_closed(&in_file);
	return 0;
    }

    fseek(in_file, 0, SEEK_END);
    long in_file_size = ftell(in_file);
    fseek(in_file, 0, SEEK_SET);

    unsigned char *jpegBuf = (unsigned char *)malloc(in_file_size);
    fread(jpegBuf, 1, in_file_size, in_file);

    assert_file_closed(&in_file);;

    int width, height;
    int jpegSubsamp, jpegColorspace;
    tjDecompressHeader3(handle, jpegBuf, in_file_size, &width, &height, &jpegSubsamp, &jpegColorspace);
    if (tjGetErrorCode(handle) != 0) {
        tjDestroy(handle);
        free(jpegBuf);
        assert_file_closed(&in_file);
	return 0;
    }

    unsigned char *dstBuf = (unsigned char *)malloc(width * height * tjPixelSize[TJPF_RGB]);
    tjDecompressToYUV2(handle, jpegBuf, in_file_size, dstBuf, width, TJPF_RGB, height, fuzz_int32_t_1);
    if (tjGetErrorCode(handle) != 0) {
        tjDestroy(handle);
        free(jpegBuf);
        free(dstBuf);
        assert_file_closed(&in_file);
	return 0;
    }

    free(jpegBuf);
    free(dstBuf);
    tjDestroy(handle);

    assert_file_closed(&in_file);
	return 0;
}