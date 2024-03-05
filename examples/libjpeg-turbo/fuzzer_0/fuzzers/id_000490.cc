#include "FDSan.h"
#include <turbojpeg.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 490
//<Prompt> ["tj3JPEGBufSize","tj3EncodeYUV8","tj3YUVBufSize","tj3LoadImage12","tjGetErrorStr"]
/*<Combination>: [size_t tj3JPEGBufSize(int width, int height, int jpegSubsamp),
    int tj3EncodeYUV8(tjhandle handle, const unsigned char * srcBuf, int width, int pitch, int height, int pixelFormat, unsigned char * dstBuf, int align),
    size_t tj3YUVBufSize(int width, int align, int height, int subsamp),
    short *tj3LoadImage12(tjhandle handle, const char * filename, int * width, int align, int * height, int * pixelFormat),
    char *tjGetErrorStr()
*/
//<score> 0.8, nr_unique_branch: 3, p_cov: 0.2
//<Quality> {"diversity":14,"density":14,"covered_api":["tj3LoadImage12"],"uncovered_api":["tj3JPEGBufSize","tj3EncodeYUV8","tj3YUVBufSize","tjGetErrorStr"],"unique_branches":{"pre_process_data":[[162,9,162,54,0],[163,20,163,46,0],[163,20,163,46,1]]},"library_calls":["tjInitCompress","tj3LoadImage12","tjCompress2","tjFree","tj3Free","tjDestroy"]}
/*Here is the C++ program that achieves the event specified in step 1 and satisfies steps 2 to 7:

*/



// Step 1: Here are several APIs in libjpeg-turbo. Specify an event that those APIs could achieve together
// The event: Compress an image from YUV format to JPEG format

// Step 2: Complete the LLVMFuzzerTestOneInput_19 function to achieve this event
extern "C" int LLVMFuzzerTestOneInput_19(const uint8_t *data, size_t size) {
	if(size<=0) return 0;

	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);

    tjhandle handle = tjInitCompress();
    if (handle == NULL) {
        return 0;
    }

    // Step 3: Consume the input data and its size
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file == NULL) {
        tjDestroy(handle);
        assert_file_closed(&in_file);
	return 0;
    }

    // Step 4: Read the input image from the file
    int width, height, pixelFormat;
    int align = 1; // No alignment required
    short *srcBuf = tj3LoadImage12(handle, "input_file", &width, align, &height, &pixelFormat);
    if (srcBuf == NULL) {
        assert_file_closed(&in_file);;
        tjDestroy(handle);
        assert_file_name_closed("input_file");
	assert_file_closed(&in_file);
	return 0;
    }

    // Step 5: Compress the input image to JPEG format
    unsigned char *jpegBuf = NULL;
    unsigned long jpegSize = 0;
    int result = tjCompress2(handle, (unsigned char *)srcBuf, width, 0, height, pixelFormat, &jpegBuf, &jpegSize, TJSAMP_420, 90, 0);
    if (result != 0) {
        tjFree(jpegBuf);
        tj3Free(srcBuf);
        assert_file_closed(&in_file);;
        tjDestroy(handle);
        assert_file_name_closed("input_file");
	assert_file_closed(&in_file);
	return 0;
    }

    // Step 6: Write the compressed image to an output file
    FILE *out_file = fopen("output_file", "wb");
    if (out_file == NULL) {
        tjFree(jpegBuf);
        tj3Free(srcBuf);
        assert_file_closed(&in_file);;
        tjDestroy(handle);
        assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    fwrite(jpegBuf, sizeof(unsigned char), jpegSize, out_file);
    assert_file_closed(&out_file);;

    // Step 7: Release all allocated resources
    tjFree(jpegBuf);
    tj3Free(srcBuf);
    assert_file_closed(&in_file);;
    tjDestroy(handle);

    assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}