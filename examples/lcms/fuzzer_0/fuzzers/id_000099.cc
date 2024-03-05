#include "FDSan.h"
#include <lcms2.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
//<ID> 11873
//<Prompt> ["cmsOpenProfileFromFile","cmsCreateBCHSWabstractProfile","cmsCreateInkLimitingDeviceLinkTHR","cmsReverseToneCurve","cmsGetToneCurveEstimatedTableEntries","cmsIT8LoadFromFile","cmsLabEncoded2FloatV2"]
/*<Combination>: [cmsHPROFILE cmsOpenProfileFromFile(const char * ICCProfile, const char * sAccess),
    cmsHPROFILE cmsCreateBCHSWabstractProfile(cmsUInt32Number nLUTPoints, cmsFloat64Number Bright, cmsFloat64Number Contrast, cmsFloat64Number Hue, cmsFloat64Number Saturation, cmsUInt32Number TempSrc, cmsUInt32Number TempDest),
    cmsHPROFILE cmsCreateInkLimitingDeviceLinkTHR(cmsContext ContextID, cmsColorSpaceSignature ColorSpace, cmsFloat64Number Limit),
    cmsToneCurve *cmsReverseToneCurve(const cmsToneCurve * InGamma),
    cmsUInt32Number cmsGetToneCurveEstimatedTableEntries(const cmsToneCurve * t),
    cmsHANDLE cmsIT8LoadFromFile(cmsContext ContextID, const char * cFileName),
    void cmsLabEncoded2FloatV2(cmsCIELab * Lab, const cmsUInt16Number wLab[3])
*/
//<score> 0.6, nr_unique_branch: 1, p_cov: 1
//<Quality> {"diversity":20,"density":6,"covered_api":["cmsOpenProfileFromFile","cmsCreateBCHSWabstractProfile","cmsCreateInkLimitingDeviceLinkTHR","cmsReverseToneCurve","cmsGetToneCurveEstimatedTableEntries","cmsIT8LoadFromFile","cmsLabEncoded2FloatV2"],"uncovered_api":[],"unique_branches":{"cmsDeleteContext":[[1004,21,1004,40,1]]},"library_calls":["cmsCreateContext","cmsOpenProfileFromFile","cmsCreateBCHSWabstractProfile","cmsCreateInkLimitingDeviceLinkTHR","cmsBuildGamma","cmsCreateContext","cmsReverseToneCurve","cmsFreeToneCurve","cmsFreeToneCurve","cmsBuildGamma","cmsCreateContext","cmsGetToneCurveEstimatedTableEntries","cmsFreeToneCurve","cmsIT8LoadFromFile","cmsLabEncoded2FloatV2","cmsCloseProfile","cmsCloseProfile","cmsCloseProfile","cmsIT8Free","cmsDeleteContext"]}
/**/


extern "C" int LLVMFuzzerTestOneInput_99(const uint8_t *data, size_t size) {
	if(size<=0) return 0;

    cmsContext context = cmsCreateContext(nullptr, nullptr);

    FILE *in_file = fmemopen((void *)data, size, "rb");
    FILE *out_file = fopen("output_file", "wb");

    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);

    // Open profile from file
    cmsHPROFILE profile = cmsOpenProfileFromFile("input.icc", "r");

    // Create abstract profile
    cmsHPROFILE abstractProfile = cmsCreateBCHSWabstractProfile(256, 0.5, 1.0, 0.0, 1.0, 5000, 6500);

    // Create ink limiting device link profile
    cmsHPROFILE inkLimitingProfile = cmsCreateInkLimitingDeviceLinkTHR(context, cmsSigGrayData, 0.8);

    // Reverse a tone curve
    cmsToneCurve *originalCurve = cmsBuildGamma(cmsCreateContext(nullptr, nullptr), 2.2);
    cmsToneCurve *reversedCurve = cmsReverseToneCurve(originalCurve);
    cmsFreeToneCurve(originalCurve);
    cmsFreeToneCurve(reversedCurve);

    // Get estimated table entries for a tone curve
    cmsToneCurve *toneCurve = cmsBuildGamma(cmsCreateContext(nullptr, nullptr), 2.2);
    cmsUInt32Number tableEntries = cmsGetToneCurveEstimatedTableEntries(toneCurve);
    cmsFreeToneCurve(toneCurve);

    // Load IT8 data from file
    cmsHANDLE it8Data = cmsIT8LoadFromFile(context, "it8data.txt");

    // Convert encoded Lab to float Lab
    cmsUInt16Number labEncoded[3] = {65535, 32768, 49152};
    cmsCIELab labFloat;
    cmsLabEncoded2FloatV2(&labFloat, labEncoded);

    // Cleanup
    cmsCloseProfile(profile);
    cmsCloseProfile(abstractProfile);
    cmsCloseProfile(inkLimitingProfile);
    cmsIT8Free(it8Data);

    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;

    cmsDeleteContext(context);

    assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}