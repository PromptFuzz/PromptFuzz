#include "FDSan.h"
#include <turbojpeg.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 1941
//<Prompt> ["tjEncodeYUV2","tj3Compress8","tjEncodeYUV3","tj3Alloc","tjDecodeYUV"]
/*<Combination>: [int tjEncodeYUV2(tjhandle handle, unsigned char * srcBuf, int width, int pitch, int height, int pixelFormat, unsigned char * dstBuf, int subsamp, int flags),
    int tj3Compress8(tjhandle handle, const unsigned char * srcBuf, int width, int pitch, int height, int pixelFormat, unsigned char ** jpegBuf, size_t * jpegSize),
    int tjEncodeYUV3(tjhandle handle, const unsigned char * srcBuf, int width, int pitch, int height, int pixelFormat, unsigned char * dstBuf, int align, int subsamp, int flags),
    void *tj3Alloc(size_t bytes),
    int tjDecodeYUV(tjhandle handle, const unsigned char * srcBuf, int align, int subsamp, unsigned char * dstBuf, int width, int pitch, int height, int pixelFormat, int flags)
*/
//<score> 0.8, nr_unique_branch: 1, p_cov: 0.4
//<Quality> {"diversity":6,"density":6,"covered_api":["tj3Compress8","tjDecodeYUV"],"uncovered_api":["tjEncodeYUV2","tjEncodeYUV3","tj3Alloc"],"unique_branches":{"tj3Set":[[623,9,623,35,0]]},"library_calls":["tjInitCompress","tj3Set","tj3Compress8","tjDecodeYUV","tjDestroy","tj3Free"]}
/**/


// Function to encode YUV data to JPEG
int  id_001941_encodeYUVtoJPEG(uint8_t *srcBuf, int width, int pitch, int height, int pixelFormat, uint8_t *dstBuf, int subsamp, int flags) {
    tjhandle handle = tjInitCompress();
    int result = tjEncodeYUV2(handle, srcBuf, width, pitch, height, pixelFormat, dstBuf, subsamp, flags);
    tjDestroy(handle);
    return result;
}

// Function to decode JPEG to YUV data
int  id_001941_decodeJPEGtoYUV(uint8_t *srcBuf, int align, int subsamp, uint8_t *dstBuf, int width, int pitch, int height, int pixelFormat, int flags) {
    tjhandle handle = tjInitDecompress();
    int result = tjDecodeYUV(handle, srcBuf, align, subsamp, dstBuf, width, pitch, height, pixelFormat, flags);
    tjDestroy(handle);
    return result;
}

// LLVM Fuzzer Test One Input function
extern "C" int LLVMFuzzerTestOneInput_49(const uint8_t *data, size_t size) {
	if(size<=0) return 0;

    // Create an input file stream from the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");

    // Create an output file stream
    FILE *out_file = fopen("output_file", "wb");

    // Check if file streams were successfully created
    if (in_file == NULL || out_file == NULL) {
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Get the file descriptors for reading and writing
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);

    // Read the input data from the file stream
    uint8_t *input_data = new uint8_t[size];
    fread(input_data, 1, size, in_file);

    // Encode the input data to JPEG
    int width = 640;
    int height = 480;
    int pixelFormat = TJPF_RGB;
    int subsamp = TJSAMP_420;
    int flags = 0;

    size_t jpegSize = 0;
    uint8_t *jpegBuf = NULL;

    tjhandle handle = tjInitCompress();
    tj3Set(handle, TJXOP_ROT180, 1);

    int result = tj3Compress8(handle, input_data, width, 0, height, pixelFormat, &jpegBuf, &jpegSize);

    if (result == 0) {
        // Write the compressed JPEG data to the output file stream
        fwrite(jpegBuf, 1, jpegSize, out_file);
    }

    // Decode the JPEG data to YUV
    uint8_t *decodedBuf = new uint8_t[width * height * 3 / 2];
    result = tjDecodeYUV(handle, jpegBuf, 0, subsamp, decodedBuf, width, 0, height, pixelFormat, flags);

    tjDestroy(handle);

    // Clean up resources
    delete[] input_data;
    delete[] decodedBuf;
    tj3Free(jpegBuf);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;

    assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}