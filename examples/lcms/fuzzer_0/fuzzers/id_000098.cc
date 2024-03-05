#include "FDSan.h"
#include <lcms2.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
//<ID> 11854
//<Prompt> ["cmsOpenProfileFromFile","cmsCreateBCHSWabstractProfile","cmsCreateInkLimitingDeviceLinkTHR","cmsReverseToneCurve","cmsIT8LoadFromFile","cmsLabEncoded2FloatV2"]
/*<Combination>: [cmsHPROFILE cmsOpenProfileFromFile(const char * ICCProfile, const char * sAccess),
    cmsHPROFILE cmsCreateBCHSWabstractProfile(cmsUInt32Number nLUTPoints, cmsFloat64Number Bright, cmsFloat64Number Contrast, cmsFloat64Number Hue, cmsFloat64Number Saturation, cmsUInt32Number TempSrc, cmsUInt32Number TempDest),
    cmsHPROFILE cmsCreateInkLimitingDeviceLinkTHR(cmsContext ContextID, cmsColorSpaceSignature ColorSpace, cmsFloat64Number Limit),
    cmsToneCurve *cmsReverseToneCurve(const cmsToneCurve * InGamma),
    cmsHANDLE cmsIT8LoadFromFile(cmsContext ContextID, const char * cFileName),
    void cmsLabEncoded2FloatV2(cmsCIELab * Lab, const cmsUInt16Number wLab[3])
*/
//<score> 5.5384617, nr_unique_branch: 17, p_cov: 1
//<Quality> {"diversity":13,"density":4,"covered_api":["cmsOpenProfileFromFile","cmsCreateBCHSWabstractProfile","cmsCreateInkLimitingDeviceLinkTHR","cmsReverseToneCurve","cmsIT8LoadFromFile","cmsLabEncoded2FloatV2"],"uncovered_api":[],"unique_branches":{"cmsCreateInkLimitingDeviceLinkTHR":[[402,9,402,37,1],[407,9,407,20,1],[407,24,407,35,1],[416,9,416,14,1],[430,9,430,20,1],[436,9,436,21,1],[438,9,438,78,1],[440,9,440,102,1],[441,9,441,54,1],[442,9,442,100,1],[446,9,446,53,1],[448,9,448,56,1],[449,9,449,54,1]],"InkLimitingSampler":[[374,9,374,27,0],[374,9,374,27,1],[377,13,377,22,0],[377,13,377,22,1]]},"library_calls":["cmsOpenProfileFromFile","cmsCloseProfile","cmsCreateInkLimitingDeviceLinkTHR","cmsCloseProfile","cmsCreateBCHSWabstractProfile","cmsCloseProfile","cmsBuildGamma","cmsReverseToneCurve","cmsFreeToneCurve","cmsFreeToneCurve","cmsIT8LoadFromFile","cmsIT8Free","cmsLabEncoded2FloatV2"]}
/**/



extern "C" int LLVMFuzzerTestOneInput_98(const uint8_t *data, size_t size)
{
	if(size<=0) return 0;

	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);

    // Create a profile from a file
    cmsHPROFILE profileFromFile = cmsOpenProfileFromFile("input_file", "r");
    cmsCloseProfile(profileFromFile);

    // Create an ink limiting device link profile
    cmsHPROFILE inkLimitingProfile = cmsCreateInkLimitingDeviceLinkTHR(NULL, cmsSigCmykData, 10.0);
    cmsCloseProfile(inkLimitingProfile);

    // Create a BCHSW abstract profile
    cmsHPROFILE bchswAbstractProfile = cmsCreateBCHSWabstractProfile(256, 1.0, 1.0, 0.0, 1.0, 5000, 6500);
    cmsCloseProfile(bchswAbstractProfile);

    // Reverse a tone curve
    cmsToneCurve *inputCurve = cmsBuildGamma(NULL, 2.2);
    cmsToneCurve *reverseCurve = cmsReverseToneCurve(inputCurve);
    cmsFreeToneCurve(inputCurve);
    cmsFreeToneCurve(reverseCurve);

    // Load an IT8 file
    cmsHANDLE it8Handle = cmsIT8LoadFromFile(NULL, "input_file");
    cmsIT8Free(it8Handle);

    // Convert Lab encoded values to float values
    cmsCIELab lab;
    cmsUInt16Number wLab[3] = {50000, 0, 0};
    cmsLabEncoded2FloatV2(&lab, wLab);

    assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
}