#include "FDSan.h"
#include <turbojpeg.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 3875
//<Prompt> ["tjPlaneWidth","tj3DecompressHeader","tj3Decompress8","tjInitCompress","tjCompressFromYUVPlanes"]
/*<Combination>: [int tjPlaneWidth(int componentID, int width, int subsamp),
    int tj3DecompressHeader(tjhandle handle, const unsigned char * jpegBuf, size_t jpegSize),
    int tj3Decompress8(tjhandle handle, const unsigned char * jpegBuf, size_t jpegSize, unsigned char * dstBuf, int pitch, int pixelFormat),
    tjhandle tjInitCompress(),
    int tjCompressFromYUVPlanes(tjhandle handle, const unsigned char ** srcPlanes, int width, const int * strides, int height, int subsamp, unsigned char ** jpegBuf, unsigned long * jpegSize, int jpegQual, int flags)
*/
//<score> 2, nr_unique_branch: 2, p_cov: 1
//<Quality> {"diversity":6,"density":4,"covered_api":["tjPlaneWidth","tj3DecompressHeader","tj3Decompress8","tjInitCompress","tjCompressFromYUVPlanes"],"uncovered_api":[],"unique_branches":{"tj3Decompress8":[[290,7,290,12,0]],"tj3DecompressHeader":[[290,7,290,12,0]]},"library_calls":["tjPlaneWidth","tj3DecompressHeader","tj3Decompress8","tjInitCompress","tjCompressFromYUVPlanes","tjDestroy"]}
/**/


extern "C" int LLVMFuzzerTestOneInput_78(const uint8_t *data, size_t size) {
	if(size<=0) return 0;

	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);

    // Step 4: Create FILE* variables for reading and writing
    FILE *in_file = fmemopen((void *)data, size, "rb");
    FILE *out_file = fopen("output_file", "wb");

    if (in_file == nullptr || out_file == nullptr) {
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Step 5: Get file descriptors for reading and writing
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);

    // Step 6: Define file names
    const char* input_file = "input_file";
    const char* output_file = "output_file";

    // Step 2: Use libjpeg-turbo APIs
    int width = 0;
    int subsamp = 0;
    int pixelFormat = TJPF_RGB;
    int componentID = 0;
    int pitch = 0;
    unsigned char **jpegBuf = nullptr;
    size_t jpegSize = 0;
    unsigned char *dstBuf = nullptr;
    const unsigned char **srcPlanes = nullptr;
    int *strides = nullptr;
    int height = 0;
    int jpegQual = 90;
    int flags = 0;

    int planeWidth = tjPlaneWidth(componentID, width, subsamp);
    int decompressHeader = tj3DecompressHeader(nullptr, nullptr, 0);
    int decompress8 = tj3Decompress8(nullptr, nullptr, 0, nullptr, pitch, pixelFormat);
    tjhandle compressHandle = tjInitCompress();
    int compressFromYUVPlanes = tjCompressFromYUVPlanes(compressHandle, srcPlanes, width, strides, height, subsamp, jpegBuf, &jpegSize, jpegQual, flags);

    // Step 7: Release allocated resources
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    tjDestroy(compressHandle);

    assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}