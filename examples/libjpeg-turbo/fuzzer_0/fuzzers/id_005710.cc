#include "FDSan.h"
#include <turbojpeg.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 5710
//<Prompt> ["tjBufSizeYUV","tjBufSize","TJBUFSIZE","tjDecodeYUV","tjDecompress2"]
/*<Combination>: [unsigned long tjBufSizeYUV(int width, int height, int subsamp),
    unsigned long tjBufSize(int width, int height, int jpegSubsamp),
    unsigned long TJBUFSIZE(int width, int height),
    int tjDecodeYUV(tjhandle handle, const unsigned char * srcBuf, int align, int subsamp, unsigned char * dstBuf, int width, int pitch, int height, int pixelFormat, int flags),
    int tjDecompress2(tjhandle handle, const unsigned char * jpegBuf, unsigned long jpegSize, unsigned char * dstBuf, int width, int pitch, int height, int pixelFormat, int flags)
*/
//<score> 68, nr_unique_branch: 84, p_cov: 0.8
//<Quality> {"diversity":9,"density":9,"covered_api":["tjBufSizeYUV","tjBufSize","TJBUFSIZE","tjDecodeYUV"],"uncovered_api":["tjDecompress2"],"unique_branches":{"jpeg_calc_output_dimensions":[[367,7,367,33,0]],"setDecodeDefaults":[[2006,7,2006,35,1],[2019,15,2019,45,0],[2019,15,2019,45,1],[2022,30,2022,38,0],[2022,30,2022,38,1],[2023,30,2023,38,0],[2023,30,2023,38,1],[2027,28,2027,36,0],[2027,28,2027,36,1],[2031,15,2031,20,0],[2031,15,2031,20,1],[2032,9,2032,46,0]],"jsimd_h2v2_merged_upsample":[[572,7,572,26,1],[576,3,576,19,0],[580,3,580,20,1],[581,3,581,20,1],[585,3,585,19,1],[589,3,589,20,1],[590,3,590,20,1],[594,3,594,20,1],[595,3,595,20,1],[599,3,599,20,1],[600,3,600,20,1],[604,3,604,10,1],[610,7,610,32,0]],"tj3DecodeYUVPlanes8":[[2085,7,2085,38,1],[2087,7,2087,31,1],[2090,7,2090,17,0],[2107,28,2107,41,1],[2116,7,2116,17,1],[2118,7,2118,73,1],[2120,15,2120,25,0],[2120,15,2120,25,1],[2121,9,2121,23,1],[2126,7,2126,19,1],[2129,15,2129,40,0],[2129,15,2129,40,1],[2134,9,2134,20,1],[2137,9,2137,19,1],[2139,19,2139,47,0],[2139,19,2139,47,1],[2149,9,2149,18,1],[2152,19,2152,30,0],[2152,19,2152,30,1],[2154,15,2154,22,0],[2154,26,2154,41,0],[2163,17,2163,26,0],[2163,17,2163,26,1],[2166,45,2166,70,0],[2166,45,2166,70,1]],"master_selection":[[606,9,606,38,0],[608,11,608,38,1],[610,16,610,43,1]],"use_merged_upsample":[[45,7,45,43,1],[45,47,45,73,1],[46,8,46,41,0],[47,8,47,44,0],[48,8,48,45,1],[59,8,59,44,1],[61,8,61,44,0],[62,8,62,76,1],[65,7,65,45,1],[66,7,66,45,1],[67,7,67,45,1],[68,7,68,45,1],[69,7,69,45,1],[70,7,70,45,1],[73,7,73,74,1],[74,7,74,74,1],[75,7,75,74,1]],"tj3DecodeYUV8":[[2230,7,2230,38,1],[2237,7,2237,35,1]],"jsimd_can_h2v2_merged_upsample":[[534,7,534,34,0],[32,30,32,50,0]],"jinit_merged_upsampler":[[541,7,541,47,1],[553,7,553,36,0],[556,9,556,41,0],[561,9,561,45,1]],"merged_2v_upsample":[[232,7,232,27,1],[245,9,245,40,1],[249,9,249,34,1],[253,9,253,21,0],[267,7,267,28,0]]},"library_calls":["tjBufSizeYUV","tjBufSize","TJBUFSIZE","tjInitDecompress","tjDecodeYUV","tjCompress2","tjDestroy"]}
/**/


extern "C" int LLVMFuzzerTestOneInput_97(const uint8_t *data, size_t size)
{
	if(size<=0) return 0;

	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);

    // Step 4: Open input file
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file == NULL)
    {
        assert_file_closed(&in_file);
	return 0; // Failed to open input file
    }

    // Step 5: Get file descriptor for reading
    int in_fd = fuzz_fileno(in_file);

    // Step 6: Specify input file name
    const char *input_file = "input_file";

    // Step 7: Open output file
    FILE *out_file = fopen("output_file", "wb");
    if (out_file == NULL)
    {
        assert_file_closed(&in_file);;
        assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0; // Failed to open output file
    }

    // Step 5: Get file descriptor for writing
    int out_fd = fuzz_fileno(out_file);

    // Step 6: Specify output file name
    const char *output_file = "output_file";

    // Step 1: Calculate buffer sizes
    int width = 1920;  // Input image width
    int height = 1080; // Input image height
    int subsamp = TJSAMP_420; // Subsampling mode

    unsigned long yuv_buf_size = tjBufSizeYUV(width, height, subsamp); // YUV buffer size
    unsigned long jpeg_buf_size = tjBufSize(width, height, subsamp);  // JPEG buffer size
    unsigned long buf_size = TJBUFSIZE(width, height); // Output buffer size

    // Step 2: Allocate memory for buffers
    unsigned char *yuv_buf = (unsigned char *)malloc(yuv_buf_size);
    unsigned char *jpeg_buf = (unsigned char *)malloc(jpeg_buf_size);
    unsigned char *buf = (unsigned char *)malloc(buf_size);

    if (yuv_buf == NULL || jpeg_buf == NULL || buf == NULL)
    {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        free(yuv_buf);
        free(jpeg_buf);
        free(buf);
        assert_file_name_closed("output_file");
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0; // Failed to allocate memory for buffers
    }

    // Step 3: Read input data from file
    fread(yuv_buf, 1, yuv_buf_size, in_file);

    // Step 8: Create TurboJPEG decompressor
    tjhandle handle = tjInitDecompress();
    if (handle == NULL)
    {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        free(yuv_buf);
        free(jpeg_buf);
        free(buf);
        assert_file_name_closed("output_file");
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0; // Failed to create TurboJPEG decompressor
    }

    // Step 9: Decode YUV image to RGB
    int align = 1; // Assume no padding is needed
    int pixelFormat = TJPF_RGB; // Output pixel format
    int flags = 0; // No flags
    int result = tjDecodeYUV(handle, yuv_buf, align, subsamp, buf, width, 0, height, pixelFormat, flags);
    if (result != 0)
    {
        tjDestroy(handle);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        free(yuv_buf);
        free(jpeg_buf);
        free(buf);
        assert_file_name_closed("output_file");
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0; // Failed to decode YUV image
    }

    // Step 10: Compress RGB image to JPEG
    int pitch = width * tjPixelSize[pixelFormat]; // Image pitch
    result = tjCompress2(handle, buf, width, pitch, height, pixelFormat, &jpeg_buf, &jpeg_buf_size, subsamp, 90, 0);
    if (result != 0)
    {
        tjDestroy(handle);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        free(yuv_buf);
        free(jpeg_buf);
        free(buf);
        assert_file_name_closed("output_file");
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0; // Failed to compress RGB image
    }

    // Step 11: Write output data to file
    fwrite(jpeg_buf, 1, jpeg_buf_size, out_file);

    // Step 12: Destroy TurboJPEG decompressor
    tjDestroy(handle);

    // Step 13: Release allocated resources
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    free(yuv_buf);
    free(jpeg_buf);
    free(buf);

    assert_file_name_closed("output_file");
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}