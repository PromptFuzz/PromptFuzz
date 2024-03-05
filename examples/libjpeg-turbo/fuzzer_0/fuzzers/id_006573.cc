#include "FDSan.h"
#include <turbojpeg.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 6573
//<Prompt> ["tj3DecodeYUV8","tjCompress","tjInitTransform","tj3CompressFromYUV8","tjEncodeYUV"]
/*<Combination>: [int tj3DecodeYUV8(tjhandle handle, const unsigned char * srcBuf, int align, unsigned char * dstBuf, int width, int pitch, int height, int pixelFormat),
    int tjCompress(tjhandle handle, unsigned char * srcBuf, int width, int pitch, int height, int pixelSize, unsigned char * dstBuf, unsigned long * compressedSize, int jpegSubsamp, int jpegQual, int flags),
    tjhandle tjInitTransform(),
    int tj3CompressFromYUV8(tjhandle handle, const unsigned char * srcBuf, int width, int align, int height, unsigned char ** jpegBuf, size_t * jpegSize),
    int tjEncodeYUV(tjhandle handle, unsigned char * srcBuf, int width, int pitch, int height, int pixelSize, unsigned char * dstBuf, int subsamp, int flags)
*/
//<score> 0.8, nr_unique_branch: 1, p_cov: 0.4
//<Quality> {"diversity":23,"density":23,"covered_api":["tjInitTransform","tj3CompressFromYUV8"],"uncovered_api":["tj3DecodeYUV8","tjCompress","tjEncodeYUV"],"unique_branches":{"jpeg_mem_src_tj":[[168,27,168,38,0]]},"library_calls":["tjInitCompress","tjInitDecompress","tjInitTransform","tjAlloc","tjAlloc","tjDecompressToYUV2","tj3CompressFromYUV8","tjTransform","tjFree","tjFree","tjDestroy","tjDestroy","tjDestroy"]}
/**/

extern "C" int LLVMFuzzerTestOneInput_102(const uint8_t *data, size_t size) {
	if(size<=0) return 0;

    // Step 1: Initialize variables
    tjhandle tjCompressHandle = tjInitCompress();
    tjhandle tjDecompressHandle = tjInitDecompress();
    tjhandle tjTransformHandle = tjInitTransform();
    unsigned char *jpegBuf = nullptr;
    unsigned long jpegSize = 0;
    unsigned char *dstBuf = nullptr;
    int width, height, pixelFormat;
    int jpegSubsamp, jpegQual, flags;
    int pixelSize = 3; // assuming RGB input data

    // Step 2: Read input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    fseek(in_file, 0, SEEK_END);
    size = ftell(in_file);
    rewind(in_file);

    // Step 3: Allocate memory for decompressed and compressed data
    dstBuf = (unsigned char *)tjAlloc(size);
    jpegBuf = (unsigned char *)tjAlloc(size);
    if (!dstBuf || !jpegBuf) {
        assert_file_closed(&in_file);;
        tjFree(dstBuf);
        tjFree(jpegBuf);
        tjDestroy(tjCompressHandle);
        tjDestroy(tjDecompressHandle);
        tjDestroy(tjTransformHandle);
        assert_file_closed(&in_file);
	return 0;
    }

    // Step 4: Decompress input data
    tjDecompressToYUV2(tjDecompressHandle, data, size, dstBuf, width, 0, height, flags);

    // Step 5: Compress decompressed data
    tj3CompressFromYUV8(tjCompressHandle, dstBuf, width, 0, height, &jpegBuf, &jpegSize);

    // Step 6: Transform compressed data
    tjTransform(tjTransformHandle, jpegBuf, jpegSize, 1, &jpegBuf, &jpegSize, nullptr, flags);

    // Step 7: Write output data
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        tjFree(dstBuf);
        tjFree(jpegBuf);
        tjDestroy(tjCompressHandle);
        tjDestroy(tjDecompressHandle);
        tjDestroy(tjTransformHandle);
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    fwrite(jpegBuf, 1, jpegSize, out_file);
    assert_file_closed(&out_file);;

    // Step 8: Release resources
    assert_file_closed(&in_file);;
    tjFree(dstBuf);
    tjFree(jpegBuf);
    tjDestroy(tjCompressHandle);
    tjDestroy(tjDecompressHandle);
    tjDestroy(tjTransformHandle);

    assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}