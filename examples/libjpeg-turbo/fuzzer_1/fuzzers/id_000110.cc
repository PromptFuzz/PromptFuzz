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
//<ID> 7220
//<Prompt> ["tj3JPEGBufSize","tj3CompressFromYUV8","tjDecodeYUVPlanes","tjInitTransform","tj3DecompressToYUVPlanes8","tj3YUVPlaneHeight"]
/*<Combination>: [size_t tj3JPEGBufSize(int width, int height, int jpegSubsamp),
    int tj3CompressFromYUV8(tjhandle handle, const unsigned char * srcBuf, int width, int align, int height, unsigned char ** jpegBuf, size_t * jpegSize),
    int tjDecodeYUVPlanes(tjhandle handle, const unsigned char ** srcPlanes, const int * strides, int subsamp, unsigned char * dstBuf, int width, int pitch, int height, int pixelFormat, int flags),
    tjhandle tjInitTransform(),
    int tj3DecompressToYUVPlanes8(tjhandle handle, const unsigned char * jpegBuf, size_t jpegSize, unsigned char ** dstPlanes, int * strides),
    int tj3YUVPlaneHeight(int componentID, int height, int subsamp)
*/
//<score> 1.3333334, nr_unique_branch: 1, p_cov: 0.6666667
//<Quality> {"diversity":6,"density":6,"covered_api":["tj3CompressFromYUV8","tjDecodeYUVPlanes","tjInitTransform","tj3DecompressToYUVPlanes8"],"uncovered_api":["tj3JPEGBufSize","tj3YUVPlaneHeight"],"unique_branches":{"tj3DecompressToYUVPlanes8":[[2306,26,2306,39,0]]},"library_calls":["tjInitTransform","tjDecodeYUVPlanes","tj3CompressFromYUV8","tj3DecompressToYUVPlanes8","tjFree","tjDestroy"]}
/*Here is the C++ program that uses the libjpeg-turbo library APIs to achieve the event:

*/


extern "C" int LLVMFuzzerTestOneInput_110(const uint8_t* f_data, size_t f_size) {
	if(f_size<=16) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
	//fuzzer shim end}




    // Declare variables
    tjhandle handle;
    unsigned char *jpegBuf = nullptr;
    size_t jpegSize;
    unsigned char *dstBuf[3] = { nullptr, nullptr, nullptr };
    int strides[3];
    int width, height, jpegSubsamp;

    // Initialize libjpeg-turbo
    handle = tjInitTransform();
    if (handle == nullptr) {
        return 0;
    }

    // Read input data from memory
    FILE *in_file = fmemopen((void *)data, size, "rb");
    int in_fd = fuzz_fileno(in_file);

    // Decode YUV planes from input data
    tjDecodeYUVPlanes(handle, nullptr, strides, 0, nullptr, width, fuzz_int32_t_1, height, fuzz_int32_t_2, fuzz_int32_t_3);

    // Compress YUV data to JPEG
    tj3CompressFromYUV8(handle, nullptr, width, fuzz_int32_t_4, height, &jpegBuf, &jpegSize);
    
    // Write output data to file
    FILE *out_file = fopen("output_file", "wb");
    int out_fd = fuzz_fileno(out_file);
    fwrite(jpegBuf, 1, jpegSize, out_file);

    // Read output data from file
    fseek(out_file, 0, SEEK_END);
    size_t out_size = ftell(out_file);
    rewind(out_file);
    unsigned char *out_data = new unsigned char[out_size];
    fread(out_data, 1, out_size, out_file);
    
    // Decompress JPEG to YUV planes
    tj3DecompressToYUVPlanes8(handle, out_data, out_size, dstBuf, strides);

    // Clean up
    tjFree(jpegBuf);
    tjDestroy(handle);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    delete[] out_data;

    assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}