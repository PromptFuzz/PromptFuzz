#include "FDSan.h"
#include <turbojpeg.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 6679
//<Prompt> ["tj3YUVBufSize","tjDecompressHeader2","tj3LoadImage16","tj3Decompress12","tjPlaneHeight"]
/*<Combination>: [size_t tj3YUVBufSize(int width, int align, int height, int subsamp),
    int tjDecompressHeader2(tjhandle handle, unsigned char * jpegBuf, unsigned long jpegSize, int * width, int * height, int * jpegSubsamp),
    unsigned short *tj3LoadImage16(tjhandle handle, const char * filename, int * width, int align, int * height, int * pixelFormat),
    int tj3Decompress12(tjhandle handle, const unsigned char * jpegBuf, size_t jpegSize, short * dstBuf, int pitch, int pixelFormat),
    int tjPlaneHeight(int componentID, int height, int subsamp)
*/
//<score> 6.4, nr_unique_branch: 7, p_cov: 0.8
//<Quality> {"diversity":9,"density":9,"covered_api":["tjDecompressHeader2","tj3LoadImage16","tj3Decompress12","tjPlaneHeight"],"uncovered_api":["tj3YUVBufSize"],"unique_branches":{"tj3Decompress12":[[158,43,158,57,1],[158,61,158,70,1],[159,7,159,22,1],[159,26,159,49,1],[162,7,162,22,1],[175,7,175,42,0],[175,7,175,42,1]]},"library_calls":["tjInitDecompress","tjDecompressHeader2","tj3LoadImage16","tjPlaneHeight","tj3Decompress12","tjDestroy"]}
/**/


extern "C" int LLVMFuzzerTestOneInput_103(const uint8_t *data, size_t size) {
	if(size<=0) return 0;

	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);

    // Step 4: Create input file stream
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Step 5: Get file descriptor for reading
    int in_fd = fuzz_fileno(in_file);
    if (in_fd == -1) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Step 6: Define file name
    const char *filename = "input_file";
    
    // Step 2: Allocate variables for the libjpeg-turbo APIs
    tjhandle handle = tjInitDecompress();
    int width, height, jpegSubsamp;
    
    // Step 1: Call tjDecompressHeader2 to get image dimensions
    unsigned char *jpegBuf = nullptr;
    unsigned long jpegSize = 0;
    if (fseek(in_file, 0, SEEK_END) == 0) {
        jpegSize = ftell(in_file);
        jpegBuf = new unsigned char[jpegSize];
        fseek(in_file, 0, SEEK_SET);
        fread(jpegBuf, 1, jpegSize, in_file);
    }
    int result = tjDecompressHeader2(handle, jpegBuf, jpegSize, &width, &height, &jpegSubsamp);
    delete[] jpegBuf;
    
    if (result == -1) {
        assert_file_closed(&in_file);;
        tjDestroy(handle);
        assert_file_name_closed("input_file");
	assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Step 3: Call tj3LoadImage16 to load the input image
    int align = 1;
    int pixelFormat;
    unsigned short *image = tj3LoadImage16(handle, filename, &width, align, &height, &pixelFormat);
    if (!image) {
        assert_file_closed(&in_file);;
        tjDestroy(handle);
        assert_file_name_closed("input_file");
	assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Step 1: Call tjPlaneHeight to get the height of a plane
    int componentID = 0;
    int subsamp = 0;
    int planeHeight = tjPlaneHeight(componentID, height, subsamp);
    
    // Step 2: Allocate variables for tj3Decompress12
    int pitch = width * sizeof(unsigned short);
    short *dstBuf = new short[pitch * planeHeight];
    
    // Step 2: Call tj3Decompress12 to decompress the image
    result = tj3Decompress12(handle, (const unsigned char *)image, width * sizeof(unsigned short), dstBuf, pitch, pixelFormat);
    delete[] image;
    
    if (result != 0) {
        assert_file_closed(&in_file);;
        tjDestroy(handle);
        delete[] dstBuf;
        assert_file_name_closed("input_file");
	assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Release resources
    assert_file_closed(&in_file);;
    tjDestroy(handle);
    delete[] dstBuf;
    
    assert_file_name_closed("input_file");
	assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
}