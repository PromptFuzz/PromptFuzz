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
//<ID> 2997
//<Prompt> ["tjSaveImage","tj3LoadImage12","tjPlaneSizeYUV","tj3Free","tjEncodeYUVPlanes"]
/*<Combination>: [int tjSaveImage(const char * filename, unsigned char * buffer, int width, int pitch, int height, int pixelFormat, int flags),
    short *tj3LoadImage12(tjhandle handle, const char * filename, int * width, int align, int * height, int * pixelFormat),
    unsigned long tjPlaneSizeYUV(int componentID, int width, int stride, int height, int subsamp),
    void tj3Free(void * buffer),
    int tjEncodeYUVPlanes(tjhandle handle, const unsigned char * srcBuf, int width, int pitch, int height, int pixelFormat, unsigned char ** dstPlanes, int * strides, int subsamp, int flags)
*/
//<score> 2, nr_unique_branch: 1, p_cov: 1
//<Quality> {"diversity":8,"density":8,"covered_api":["tjSaveImage","tj3LoadImage12","tjPlaneSizeYUV","tj3Free","tjEncodeYUVPlanes"],"uncovered_api":[],"unique_branches":{"tjPlaneSizeYUV":[[1132,10,1132,23,0]]},"library_calls":["tjSaveImage","tj3LoadImage12","tjPlaneSizeYUV","tjEncodeYUVPlanes","tj3Free","tj3Free","tj3Free","tj3Free"]}
/**/

extern "C" int LLVMFuzzerTestOneInput_68(const uint8_t* f_data, size_t f_size) {
	if(f_size<=0) return 0;

	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(f_data, sizeof(uint8_t), f_size, input_file_ptr);
	fclose(input_file_ptr);

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Step 4: Create a FILE pointer for reading the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
  
    // Step 5: Get the file descriptor for reading
    int in_fd = fuzz_fileno(in_file);

    // Step 6: Specify the input file name
    const char *input_file = "input_file";
    
    // Step 4: Create a FILE pointer for writing the output data
    FILE *out_file = fopen("output_file", "wb");

    // Step 5: Get the file descriptor for writing
    int out_fd = fuzz_fileno(out_file);

    // Step 6: Specify the output file name
    const char *output_file = "output_file";

    // Step 1: Save the input data as an image
    unsigned char *buffer = nullptr;
    int width = 0, height = 0, pixelFormat = 0;
    int flags = 0;  // Specify any required flags
    int result = tjSaveImage(input_file, buffer, width, 0, height, pixelFormat, flags);

    // Step 2: Load the saved image data
    short *loaded_image = nullptr;
    int align = 0;
    int loaded_width = 0, loaded_height = 0, loaded_pixelFormat = 0;
    loaded_image = tj3LoadImage12(nullptr, input_file, &loaded_width, align, &loaded_height, &loaded_pixelFormat);

    // Step 3: Calculate the size of a YUV plane
    int componentID = 0;
    int stride = 0;
    unsigned long plane_size = tjPlaneSizeYUV(componentID, loaded_width, stride, loaded_height, 0);

    // Step 1: Encode the loaded image as YUV planes
    unsigned char **dstPlanes = nullptr;
    int *strides = nullptr;
    int subsamp = 0;
    result = tjEncodeYUVPlanes(nullptr, (const unsigned char *)loaded_image, loaded_width, 0, loaded_height, loaded_pixelFormat, dstPlanes, strides, subsamp, flags);

    // Step 7: Release all allocated resources
    tj3Free(buffer);
    tj3Free(loaded_image);
    tj3Free(dstPlanes);
    tj3Free(strides);

    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_name_closed("output_file");
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}