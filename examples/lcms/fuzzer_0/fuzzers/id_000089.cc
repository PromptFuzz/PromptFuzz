#include "FDSan.h"
#include <lcms2.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
//<ID> 10506
//<Prompt> ["cmsIT8GetProperty","cmsSaveProfileToFile","cmsCreateGrayProfileTHR","cmsGetHeaderFlags","cmsEvalToneCurveFloat"]
/*<Combination>: [const char *cmsIT8GetProperty(cmsHANDLE hIT8, const char * cProp),
    cmsBool cmsSaveProfileToFile(cmsHPROFILE hProfile, const char * FileName),
    cmsHPROFILE cmsCreateGrayProfileTHR(cmsContext ContextID, const cmsCIExyY * WhitePoint, const cmsToneCurve * TransferFunction),
    cmsUInt32Number cmsGetHeaderFlags(cmsHPROFILE hProfile),
    cmsFloat32Number cmsEvalToneCurveFloat(const cmsToneCurve * Curve, cmsFloat32Number v)
*/
//<score> 1, nr_unique_branch: 0, p_cov: 1
//<Quality> {"diversity":15,"density":15,"covered_api":["cmsIT8GetProperty","cmsSaveProfileToFile","cmsCreateGrayProfileTHR","cmsGetHeaderFlags","cmsEvalToneCurveFloat"],"uncovered_api":[],"unique_branches":{},"library_calls":["cmsCreateContext","cmsIT8Alloc","cmsIT8SetPropertyDbl","cmsIT8SetPropertyDbl","cmsIT8GetProperty","cmsIT8GetProperty","cmsBuildGamma","cmsCreateGrayProfileTHR","cmsSaveProfileToFile","cmsGetHeaderFlags","cmsEvalToneCurveFloat","cmsCloseProfile","cmsFreeToneCurve","cmsIT8Free","cmsDeleteContext"]}
/**/

extern "C" int LLVMFuzzerTestOneInput_89(const uint8_t *data, size_t size) {
	if(size<=0) return 0;

    // Create a lcms context
    cmsContext context = cmsCreateContext(nullptr, nullptr);
  
    // Create a IT8 handle
    cmsHANDLE hIT8 = cmsIT8Alloc(context);
  
    // Set the IT8 properties
    cmsIT8SetPropertyDbl(hIT8, "property1", 1.0);
    cmsIT8SetPropertyDbl(hIT8, "property2", 2.0);
  
    // Get the IT8 properties
    const char* prop1 = cmsIT8GetProperty(hIT8, "property1");
    const char* prop2 = cmsIT8GetProperty(hIT8, "property2");
  
    // Print the IT8 properties
    printf("property1: %s\n", prop1);
    printf("property2: %s\n", prop2);
  
    // Create a gray profile
    cmsCIExyY whitePoint = {0.3127, 0.3290, 1.0};
    cmsToneCurve* transferFunction = cmsBuildGamma(context, 2.2);
    cmsHPROFILE grayProfile = cmsCreateGrayProfileTHR(context, &whitePoint, transferFunction);
  
    // Save the profile to a file
    cmsSaveProfileToFile(grayProfile, "gray_profile.icc");

    // Get the header flags of the profile
    cmsUInt32Number headerFlags = cmsGetHeaderFlags(grayProfile);
  
    // Evaluate the tone curve
    cmsFloat32Number value = 0.5;
    cmsFloat32Number result = cmsEvalToneCurveFloat(transferFunction, value);
  
    // Print the header flags and evaluated tone curve value
    printf("Header Flags: %u\n", headerFlags);
    printf("Evaluated Tone Curve Value: %f\n", result);
  
    // Free the resources
    cmsCloseProfile(grayProfile);
    cmsFreeToneCurve(transferFunction);
    cmsIT8Free(hIT8);
    cmsDeleteContext(context);
  
    return 0;
}