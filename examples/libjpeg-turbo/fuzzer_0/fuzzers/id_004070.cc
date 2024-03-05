#include "FDSan.h"
#include <turbojpeg.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 4070
//<Prompt> ["tjPlaneSizeYUV","tjGetErrorCode","tjDecompressHeader2","tjFree","tjEncodeYUV","tj3CompressFromYUVPlanes8"]
/*<Combination>: [unsigned long tjPlaneSizeYUV(int componentID, int width, int stride, int height, int subsamp),
    int tjGetErrorCode(tjhandle handle),
    int tjDecompressHeader2(tjhandle handle, unsigned char * jpegBuf, unsigned long jpegSize, int * width, int * height, int * jpegSubsamp),
    void tjFree(unsigned char * buffer),
    int tjEncodeYUV(tjhandle handle, unsigned char * srcBuf, int width, int pitch, int height, int pixelSize, unsigned char * dstBuf, int subsamp, int flags),
    int tj3CompressFromYUVPlanes8(tjhandle handle, const unsigned char *const * srcPlanes, int width, const int * strides, int height, unsigned char ** jpegBuf, size_t * jpegSize)
*/
//<score> 1.254902, nr_unique_branch: 1, p_cov: 0.6666667
//<Quality> {"diversity":17,"density":16,"covered_api":["tjPlaneSizeYUV","tjDecompressHeader2","tjFree","tj3CompressFromYUVPlanes8"],"uncovered_api":["tjGetErrorCode","tjEncodeYUV"],"unique_branches":{"tj3CompressFromYUVPlanes8":[[1495,7,1495,26,0]]},"library_calls":["tjInitDecompress","tjInitCompress","tjDecompressHeader2","tjPlaneSizeYUV","tj3CompressFromYUVPlanes8","tjFree","tjDestroy","tjDestroy"]}
/**/


extern "C" int LLVMFuzzerTestOneInput_80(const uint8_t *data, size_t size) {
	if(size<=0) return 0;

    tjhandle jpegDecompressor = tjInitDecompress();
    tjhandle jpegCompressor = tjInitCompress();
    unsigned char *jpegBuf = NULL;
    unsigned long jpegSize = 0;
    int width = 0;
    int height = 0;
    int jpegSubsamp = 0;

    // Decompress Header
    if (tjDecompressHeader2(jpegDecompressor, (unsigned char *)data, size, &width, &height, &jpegSubsamp) != 0) {
        fprintf(stderr, "Failed to decompress JPEG header\n");
        tjFree(jpegBuf);
        tjDestroy(jpegDecompressor);
        tjDestroy(jpegCompressor);
        return 0;
    }

    // Calculate YUV plane size
    int planeSizeYUV = tjPlaneSizeYUV(0, width, width, height, jpegSubsamp);

    // Allocate memory for YUV planes
    unsigned char **srcPlanes = (unsigned char **)malloc(3 * sizeof(unsigned char *));
    srcPlanes[0] = (unsigned char *)malloc(planeSizeYUV);
    srcPlanes[1] = (unsigned char *)malloc(planeSizeYUV);
    srcPlanes[2] = (unsigned char *)malloc(planeSizeYUV);

    // Set up strides
    int strides[3] = { width, width / 2, width / 2 };

    // Compress from YUV planes
    if (tj3CompressFromYUVPlanes8(jpegCompressor, (const unsigned char *const *)srcPlanes,
            width, strides, height, &jpegBuf, &jpegSize) != 0) {
        fprintf(stderr, "Failed to compress from YUV planes\n");
        tjFree(jpegBuf);
        tjDestroy(jpegDecompressor);
        tjDestroy(jpegCompressor);
        free(srcPlanes[0]);
        free(srcPlanes[1]);
        free(srcPlanes[2]);
        free(srcPlanes);
        return 0;
    }

    // Write JPEG data to a file
    FILE *outFile = fopen("output.jpg", "wb");
    if (outFile == NULL) {
        fprintf(stderr, "Failed to open output file\n");
        tjFree(jpegBuf);
        tjDestroy(jpegDecompressor);
        tjDestroy(jpegCompressor);
        free(srcPlanes[0]);
        free(srcPlanes[1]);
        free(srcPlanes[2]);
        free(srcPlanes);
        assert_file_closed(&outFile);
	return 0;
    }
    fwrite(jpegBuf, 1, jpegSize, outFile);
    assert_file_closed(&outFile);;

    // Free memory and destroy handles
    tjFree(jpegBuf);
    tjDestroy(jpegDecompressor);
    tjDestroy(jpegCompressor);
    free(srcPlanes[0]);
    free(srcPlanes[1]);
    free(srcPlanes[2]);
    free(srcPlanes);

    assert_file_closed(&outFile);
	return 0;
}