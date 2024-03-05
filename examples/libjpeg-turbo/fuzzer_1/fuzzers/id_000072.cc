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
//<ID> 3273
//<Prompt> ["tj3Destroy","tjInitCompress","tj3EncodeYUVPlanes8","tj3SaveImage12","tj3Get"]
/*<Combination>: [void tj3Destroy(tjhandle handle),
    tjhandle tjInitCompress(),
    int tj3EncodeYUVPlanes8(tjhandle handle, const unsigned char * srcBuf, int width, int pitch, int height, int pixelFormat, unsigned char ** dstPlanes, int * strides),
    int tj3SaveImage12(tjhandle handle, const char * filename, const short * buffer, int width, int pitch, int height, int pixelFormat),
    int tj3Get(tjhandle handle, int param)
*/
//<score> 2, nr_unique_branch: 1, p_cov: 1
//<Quality> {"diversity":5,"density":5,"covered_api":["tj3Destroy","tjInitCompress","tj3EncodeYUVPlanes8","tj3SaveImage12","tj3Get"],"uncovered_api":[],"unique_branches":{"tj3Get":[[723,3,723,29,0]]},"library_calls":["tjInitCompress","tj3EncodeYUVPlanes8","tj3SaveImage12","tj3Get","tj3Destroy"]}
/*Here is the C++ program that achieves the event using the libjpeg-turbo APIs:

*/


extern "C" int LLVMFuzzerTestOneInput_72(const uint8_t* f_data, size_t f_size) {
	if(f_size<=32) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_7, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_8, fdp);
	//fuzzer shim end}




    tjhandle handle = tjInitCompress();
    int paramValue;

    // Check if the handle is valid
    if (handle == nullptr) {
        return 0;
    }

    // Set input parameters
    tj3EncodeYUVPlanes8(handle, data, /*width=*/0, /*pitch=*/fuzz_int32_t_1, /*height=*/fuzz_int32_t_2, /*pixelFormat=*/fuzz_int32_t_3, nullptr, nullptr);

    // Save image using tj3SaveImage12
    tj3SaveImage12(handle, "output_file", nullptr, /*width=*/fuzz_int32_t_4, /*pitch=*/fuzz_int32_t_5, /*height=*/fuzz_int32_t_6, /*pixelFormat=*/fuzz_int32_t_7);

    // Get the value of a parameter
    paramValue = tj3Get(handle, /*param=*/fuzz_int32_t_8);

    // Print the value of the parameter
    printf("Parameter value: %d\n", paramValue);

    // Destroy the handle
    tj3Destroy(handle);

    assert_file_name_closed("output_file");
	return 0;
}