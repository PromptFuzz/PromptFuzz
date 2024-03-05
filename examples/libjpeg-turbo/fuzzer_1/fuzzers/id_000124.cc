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
//<ID> 8746
//<Prompt> ["tj3SaveImage16","tj3YUVPlaneSize","tjDecompressHeader3","tjDecompressHeader2","tj3LoadImage16"]
/*<Combination>: [int tj3SaveImage16(tjhandle handle, const char * filename, const unsigned short * buffer, int width, int pitch, int height, int pixelFormat),
    size_t tj3YUVPlaneSize(int componentID, int width, int stride, int height, int subsamp),
    int tjDecompressHeader3(tjhandle handle, const unsigned char * jpegBuf, unsigned long jpegSize, int * width, int * height, int * jpegSubsamp, int * jpegColorspace),
    int tjDecompressHeader2(tjhandle handle, unsigned char * jpegBuf, unsigned long jpegSize, int * width, int * height, int * jpegSubsamp),
    unsigned short *tj3LoadImage16(tjhandle handle, const char * filename, int * width, int align, int * height, int * pixelFormat)
*/
//<score> 1.2, nr_unique_branch: 1, p_cov: 1
//<Quality> {"diversity":5,"density":3,"covered_api":["tj3SaveImage16","tj3YUVPlaneSize","tjDecompressHeader3","tjDecompressHeader2","tj3LoadImage16"],"uncovered_api":[],"unique_branches":{"tj3LoadImage16":[[301,7,301,12,0]]},"library_calls":["tj3LoadImage16","tj3SaveImage16","tjDecompressHeader3","tjDecompressHeader2","tj3YUVPlaneSize"]}
/**/


extern "C" int LLVMFuzzerTestOneInput_124(const uint8_t* f_data, size_t f_size) {
	if(f_size<=12) return 0;

	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(f_data, sizeof(uint8_t), f_size, input_file_ptr);
	fclose(input_file_ptr);

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
	//fuzzer shim end}




    // Step 1: Call tj3LoadImage16 to load the input image
    int width, height, pixelFormat;
    unsigned short *image = tj3LoadImage16(nullptr, "input_file", &width, fuzz_int32_t_1, &height, &pixelFormat);
    
    // Step 2: Write the loaded image to a file using tj3SaveImage16
    tj3SaveImage16(nullptr, "output_file", image, width, fuzz_int32_t_2, height, pixelFormat);

    // Step 3: Call tjDecompressHeader3 to get the image width, height and subsampling
    int imgWidth, imgHeight, imgSubsamp;
    tjDecompressHeader3(nullptr, data, size, &imgWidth, &imgHeight, &imgSubsamp, nullptr);

    // Step 4: Call tjDecompressHeader2 to get the image width, height and subsampling
    int imgWidth2, imgHeight2, imgSubsamp2;
    tjDecompressHeader2(nullptr, const_cast<unsigned char *>(data), size, &imgWidth2, &imgHeight2, &imgSubsamp2);

    // Step 5: Call tj3YUVPlaneSize to get the size of the YUV planes
    int yuvPlaneSize = tj3YUVPlaneSize(fuzz_int32_t_3, imgWidth, imgWidth, imgHeight, imgSubsamp);

    // Step 6: Do something with the YUV plane size

    // Step 7: Clean up
    free(image);

    assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	return 0;
}