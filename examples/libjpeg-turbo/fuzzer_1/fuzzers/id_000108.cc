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
//<ID> 7189
//<Prompt> ["tjDecompressToYUV","tj3LoadImage8","tj3EncodeYUV8","tjDecompressToYUV2","tj3JPEGBufSize"]
/*<Combination>: [int tjDecompressToYUV(tjhandle handle, unsigned char * jpegBuf, unsigned long jpegSize, unsigned char * dstBuf, int flags),
    unsigned char *tj3LoadImage8(tjhandle handle, const char * filename, int * width, int align, int * height, int * pixelFormat),
    int tj3EncodeYUV8(tjhandle handle, const unsigned char * srcBuf, int width, int pitch, int height, int pixelFormat, unsigned char * dstBuf, int align),
    int tjDecompressToYUV2(tjhandle handle, const unsigned char * jpegBuf, unsigned long jpegSize, unsigned char * dstBuf, int width, int align, int height, int flags),
    size_t tj3JPEGBufSize(int width, int height, int jpegSubsamp)
*/
//<score> 0, nr_unique_branch: 3, p_cov: 0
//<Quality> {"diversity":0,"density":1,"covered_api":[],"uncovered_api":["tjDecompressToYUV","tj3LoadImage8","tj3EncodeYUV8","tjDecompressToYUV2","tj3JPEGBufSize"],"unique_branches":{"tjDecompressToYUV2":[[2572,15,2572,24,1],[2575,29,2575,46,1],[2578,7,2578,17,0]]},"library_calls":[]}
/**/


extern "C" {
}

// Function to decompress JPEG data to YUV format
void  id_000108_decompressToYUV(const uint8_t *jpegData, size_t jpegSize, uint8_t *yuvData, int width, int height) {
    tjhandle handle = tjInitDecompress();

    // Get the size of the YUV buffer
    int yuvBufferSize = tjBufSizeYUV(width, height, TJSAMP_420);

    // Decompress the JPEG data to YUV format
    tjDecompressToYUV2(handle, jpegData, jpegSize, yuvData, width, 0, height, 0);

    tjDestroy(handle);
}

// Function to load JPEG image and encode it to YUV format
void  id_000108_loadAndEncodeToYUV(const char *filename, uint8_t *yuvData, int *width, int *height) {
    tjhandle handle = tjInitCompress();

    int pixelFormat;
    unsigned char *jpegData = tjLoadImage(filename, width, 0, height, &pixelFormat, 0);

    // Get the size of the JPEG buffer
    size_t jpegBufferSize = tj3JPEGBufSize(*width, *height, TJSAMP_420);

    // Encode the loaded image to YUV format
    tj3EncodeYUV8(handle, jpegData, *width, *width, *height, pixelFormat, yuvData, 0);

    tjFree(jpegData);
    tjDestroy(handle);
}

// Fuzz driver function
extern "C" int LLVMFuzzerTestOneInput_108(const uint8_t* f_data, size_t f_size) {
	if(f_size<=0) return 0;

	FILE *input_file_ptr = fopen("input_file.jpg", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(f_data, sizeof(uint8_t), f_size, input_file_ptr);
	fclose(input_file_ptr);

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Decompress JPEG data to YUV format
    uint8_t yuvData[1024 * 1024];  // Change the buffer size as per your requirement
     id_000108_decompressToYUV(data, size, yuvData, 640, 480);  // Change the width and height as per your requirement

    // Load JPEG image and encode it to YUV format
    const char *filename = "input_file.jpg";  // Change the filename as per your requirement
    int width, height;
     id_000108_loadAndEncodeToYUV(filename, yuvData, &width, &height);

    assert_file_name_closed("input_file.jpg");
	return 0;
}