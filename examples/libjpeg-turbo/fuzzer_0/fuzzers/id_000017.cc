#include "FDSan.h"
#include <turbojpeg.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 17
//<Prompt> ["tjDecompressToYUV2","tjEncodeYUV2","tj3Set","tjBufSize","tjGetErrorCode"]
/*<Combination>: [int tjDecompressToYUV2(tjhandle handle, const unsigned char * jpegBuf, unsigned long jpegSize, unsigned char * dstBuf, int width, int align, int height, int flags),
    int tjEncodeYUV2(tjhandle handle, unsigned char * srcBuf, int width, int pitch, int height, int pixelFormat, unsigned char * dstBuf, int subsamp, int flags),
    int tj3Set(tjhandle handle, int param, int value),
    unsigned long tjBufSize(int width, int height, int jpegSubsamp),
    int tjGetErrorCode(tjhandle handle)
*/
//<score> 1.2, nr_unique_branch: 1, p_cov: 0.6
//<Quality> {"diversity":9,"density":9,"covered_api":["tjDecompressToYUV2","tjEncodeYUV2","tjBufSize"],"uncovered_api":["tj3Set","tjGetErrorCode"],"unique_branches":{"tj3EncodeYUVPlanes8":[[1222,7,1222,35,0]]},"library_calls":["tjInitDecompress","tjAlloc","tjDecompressToYUV2","tjAlloc","tjBufSize","tjEncodeYUV2","tjFree","tjFree","tjDestroy"]}
/**/


extern "C" int LLVMFuzzerTestOneInput_0(const uint8_t *data, size_t size) {
	if(size<=0) return 0;

    // Declare variables
    tjhandle handle = NULL;
    unsigned char *jpegBuf = NULL;
    unsigned char *dstBuf = NULL;
    unsigned long jpegSize = 0;
    int width = 0;
    int height = 0;
    int pixelFormat = TJPF_RGB;
    int subsamp = TJSAMP_444;
    int flags = 0;
    int errorCode = 0;

    // Create input and output file streams
    FILE *in_file = fmemopen((void *)data, size, "rb");
    FILE *out_file = fopen("output_file", "wb");

    if (in_file == NULL || out_file == NULL) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Get file descriptors
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);

    // Initialize libjpeg-turbo
    handle = tjInitDecompress();
    if (handle == NULL) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }

    // Decompress input JPEG file to YUV
    fseek(in_file, 0, SEEK_END);
    size_t fileSize = ftell(in_file);
    fseek(in_file, 0, SEEK_SET);
    jpegBuf = (unsigned char *)tjAlloc(fileSize);
    fread(jpegBuf, 1, fileSize, in_file);
    tjDecompressToYUV2(handle, jpegBuf, fileSize, dstBuf, width, 0, height, flags);

    // Compress YUV data to JPEG
    width = 640;
    height = 480;
    dstBuf = (unsigned char *)tjAlloc(tjBufSize(width, height, subsamp));
    tjEncodeYUV2(handle, dstBuf, width, 0, height, pixelFormat, jpegBuf, subsamp, flags);

    // Write output JPEG to file
    fwrite(jpegBuf, 1, jpegSize, out_file);

    // Clean up
    tjFree(jpegBuf);
    tjFree(dstBuf);

    tjDestroy(handle);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;

    assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}