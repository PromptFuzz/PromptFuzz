#include "FDSan.h"
#include <lcms2.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
//<ID> 12258
//<Prompt> ["cmsOpenProfileFromFile","cmsCreateInkLimitingDeviceLinkTHR","cmsGetDeviceClass","cmsReverseToneCurve","cmsIT8LoadFromFile","cmsLabEncoded2FloatV2"]
/*<Combination>: [cmsHPROFILE cmsOpenProfileFromFile(const char * ICCProfile, const char * sAccess),
    cmsHPROFILE cmsCreateInkLimitingDeviceLinkTHR(cmsContext ContextID, cmsColorSpaceSignature ColorSpace, cmsFloat64Number Limit),
    cmsProfileClassSignature cmsGetDeviceClass(cmsHPROFILE hProfile),
    cmsToneCurve *cmsReverseToneCurve(const cmsToneCurve * InGamma),
    cmsHANDLE cmsIT8LoadFromFile(cmsContext ContextID, const char * cFileName),
    void cmsLabEncoded2FloatV2(cmsCIELab * Lab, const cmsUInt16Number wLab[3])
*/
//<score> 0.6, nr_unique_branch: 0, p_cov: 1
//<Quality> {"diversity":25,"density":15,"covered_api":["cmsOpenProfileFromFile","cmsCreateInkLimitingDeviceLinkTHR","cmsGetDeviceClass","cmsReverseToneCurve","cmsIT8LoadFromFile","cmsLabEncoded2FloatV2"],"uncovered_api":[],"unique_branches":{},"library_calls":["cmsOpenProfileFromFile","cmsCreateInkLimitingDeviceLinkTHR","cmsGetColorSpace","cmsGetDeviceClass","cmsBuildGamma","cmsReverseToneCurve","cmsIT8LoadFromFile","cmsLabEncoded2FloatV2","cmsCloseProfile","cmsCloseProfile","cmsFreeToneCurve","cmsFreeToneCurve","cmsIT8Free"]}
/**/


extern "C" int LLVMFuzzerTestOneInput_101(const uint8_t *data, size_t size) {
	if(size<=0) return 0;

	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);

    // Open the profile from file
    cmsHPROFILE hProfile = cmsOpenProfileFromFile("input_file", "r");
    if (hProfile == NULL) {
        // Handle error
        assert_file_name_closed("input_file");
	return 0;
    }

    // Create an ink limiting device link
    cmsHPROFILE hDeviceLink = cmsCreateInkLimitingDeviceLinkTHR(NULL, cmsGetColorSpace(hProfile), 0.5);
    if (hDeviceLink == NULL) {
        // Handle error
        cmsCloseProfile(hProfile);
        assert_file_name_closed("input_file");
	return 0;
    }

    // Get the device class of the profile
    cmsProfileClassSignature deviceClass = cmsGetDeviceClass(hProfile);
    if (deviceClass == cmsSigAbstractClass) {
        // Handle error
        cmsCloseProfile(hProfile);
        cmsCloseProfile(hDeviceLink);
        assert_file_name_closed("input_file");
	return 0;
    }

    // Reverse the tone curve
    cmsToneCurve* inGamma = cmsBuildGamma(NULL, 2.2);
    if (inGamma == NULL) {
        // Handle error
        cmsCloseProfile(hProfile);
        cmsCloseProfile(hDeviceLink);
        assert_file_name_closed("input_file");
	return 0;
    }
    cmsToneCurve* outGamma = cmsReverseToneCurve(inGamma);
    if (outGamma == NULL) {
        // Handle error
        cmsCloseProfile(hProfile);
        cmsCloseProfile(hDeviceLink);
        cmsFreeToneCurve(inGamma);
        assert_file_name_closed("input_file");
	return 0;
    }

    // Load IT8 from file
    cmsHANDLE hIT8 = cmsIT8LoadFromFile(NULL, "input_file");
    if (hIT8 == NULL) {
        // Handle error
        cmsCloseProfile(hProfile);
        cmsCloseProfile(hDeviceLink);
        cmsFreeToneCurve(inGamma);
        cmsFreeToneCurve(outGamma);
        assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
    }

    // Convert Lab from encoded to float
    cmsCIELab Lab;
    cmsUInt16Number wLab[3];
    // Assuming data is a byte stream of wLab
    if (size >= sizeof(wLab)) {
        memcpy(wLab, data, sizeof(wLab));
        cmsLabEncoded2FloatV2(&Lab, wLab);
    }

    // Cleanup resources
    cmsCloseProfile(hProfile);
    cmsCloseProfile(hDeviceLink);
    cmsFreeToneCurve(inGamma);
    cmsFreeToneCurve(outGamma);
    cmsIT8Free(hIT8);

    assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
}