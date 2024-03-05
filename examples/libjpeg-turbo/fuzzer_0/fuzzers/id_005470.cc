#include "FDSan.h"
#include <turbojpeg.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 5470
//<Prompt> ["tjCompressFromYUV","tj3Init","tj3Compress8","tj3CompressFromYUVPlanes8","tj3SaveImage8","tjDestroy"]
/*<Combination>: [int tjCompressFromYUV(tjhandle handle, const unsigned char * srcBuf, int width, int align, int height, int subsamp, unsigned char ** jpegBuf, unsigned long * jpegSize, int jpegQual, int flags),
    tjhandle tj3Init(int initType),
    int tj3Compress8(tjhandle handle, const unsigned char * srcBuf, int width, int pitch, int height, int pixelFormat, unsigned char ** jpegBuf, size_t * jpegSize),
    int tj3CompressFromYUVPlanes8(tjhandle handle, const unsigned char *const * srcPlanes, int width, const int * strides, int height, unsigned char ** jpegBuf, size_t * jpegSize),
    int tj3SaveImage8(tjhandle handle, const char * filename, const unsigned char * buffer, int width, int pitch, int height, int pixelFormat),
    int tjDestroy(tjhandle handle)
*/
//<score> 0.6666667, nr_unique_branch: 1, p_cov: 0.33333334
//<Quality> {"diversity":7,"density":7,"covered_api":["tjCompressFromYUV","tjDestroy"],"uncovered_api":["tj3Init","tj3Compress8","tj3CompressFromYUVPlanes8","tj3SaveImage8"],"unique_branches":{"jpeg_add_quant_table":[[58,9,58,19,0]]},"library_calls":["tjInitCompress","tjCompressFromYUV","tjDestroy"]}
/**/



extern "C" int LLVMFuzzerTestOneInput_90(const uint8_t *data, size_t size) {
	if(size<=0) return 0;

    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file == NULL) {
        assert_file_closed(&in_file);
	return 0;
    }

    FILE *out_file = fopen("output_file", "wb");
    if (out_file == NULL) {
        assert_file_closed(&in_file);;
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    tjhandle handle = tjInitCompress();
    if (handle == NULL) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    int width = 640;
    int height = 480;
    int subsamp = TJSAMP_420;
    unsigned char *srcBuf = (unsigned char *)malloc(width * height * 3);
    if (srcBuf == NULL) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        tjDestroy(handle);
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    unsigned char *jpegBuf = NULL;
    unsigned long jpegSize = 0;
    int jpegQual = 100;
    int flags = 0;

    // Read input data
    size_t bytesRead = fread(srcBuf, 1, width * height * 3, in_file);
    if (bytesRead != width * height * 3) {
        free(srcBuf);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        tjDestroy(handle);
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Compress the image
    int ret = tjCompressFromYUV(handle, srcBuf, width, 1, height, subsamp, &jpegBuf, &jpegSize, jpegQual, flags);
    if (ret != 0) {
        free(srcBuf);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        tjDestroy(handle);
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Write output data
    size_t bytesWritten = fwrite(jpegBuf, 1, jpegSize, out_file);
    if (bytesWritten != jpegSize) {
        free(srcBuf);
        free(jpegBuf);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        tjDestroy(handle);
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    free(srcBuf);
    free(jpegBuf);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    tjDestroy(handle);

    assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}