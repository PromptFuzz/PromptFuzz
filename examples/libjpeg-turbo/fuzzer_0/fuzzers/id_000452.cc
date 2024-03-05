#include "FDSan.h"
#include <turbojpeg.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 452
//<Prompt> ["tj3JPEGBufSize","tj3EncodeYUV8","tj3YUVBufSize","tjGetErrorStr","tj3DecompressToYUVPlanes8"]
/*<Combination>: [size_t tj3JPEGBufSize(int width, int height, int jpegSubsamp),
    int tj3EncodeYUV8(tjhandle handle, const unsigned char * srcBuf, int width, int pitch, int height, int pixelFormat, unsigned char * dstBuf, int align),
    size_t tj3YUVBufSize(int width, int align, int height, int subsamp),
    char *tjGetErrorStr(),
    int tj3DecompressToYUVPlanes8(tjhandle handle, const unsigned char * jpegBuf, size_t jpegSize, unsigned char ** dstPlanes, int * strides)
*/
//<score> 1.2, nr_unique_branch: 1, p_cov: 1
//<Quality> {"diversity":5,"density":3,"covered_api":["tj3JPEGBufSize","tj3EncodeYUV8","tj3YUVBufSize","tjGetErrorStr","tj3DecompressToYUVPlanes8"],"uncovered_api":[],"unique_branches":{"tj3EncodeYUV8":[[301,7,301,12,0]]},"library_calls":["tj3JPEGBufSize","tj3EncodeYUV8","tj3YUVBufSize","tjGetErrorStr","tj3DecompressToYUVPlanes8"]}
/**/


extern "C" int LLVMFuzzerTestOneInput_17(const uint8_t *data, size_t size) {
	if(size<=0) return 0;

    // Step 4: Create input and output file variables
    FILE *in_file = fmemopen((void *)data, size, "rb");
    FILE *out_file = fopen("output_file", "wb");

    if (in_file == nullptr || out_file == nullptr) {
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Step 5: Get file descriptors
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);

    // Step 1: Calculate JPEG buffer size
    int width = 0;
    int height = 0;
    int jpegSubsamp = 0;
    size_t jpegBufSize = tj3JPEGBufSize(width, height, jpegSubsamp);

    // Step 2: Encode YUV data to JPEG
    uint8_t *srcBuf = (uint8_t *)malloc(size);
    int pitch = width;
    int pixelFormat = TJPF_RGB;
    int align = 1;
    uint8_t *dstBuf = (uint8_t *)malloc(jpegBufSize);
    int result = tj3EncodeYUV8(nullptr, srcBuf, width, pitch, height, pixelFormat, dstBuf, align);

    // Step 3: Calculate YUV buffer size
    size_t yuvBufSize = tj3YUVBufSize(width, align, height, jpegSubsamp);

    // Step 6: Get error string
    char *errorStr = tjGetErrorStr();

    // Step 7: Decompress JPEG to YUV planes
    uint8_t *jpegBuf = (uint8_t *)malloc(size);
    uint8_t **dstPlanes = (uint8_t **)malloc(sizeof(uint8_t *) * 3);
    int *strides = (int *)malloc(sizeof(int) * 3);
    result = tj3DecompressToYUVPlanes8(nullptr, jpegBuf, size, dstPlanes, strides);

    // Step 4: Clean up resources
    free(srcBuf);
    free(dstBuf);
    free(jpegBuf);
    free(dstPlanes);
    free(strides);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;

    assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}