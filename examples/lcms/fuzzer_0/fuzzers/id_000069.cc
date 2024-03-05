#include "FDSan.h"
#include <lcms2.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
//<ID> 8033
//<Prompt> ["cmsGetSupportedIntentsTHR","cmsDupNamedColorList","cmsCreateGrayProfileTHR","cmsOpenProfileFromFile","cmsWhitePointFromTemp","cmsGetHeaderAttributes"]
/*<Combination>: [cmsUInt32Number cmsGetSupportedIntentsTHR(cmsContext ContextID, cmsUInt32Number nMax, cmsUInt32Number * Codes, char ** Descriptions),
    cmsNAMEDCOLORLIST *cmsDupNamedColorList(const cmsNAMEDCOLORLIST * v),
    cmsHPROFILE cmsCreateGrayProfileTHR(cmsContext ContextID, const cmsCIExyY * WhitePoint, const cmsToneCurve * TransferFunction),
    cmsHPROFILE cmsOpenProfileFromFile(const char * ICCProfile, const char * sAccess),
    cmsBool cmsWhitePointFromTemp(cmsCIExyY * WhitePoint, cmsFloat64Number TempK),
    void cmsGetHeaderAttributes(cmsHPROFILE hProfile, cmsUInt64Number * Flags)
*/
//<score> 0.61538464, nr_unique_branch: 1, p_cov: 1
//<Quality> {"diversity":13,"density":4,"covered_api":["cmsGetSupportedIntentsTHR","cmsDupNamedColorList","cmsCreateGrayProfileTHR","cmsOpenProfileFromFile","cmsWhitePointFromTemp","cmsGetHeaderAttributes"],"uncovered_api":[],"unique_branches":{"cmsFreeNamedColorList":[[578,9,578,18,0]]},"library_calls":["cmsOpenProfileFromMem","cmsGetSupportedIntentsTHR","cmsDupNamedColorList","cmsBuildTabulatedToneCurve16","cmsCreateGrayProfileTHR","cmsOpenProfileFromFile","cmsGetHeaderAttributes","cmsWhitePointFromTemp","cmsCloseProfile","cmsFreeToneCurve","cmsCloseProfile","cmsCloseProfile","cmsFreeNamedColorList"]}
/**/


extern "C" int LLVMFuzzerTestOneInput_69(const uint8_t *data, size_t size)
{
	if(size<=0) return 0;

	FILE *input_file_ptr = fopen("input_file.icc", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);

    // Open the profile from memory
    cmsHPROFILE hProfile = cmsOpenProfileFromMem(data, size);
    if (hProfile == nullptr) {
        return 0;
    }

    // Get the supported intents
    cmsUInt32Number intents[10] = {0};
    char *descriptions[10] = {nullptr};
    cmsUInt32Number numIntents = cmsGetSupportedIntentsTHR(nullptr, 10, intents, descriptions);

    // Duplicate the named color list
    cmsNAMEDCOLORLIST *namedColorList = cmsDupNamedColorList(nullptr);

    // Create a gray profile
    cmsCIExyY whitePoint;
    cmsToneCurve *transferFunction = cmsBuildTabulatedToneCurve16(nullptr, 256, nullptr);
    cmsHPROFILE grayProfile = cmsCreateGrayProfileTHR(nullptr, &whitePoint, transferFunction);

    // Open a profile from a file
    const char *iccProfile = "input_file.icc";
    const char *access = "r";
    cmsHPROFILE fileProfile = cmsOpenProfileFromFile(iccProfile, access);

    // Get header attributes
    cmsUInt64Number flags;
    cmsGetHeaderAttributes(hProfile, &flags);

    // White point from temperature
    cmsFloat64Number temperature = 6500.0;
    cmsCIExyY newWhitePoint;
    cmsWhitePointFromTemp(&newWhitePoint, temperature);

    // Clean up
    cmsCloseProfile(hProfile);
    cmsFreeToneCurve(transferFunction);
    cmsCloseProfile(grayProfile);
    cmsCloseProfile(fileProfile);
    cmsFreeNamedColorList(namedColorList);

    assert_file_name_closed("input_file.icc");
	return 0;
}