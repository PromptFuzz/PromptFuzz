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
//<ID> 6237
//<Prompt> ["tj3Compress12","tjAlloc","tj3CompressFromYUV8","tjEncodeYUV3","tj3LoadImage8"]
/*<Combination>: [int tj3Compress12(tjhandle handle, const short * srcBuf, int width, int pitch, int height, int pixelFormat, unsigned char ** jpegBuf, size_t * jpegSize),
    unsigned char *tjAlloc(int bytes),
    int tj3CompressFromYUV8(tjhandle handle, const unsigned char * srcBuf, int width, int align, int height, unsigned char ** jpegBuf, size_t * jpegSize),
    int tjEncodeYUV3(tjhandle handle, const unsigned char * srcBuf, int width, int pitch, int height, int pixelFormat, unsigned char * dstBuf, int align, int subsamp, int flags),
    unsigned char *tj3LoadImage8(tjhandle handle, const char * filename, int * width, int align, int * height, int * pixelFormat)
*/
//<score> 1.6, nr_unique_branch: 3, p_cov: 0.4
//<Quality> {"diversity":10,"density":10,"covered_api":["tj3CompressFromYUV8","tj3LoadImage8"],"uncovered_api":["tj3Compress12","tjAlloc","tjEncodeYUV3"],"unique_branches":{"tj3LoadImage8":[[317,7,317,45,1],[321,7,321,31,0],[395,7,395,11,0]]},"library_calls":["tjInitCompress","tjInitDecompress","tj3CompressFromYUV8","tj3LoadImage8","tjDecompressToYUV2","tjFree","tjFree","tjFree","tjDestroy","tjDestroy"]}
/**/


extern "C" int LLVMFuzzerTestOneInput_101(const uint8_t* f_data, size_t f_size) {
	if(f_size<=16) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
	//fuzzer shim end}




   // Create a handle for the compressor
   tjhandle compressor = tjInitCompress();

   // Create a handle for the decompressor
   tjhandle decompressor = tjInitDecompress();

   // Compress image from YUV to JPEG
   int width = 640;
   int height = 480;
   int subsamp = TJSAMP_444;
   int pixelFormat = TJPF_RGB;

   unsigned char *jpegBuf = nullptr;
   size_t jpegSize = 0;

   tj3CompressFromYUV8(compressor, data, width, fuzz_int32_t_1, height, &jpegBuf, &jpegSize);

   // Save the compressed image to a file
   FILE *out_file = fopen("output_file.jpg", "wb");
   fwrite(jpegBuf, 1, jpegSize, out_file);
   assert_file_closed(&out_file);;
   
   // Load the compressed image from a file
   int jpegWidth = 0;
   int jpegHeight = 0;
   int jpegPixelFormat = 0;
   unsigned char *jpegData = tj3LoadImage8(decompressor, "output_file.jpg", &jpegWidth, fuzz_int32_t_2, &jpegHeight, &jpegPixelFormat);
   
   // Decompress the image from JPEG to YUV
   int yuvWidth = 0;
   int yuvHeight = 0;
   unsigned long yuvSize = 0;
   unsigned char *yuvData = nullptr;

   tjDecompressToYUV2(decompressor, jpegData, jpegSize, yuvData, yuvWidth, fuzz_int32_t_3, yuvHeight, fuzz_int32_t_4);

   // Clean up allocated resources
   tjFree(jpegBuf);
   tjFree(jpegData);
   tjFree(yuvData);
   tjDestroy(compressor);
   tjDestroy(decompressor);

   assert_file_name_closed("output_file.jpg");
	assert_file_name_closed("output_file.jpg");
	assert_file_closed(&out_file);
	return 0;
}