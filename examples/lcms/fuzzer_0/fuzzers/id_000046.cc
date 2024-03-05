#include "FDSan.h"
#include <lcms2.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
//<ID> 5333
//<Prompt> ["cmsCreateNULLProfile","cmsIT8GetPatchByName","cmsStageAllocCLut16bit","cmsChannelsOfColorSpace","cmsCMCdeltaE"]
/*<Combination>: [cmsHPROFILE cmsCreateNULLProfile(),
    int cmsIT8GetPatchByName(cmsHANDLE hIT8, const char * cPatch),
    cmsStage *cmsStageAllocCLut16bit(cmsContext ContextID, cmsUInt32Number nGridPoints, cmsUInt32Number inputChan, cmsUInt32Number outputChan, const cmsUInt16Number * Table),
    cmsInt32Number cmsChannelsOfColorSpace(cmsColorSpaceSignature ColorSpace),
    cmsFloat64Number cmsCMCdeltaE(const cmsCIELab * Lab1, const cmsCIELab * Lab2, cmsFloat64Number l, cmsFloat64Number c)
*/
//<score> 0.6666667, nr_unique_branch: 1, p_cov: 1
//<Quality> {"diversity":9,"density":3,"covered_api":["cmsCreateNULLProfile","cmsIT8GetPatchByName","cmsStageAllocCLut16bit","cmsChannelsOfColorSpace","cmsCMCdeltaE"],"uncovered_api":[],"unique_branches":{"LocatePatch":[[2651,15,2651,31,1]]},"library_calls":["cmsCreateNULLProfile","cmsIT8Alloc","cmsIT8GetPatchByName","cmsStageAllocCLut16bit","cmsChannelsOfColorSpace","cmsCMCdeltaE","cmsIT8Free","cmsCloseProfile","cmsStageFree"]}
/**/

extern "C" int LLVMFuzzerTestOneInput_46(const uint8_t *data, size_t size) {
	if(size<=0) return 0;

    // Create a NULL profile
    cmsHPROFILE hProfile = cmsCreateNULLProfile();

    // Create an IT8 handle
    cmsHANDLE hIT8 = cmsIT8Alloc(nullptr);
    
    // Read data from the input
    FILE *in_file = fmemopen((void *)data, size, "rb");
    
    // Get the patch by name
    const char *cPatch = "patch_name";
    int patchIndex = cmsIT8GetPatchByName(hIT8, cPatch);
    
    // Create a CLUT stage
    cmsStage *clutStage = cmsStageAllocCLut16bit(nullptr, 8, 3, 3, nullptr);
    
    // Get the number of channels for a color space
    cmsColorSpaceSignature colorSpace = cmsSigLabData;
    int numChannels = cmsChannelsOfColorSpace(colorSpace);
    
    // Calculate CMC delta E
    cmsCIELab Lab1, Lab2;
    cmsFloat64Number l = 2.0, c = 1.0;
    cmsFloat64Number deltaE = cmsCMCdeltaE(&Lab1, &Lab2, l, c);

    // Create an output file
    FILE *out_file = fopen("output_file", "wb");
    
    // Get the file descriptor for reading
    int in_fd = fuzz_fileno(in_file);
    
    // Get the file descriptor for writing
    int out_fd = fuzz_fileno(out_file);
    
    // Clean up the resources
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    cmsIT8Free(hIT8);
    cmsCloseProfile(hProfile);
    cmsStageFree(clutStage);
    
    assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}