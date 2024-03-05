#include "FDSan.h"
#include <lcms2.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
//<ID> 9304
//<Prompt> ["cmsCreateMultiprofileTransformTHR","cmsIT8SetSheetType","cmsGetToneCurveParametricType","cmsIT8GetPropertyMulti","cmsstrcasecmp"]
/*<Combination>: [cmsHTRANSFORM cmsCreateMultiprofileTransformTHR(cmsContext ContextID, cmsHPROFILE * hProfiles, cmsUInt32Number nProfiles, cmsUInt32Number InputFormat, cmsUInt32Number OutputFormat, cmsUInt32Number Intent, cmsUInt32Number dwFlags),
    cmsBool cmsIT8SetSheetType(cmsHANDLE hIT8, const char * Type),
    cmsInt32Number cmsGetToneCurveParametricType(const cmsToneCurve * t),
    const char *cmsIT8GetPropertyMulti(cmsHANDLE hIT8, const char * Key, const char * SubKey),
    int cmsstrcasecmp(const char * s1, const char * s2)
*/
//<score> 0.8125, nr_unique_branch: 0, p_cov: 1
//<Quality> {"diversity":16,"density":13,"covered_api":["cmsCreateMultiprofileTransformTHR","cmsIT8SetSheetType","cmsGetToneCurveParametricType","cmsIT8GetPropertyMulti","cmsstrcasecmp"],"uncovered_api":[],"unique_branches":{},"library_calls":["cmsCreateContext","cmsCreate_sRGBProfile","cmsCreate_sRGBProfile","cmsCreateMultiprofileTransformTHR","cmsIT8Alloc","cmsIT8SetSheetType","cmsBuildGamma","cmsGetToneCurveParametricType","cmsIT8GetPropertyMulti","cmsstrcasecmp","cmsDeleteTransform","cmsCloseProfile","cmsCloseProfile","cmsIT8Free","cmsFreeToneCurve","cmsDeleteContext"]}
/*Here is the C++ program that fulfills the requirements mentioned:

*/


extern "C" int LLVMFuzzerTestOneInput_83(const uint8_t *data, size_t size) {
	if(size<=0) return 0;

    // Create a lcms context
    cmsContext ctx = cmsCreateContext(NULL, NULL);

    // Create a multiprofile transform
    cmsHPROFILE profiles[2];
    profiles[0] = cmsCreate_sRGBProfile();
    profiles[1] = cmsCreate_sRGBProfile();
    cmsHTRANSFORM transform = cmsCreateMultiprofileTransformTHR(ctx, profiles, 2, TYPE_RGB_8, TYPE_RGB_8, INTENT_PERCEPTUAL, 0);

    // Set a sheet type in IT8 handle
    cmsHANDLE it8 = cmsIT8Alloc(ctx);
    cmsIT8SetSheetType(it8, "Sheet Type");

    // Get the parametric type of a tone curve
    cmsToneCurve* curve = cmsBuildGamma(ctx, 2.4);
    cmsInt32Number paramType = cmsGetToneCurveParametricType(curve);

    // Get a property value from IT8 handle
    const char* value = cmsIT8GetPropertyMulti(it8, "Key", "SubKey");

    // Compare two strings in a case-insensitive manner
    int strcmpResult = cmsstrcasecmp("String1", "String2");

    // Clean up resources
    cmsDeleteTransform(transform);
    cmsCloseProfile(profiles[0]);
    cmsCloseProfile(profiles[1]);
    cmsIT8Free(it8);
    cmsFreeToneCurve(curve);
    cmsDeleteContext(ctx);

    return 0;
}