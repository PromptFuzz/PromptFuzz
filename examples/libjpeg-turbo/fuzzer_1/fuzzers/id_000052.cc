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
//<ID> 2184
//<Prompt> ["tjPlaneWidth","tjBufSizeYUV2","tj3Destroy","tj3DecodeYUV8","tjInitDecompress","tj3SetScalingFactor"]
/*<Combination>: [int tjPlaneWidth(int componentID, int width, int subsamp),
    unsigned long tjBufSizeYUV2(int width, int align, int height, int subsamp),
    void tj3Destroy(tjhandle handle),
    int tj3DecodeYUV8(tjhandle handle, const unsigned char * srcBuf, int align, unsigned char * dstBuf, int width, int pitch, int height, int pixelFormat),
    tjhandle tjInitDecompress(),
    int tj3SetScalingFactor(tjhandle handle, tjscalingfactor scalingFactor)
*/
//<score> 2, nr_unique_branch: 1, p_cov: 1
//<Quality> {"diversity":6,"density":6,"covered_api":["tjPlaneWidth","tjBufSizeYUV2","tj3Destroy","tj3DecodeYUV8","tjInitDecompress","tj3SetScalingFactor"],"uncovered_api":[],"unique_branches":{"tj3DecodeYUV8":[[2230,7,2230,38,0]]},"library_calls":["tjInitDecompress","tj3SetScalingFactor","tjPlaneWidth","tjBufSizeYUV2","tj3DecodeYUV8","tj3Destroy"]}
/*Here is the C++ program that achieves the event using the libjpeg-turbo APIs:

*/


extern "C" int LLVMFuzzerTestOneInput_52(const uint8_t* f_data, size_t f_size)
{
	if(f_size<=8) return 0;

	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(f_data, sizeof(uint8_t), f_size, input_file_ptr);
	fclose(input_file_ptr);

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
	//fuzzer shim end}




    // Step 1: Initialize variables and data structures
    tjhandle handle = tjInitDecompress();
    tjscalingfactor scalingFactor;
    scalingFactor.num = 1;
    scalingFactor.denom = 2;

    // Step 2: Set scaling factor
    tj3SetScalingFactor(handle, scalingFactor);

    // Step 3: Create input and output file pointers
    FILE *in_file = fmemopen((void *)data, size, "rb");
    FILE *out_file = fopen("output_file", "wb");

    // Step 4: Get file descriptors
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);

    // Step 5: Get file names
    const char *in_file_name = "input_file";
    const char *out_file_name = "output_file";

    // Step 6: Read input data
    uint8_t *input_data = (uint8_t *)malloc(size);
    fread(input_data, 1, size, in_file);

    // Step 7: Process the input data using libjpeg-turbo APIs
    int width = tjPlaneWidth(0, 1024, TJSAMP_444);
    unsigned long buf_size = tjBufSizeYUV2(width, 4, 768, TJSAMP_444);
    uint8_t *output_data = (uint8_t *)malloc(buf_size);

    tj3DecodeYUV8(handle, input_data, fuzz_int32_t_1, output_data, width, 0, fuzz_int32_t_2, TJPF_RGB);

    // Step 8: Write output data
    fwrite(output_data, 1, buf_size, out_file);

    // Step 9: Release resources
    free(input_data);
    free(output_data);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    tj3Destroy(handle);

    assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}