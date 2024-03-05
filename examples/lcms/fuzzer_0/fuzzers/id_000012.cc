#include "FDSan.h"
#include <lcms2.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
//<ID> 1697
//<Prompt> ["cmsLinkTag","cmsWhitePointFromTemp","cmsCreateLinearizationDeviceLinkTHR","cmsSetColorSpace","cmsIsCLUT"]
/*<Combination>: [cmsBool cmsLinkTag(cmsHPROFILE hProfile, cmsTagSignature sig, cmsTagSignature dest),
    cmsBool cmsWhitePointFromTemp(cmsCIExyY * WhitePoint, cmsFloat64Number TempK),
    cmsHPROFILE cmsCreateLinearizationDeviceLinkTHR(cmsContext ContextID, cmsColorSpaceSignature ColorSpace, cmsToneCurve *const * TransferFunctions),
    void cmsSetColorSpace(cmsHPROFILE hProfile, cmsColorSpaceSignature sig),
    cmsBool cmsIsCLUT(cmsHPROFILE hProfile, cmsUInt32Number Intent, cmsUInt32Number UsedDirection)
*/
//<score> 6.416667, nr_unique_branch: 6, p_cov: 1
//<Quality> {"diversity":12,"density":11,"covered_api":["cmsLinkTag","cmsWhitePointFromTemp","cmsCreateLinearizationDeviceLinkTHR","cmsSetColorSpace","cmsIsCLUT"],"uncovered_api":[],"unique_branches":{"cmsStageAllocToneCurves":[[283,13,283,43,0]],"cmsDupToneCurve":[[966,9,966,19,0]],"cmsCreateLinearizationDeviceLinkTHR":[[317,9,317,121,0],[333,9,333,13,0]],"CurveSetElemTypeFree":[[199,17,199,44,1]],"cmsPipelineInsertStage":[[1517,24,1517,35,0]]},"library_calls":["cmsCreateContext","cmsCreateProfilePlaceholder","cmsSetColorSpace","cmsLinkTag","cmsWhitePointFromTemp","cmsBuildTabulatedToneCurveFloat","cmsCreateLinearizationDeviceLinkTHR","cmsIsCLUT","cmsCloseProfile","cmsCloseProfile","cmsFreeToneCurve","cmsDeleteContext"]}
/**/

extern "C" int LLVMFuzzerTestOneInput_12(const uint8_t* data, size_t size) {
	if(size<=0) return 0;

    cmsContext ctx = cmsCreateContext(NULL, NULL); // Create a LCMS context
    
    // Create an input file from the data
    FILE* in_file = fmemopen((void*)data, size, "rb");
    
    // Create an output file
    FILE* out_file = fopen("output_file", "wb");
    
    // Get the file descriptors
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);
    
    // Create a profile with the specified color space
    cmsHPROFILE profile = cmsCreateProfilePlaceholder(ctx);
    cmsColorSpaceSignature colorSpace = cmsSigRgbData;
    cmsSetColorSpace(profile, colorSpace);
    
    // Link the tag from the source profile to the destination profile
    cmsTagSignature sourceTag = cmsSigViewingConditionsTag;
    cmsTagSignature destinationTag = cmsSigChromaticAdaptationTag;
    cmsLinkTag(profile, sourceTag, destinationTag);
    
    // Convert the temperature value to white point coordinates
    cmsCIExyY whitePoint;
    cmsFloat64Number temperature = 5000.0;
    cmsWhitePointFromTemp(&whitePoint, temperature);
    
    // Create a linearization device link profile with the specified color space and transfer functions
    cmsToneCurve* transferFunctions[3];
    for (int i = 0; i < 3; i++) {
        transferFunctions[i] = cmsBuildTabulatedToneCurveFloat(ctx, 256, NULL);
    }
    cmsHPROFILE linearizationProfile = cmsCreateLinearizationDeviceLinkTHR(ctx, colorSpace, transferFunctions);
    
    // Check if the profile is a CLUT (Color Look-Up Table)
    cmsUInt32Number intent = 0;
    cmsUInt32Number usedDirection = 0;
    cmsBool isCLUT = cmsIsCLUT(profile, intent, usedDirection);
    
    // Clean up
    cmsCloseProfile(profile);
    cmsCloseProfile(linearizationProfile);
    for (int i = 0; i < 3; i++) {
        cmsFreeToneCurve(transferFunctions[i]);
    }
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    cmsDeleteContext(ctx);
    
    assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}