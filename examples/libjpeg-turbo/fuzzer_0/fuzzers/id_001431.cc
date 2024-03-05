#include "FDSan.h"
#include <turbojpeg.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 1431
//<Prompt> ["tj3Decompress8","tj3Destroy","tjDecompressHeader","tjDecodeYUVPlanes","tj3SetScalingFactor"]
/*<Combination>: [int tj3Decompress8(tjhandle handle, const unsigned char * jpegBuf, size_t jpegSize, unsigned char * dstBuf, int pitch, int pixelFormat),
    void tj3Destroy(tjhandle handle),
    int tjDecompressHeader(tjhandle handle, unsigned char * jpegBuf, unsigned long jpegSize, int * width, int * height),
    int tjDecodeYUVPlanes(tjhandle handle, const unsigned char ** srcPlanes, const int * strides, int subsamp, unsigned char * dstBuf, int width, int pitch, int height, int pixelFormat, int flags),
    int tj3SetScalingFactor(tjhandle handle, tjscalingfactor scalingFactor)
*/
//<score> 0, nr_unique_branch: 1, p_cov: 0
//<Quality> {"diversity":0,"density":1,"covered_api":[],"uncovered_api":["tj3Decompress8","tj3Destroy","tjDecompressHeader","tjDecodeYUVPlanes","tj3SetScalingFactor"],"unique_branches":{"tjDecodeYUVPlanes":[[2201,7,2201,18,0]]},"library_calls":[]}
/**/


// Function to decompress the JPEG data
void  id_001431_decompressJPEG(const uint8_t* data, size_t size) {
    tjhandle handle = tjInitDecompress();

    // Decompress the JPEG header to get the image dimensions
    int width, height;
    int jpegSubsamp, jpegColorspace;
    tjDecompressHeader3(handle, data, size, &width, &height, &jpegSubsamp, &jpegColorspace);

    // Calculate the buffer size needed for the decompressed image
    int pixelSize = tjPixelSize[TJPF_RGB];
    int pitch = width * pixelSize;
    unsigned char* dstBuf = new unsigned char[pitch * height];

    // Decompress the JPEG data
    tjDecompressToYUVPlanes(handle, data, size, NULL, width, NULL, height, 0);
    tjDecodeYUVPlanes(handle, NULL, NULL, jpegSubsamp, dstBuf, width, pitch, height, TJPF_RGB, 0);

    // Clean up
    tjDestroy(handle);
    delete[] dstBuf;
}

extern "C" int LLVMFuzzerTestOneInput_41(const uint8_t* data, size_t size) {
	if(size<=0) return 0;

    // Create a file stream for reading the input data
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (in_file == nullptr) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Read the file contents into a buffer
    fseek(in_file, 0, SEEK_END);
    long file_size = ftell(in_file);
    rewind(in_file);
    unsigned char* buffer = new unsigned char[file_size];
    fread(buffer, sizeof(unsigned char), file_size, in_file);

    // Call the  id_001431_decompressJPEG function
     id_001431_decompressJPEG(buffer, file_size);

    // Clean up
    delete[] buffer;
    assert_file_closed(&in_file);;

    assert_file_closed(&in_file);
	return 0;
}