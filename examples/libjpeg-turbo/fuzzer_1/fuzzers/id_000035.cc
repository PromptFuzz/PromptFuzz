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
//<ID> 992
//<Prompt> ["tj3Decompress8","tjInitDecompress","tj3Decompress12","tjCompressFromYUVPlanes","tjCompress2"]
/*<Combination>: [int tj3Decompress8(tjhandle handle, const unsigned char * jpegBuf, size_t jpegSize, unsigned char * dstBuf, int pitch, int pixelFormat),
    tjhandle tjInitDecompress(),
    int tj3Decompress12(tjhandle handle, const unsigned char * jpegBuf, size_t jpegSize, short * dstBuf, int pitch, int pixelFormat),
    int tjCompressFromYUVPlanes(tjhandle handle, const unsigned char ** srcPlanes, int width, const int * strides, int height, int subsamp, unsigned char ** jpegBuf, unsigned long * jpegSize, int jpegQual, int flags),
    int tjCompress2(tjhandle handle, const unsigned char * srcBuf, int width, int pitch, int height, int pixelFormat, unsigned char ** jpegBuf, unsigned long * jpegSize, int jpegSubsamp, int jpegQual, int flags)
*/
//<score> 0.8, nr_unique_branch: 1, p_cov: 0.4
//<Quality> {"diversity":7,"density":7,"covered_api":["tjInitDecompress","tjCompress2"],"uncovered_api":["tj3Decompress8","tj3Decompress12","tjCompressFromYUVPlanes"],"unique_branches":{"tjCompress2":[[301,7,301,12,0]]},"library_calls":["tjInitDecompress","tjDecompressHeader3","tjDecompressToYUVPlanes","tjFree","tjDestroy","tjCompress2","tjFree"]}
/**/


// Function to handle JPEG decompression
void  id_000035_decompressJPEG(const unsigned char* jpegBuf, size_t jpegSize, unsigned char* dstBuf, int pitch, int pixelFormat) {
    tjhandle handle = tjInitDecompress();

    tj3Decompress8(handle, jpegBuf, jpegSize, dstBuf, pitch, pixelFormat);

    tjDestroy(handle);
}

// Function to handle YUV plane compression
void  id_000035_compressYUVPlanes(const unsigned char** srcPlanes, int width, const int* strides, int height, int subsamp, unsigned char** jpegBuf, unsigned long* jpegSize, int jpegQual, int flags) {
    tjhandle handle = tjInitCompress();

    tjCompressFromYUVPlanes(handle, srcPlanes, width, strides, height, subsamp, jpegBuf, jpegSize, jpegQual, flags);

    tjDestroy(handle);
}

// Function to handle general compression
void  id_000035_compressImage(const unsigned char* srcBuf, int width, int pitch, int height, int pixelFormat, unsigned char** jpegBuf, unsigned long* jpegSize, int jpegSubsamp, int jpegQual, int flags) {
    tjhandle handle = tjInitCompress();

    tjCompress2(handle, srcBuf, width, pitch, height, pixelFormat, jpegBuf, jpegSize, jpegSubsamp, jpegQual, flags);

    tjDestroy(handle);
}

extern "C"
int LLVMFuzzerTestOneInput_35(const uint8_t* f_data, size_t f_size) {
	if(f_size<=12) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
	//fuzzer shim end}




    // Create a file from input data
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Read the input file and get the size
    fseek(in_file, 0, SEEK_END);
    size_t file_size = ftell(in_file);
    fseek(in_file, 0, SEEK_SET);

    // Allocate memory to store the input data
    unsigned char* jpeg_data = (unsigned char*)malloc(file_size);
    if (!jpeg_data) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // Read the input file data into memory
    fread(jpeg_data, 1, file_size, in_file);

    // Close the input file
    assert_file_closed(&in_file);;

    // Create an output file
    FILE* out_file = fopen("output_file.jpg", "wb");
    if (!out_file) {
        free(jpeg_data);
        assert_file_name_closed("output_file.jpg");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Decompress the input JPEG data
    int width, height, subsamp, colorspace;
    unsigned char* rgb_data = NULL;
    unsigned long rgb_size = 0;
    int pixelFormat = TJPF_RGB;

    tjhandle decompress_handle = tjInitDecompress();
    tjDecompressHeader3(decompress_handle, jpeg_data, file_size, &width, &height, &subsamp, &colorspace);
    tjDecompressToYUVPlanes(decompress_handle, jpeg_data, file_size, (unsigned char**)&rgb_data, width, NULL, height, fuzz_int32_t_1);
    tjFree(rgb_data);
    tjDestroy(decompress_handle);

    // Compress the RGB data to JPEG
    unsigned char* compressed_data = NULL;
    unsigned long compressed_size = 0;
    tjCompress2(
        NULL, rgb_data, width, 0, height, pixelFormat, &compressed_data, &compressed_size, subsamp, fuzz_int32_t_2, fuzz_int32_t_3
    );

    // Write the compressed JPEG data to the output file
    fwrite(compressed_data, 1, compressed_size, out_file);

    // Close the output file
    assert_file_closed(&out_file);;

    // Free the allocated memory
    free(jpeg_data);
    tjFree(compressed_data);

    assert_file_name_closed("output_file.jpg");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}