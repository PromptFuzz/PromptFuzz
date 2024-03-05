#include "FDSan.h"
#include <turbojpeg.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 8696
//<Prompt> ["tjInitCompress","tj3Init","tj3YUVBufSize","tj3Get","tj3Decompress8"]
/*<Combination>: [tjhandle tjInitCompress(),
    tjhandle tj3Init(int initType),
    size_t tj3YUVBufSize(int width, int align, int height, int subsamp),
    int tj3Get(tjhandle handle, int param),
    int tj3Decompress8(tjhandle handle, const unsigned char * jpegBuf, size_t jpegSize, unsigned char * dstBuf, int pitch, int pixelFormat)
*/
//<score> 1.2, nr_unique_branch: 2, p_cov: 0.4
//<Quality> {"diversity":18,"density":18,"covered_api":["tjInitCompress","tj3YUVBufSize"],"uncovered_api":["tj3Init","tj3Get","tj3Decompress8"],"unique_branches":{"smoothing_ok":[[396,9,396,39,0]],"processFlags":[[515,32,515,58,0]]},"library_calls":["tjInitCompress","tjInitDecompress","tj3YUVBufSize","tjDecompressToYUV2","tjCompressFromYUV","tjFree","tjDestroy","tjDestroy"]}
/**/


extern "C" int LLVMFuzzerTestOneInput_123(const uint8_t *data, size_t size) {
	if(size<=0) return 0;

    // Step 4: Open input file
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Step 5: Get input file descriptor
    int in_fd = fuzz_fileno(in_file);
    if (in_fd < 0) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
    }

    // Step 6: Set output file name
    const char *out_file_name = "output.jpg";

    // Step 1: Initialize compression instance
    tjhandle compress_handle = tjInitCompress();
    if (!compress_handle) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
    }

    // Step 2: Initialize decompression instance
    tjhandle decompress_handle = tjInitDecompress();
    if (!decompress_handle) {
        tjDestroy(compress_handle);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
    }

    // Step 3: Calculate buffer size using tj3YUVBufSize
    int width = 640;
    int height = 480;
    int align = 4; // or any other value
    int subsamp = TJSAMP_420; // or any other value
    size_t yuv_buf_size = tj3YUVBufSize(width, align, height, subsamp);
    unsigned char *yuv_buf = (unsigned char *)malloc(yuv_buf_size);
    if (!yuv_buf) {
        tjDestroy(compress_handle);
        tjDestroy(decompress_handle);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
    }

    // Step 7: Read the input JPEG file using fread
    fseek(in_file, 0, SEEK_END);
    size_t jpeg_size = ftell(in_file);
    fseek(in_file, 0, SEEK_SET);
    unsigned char *jpeg_buf = (unsigned char *)malloc(jpeg_size);
    if (!jpeg_buf) {
        free(yuv_buf);
        tjDestroy(compress_handle);
        tjDestroy(decompress_handle);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
    }
    fread(jpeg_buf, sizeof(unsigned char), jpeg_size, in_file);

    // Step 9: Decompress JPEG to YUV
    int pitch = width * tjPixelSize[TJPF_RGB];
    int pixel_format = TJPF_RGB;
    int decompress_result = tjDecompressToYUV2(decompress_handle, jpeg_buf, jpeg_size, yuv_buf, width, align, height, TJFLAG_ACCURATEDCT);
    if (decompress_result < 0) {
        free(jpeg_buf);
        free(yuv_buf);
        tjDestroy(compress_handle);
        tjDestroy(decompress_handle);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
    }

    // Step 10: Compress YUV to JPEG
    unsigned char *compressed_buf = NULL;
    unsigned long compressed_size = 0;
    int compress_result = tjCompressFromYUV(compress_handle, yuv_buf, width, align, height, subsamp, &compressed_buf, &compressed_size, 80, TJFLAG_ACCURATEDCT);
    if (compress_result < 0) {
        free(jpeg_buf);
        free(yuv_buf);
        tjFree(compressed_buf);
        tjDestroy(compress_handle);
        tjDestroy(decompress_handle);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
    }

    // Step 11: Write the compressed JPEG to output file
    FILE *out_file = fopen(out_file_name, "wb");
    if (out_file) {
        fwrite(compressed_buf, sizeof(unsigned char), compressed_size, out_file);
        assert_file_closed(&out_file);;
    }

    // Step 12: Clean up
    free(jpeg_buf);
    free(yuv_buf);
    tjFree(compressed_buf);
    tjDestroy(compress_handle);
    tjDestroy(decompress_handle);
    assert_file_closed(&in_file);;

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
}