#include "FDSan.h"
#include <lcms2.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
//<ID> 12082
//<Prompt> ["cmsOpenProfileFromFile","cmsCreateBCHSWabstractProfile","cmsCreateInkLimitingDeviceLinkTHR","cmsReverseToneCurve","cmsIT8LoadFromFile","cmsGetContextUserData","cmsLabEncoded2FloatV2"]
/*<Combination>: [cmsHPROFILE cmsOpenProfileFromFile(const char * ICCProfile, const char * sAccess),
    cmsHPROFILE cmsCreateBCHSWabstractProfile(cmsUInt32Number nLUTPoints, cmsFloat64Number Bright, cmsFloat64Number Contrast, cmsFloat64Number Hue, cmsFloat64Number Saturation, cmsUInt32Number TempSrc, cmsUInt32Number TempDest),
    cmsHPROFILE cmsCreateInkLimitingDeviceLinkTHR(cmsContext ContextID, cmsColorSpaceSignature ColorSpace, cmsFloat64Number Limit),
    cmsToneCurve *cmsReverseToneCurve(const cmsToneCurve * InGamma),
    cmsHANDLE cmsIT8LoadFromFile(cmsContext ContextID, const char * cFileName),
    void *cmsGetContextUserData(cmsContext ContextID),
    void cmsLabEncoded2FloatV2(cmsCIELab * Lab, const cmsUInt16Number wLab[3])
*/
//<score> 0.7222222, nr_unique_branch: 0, p_cov: 1
//<Quality> {"diversity":18,"density":13,"covered_api":["cmsOpenProfileFromFile","cmsCreateBCHSWabstractProfile","cmsCreateInkLimitingDeviceLinkTHR","cmsReverseToneCurve","cmsIT8LoadFromFile","cmsGetContextUserData","cmsLabEncoded2FloatV2"],"uncovered_api":[],"unique_branches":{},"library_calls":["cmsCreateContext","cmsGetContextUserData","cmsGetProfileContextID","cmsOpenProfileFromFile","cmsOpenProfileFromFile","cmsCreateBCHSWabstractProfile","cmsCreateInkLimitingDeviceLinkTHR","cmsBuildGamma","cmsReverseToneCurve","cmsIT8LoadFromFile","cmsLabEncoded2FloatV2","cmsCloseProfile","cmsCloseProfile","cmsCloseProfile","cmsFreeToneCurve","cmsFreeToneCurve","cmsIT8Free","cmsDeleteContext"]}
/**/


extern "C" int LLVMFuzzerTestOneInput_100(const uint8_t *data, size_t size) {
	if(size<=0) return 0;

    FILE *in_file = fmemopen((void *)data, size, "rb");
    FILE *out_file = fopen("output_file", "wb");

    // Create a lcms context
    cmsContext ctx = cmsCreateContext(nullptr, cmsGetContextUserData(cmsGetProfileContextID(cmsOpenProfileFromFile("input_profile.icc", "r"))));

    // Open a profile from a file
    cmsHPROFILE profile = cmsOpenProfileFromFile("input_profile.icc", "r");

    // Create a BCHSW abstract profile
    cmsHPROFILE bchswProfile = cmsCreateBCHSWabstractProfile(256, 1.0, 1.0, 0.0, 1.0, 6500, 5000);

    // Create an ink limiting device link profile
    cmsHPROFILE inkLimitProfile = cmsCreateInkLimitingDeviceLinkTHR(ctx, cmsSigLabData, 0.5);

    // Reverse a tone curve
    cmsToneCurve *curve = cmsBuildGamma(ctx, 1.8);
    cmsToneCurve *reverseCurve = cmsReverseToneCurve(curve);

    // Load an IT8 file
    cmsHANDLE it8Handle = cmsIT8LoadFromFile(ctx, "it8_file.it8");

    // Convert encoded Lab to float Lab
    cmsCIELab lab;
    cmsUInt16Number wLab[3] = {5000, 0, 0};
    cmsLabEncoded2FloatV2(&lab, wLab);

    // Release resources
    cmsCloseProfile(profile);
    cmsCloseProfile(bchswProfile);
    cmsCloseProfile(inkLimitProfile);
    cmsFreeToneCurve(curve);
    cmsFreeToneCurve(reverseCurve);
    cmsIT8Free(it8Handle);
    cmsDeleteContext(ctx);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
  
    assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}