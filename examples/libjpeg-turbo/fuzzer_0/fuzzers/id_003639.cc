#include "FDSan.h"
#include <turbojpeg.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 3639
//<Prompt> ["tj3GetErrorCode","tjGetErrorStr2","tjGetScalingFactors","tj3Get","tjDecompressHeader3"]
/*<Combination>: [int tj3GetErrorCode(tjhandle handle),
    char *tjGetErrorStr2(tjhandle handle),
    tjscalingfactor *tjGetScalingFactors(int * numscalingfactors),
    int tj3Get(tjhandle handle, int param),
    int tjDecompressHeader3(tjhandle handle, const unsigned char * jpegBuf, unsigned long jpegSize, int * width, int * height, int * jpegSubsamp, int * jpegColorspace)
*/
//<score> 1.7142857, nr_unique_branch: 1, p_cov: 1
//<Quality> {"diversity":7,"density":6,"covered_api":["tj3GetErrorCode","tjGetErrorStr2","tjGetScalingFactors","tj3Get","tjDecompressHeader3"],"uncovered_api":[],"unique_branches":{"tj3Get":[[727,3,727,25,0]]},"library_calls":["tjInitDecompress","tjDecompressHeader3","tj3GetErrorCode","tjGetErrorStr2","tjGetScalingFactors","tj3Get","tjDestroy"]}
/**/



// Fuzz driver function
extern "C" int LLVMFuzzerTestOneInput_75(const uint8_t *data, size_t size) {
	if(size<=0) return 0;

    // File variables
    FILE *in_file = fmemopen((void *)data, size, "rb");
    FILE *out_file = fopen("output_file.jpg", "wb");
    
    // File descriptors
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);
    
    // Read input data from file
    fseek(in_file, 0, SEEK_END);
    size_t in_size = ftell(in_file);
    rewind(in_file);
    unsigned char *in_data = (unsigned char *)malloc(in_size);
    fread(in_data, sizeof(unsigned char), in_size, in_file);
    
    // Output data buffer
    unsigned char *out_data = NULL;
    unsigned long out_size = 0;
    
    // Initialize libjpeg-turbo handle
    tjhandle handle = tjInitDecompress();
    
    // Decompress JPEG header
    int width, height, jpegSubsamp, jpegColorspace;
    tjDecompressHeader3(handle, in_data, in_size, &width, &height, &jpegSubsamp, &jpegColorspace);
    
    // Get error code and error message
    int errorCode = tj3GetErrorCode(handle);
    char *errorMsg = tjGetErrorStr2(handle);
    
    // Get scaling factors
    int numScalingFactors;
    tjscalingfactor *scalingFactors = tjGetScalingFactors(&numScalingFactors);
    
    // Get parameter value
    int paramValue = tj3Get(handle, TJXOPT_TRIM);
    
    // Cleanup
    tjDestroy(handle);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    free(in_data);
    
    assert_file_name_closed("output_file.jpg");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}