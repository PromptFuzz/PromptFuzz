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
//<ID> 2932
//<Prompt> ["tjAlloc","tjPlaneHeight","tj3EncodeYUVPlanes8","tjEncodeYUVPlanes","tjEncodeYUV3"]
/*<Combination>: [unsigned char *tjAlloc(int bytes),
    int tjPlaneHeight(int componentID, int height, int subsamp),
    int tj3EncodeYUVPlanes8(tjhandle handle, const unsigned char * srcBuf, int width, int pitch, int height, int pixelFormat, unsigned char ** dstPlanes, int * strides),
    int tjEncodeYUVPlanes(tjhandle handle, const unsigned char * srcBuf, int width, int pitch, int height, int pixelFormat, unsigned char ** dstPlanes, int * strides, int subsamp, int flags),
    int tjEncodeYUV3(tjhandle handle, const unsigned char * srcBuf, int width, int pitch, int height, int pixelFormat, unsigned char * dstBuf, int align, int subsamp, int flags)
*/
//<score> 1.0105264, nr_unique_branch: 1, p_cov: 0.6
//<Quality> {"diversity":19,"density":16,"covered_api":["tjAlloc","tjPlaneHeight","tj3EncodeYUVPlanes8"],"uncovered_api":["tjEncodeYUVPlanes","tjEncodeYUV3"],"unique_branches":{"tj3EncodeYUVPlanes8":[[1225,52,1225,63,0]]},"library_calls":["tjPlaneHeight","tjAlloc","tjAlloc","tjAlloc","tjInitCompress","tj3EncodeYUVPlanes8","tjDestroy","tjFree","tjFree","tjFree"]}
/**/


extern "C" int LLVMFuzzerTestOneInput_67(const uint8_t* f_data, size_t f_size) {
	if(f_size<=4) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
	//fuzzer shim end}




    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file == nullptr) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Read input data from file
    fseek(in_file, 0, SEEK_END);
    long in_file_size = ftell(in_file);
    fseek(in_file, 0, SEEK_SET);

    unsigned char *input_data = (unsigned char *)malloc(in_file_size);
    if (input_data == nullptr) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    fread(input_data, 1, in_file_size, in_file);
    assert_file_closed(&in_file);;

    // Calculate the plane height
    int height = tjPlaneHeight(TJ_GRAYSCALE, in_file_size, TJSAMP_GRAY);

    // Allocate the memory for the output planes
    unsigned char *output_planes[3];
    output_planes[0] = (unsigned char *)tjAlloc(in_file_size);
    output_planes[1] = (unsigned char *)tjAlloc(in_file_size);
    output_planes[2] = (unsigned char *)tjAlloc(in_file_size);

    if (output_planes[0] == nullptr || output_planes[1] == nullptr || output_planes[2] == nullptr) {
        free(input_data);
        tjFree(output_planes[0]);
        tjFree(output_planes[1]);
        tjFree(output_planes[2]);
        assert_file_closed(&in_file);
	return 0;
    }

    // Set the strides for the output planes
    int strides[3];
    strides[0] = in_file_size;
    strides[1] = in_file_size;
    strides[2] = in_file_size;

    // Compress input data to YUV format
    tjhandle handle = tjInitCompress();
    int result = tj3EncodeYUVPlanes8(handle, input_data, in_file_size, fuzz_int32_t_1, height, TJ_GRAYSCALE, output_planes, strides);
    tjDestroy(handle);

    if (result != 0) {
        free(input_data);
        tjFree(output_planes[0]);
        tjFree(output_planes[1]);
        tjFree(output_planes[2]);
        assert_file_closed(&in_file);
	return 0;
    }

    // Save output planes to file
    FILE *out_file = fopen("output_file", "wb");
    if (out_file == nullptr) {
        free(input_data);
        tjFree(output_planes[0]);
        tjFree(output_planes[1]);
        tjFree(output_planes[2]);
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    fwrite(output_planes[0], 1, in_file_size, out_file);
    fwrite(output_planes[1], 1, in_file_size, out_file);
    fwrite(output_planes[2], 1, in_file_size, out_file);
    assert_file_closed(&out_file);;

    // Free allocated memory
    free(input_data);
    tjFree(output_planes[0]);
    tjFree(output_planes[1]);
    tjFree(output_planes[2]);

    assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}