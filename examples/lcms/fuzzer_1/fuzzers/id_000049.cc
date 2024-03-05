#include "FDSan.h"
#include "FuzzedDataProvider.h"
#include <lcms2.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
//<ID> 6008
//<Prompt> ["cmsGetAlarmCodesTHR","cmsD50_XYZ","cmsGetTagSignature","cmsIT8SaveToFile","cmsIsIntentSupported","cmsGetNamedColorList","cmsSliceSpace16","cmsSetProfileVersion"]
/*<Combination>: [void cmsGetAlarmCodesTHR(cmsContext ContextID, cmsUInt16Number AlarmCodes[16]),
    const cmsCIEXYZ *cmsD50_XYZ(),
    cmsTagSignature cmsGetTagSignature(cmsHPROFILE hProfile, cmsUInt32Number n),
    cmsBool cmsIT8SaveToFile(cmsHANDLE hIT8, const char * cFileName),
    cmsBool cmsIsIntentSupported(cmsHPROFILE hProfile, cmsUInt32Number Intent, cmsUInt32Number UsedDirection),
    cmsNAMEDCOLORLIST *cmsGetNamedColorList(cmsHTRANSFORM xform),
    cmsBool cmsSliceSpace16(cmsUInt32Number nInputs, const cmsUInt32Number * clutPoints, cmsSAMPLER16 Sampler, void * Cargo),
    void cmsSetProfileVersion(cmsHPROFILE hProfile, cmsFloat64Number Version)
*/
//<score> 2.5, nr_unique_branch: 4, p_cov: 1
//<Quality> {"diversity":16,"density":8,"covered_api":["cmsGetAlarmCodesTHR","cmsD50_XYZ","cmsGetTagSignature","cmsIT8SaveToFile","cmsIsIntentSupported","cmsGetNamedColorList","cmsSliceSpace16","cmsSetProfileVersion"],"uncovered_api":[],"unique_branches":{"cmsGetNamedColorList":[[768,9,768,18,1],[769,9,769,23,1],[772,9,772,20,1],[774,9,774,47,0]]},"library_calls":["cmsCreateContext","cmsCreate_sRGBProfile","cmsGetAlarmCodesTHR","cmsD50_XYZ","cmsGetTagSignature","cmsIT8Alloc","cmsIT8SaveToFile","cmsIsIntentSupported","cmsCreateTransform","cmsGetNamedColorList","cmsSliceSpace16","cmsSetProfileVersion","cmsIT8Free","cmsDeleteTransform","cmsCloseProfile","cmsDeleteContext"]}
/**/


extern "C" int LLVMFuzzerTestOneInput_49(const uint8_t* f_data, size_t f_size) {
	if(f_size<=4) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_1, fdp);
	//fuzzer shim end}




    // Create a lcms context
    cmsContext context = cmsCreateContext(nullptr, nullptr);

    // Create a dummy profile
    cmsHPROFILE hProfile = cmsCreate_sRGBProfile();

    // Get the alarm codes
    cmsUInt16Number alarmCodes[16];
    cmsGetAlarmCodesTHR(context, alarmCodes);

    // Get the D50 XYZ values
    const cmsCIEXYZ *d50XYZ = cmsD50_XYZ();

    // Get the tag signature
    cmsTagSignature tagSignature = cmsGetTagSignature(hProfile, fuzz_uint32_t_1);

    // Create an IT8 handle
    cmsHANDLE hIT8 = cmsIT8Alloc(nullptr);

    // Save IT8 handle to a file
    cmsBool saveResult = cmsIT8SaveToFile(hIT8, "output_file");

    // Check if the intent is supported
    cmsBool isIntentSupported = cmsIsIntentSupported(hProfile, INTENT_ABSOLUTE_COLORIMETRIC, FALSE);

    // Get the named color list from transform
    cmsHTRANSFORM hTransform = cmsCreateTransform(hProfile, TYPE_RGB_8, hProfile, TYPE_RGB_8, INTENT_PERCEPTUAL, 0);
    cmsNAMEDCOLORLIST *namedColorList = cmsGetNamedColorList(hTransform);

    // Slice the 16-bit color space
    cmsUInt32Number clutPoints[3] = {256, 256, 256};
    cmsBool sliceResult = cmsSliceSpace16(3, clutPoints, nullptr, nullptr);

    // Set the profile version
    cmsSetProfileVersion(hProfile, 2.0);

    // Release the resources
    cmsIT8Free(hIT8);
    cmsDeleteTransform(hTransform);
    cmsCloseProfile(hProfile);
    cmsDeleteContext(context);

    assert_file_name_closed("output_file");
	return 0;
}