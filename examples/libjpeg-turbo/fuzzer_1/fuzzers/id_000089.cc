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
//<ID> 5395
//<Prompt> ["tjCompressFromYUV","tjPlaneWidth","tjEncodeYUV2","tjDestroy","tj3Decompress16","tj3DecodeYUVPlanes8"]
/*<Combination>: [int tjCompressFromYUV(tjhandle handle, const unsigned char * srcBuf, int width, int align, int height, int subsamp, unsigned char ** jpegBuf, unsigned long * jpegSize, int jpegQual, int flags),
    int tjPlaneWidth(int componentID, int width, int subsamp),
    int tjEncodeYUV2(tjhandle handle, unsigned char * srcBuf, int width, int pitch, int height, int pixelFormat, unsigned char * dstBuf, int subsamp, int flags),
    int tjDestroy(tjhandle handle),
    int tj3Decompress16(tjhandle handle, const unsigned char * jpegBuf, size_t jpegSize, unsigned short * dstBuf, int pitch, int pixelFormat),
    int tj3DecodeYUVPlanes8(tjhandle handle, const unsigned char *const * srcPlanes, const int * strides, unsigned char * dstBuf, int width, int pitch, int height, int pixelFormat)
*/
//<score> 8.4, nr_unique_branch: 17, p_cov: 0.6666667
//<Quality> {"diversity":10,"density":7,"covered_api":["tjCompressFromYUV","tjPlaneWidth","tjDestroy","tj3Decompress16"],"uncovered_api":["tjEncodeYUV2","tj3DecodeYUVPlanes8"],"unique_branches":{"jpeg16_read_scanlines":[[316,7,316,47,0],[319,7,319,45,1],[321,7,321,53,1],[327,7,327,30,1]],"tj3Decompress16":[[158,43,158,57,1],[158,61,158,70,1],[159,7,159,22,1],[159,26,159,49,1],[162,7,162,22,1],[175,7,175,42,0],[185,28,185,41,1],[210,7,210,17,1],[217,7,218,71,1],[224,15,224,37,0],[225,9,225,23,1],[263,12,263,57,0],[270,7,270,41,0]]},"library_calls":["tjInitCompress","tjPlaneWidth","tjPlaneWidth","tjBufSize","tjCompressFromYUV","tjInitDecompress","tj3Decompress16","tjDestroy","tjDestroy","tjFree"]}
/**/


extern "C" int LLVMFuzzerTestOneInput_89(const uint8_t* f_data, size_t f_size) {
	if(f_size<=0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Step 4: Read the input data from a file
    FILE *in_file = fmemopen((void *)data, size, "rb");
    
    // Step 5: Get the file descriptor for reading
    int in_fd = fuzz_fileno(in_file);
    
    // Step 6: Provide the file name for output
    const char* output_file = "output.jpg";
    
    // Step 1: Compress YUV data to JPEG
    tjhandle compress_handle = tjInitCompress();
    int width = 640;
    int height = 480;
    int subsamp = TJSAMP_420;
    unsigned char *jpegBuf = NULL;
    unsigned long jpegSize = 0;
    int jpegQual = 90;
    int flags = 0;
    
    unsigned char *srcBuf = new unsigned char[tjPlaneWidth(0, width, subsamp) * tjPlaneWidth(0, height, subsamp)];
    unsigned char *dstBuf = new unsigned char[tjBufSize(width, height, subsamp)];
    
    int compress_result = tjCompressFromYUV(compress_handle, srcBuf, width, 1, height, subsamp, &jpegBuf, &jpegSize, jpegQual, flags);
    
    // Step 2: Save the compressed JPEG to a file
    FILE *out_file = fopen(output_file, "wb");
    fwrite(jpegBuf, 1, jpegSize, out_file);
    assert_file_closed(&out_file);;
    
    // Step 3: Decompress the JPEG back to YUV
    tjhandle decompress_handle = tjInitDecompress();
    unsigned short *decompressedBuf = new unsigned short[width * height * tjPixelSize[TJPF_RGBX]];
    int pitch = width * tjPixelSize[TJPF_RGBX];
    int pixelFormat = TJPF_RGBX;
    
    int decompress_result = tj3Decompress16(decompress_handle, jpegBuf, jpegSize, decompressedBuf, pitch, pixelFormat);
    
    // Cleanup
    tjDestroy(compress_handle);
    tjDestroy(decompress_handle);
    
    delete[] srcBuf;
    delete[] dstBuf;
    delete[] decompressedBuf;
    tjFree(jpegBuf);
    
    assert_file_closed(&in_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
}