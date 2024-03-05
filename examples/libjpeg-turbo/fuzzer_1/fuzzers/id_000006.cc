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
//<ID> 107
//<Prompt> ["tjDecompress","tj3DecodeYUVPlanes8","tjDecompressToYUV","tj3Decompress8","tjGetErrorCode"]
/*<Combination>: [int tjDecompress(tjhandle handle, unsigned char * jpegBuf, unsigned long jpegSize, unsigned char * dstBuf, int width, int pitch, int height, int pixelSize, int flags),
    int tj3DecodeYUVPlanes8(tjhandle handle, const unsigned char *const * srcPlanes, const int * strides, unsigned char * dstBuf, int width, int pitch, int height, int pixelFormat),
    int tjDecompressToYUV(tjhandle handle, unsigned char * jpegBuf, unsigned long jpegSize, unsigned char * dstBuf, int flags),
    int tj3Decompress8(tjhandle handle, const unsigned char * jpegBuf, size_t jpegSize, unsigned char * dstBuf, int pitch, int pixelFormat),
    int tjGetErrorCode(tjhandle handle)
*/
//<score> 0, nr_unique_branch: 7, p_cov: 0
//<Quality> {"diversity":0,"density":1,"covered_api":[],"uncovered_api":["tjDecompress","tj3DecodeYUVPlanes8","tjDecompressToYUV","tj3Decompress8","tjGetErrorCode"],"unique_branches":{"jpeg_idct_10x10":[[1094,17,1094,24,0],[1094,17,1094,24,1],[1166,17,1166,25,0],[1166,17,1166,25,1]],"jinit_upsampler":[[472,11,472,19,1],[488,49,488,57,1]],"start_pass":[[200,5,200,12,0]]},"library_calls":[]}
/**/


// Function to decompress the JPEG image
void  id_000006_decompressJPEG(const uint8_t *jpegData, size_t jpegSize, uint8_t *outputData, int width, int height, int pixelSize) {
    tjhandle handle = tjInitDecompress();

    // Decompress the JPEG image data
    tjDecompress(handle, const_cast<uint8_t *>(jpegData), jpegSize, outputData, width, 0, height, pixelSize, 0);

    tjDestroy(handle);
}

extern "C" int LLVMFuzzerTestOneInput_6(const uint8_t* f_data, size_t f_size) {
	if(f_size<=0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file == nullptr) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Get the size of the input file
    fseek(in_file, 0L, SEEK_END);
    size_t inputFileSize = ftell(in_file);
    rewind(in_file);

    // Read the input data into a buffer
    uint8_t *inputData = new uint8_t[inputFileSize];
    fread(inputData, sizeof(uint8_t), inputFileSize, in_file);

    assert_file_closed(&in_file);;

    // Create output buffer
    const int width = 640;
    const int height = 480;
    uint8_t *outputData = new uint8_t[width * height * 3];

    // Decompress the JPEG image
     id_000006_decompressJPEG(inputData, inputFileSize, outputData, width, height, 3);

    // Clean up resources
    delete[] inputData;
    delete[] outputData;

    assert_file_closed(&in_file);
	return 0;
}