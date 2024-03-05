#include "FDSan.h"
#include <turbojpeg.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 5571
//<Prompt> ["tjGetErrorStr","tjPlaneHeight","tj3Set","tjPlaneWidth","tj3GetScalingFactors","tj3GetErrorCode","tjInitCompress"]
/*<Combination>: [char *tjGetErrorStr(),
    int tjPlaneHeight(int componentID, int height, int subsamp),
    int tj3Set(tjhandle handle, int param, int value),
    int tjPlaneWidth(int componentID, int width, int subsamp),
    tjscalingfactor *tj3GetScalingFactors(int * numScalingFactors),
    int tj3GetErrorCode(tjhandle handle),
    tjhandle tjInitCompress()
*/
//<score> 1.2, nr_unique_branch: 1, p_cov: 1
//<Quality> {"diversity":10,"density":6,"covered_api":["tjGetErrorStr","tjPlaneHeight","tj3Set","tjPlaneWidth","tj3GetScalingFactors","tj3GetErrorCode","tjInitCompress"],"uncovered_api":[],"unique_branches":{"tj3Set":[[605,9,605,33,1]]},"library_calls":["tjInitCompress","tjGetErrorStr","tjPlaneHeight","tjPlaneWidth","tj3Set","tj3GetScalingFactors","tj3GetErrorCode","tjDestroy"]}
/**/


extern "C" int LLVMFuzzerTestOneInput_92(const uint8_t *data, size_t size) {
	if(size<=0) return 0;

    // Step 1: Initialize
    tjhandle handle = tjInitCompress();
    if (handle == NULL) {
        return 0;
    }
    
    // Step 2: Read input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file == NULL) {
        tjDestroy(handle);
        assert_file_closed(&in_file);
	return 0;
    }
    int in_fd = fuzz_fileno(in_file);
    
    // Step 3: Write output data
    FILE *out_file = fopen("output_file", "wb");
    if (out_file == NULL) {
        assert_file_closed(&in_file);;
        tjDestroy(handle);
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
    }
    int out_fd = fuzz_fileno(out_file);
    
    // Step 4: Call libjpeg-turbo APIs
    char *error_str = tjGetErrorStr();
    int plane_height = tjPlaneHeight(0, 100, 2);
    int plane_width = tjPlaneWidth(0, 100, 2);
    
    int param = TJXOPT_TRIM;
    int value = 1;
    int set_result = tj3Set(handle, param, value);
    
    int num_scaling_factors;
    tjscalingfactor *scaling_factors = tj3GetScalingFactors(&num_scaling_factors);
    
    int error_code = tj3GetErrorCode(handle);
    
    // Step 5: Release resources
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    tjDestroy(handle);
    
    assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}