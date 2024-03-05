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
//<ID> 4027
//<Prompt> ["tjPlaneSizeYUV","tj3YUVPlaneWidth","tjGetErrorCode","tjDecompressHeader2","tjFree","tj3Decompress12"]
/*<Combination>: [unsigned long tjPlaneSizeYUV(int componentID, int width, int stride, int height, int subsamp),
    int tj3YUVPlaneWidth(int componentID, int width, int subsamp),
    int tjGetErrorCode(tjhandle handle),
    int tjDecompressHeader2(tjhandle handle, unsigned char * jpegBuf, unsigned long jpegSize, int * width, int * height, int * jpegSubsamp),
    void tjFree(unsigned char * buffer),
    int tj3Decompress12(tjhandle handle, const unsigned char * jpegBuf, size_t jpegSize, short * dstBuf, int pitch, int pixelFormat)
*/
//<score> 1.6666666, nr_unique_branch: 1, p_cov: 0.8333333
//<Quality> {"diversity":9,"density":9,"covered_api":["tjPlaneSizeYUV","tjGetErrorCode","tjDecompressHeader2","tjFree","tj3Decompress12"],"uncovered_api":["tj3YUVPlaneWidth"],"unique_branches":{"tj3YUVPlaneSize":[[1116,7,1116,18,0]]},"library_calls":["tjInitDecompress","tjDecompressHeader2","tjPlaneSizeYUV","tjFree","tjGetErrorCode","tj3Decompress12","tjDestroy"]}
/**/


extern "C" int LLVMFuzzerTestOneInput_79(const uint8_t* f_data, size_t f_size) {
	if(f_size<=0) return 0;

	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(f_data, sizeof(uint8_t), f_size, input_file_ptr);
	fclose(input_file_ptr);

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Step 4: Create a file for reading the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Step 5: Get the file descriptor for reading
    int in_fd = fuzz_fileno(in_file);

    // Step 6: Set the input file name
    const char *input_file = "input_file";

    // Step 2: Allocate a tjhandle for decompression
    tjhandle handle = tjInitDecompress();
    if (!handle) {
        assert_file_closed(&in_file);;
        assert_file_name_closed("input_file");
	assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
    }

    // Step 2: Allocate a buffer for the decompressed image
    int width, height, subsamp;
    unsigned char *dstBuf = NULL;
  
    // Step 2: Read the JPEG header to get the image dimensions and subsampling
    if (tjDecompressHeader2(handle, (unsigned char *)data, size, &width, &height, &subsamp) != 0) {
        tjDestroy(handle);
        assert_file_closed(&in_file);;
        assert_file_name_closed("input_file");
	assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
    }

    // Step 2: Calculate the buffer size required for the YUV plane
    int planeSize = tjPlaneSizeYUV(0, width, 0, height, subsamp);

    // Step 7: Free the allocated buffer
    tjFree(dstBuf);

    // Step 3: Get the error code
    int errorCode = tjGetErrorCode(handle);

    // Step 1: Call the API tj3Decompress12
    if (tj3Decompress12(handle, (unsigned char *)data, size, (short *)dstBuf, width, subsamp) != 0) {
        tjDestroy(handle);
        assert_file_closed(&in_file);;
        assert_file_name_closed("input_file");
	assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
    }

    // Step 7: Destroy the tjhandle
    tjDestroy(handle);

    // Step 4: Close the input file
    assert_file_closed(&in_file);;

    assert_file_name_closed("input_file");
	assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
}