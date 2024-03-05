#include "FDSan.h"
#include <turbojpeg.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 3789
//<Prompt> ["tjPlaneWidth","tj3Decompress8","tj3SetCroppingRegion","tjDestroy","tjDecompress"]
/*<Combination>: [int tjPlaneWidth(int componentID, int width, int subsamp),
    int tj3Decompress8(tjhandle handle, const unsigned char * jpegBuf, size_t jpegSize, unsigned char * dstBuf, int pitch, int pixelFormat),
    int tj3SetCroppingRegion(tjhandle handle, tjregion croppingRegion),
    int tjDestroy(tjhandle handle),
    int tjDecompress(tjhandle handle, unsigned char * jpegBuf, unsigned long jpegSize, unsigned char * dstBuf, int width, int pitch, int height, int pixelSize, int flags)
*/
//<score> 2.4, nr_unique_branch: 2, p_cov: 0.8
//<Quality> {"diversity":6,"density":6,"covered_api":["tjPlaneWidth","tj3SetCroppingRegion","tjDestroy","tjDecompress"],"uncovered_api":["tj3Decompress8"],"unique_branches":{"tj3SetCroppingRegion":[[1900,55,1900,75,0],[1907,7,1907,38,0]]},"library_calls":["tjInitDecompress","tjDecompressHeader","tj3SetCroppingRegion","tjDecompress","tjPlaneWidth","tjDestroy"]}
/**/


// Step 1: Event achieved using libjpeg-turbo APIs:
// Given a byte stream of libjpeg-turbo's output data, the event is to decompress the data and obtain the width of a specific plane.

// Step 2: Complete the LLVMFuzzerTestOneInput_76 function
extern "C" int LLVMFuzzerTestOneInput_76(const uint8_t *data, size_t size) {
	if(size<=0) return 0;

    tjhandle handle = tjInitDecompress();

    FILE *in_file = fmemopen((void *)data, size, "rb");
    int in_fd = fuzz_fileno(in_file);

    // Read the input data
    fseek(in_file, 0, SEEK_END);
    long in_size = ftell(in_file);
    fseek(in_file, 0, SEEK_SET);

    unsigned char *jpegBuf = (unsigned char *)malloc(in_size);
    fread(jpegBuf, 1, in_size, in_file);

    // Decompress the data
    int width, height;
    tjDecompressHeader(handle, jpegBuf, in_size, &width, &height);

    // Set the cropping region
    tjregion croppingRegion;
    croppingRegion.x = 0;
    croppingRegion.y = 0;
    croppingRegion.w = width;
    croppingRegion.h = height;
    tj3SetCroppingRegion(handle, croppingRegion);

    // Allocate memory for the decompressed image
    int pitch = width * tjPixelSize[TJPF_RGB];
    unsigned char *dstBuf = (unsigned char *)malloc(pitch * height);

    // Perform the decompression
    tjDecompress(handle, jpegBuf, in_size, dstBuf, width, pitch, height, TJPF_RGB, 0);

    // Step 1: Obtain the width of the specific plane
    int planeWidth = tjPlaneWidth(0, width, TJSAMP_420);

    // Cleanup
    tjDestroy(handle);
    assert_file_closed(&in_file);;
    free(jpegBuf);
    free(dstBuf);

    assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
}