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
//<ID> 4367
//<Prompt> ["tj3GetScalingFactors","tjLoadImage","tj3CompressFromYUVPlanes8","tjBufSizeYUV2","tjBufSize"]
/*<Combination>: [tjscalingfactor *tj3GetScalingFactors(int * numScalingFactors),
    unsigned char *tjLoadImage(const char * filename, int * width, int align, int * height, int * pixelFormat, int flags),
    int tj3CompressFromYUVPlanes8(tjhandle handle, const unsigned char *const * srcPlanes, int width, const int * strides, int height, unsigned char ** jpegBuf, size_t * jpegSize),
    unsigned long tjBufSizeYUV2(int width, int align, int height, int subsamp),
    unsigned long tjBufSize(int width, int height, int jpegSubsamp)
*/
//<score> 1.0285715, nr_unique_branch: 1, p_cov: 0.6
//<Quality> {"diversity":7,"density":6,"covered_api":["tjLoadImage","tj3CompressFromYUVPlanes8","tjBufSize"],"uncovered_api":["tj3GetScalingFactors","tjBufSizeYUV2"],"unique_branches":{"tjBufSize":[[962,10,962,23,0]]},"library_calls":["tjInitCompress","tjLoadImage","tjGetScalingFactors","tj3CompressFromYUVPlanes8","tjBufSize","tjDestroy","tjFree"]}
/**/

// Event: Load a JPEG image from memory, compress it to a new JPEG image, and get the size of the compressed image
extern "C" int LLVMFuzzerTestOneInput_83(const uint8_t* f_data, size_t f_size) {
	if(f_size<=8) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
	//fuzzer shim end}




    // Declare variables
    tjhandle tjInstance = tjInitCompress();
    unsigned char* jpegBuf = NULL;
    unsigned long jpegSize = 0;
    int width, height, pixelFormat;

    // Step 4: Create a FILE * variable for reading the input data
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (in_file == NULL) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Step 5: Get the file descriptor for reading
    int in_fd = fuzz_fileno(in_file);

    // Step 6: Specify the input file name
    const char* input_file = "input.jpg";
    
    // Load the JPEG image from memory
    tjLoadImage(input_file, &width, fuzz_int32_t_1, &height, &pixelFormat, fuzz_int32_t_2);

    // Get the scaling factors
    int numScalingFactors;
    tjscalingfactor* scalingFactors = tjGetScalingFactors(&numScalingFactors);

    // Step 4: Create a FILE * variable for writing the output data
    FILE* out_file = fopen("output.jpg", "wb");
    if (out_file == NULL) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
    }

    // Step 5: Get the file descriptor for writing
    int out_fd = fuzz_fileno(out_file);

    // Compress the image
    tj3CompressFromYUVPlanes8(tjInstance, NULL, width, NULL, height, &jpegBuf, &jpegSize);

    // Get the size of the compressed image
    unsigned long bufSize = tjBufSize(width, height, TJSAMP_444);

    // Step 7: Release resources
    tjDestroy(tjInstance);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    tjFree(jpegBuf);

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}