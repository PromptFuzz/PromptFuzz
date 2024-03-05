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

extern "C" int LLVMFuzzerTestOneInput_14(const uint8_t* f_data, size_t f_size) {
	if(f_size<=60) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_7, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_8, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_9, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_10, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_11, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_12, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_13, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_14, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_15, fdp);
	//fuzzer shim end}




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
    tjCompress(compress_handle, input_data, /* width */ fuzz_int32_t_1, /* pitch */ fuzz_int32_t_2, /* height */ fuzz_int32_t_3, /* pixelSize */ fuzz_int32_t_4, compressed_data, &compressed_size, /* jpegSubsamp */ fuzz_int32_t_5, /* jpegQual */ fuzz_int32_t_6, /* flags */ 0);

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
    tjEncodeYUV(compress_handle, yuv_planes[0], /* width */ fuzz_int32_t_7, /* pitch */ strides[0], /* height */ fuzz_int32_t_8, /* pixelSize */ fuzz_int32_t_9, output_jpeg_data, /* subsamp */ fuzz_int32_t_10, /* flags */ fuzz_int32_t_11);
    tjCompressFromYUV(compress_handle, yuv_planes[0], /* width */ 0, /* align */ fuzz_int32_t_12, /* height */ 0, /* subsamp */ fuzz_int32_t_13, &output_jpeg_data, &output_jpeg_size, /* jpegQual */ fuzz_int32_t_14, /* flags */ fuzz_int32_t_15);
    
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