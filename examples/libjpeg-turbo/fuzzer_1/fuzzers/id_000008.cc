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
//<ID> 235
//<Prompt> ["tjInitTransform","tjCompress","tjInitDecompress","tjGetScalingFactors","tjDecompressHeader3"]
/*<Combination>: [tjhandle tjInitTransform(),
    int tjCompress(tjhandle handle, unsigned char * srcBuf, int width, int pitch, int height, int pixelSize, unsigned char * dstBuf, unsigned long * compressedSize, int jpegSubsamp, int jpegQual, int flags),
    tjhandle tjInitDecompress(),
    tjscalingfactor *tjGetScalingFactors(int * numscalingfactors),
    int tjDecompressHeader3(tjhandle handle, const unsigned char * jpegBuf, unsigned long jpegSize, int * width, int * height, int * jpegSubsamp, int * jpegColorspace)
*/
//<score> 1.1636364, nr_unique_branch: 1, p_cov: 0.8
//<Quality> {"diversity":11,"density":8,"covered_api":["tjInitTransform","tjCompress","tjInitDecompress","tjDecompressHeader3"],"uncovered_api":["tjGetScalingFactors"],"unique_branches":{"tj3JPEGBufSize":[[941,14,941,40,0]]},"library_calls":["tjInitTransform","tjInitDecompress","tjAlloc","tjDecompressHeader3","tjAlloc","tjBufSize","tjCompress","tjFree","tjFree","tjDestroy","tjDestroy"]}
/*Here is the complete C++ program that achieves the given event using the libjpeg-turbo library APIs:

*/


extern "C" int LLVMFuzzerTestOneInput_8(const uint8_t* f_data, size_t f_size) {
	if(f_size<=8) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
	//fuzzer shim end}




    tjhandle compressHandle = tjInitTransform();
    tjhandle decompressHandle = tjInitDecompress();

    FILE *in_file = fmemopen((void *)data, size, "rb");
    FILE *out_file = fopen("output_file", "wb");

    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);

    unsigned char *srcBuf = nullptr;
    unsigned char *dstBuf = nullptr;
    unsigned long compressedSize = 0;
    int width = 0;
    int height = 0;
    int jpegSubsamp = 0;
    int jpegColorspace = 0;

    // Read data from input file
    fseek(in_file, 0, SEEK_END);
    long file_size = ftell(in_file);
    fseek(in_file, 0, SEEK_SET);

    srcBuf = (unsigned char*)tjAlloc(file_size);
    fread(srcBuf, file_size, 1, in_file);

    // Compress the input image
    int ret = tjDecompressHeader3(decompressHandle, srcBuf, file_size, &width, &height, &jpegSubsamp, &jpegColorspace);
    if (ret != 0) {
        goto cleanup;
    }

    dstBuf = (unsigned char *)tjAlloc(tjBufSize(width, height, jpegSubsamp));

    ret = tjCompress(compressHandle, srcBuf, width, fuzz_int32_t_1, height, TJPF_RGB, dstBuf, &compressedSize, jpegSubsamp, fuzz_int32_t_2, 0);
    if (ret != 0) {
        goto cleanup;
    }

    // Write compressed data to output file
    fwrite(dstBuf, compressedSize, 1, out_file);

cleanup:
    if (srcBuf) {
        tjFree(srcBuf);
    }
    if (dstBuf) {
        tjFree(dstBuf);
    }
    if (in_file) {
        assert_file_closed(&in_file);;
    }
    if (out_file) {
        assert_file_closed(&out_file);;
    }
    if (compressHandle) {
        tjDestroy(compressHandle);
    }
    if (decompressHandle) {
        tjDestroy(decompressHandle);
    }

    assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}