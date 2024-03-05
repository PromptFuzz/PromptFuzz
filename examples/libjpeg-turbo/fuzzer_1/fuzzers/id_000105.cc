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
//<ID> 6855
//<Prompt> ["tj3SetCroppingRegion","tj3CompressFromYUVPlanes8","tj3DecompressHeader","tj3Set","tj3Compress16"]
/*<Combination>: [int tj3SetCroppingRegion(tjhandle handle, tjregion croppingRegion),
    int tj3CompressFromYUVPlanes8(tjhandle handle, const unsigned char *const * srcPlanes, int width, const int * strides, int height, unsigned char ** jpegBuf, size_t * jpegSize),
    int tj3DecompressHeader(tjhandle handle, const unsigned char * jpegBuf, size_t jpegSize),
    int tj3Set(tjhandle handle, int param, int value),
    int tj3Compress16(tjhandle handle, const unsigned short * srcBuf, int width, int pitch, int height, int pixelFormat, unsigned char ** jpegBuf, size_t * jpegSize)
*/
//<score> 3, nr_unique_branch: 2, p_cov: 1
//<Quality> {"diversity":14,"density":14,"covered_api":["tj3SetCroppingRegion","tj3CompressFromYUVPlanes8","tj3DecompressHeader","tj3Set","tj3Compress16"],"uncovered_api":[],"unique_branches":{"tj3Set":[[617,3,617,25,0],[618,9,618,35,1]]},"library_calls":["tjInitTransform","tj3SetCroppingRegion","tj3Alloc","tj3CompressFromYUVPlanes8","tj3Alloc","tj3DecompressHeader","tj3Set","tj3Alloc","tj3Compress16","tj3Free","tj3Free","tj3Free","tj3Free","tj3Destroy"]}
/*Here is the complete C++ program that uses the libjpeg-turbo library APIs to achieve the specified event:

*/


extern "C" int LLVMFuzzerTestOneInput_105(const uint8_t* f_data, size_t f_size) {
	if(f_size<=4) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
	//fuzzer shim end}




    FILE *in_file = fmemopen((void *)data, size, "rb");
    FILE *out_file = fopen("output_file", "wb");
    if (in_file == NULL || out_file == NULL) {
        if (in_file) assert_file_closed(&in_file);;
        if (out_file) assert_file_closed(&out_file);;
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);

    // Step 1: Set cropping region
    tjhandle handle = tjInitTransform();
    tjregion croppingRegion = {0, 0, 100, 100};
    tj3SetCroppingRegion(handle, croppingRegion);

    // Step 2: Compress from YUV planes
    int width = 100, height = 100;
    int strides[3] = {width, width / 2, width / 2};
    unsigned char *jpegBuf = NULL;
    size_t jpegSize = 0;
    unsigned char *buffer[3];  // Assuming 3 planes for YUV input
    for (int i = 0; i < 3; i++) {
        buffer[i] = (unsigned char *)tj3Alloc(width * height * sizeof(unsigned char));
        // Read YUV plane data from input file
        fread(buffer[i], 1, width * height, in_file);
    }
    tj3CompressFromYUVPlanes8(handle, buffer, width, strides, height, &jpegBuf, &jpegSize);

    // Step 3: Decompress header
    unsigned char *jpegHeaderBuf = (unsigned char *)tj3Alloc(jpegSize);
    fread(jpegHeaderBuf, 1, jpegSize, in_file);
    tj3DecompressHeader(handle, jpegHeaderBuf, jpegSize);

    // Step 4: Set parameter
    tj3Set(handle, TJXOP_ROT90, fuzz_int32_t_1);

    // Step 5: Compress 16
    unsigned short *srcBuf = (unsigned short *)tj3Alloc(width * height * sizeof(unsigned short));
    // Read 16-bit image data from input file
    fread(srcBuf, sizeof(unsigned short), width * height, in_file);
    tj3Compress16(handle, srcBuf, width, width * sizeof(unsigned short), height, TJPF_GRAY, &jpegBuf, &jpegSize);

    // Step 6: Write output to file
    fwrite(jpegBuf, 1, jpegSize, out_file);

    // Step 7: Cleanup
    tj3Free(jpegBuf);
    tj3Free(jpegHeaderBuf);
    for (int i = 0; i < 3; i++) {
        tj3Free(buffer[i]);
    }
    tj3Free(srcBuf);
    tj3Destroy(handle);

    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;

    assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}