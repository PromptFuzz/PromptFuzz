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
//<ID> 1095
//<Prompt> ["tj3SetCroppingRegion","tjDecodeYUVPlanes","tjGetErrorStr","tj3SaveImage8","tj3SaveImage16","tjEncodeYUVPlanes","tjDecompressHeader3"]
/*<Combination>: [int tj3SetCroppingRegion(tjhandle handle, tjregion croppingRegion),
    int tjDecodeYUVPlanes(tjhandle handle, const unsigned char ** srcPlanes, const int * strides, int subsamp, unsigned char * dstBuf, int width, int pitch, int height, int pixelFormat, int flags),
    char *tjGetErrorStr(),
    int tj3SaveImage8(tjhandle handle, const char * filename, const unsigned char * buffer, int width, int pitch, int height, int pixelFormat),
    int tj3SaveImage16(tjhandle handle, const char * filename, const unsigned short * buffer, int width, int pitch, int height, int pixelFormat),
    int tjEncodeYUVPlanes(tjhandle handle, const unsigned char * srcBuf, int width, int pitch, int height, int pixelFormat, unsigned char ** dstPlanes, int * strides, int subsamp, int flags),
    int tjDecompressHeader3(tjhandle handle, const unsigned char * jpegBuf, unsigned long jpegSize, int * width, int * height, int * jpegSubsamp, int * jpegColorspace)
*/
//<score> 1.8, nr_unique_branch: 1, p_cov: 1
//<Quality> {"diversity":10,"density":9,"covered_api":["tj3SetCroppingRegion","tjDecodeYUVPlanes","tjGetErrorStr","tj3SaveImage8","tj3SaveImage16","tjEncodeYUVPlanes","tjDecompressHeader3"],"uncovered_api":[],"unique_branches":{"tj3DecodeYUVPlanes8":[[2074,38,2074,52,0]]},"library_calls":["tjInitTransform","tj3SetCroppingRegion","tjDecodeYUVPlanes","tjGetErrorStr","tj3SaveImage8","tj3SaveImage16","tjEncodeYUVPlanes","tjDecompressHeader3","tjDestroy","tjFree"]}
/**/

extern "C" int LLVMFuzzerTestOneInput_37(const uint8_t* f_data, size_t f_size) {
	if(f_size<=0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    tjhandle handle = tjInitTransform();
    if (handle == NULL) {
        return 0;
    }
    
    // Set cropping region
    tjregion croppingRegion;
    croppingRegion.x = 0;
    croppingRegion.y = 0;
    croppingRegion.w = 100;
    croppingRegion.h = 100;
    tj3SetCroppingRegion(handle, croppingRegion);
    
    // Decode YUV planes
    unsigned char *dstBuf = NULL;
    const unsigned char *srcPlanes[3] = {data, NULL, NULL};
    const int strides[3] = {100, 0, 0};
    int subsamp = TJSAMP_420;
    int width = 100;
    int pitch = 100;
    int height = 100;
    int pixelFormat = TJPF_RGB;
    int flags = 0;
    tjDecodeYUVPlanes(handle, srcPlanes, strides, subsamp, dstBuf, width, pitch, height, pixelFormat, flags);
    
    // Get error string
    char *errorStr = tjGetErrorStr();
    
    // Save image as 8-bit
    const char *filename8 = "output_file8.jpg";
    const unsigned char *buffer8 = dstBuf;
    int width8 = 100;
    int pitch8 = 100;
    int height8 = 100;
    int pixelFormat8 = TJPF_RGB;
    tj3SaveImage8(handle, filename8, buffer8, width8, pitch8, height8, pixelFormat8);
    
    // Save image as 16-bit
    const char *filename16 = "output_file16.jpg";
    const unsigned short *buffer16 = (const unsigned short *)dstBuf;
    int width16 = 100;
    int pitch16 = 100;
    int height16 = 100;
    int pixelFormat16 = TJPF_RGB;
    tj3SaveImage16(handle, filename16, buffer16, width16, pitch16, height16, pixelFormat16);
    
    // Encode YUV planes
    unsigned char *srcBuf = NULL;
    int subsampEncode = TJSAMP_420;
    unsigned char *dstPlanes[3] = {NULL, NULL, NULL};
    int stridesEncode[3] = {0, 0, 0};
    int flagsEncode = 0;
    tjEncodeYUVPlanes(handle, srcBuf, width, pitch, height, pixelFormat, dstPlanes, stridesEncode, subsampEncode, flagsEncode);
    
    // Decompress header
    const unsigned char *jpegBuf = data;
    unsigned long jpegSize = size;
    int widthDecompress, heightDecompress, jpegSubsamp, jpegColorspace;
    tjDecompressHeader3(handle, jpegBuf, jpegSize, &widthDecompress, &heightDecompress, &jpegSubsamp, &jpegColorspace);
    
    tjDestroy(handle);
    tjFree(dstBuf);
    
    return 0;
}