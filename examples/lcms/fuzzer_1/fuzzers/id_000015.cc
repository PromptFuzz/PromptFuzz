#include "FDSan.h"
#include "FuzzedDataProvider.h"
#include <lcms2.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
//<ID> 1848
//<Prompt> ["cmsOpenProfileFromMemTHR","cmsGetProfileVersion","cmsLinkTag","cmsWhitePointFromTemp","cmsDetectBlackPoint","cmsIsCLUT"]
/*<Combination>: [cmsHPROFILE cmsOpenProfileFromMemTHR(cmsContext ContextID, const void * MemPtr, cmsUInt32Number dwSize),
    cmsFloat64Number cmsGetProfileVersion(cmsHPROFILE hProfile),
    cmsBool cmsLinkTag(cmsHPROFILE hProfile, cmsTagSignature sig, cmsTagSignature dest),
    cmsBool cmsWhitePointFromTemp(cmsCIExyY * WhitePoint, cmsFloat64Number TempK),
    cmsBool cmsDetectBlackPoint(cmsCIEXYZ * BlackPoint, cmsHPROFILE hProfile, cmsUInt32Number Intent, cmsUInt32Number dwFlags),
    cmsBool cmsIsCLUT(cmsHPROFILE hProfile, cmsUInt32Number Intent, cmsUInt32Number UsedDirection)
*/
//<score> 9.9, nr_unique_branch: 10, p_cov: 1
//<Quality> {"diversity":10,"density":9,"covered_api":["cmsOpenProfileFromMemTHR","cmsGetProfileVersion","cmsLinkTag","cmsWhitePointFromTemp","cmsDetectBlackPoint","cmsIsCLUT"],"uncovered_api":[],"unique_branches":{"BlackPointUsingPerceptualBlack":[[155,9,155,79,1],[162,9,162,27,0],[162,9,162,27,1],[171,9,171,22,0],[171,9,171,22,1],[179,9,179,27,0]],"cmsDetectBlackPoint":[[268,9,268,59,0],[269,9,269,56,0],[269,9,269,56,1]],"UnrollLabDoubleToFloat":[[709,31,709,44,1]]},"library_calls":["cmsCreateContext","cmsOpenProfileFromMemTHR","cmsGetProfileVersion","cmsLinkTag","cmsWhitePointFromTemp","cmsDetectBlackPoint","cmsIsCLUT","cmsCloseProfile","cmsDeleteContext"]}
/**/



extern "C" int LLVMFuzzerTestOneInput_15(const uint8_t* f_data, size_t f_size) {
	if(f_size<=4) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_1, fdp);
	//fuzzer shim end}




    // Create a memory buffer to hold the input data
    void* profileMem = malloc(size);
    memcpy(profileMem, data, size);

    // Create a context
    cmsContext context = cmsCreateContext(NULL, NULL);
    if (context == NULL) {
        free(profileMem);
        return 0;
    }

    // Open the profile from memory
    cmsHPROFILE hProfile = cmsOpenProfileFromMemTHR(context, profileMem, (cmsUInt32Number)size);
    if (hProfile == NULL) {
        cmsDeleteContext(context);
        free(profileMem);
        return 0;
    }

    // Get the profile version
    cmsFloat64Number profileVersion = cmsGetProfileVersion(hProfile);

    // Link a tag to another tag
    cmsLinkTag(hProfile, cmsSigAToB0Tag, cmsSigBToA0Tag);

    // Convert a color temperature to CIE xyY white point
    cmsCIExyY whitePoint;
    cmsWhitePointFromTemp(&whitePoint, 5500.0);

    // Detect black point of the profile
    cmsCIEXYZ blackPoint;
    cmsDetectBlackPoint(&blackPoint, hProfile, INTENT_RELATIVE_COLORIMETRIC, 0);

    // Check if the profile contains a CLUT
    cmsBool isCLUT = cmsIsCLUT(hProfile, INTENT_RELATIVE_COLORIMETRIC, fuzz_uint32_t_1);

    // Clean up resources
    cmsCloseProfile(hProfile);
    cmsDeleteContext(context);
    free(profileMem);

    return 0;
}