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
//<ID> 1972
//<Prompt> ["tjDecompressToYUVPlanes","TJBUFSIZEYUV","tjBufSizeYUV","tj3Compress12","tjPlaneWidth"]
/*<Combination>: [int tjDecompressToYUVPlanes(tjhandle handle, const unsigned char * jpegBuf, unsigned long jpegSize, unsigned char ** dstPlanes, int width, int * strides, int height, int flags),
    unsigned long TJBUFSIZEYUV(int width, int height, int jpegSubsamp),
    unsigned long tjBufSizeYUV(int width, int height, int subsamp),
    int tj3Compress12(tjhandle handle, const short * srcBuf, int width, int pitch, int height, int pixelFormat, unsigned char ** jpegBuf, size_t * jpegSize),
    int tjPlaneWidth(int componentID, int width, int subsamp)
*/
//<score> 1.6, nr_unique_branch: 1, p_cov: 0.8
//<Quality> {"diversity":9,"density":9,"covered_api":["tjDecompressToYUVPlanes","TJBUFSIZEYUV","tjBufSizeYUV","tj3Compress12"],"uncovered_api":["tjPlaneWidth"],"unique_branches":{"tj3Compress12":[[84,7,84,21,0]]},"library_calls":["tjInitDecompress","tjDecompressToYUVPlanes","TJBUFSIZEYUV","tjBufSizeYUV","tj3Init","tj3Compress12","tj3Free","tjDestroy","tjDestroy"]}
/**/


extern "C" int LLVMFuzzerTestOneInput_51(const uint8_t* f_data, size_t f_size) {
	if(f_size<=4) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
	//fuzzer shim end}




    // Step 4: Create input file
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Step 5: Get file descriptor for reading
    int in_fd = fuzz_fileno(in_file);

    // Step 6: Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
    }

    // Step 7: Get file descriptor for writing
    int out_fd = fuzz_fileno(out_file);

    // Step 1: Decompress JPEG to YUV planes
    tjhandle handle = tjInitDecompress();
    unsigned long jpeg_size = size;
    unsigned char **dst_planes = nullptr;
    int width = 0;
    int strides[3] = {0};
    int height = 0;
    int flags = 0;
    tjDecompressToYUVPlanes(handle, data, jpeg_size, dst_planes, width, strides, height, flags);

    // Step 2: Get buffer sizes
    int jpeg_width = 0;
    int jpeg_height = 0;
    int jpeg_subsamp = 0;
    unsigned long buf_size_yuv = TJBUFSIZEYUV(jpeg_width, jpeg_height, jpeg_subsamp);
    unsigned long buf_size_yuv2 = tjBufSizeYUV(jpeg_width, jpeg_height, jpeg_subsamp);

    // Step 3: Compress YUV data
    short *src_buf = nullptr;
    int pixel_format = 0;
    unsigned char *jpeg_buf = nullptr;
    size_t jpeg_size_out = 0;
    tjhandle handle2 = tj3Init(fuzz_int32_t_1);
    tj3Compress12(handle2, src_buf, jpeg_width, strides[0], jpeg_height, pixel_format, &jpeg_buf, &jpeg_size_out);

    // Free resources
    tj3Free(jpeg_buf);
    tjDestroy(handle2);
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