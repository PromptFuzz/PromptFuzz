#include "FDSan.h"
#include <lcms2.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
//<ID> 8910
//<Prompt> ["cmsCreateLab4ProfileTHR","cmsFloat2LabEncodedV2","cmsIT8SetTableByLabel","cmsCMCdeltaE","_cmsICCcolorSpace"]
/*<Combination>: [cmsHPROFILE cmsCreateLab4ProfileTHR(cmsContext ContextID, const cmsCIExyY * WhitePoint),
    void cmsFloat2LabEncodedV2(cmsUInt16Number wLab[3], const cmsCIELab * Lab),
    int cmsIT8SetTableByLabel(cmsHANDLE hIT8, const char * cSet, const char * cField, const char * ExpectedType),
    cmsFloat64Number cmsCMCdeltaE(const cmsCIELab * Lab1, const cmsCIELab * Lab2, cmsFloat64Number l, cmsFloat64Number c),
    cmsColorSpaceSignature _cmsICCcolorSpace(int OurNotation)
*/
//<score> 1.4, nr_unique_branch: 1, p_cov: 1
//<Quality> {"diversity":10,"density":7,"covered_api":["cmsCreateLab4ProfileTHR","cmsFloat2LabEncodedV2","cmsIT8SetTableByLabel","cmsCMCdeltaE","_cmsICCcolorSpace"],"uncovered_api":[],"unique_branches":{"_cmsICCcolorSpace":[[768,8,768,14,0]]},"library_calls":["cmsCreateContext","cmsCreateLab4ProfileTHR","cmsFloat2LabEncodedV2","cmsIT8Alloc","cmsIT8SetTableByLabel","cmsCMCdeltaE","_cmsICCcolorSpace","cmsCloseProfile","cmsIT8Free","cmsDeleteContext"]}
/*Here is the complete C++ code that achieves the given event by utilizing the provided lcms APIs:

*/



extern "C" int LLVMFuzzerTestOneInput_74(const uint8_t *data, size_t size) {
	if(size<=0) return 0;

    // Create a lcms context
    cmsContext context = cmsCreateContext(NULL, NULL);
    
    // Create a Lab4 profile with a given white point
    cmsCIExyY whitePoint;
    cmsHPROFILE labProfile = cmsCreateLab4ProfileTHR(context, &whitePoint);
    
    // Convert Lab color to encoded Lab
    cmsCIELab labColor;
    cmsUInt16Number encodedLab[3];
    cmsFloat2LabEncodedV2(encodedLab, &labColor);
    
    // Set a table in IT8 format by label
    cmsHANDLE it8Handle = cmsIT8Alloc(context);
    cmsIT8SetTableByLabel(it8Handle, "set", "field", "ExpectedType");
    
    // Calculate CMC delta E between two Lab colors
    cmsCIELab lab1, lab2;
    cmsFloat64Number l = 2.0, c = 1.0;
    cmsFloat64Number deltaE = cmsCMCdeltaE(&lab1, &lab2, l, c);
    
    // Get the ICC color space notation for our internal notation
    cmsColorSpaceSignature colorSpace = _cmsICCcolorSpace(1);
    
    // Clean up allocated resources
    cmsCloseProfile(labProfile);
    cmsIT8Free(it8Handle);
    cmsDeleteContext(context);
    
    return 0;
}