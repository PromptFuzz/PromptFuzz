#include "FDSan.h"
#include <turbojpeg.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 875
//<Prompt> ["tjDestroy","tjGetErrorStr","tj3Set","tjSaveImage","tj3DecompressHeader"]
/*<Combination>: [int tjDestroy(tjhandle handle),
    char *tjGetErrorStr(),
    int tj3Set(tjhandle handle, int param, int value),
    int tjSaveImage(const char * filename, unsigned char * buffer, int width, int pitch, int height, int pixelFormat, int flags),
    int tj3DecompressHeader(tjhandle handle, const unsigned char * jpegBuf, size_t jpegSize)
*/
//<score> 1.2, nr_unique_branch: 2, p_cov: 0.4
//<Quality> {"diversity":15,"density":15,"covered_api":["tjDestroy","tjSaveImage"],"uncovered_api":["tjGetErrorStr","tj3Set","tj3DecompressHeader"],"unique_branches":{"tj3SaveImage8":[[470,7,470,10,1],[487,7,487,17,1]]},"library_calls":["tjInitDecompress","tjAlloc","tjDecompressHeader","tjAlloc","tjDecompress2","tjSaveImage","tjFree","tjFree","tjDestroy"]}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_30(const uint8_t *data, size_t size) {
	if(size<=0) return 0;

    tjhandle handle = tjInitDecompress();
    if (!handle) {
        return 0;
    }

    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        tjDestroy(handle);
        assert_file_closed(&in_file);
	return 0;
    }

    int width, height;
    unsigned char *jpegBuf = nullptr;
    size_t jpegSize = 0;

    // Read the input data and obtain the width and height of the JPEG image
    fseek(in_file, 0, SEEK_END);
    size_t fileSize = ftell(in_file);
    fseek(in_file, 0, SEEK_SET);

    jpegBuf = (unsigned char *)tjAlloc(fileSize);
    if (!jpegBuf) {
        assert_file_closed(&in_file);;
        tjDestroy(handle);
        assert_file_closed(&in_file);
	return 0;
    }

    fread(jpegBuf, fileSize, 1, in_file);
    assert_file_closed(&in_file);;

    if (tjDecompressHeader(handle, jpegBuf, fileSize, &width, &height) != 0) {
        tjFree(jpegBuf);
        tjDestroy(handle);
        assert_file_closed(&in_file);
	return 0;
    }

    // Configure TJ destination buffer for the decompressed image
    const int subsamp = TJSAMP_420;
    const int pixelFormat = TJPF_RGB;
    unsigned char *dstBuf = nullptr;
    const int stride = width * tjPixelSize[pixelFormat];
    const int flags = TJFLAG_FASTDCT;  // Optional

    const int imageSize = stride * height;
    dstBuf = (unsigned char *)tjAlloc(imageSize);
    if (!dstBuf) {
        tjFree(jpegBuf);
        tjDestroy(handle);
        assert_file_closed(&in_file);
	return 0;
    }

    // Decompress the JPEG image and save the decompressed image as RGB24
    if (tjDecompress2(handle, jpegBuf, fileSize, dstBuf, width, stride, height,
                      pixelFormat, flags) == 0) {
        tjSaveImage("output_file", dstBuf, width, stride, height, pixelFormat, 0);
    }

    tjFree(jpegBuf);
    tjFree(dstBuf);
    tjDestroy(handle);

    assert_file_closed(&in_file);
	return 0;
}