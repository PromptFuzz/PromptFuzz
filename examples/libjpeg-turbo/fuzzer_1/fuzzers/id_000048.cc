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
//<ID> 1846
//<Prompt> ["tj3SetCroppingRegion","tjInitDecompress","tj3LoadImage16","tj3LoadImage12","tj3Transform"]
/*<Combination>: [int tj3SetCroppingRegion(tjhandle handle, tjregion croppingRegion),
    tjhandle tjInitDecompress(),
    unsigned short *tj3LoadImage16(tjhandle handle, const char * filename, int * width, int align, int * height, int * pixelFormat),
    short *tj3LoadImage12(tjhandle handle, const char * filename, int * width, int align, int * height, int * pixelFormat),
    int tj3Transform(tjhandle handle, const unsigned char * jpegBuf, size_t jpegSize, int n, unsigned char ** dstBufs, size_t * dstSizes, const tjtransform * transforms)
*/
//<score> 2.4, nr_unique_branch: 2, p_cov: 0.8
//<Quality> {"diversity":9,"density":9,"covered_api":["tj3SetCroppingRegion","tjInitDecompress","tj3LoadImage16","tj3Transform"],"uncovered_api":["tj3LoadImage12"],"unique_branches":{"get_text_rgb_row":[[254,7,254,28,0],[255,9,255,20,1]]},"library_calls":["tjInitDecompress","tj3LoadImage16","tj3SetCroppingRegion","tj3Transform","tjFree","tj3Free","tjDestroy"]}
/**/


extern "C" int LLVMFuzzerTestOneInput_48(const uint8_t* f_data, size_t f_size) {
	if(f_size<=4) return 0;

	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(f_data, sizeof(uint8_t), f_size, input_file_ptr);
	fclose(input_file_ptr);

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
	//fuzzer shim end}




    tjhandle handle = tjInitDecompress();
    if (handle == nullptr) {
        return 0;
    }

    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file == nullptr) {
        tjDestroy(handle);
        assert_file_closed(&in_file);
	return 0;
    }

    int width, height, pixelFormat;
    
    // Load the image from file
    unsigned short *image16 = tj3LoadImage16(handle, "input_file", &width, fuzz_int32_t_1, &height, &pixelFormat);
    if (image16 == nullptr) {
        tjDestroy(handle);
        assert_file_closed(&in_file);;
        assert_file_name_closed("input_file");
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Set the cropping region
    tjregion croppingRegion;
    croppingRegion.x = 0;
    croppingRegion.y = 0;
    croppingRegion.w = width;
    croppingRegion.h = height;
    tj3SetCroppingRegion(handle, croppingRegion);
    
    // Transform the image
    tjtransform transform;
    transform.r = croppingRegion;
    transform.op = TJXOP_NONE;
    transform.options = 0;
    transform.data = nullptr;
    transform.customFilter = nullptr;
    
    unsigned char *dstBuf;
    size_t dstSize;
    tj3Transform(handle, nullptr, 0, 1, &dstBuf, &dstSize, &transform);
    
    tjFree(dstBuf);
    tj3Free(image16);
    tjDestroy(handle);
    assert_file_closed(&in_file);;

    assert_file_name_closed("input_file");
	assert_file_closed(&in_file);
	return 0;
}