#include "FDSan.h"
#include <turbojpeg.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 397
//<Prompt> ["tjCompress","tj3DecompressToYUVPlanes8","tj3DecodeYUV8","tjGetScalingFactors","tjEncodeYUV","tjCompressFromYUV"]
/*<Combination>: [int tjCompress(tjhandle handle, unsigned char * srcBuf, int width, int pitch, int height, int pixelSize, unsigned char * dstBuf, unsigned long * compressedSize, int jpegSubsamp, int jpegQual, int flags),
    int tj3DecompressToYUVPlanes8(tjhandle handle, const unsigned char * jpegBuf, size_t jpegSize, unsigned char ** dstPlanes, int * strides),
    int tj3DecodeYUV8(tjhandle handle, const unsigned char * srcBuf, int align, unsigned char * dstBuf, int width, int pitch, int height, int pixelFormat),
    tjscalingfactor *tjGetScalingFactors(int * numscalingfactors),
    int tjEncodeYUV(tjhandle handle, unsigned char * srcBuf, int width, int pitch, int height, int pixelSize, unsigned char * dstBuf, int subsamp, int flags),
    int tjCompressFromYUV(tjhandle handle, const unsigned char * srcBuf, int width, int align, int height, int subsamp, unsigned char ** jpegBuf, unsigned long * jpegSize, int jpegQual, int flags)
*/
//<score> 1.4999999, nr_unique_branch: 1, p_cov: 0.8333333
//<Quality> {"diversity":10,"density":9,"covered_api":["tjCompress","tj3DecompressToYUVPlanes8","tjGetScalingFactors","tjEncodeYUV","tjCompressFromYUV"],"uncovered_api":["tj3DecodeYUV8"],"unique_branches":{"tj3DecompressToYUVPlanes8":[[2306,7,2306,22,0]]},"library_calls":["tjInitCompress","tjCompress","tjInitDecompress","tj3DecompressToYUVPlanes8","tjGetScalingFactors","tjEncodeYUV","tjCompressFromYUV","tjDestroy","tjDestroy"]}
/**/

extern "C" int LLVMFuzzerTestOneInput_14(const uint8_t *data, size_t size) {
	if(size<=0) return 0;

    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Read the input data from the input file
    fseek(in_file, 0, SEEK_END);
    size_t input_size = ftell(in_file);
    fseek(in_file, 0, SEEK_SET);
    unsigned char *input_data = (unsigned char *)malloc(input_size);
    fread(input_data, 1, input_size, in_file);

    // Create a handle for compression
    tjhandle compress_handle = tjInitCompress();
    if (!compress_handle) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        free(input_data);
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Compress the input data using libjpeg-turbo
    unsigned char *compressed_data = NULL;
    unsigned long compressed_size = 0;
    tjCompress(compress_handle, input_data, /* width */ 0, /* pitch */ 0, /* height */ 0, /* pixelSize */ 0, compressed_data, &compressed_size, /* jpegSubsamp */ 0, /* jpegQual */ 100, /* flags */ 0);

    // Create a handle for decompression
    tjhandle decompress_handle = tjInitDecompress();
    if (!decompress_handle) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        free(input_data);
        tjDestroy(compress_handle);
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Decompress the compressed data to YUV planes
    unsigned char *yuv_planes[3] = {NULL};
    int strides[3] = {0};
    tj3DecompressToYUVPlanes8(decompress_handle, compressed_data, compressed_size, yuv_planes, strides);

    // Get the scaling factors
    int num_scaling_factors = 0;
    tjscalingfactor *scaling_factors = tjGetScalingFactors(&num_scaling_factors);

    // Encode the YUV planes to compressed JPEG format
    unsigned char *output_jpeg_data = NULL;
    unsigned long output_jpeg_size = 0;
    tjEncodeYUV(compress_handle, yuv_planes[0], /* width */ 0, /* pitch */ strides[0], /* height */ 0, /* pixelSize */ 0, output_jpeg_data, /* subsamp */ 0, /* flags */ 0);
    tjCompressFromYUV(compress_handle, yuv_planes[0], /* width */ 0, /* align */ 0, /* height */ 0, /* subsamp */ 0, &output_jpeg_data, &output_jpeg_size, /* jpegQual */ 100, /* flags */ 0);
    
    // Write the output data to the output file
    fwrite(output_jpeg_data, 1, output_jpeg_size, out_file);

    // Clean up
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    free(input_data);
    free(compressed_data);
    tjDestroy(compress_handle);
    tjDestroy(decompress_handle);

    assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}