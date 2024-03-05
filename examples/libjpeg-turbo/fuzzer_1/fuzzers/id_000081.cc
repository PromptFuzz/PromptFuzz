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
//<ID> 4084
//<Prompt> ["tjPlaneSizeYUV","tjGetErrorCode","tjDecompressHeader2","tjFree","tjEncodeYUV"]
/*<Combination>: [unsigned long tjPlaneSizeYUV(int componentID, int width, int stride, int height, int subsamp),
    int tjGetErrorCode(tjhandle handle),
    int tjDecompressHeader2(tjhandle handle, unsigned char * jpegBuf, unsigned long jpegSize, int * width, int * height, int * jpegSubsamp),
    void tjFree(unsigned char * buffer),
    int tjEncodeYUV(tjhandle handle, unsigned char * srcBuf, int width, int pitch, int height, int pixelSize, unsigned char * dstBuf, int subsamp, int flags)
*/
//<score> 1.5, nr_unique_branch: 1, p_cov: 1
//<Quality> {"diversity":8,"density":6,"covered_api":["tjPlaneSizeYUV","tjGetErrorCode","tjDecompressHeader2","tjFree","tjEncodeYUV"],"uncovered_api":[],"unique_branches":{"tjEncodeYUV3":[[1432,7,1432,18,0]]},"library_calls":["tjPlaneSizeYUV","tjInitCompress","tjGetErrorCode","tjDecompressHeader2","tjFree","tjEncodeYUV","tjDestroy"]}
/*Here is the code that achieves the event described above using the libjpeg-turbo library APIs:

*/


extern "C" int LLVMFuzzerTestOneInput_81(const uint8_t* f_data, size_t f_size) {
	if(f_size<=4) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
	//fuzzer shim end}




    // Step 4: Create FILE pointer to read input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file == NULL) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Step 5: Obtain file descriptor for reading
    int in_fd = fuzz_fileno(in_file);
    
    // Step 6: Define output file name
    const char *output_file = "output_file.jpg";
    
    // Step 1: Define event to achieve
    unsigned long yuvSize = tjPlaneSizeYUV(TJPF_RGB, 100, 100, 3, TJSAMP_420);
    tjhandle handle = tjInitCompress();
    int width, height, jpegSubsamp;
    unsigned long jpegSize;
    unsigned char *jpegBuf = NULL;
    
    // Step 2: Call necessary libjpeg-turbo APIs
    int errorCode = tjGetErrorCode(handle);
    int headerResult = tjDecompressHeader2(handle, NULL, 0, &width, &height, &jpegSubsamp);
    tjFree(jpegBuf);
    int encodeResult = tjEncodeYUV(handle, NULL, width, width * tjPixelSize[TJPF_RGB], height, tjPixelSize[TJPF_RGB], NULL, jpegSubsamp, fuzz_int32_t_1);
    
    // Step 3: Consume input data
    fseek(in_file, 0, SEEK_END);
    size = ftell(in_file);
    fseek(in_file, 0, SEEK_SET);
    unsigned char *input_data = (unsigned char *)malloc(size);
    fread(input_data, size, 1, in_file);
    
    // Step 4: Create FILE pointer to write output data
    FILE *out_file = fopen(output_file, "wb");
    if (out_file == NULL) {
        free(input_data);
        assert_file_closed(&in_file);;
        tjDestroy(handle);
        assert_file_name_closed("output_file.jpg");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Step 5: Obtain file descriptor for writing
    int out_fd = fuzz_fileno(out_file);
    
    // Consume the input data and achieve the event using libjpeg-turbo APIs
    // ...

    // Step 7: Release resources
    free(input_data);
    assert_file_closed(&in_file);;
    tjDestroy(handle);
    assert_file_closed(&out_file);;
    
    assert_file_name_closed("output_file.jpg");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}