#include "FDSan.h"
#include <turbojpeg.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 895
//<Prompt> ["TJBUFSIZE","tj3Compress8","tjCompress2","TJBUFSIZEYUV","tjEncodeYUV3"]
/*<Combination>: [unsigned long TJBUFSIZE(int width, int height),
    int tj3Compress8(tjhandle handle, const unsigned char * srcBuf, int width, int pitch, int height, int pixelFormat, unsigned char ** jpegBuf, size_t * jpegSize),
    int tjCompress2(tjhandle handle, const unsigned char * srcBuf, int width, int pitch, int height, int pixelFormat, unsigned char ** jpegBuf, unsigned long * jpegSize, int jpegSubsamp, int jpegQual, int flags),
    unsigned long TJBUFSIZEYUV(int width, int height, int jpegSubsamp),
    int tjEncodeYUV3(tjhandle handle, const unsigned char * srcBuf, int width, int pitch, int height, int pixelFormat, unsigned char * dstBuf, int align, int subsamp, int flags)
*/
//<score> 1.6, nr_unique_branch: 1, p_cov: 0.8
//<Quality> {"diversity":8,"density":8,"covered_api":["TJBUFSIZE","tj3Compress8","TJBUFSIZEYUV","tjEncodeYUV3"],"uncovered_api":["tjCompress2"],"unique_branches":{"tj3EncodeYUV8":[[1391,54,1391,63,0]]},"library_calls":["TJBUFSIZE","tjInitCompress","tj3Compress8","tjDestroy","TJBUFSIZEYUV","tjInitCompress","tjEncodeYUV3","tjDestroy"]}
/**/


extern "C" int LLVMFuzzerTestOneInput_31(const uint8_t *data, size_t size) {
	if(size<=0) return 0;

	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);

    // Step 4: Open input file
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file == nullptr) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Step 5: Get file descriptor
    int fd = fuzz_fileno(in_file);

    // Step 7: Close input file and release resources
    assert_file_closed(&in_file);;

    // Step 6: Use file name directly
    const char *input_file = "input_file";

    // Step 4: Open output file
    FILE *out_file = fopen("output_file", "wb");
    if (out_file == nullptr) {
        assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }

    // Step 5: Get file descriptor
    int out_fd = fuzz_fileno(out_file);

    // Step 7: Close output file and release resources
    assert_file_closed(&out_file);;

    // Step 6: Use file name directly
    const char *output_file = "output_file";

    // Step 1: Calculate output buffer size
    int width = 640;  // Example width
    int height = 480; // Example height
    unsigned long buf_size = TJBUFSIZE(width, height);

    // Step 2: Compress using tj3Compress8
    unsigned char *compressed_buf = new unsigned char[buf_size];
    size_t compressed_size = 0;

    // tjhandle for compression
    tjhandle tjh = tjInitCompress();
    int ret = tj3Compress8(tjh, data, width, 0, height, TJPF_RGB, &compressed_buf, &compressed_size);
    tjDestroy(tjh);

    // Step 3: Write compressed data to output file
    if (ret == 0) {
        out_file = fopen(output_file, "wb");
        if (out_file != nullptr) {
            fwrite(compressed_buf, sizeof(unsigned char), compressed_size, out_file);
            assert_file_closed(&out_file);;
        }
    }

    delete[] compressed_buf;

    // Step 1: Calculate output buffer size
    int jpeg_width = 640;  // Example width
    int jpeg_height = 480; // Example height
    unsigned long yuv_buf_size = TJBUFSIZEYUV(jpeg_width, jpeg_height, TJSAMP_420);

    // Step 2: Compress using tjEncodeYUV3
    unsigned char *yuv_buf = new unsigned char[yuv_buf_size];
    tjhandle tjh2 = tjInitCompress();
    ret = tjEncodeYUV3(tjh2, data, jpeg_width, 0, jpeg_height, TJPF_RGB, yuv_buf, 0, TJSAMP_420, 0);
    tjDestroy(tjh2);

    // Step 3: Write YUV data to output file
    if (ret == 0) {
        out_file = fopen(output_file, "wb");
        if (out_file != nullptr) {
            fwrite(yuv_buf, sizeof(unsigned char), yuv_buf_size, out_file);
            assert_file_closed(&out_file);;
        }
    }

    delete[] yuv_buf;

    assert_file_name_closed("output_file");
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(fd);
	return 0;
}