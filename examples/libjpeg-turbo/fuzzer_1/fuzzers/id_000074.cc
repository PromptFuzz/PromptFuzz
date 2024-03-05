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
//<ID> 3595
//<Prompt> ["tj3DecodeYUVPlanes8","tj3Set","TJBUFSIZE","tj3YUVPlaneSize","tjBufSize"]
/*<Combination>: [int tj3DecodeYUVPlanes8(tjhandle handle, const unsigned char *const * srcPlanes, const int * strides, unsigned char * dstBuf, int width, int pitch, int height, int pixelFormat),
    int tj3Set(tjhandle handle, int param, int value),
    unsigned long TJBUFSIZE(int width, int height),
    size_t tj3YUVPlaneSize(int componentID, int width, int stride, int height, int subsamp),
    unsigned long tjBufSize(int width, int height, int jpegSubsamp)
*/
//<score> 1.8000001, nr_unique_branch: 2, p_cov: 0.6
//<Quality> {"diversity":6,"density":6,"covered_api":["tj3DecodeYUVPlanes8","tj3Set","TJBUFSIZE"],"uncovered_api":["tj3YUVPlaneSize","tjBufSize"],"unique_branches":{"tj3DecodeYUVPlanes8":[[2085,7,2085,38,0]],"tj3Set":[[605,9,605,33,0]]},"library_calls":["tjInitDecompress","tj3DecodeYUVPlanes8","tj3Set","TJBUFSIZE","tjCompress","tjDestroy"]}
/*Here's a C++ program that uses libjpeg-turbo library APIs to achieve the given event:

*/


extern "C" int LLVMFuzzerTestOneInput_74(const uint8_t* f_data, size_t f_size) {
	if(f_size<=4) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
	//fuzzer shim end}




    // Create a handle for JPEG decompression
    tjhandle handle = tjInitDecompress();

    // Decode the YUV planes from the input data
    const unsigned char *srcPlanes[3] = { data, data + size / 3, data + 2 * size / 3 };
    const int strides[3] = { static_cast<int>(size / 3), 0, 0 };
    int width = 640;
    int height = 480;
    int pixelFormat = TJPF_RGB;
    unsigned char *dstBuf = new unsigned char[width * height * tjPixelSize[pixelFormat]];
    int pitch = tjPixelSize[pixelFormat] * width;
    tj3DecodeYUVPlanes8(handle, srcPlanes, strides, dstBuf, width, pitch, height, pixelFormat);

    // Set the TurboJPEG parameter
    int subsamp = TJSAMP_444;
    int value = 1;
    tj3Set(handle, TJXOPT_TRIM, value);

    // Get the size of the output JPEG buffer
    unsigned long jpegBufSize = TJBUFSIZE(width, height);

    // Allocate memory for the output JPEG buffer
    unsigned char *jpegBuf = new unsigned char[jpegBufSize];

    // Compress the RGB image into JPEG format
    tjCompress(handle, dstBuf, width, pitch, height, pixelFormat, jpegBuf, &jpegBufSize, subsamp, fuzz_int32_t_1, 0);

    // Save the output JPEG buffer to a file
    FILE *out_file = fopen("output.jpg", "wb");
    fwrite(jpegBuf, 1, jpegBufSize, out_file);
    assert_file_closed(&out_file);;

    // Release allocated resources
    delete[] dstBuf;
    delete[] jpegBuf;
    tjDestroy(handle);

    assert_file_closed(&out_file);
	return 0;
}