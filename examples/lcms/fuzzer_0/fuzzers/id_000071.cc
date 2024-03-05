#include "FDSan.h"
#include <lcms2.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
//<ID> 8435
//<Prompt> ["cmsSetPCS","cmsCreateXYZProfile","cmsCreateGrayProfile","cmsBuildGamma","cmsIT8SetIndexColumn","cmsSetAlarmCodes"]
/*<Combination>: [void cmsSetPCS(cmsHPROFILE hProfile, cmsColorSpaceSignature pcs),
    cmsHPROFILE cmsCreateXYZProfile(),
    cmsHPROFILE cmsCreateGrayProfile(const cmsCIExyY * WhitePoint, const cmsToneCurve * TransferFunction),
    cmsToneCurve *cmsBuildGamma(cmsContext ContextID, cmsFloat64Number Gamma),
    cmsBool cmsIT8SetIndexColumn(cmsHANDLE hIT8, const char * cSample),
    void cmsSetAlarmCodes(const cmsUInt16Number NewAlarm[16])
*/
//<score> 1.1111112, nr_unique_branch: 1, p_cov: 1
//<Quality> {"diversity":18,"density":10,"covered_api":["cmsSetPCS","cmsCreateXYZProfile","cmsCreateGrayProfile","cmsBuildGamma","cmsIT8SetIndexColumn","cmsSetAlarmCodes"],"uncovered_api":[],"unique_branches":{"cmsIT8SetIndexColumn":[[2935,8,2935,17,0]]},"library_calls":["cmsCreateContext","cmsCreateXYZProfile","cmsSetPCS","cmsBuildGamma","cmsCreateGrayProfile","cmsFreeToneCurve","cmsIT8Alloc","cmsIT8SetIndexColumn","cmsGetAlarmCodes","cmsSetAlarmCodes","cmsIT8Free","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"]}
/**/


extern "C" int LLVMFuzzerTestOneInput_71(const uint8_t *data, size_t size) {
	if(size<=0) return 0;

    // Step 4: Open input file
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file == NULL) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Step 5: Get file descriptor for reading
    int in_fd = fuzz_fileno(in_file);

    // Create a context
    cmsContext context = cmsCreateContext(NULL, NULL);
    if (context == NULL) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
    }

    // Step 1: Set PCS (Profile Connection Space)
    cmsHPROFILE profile = cmsCreateXYZProfile();
    cmsSetPCS(profile, cmsSigLabData);

    // Step 2: Create Gray Profile
    cmsCIExyY whitePoint = {0.3457, 0.3585, 1};
    cmsToneCurve *transferFunction = cmsBuildGamma(context, 2.2);
    cmsHPROFILE grayProfile = cmsCreateGrayProfile(&whitePoint, transferFunction);
    cmsFreeToneCurve(transferFunction);

    // Step 3: Set Index Column
    cmsHANDLE it8Handle = cmsIT8Alloc(context);
    if (it8Handle != NULL) {
        const char *sample = "SampleName";
        cmsIT8SetIndexColumn(it8Handle, sample);
    }

    // Step 6: Create output file name
    const char *outputFileName = "output_file";

    // Step 7: Open output file
    FILE *out_file = fopen(outputFileName, "wb");
    if (out_file == NULL) {
        cmsIT8Free(it8Handle);
        cmsCloseProfile(profile);
        cmsCloseProfile(grayProfile);
        cmsDeleteContext(context);
        assert_file_closed(&in_file);;
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
    }

    // Step 8: Get file descriptor for writing
    int out_fd = fuzz_fileno(out_file);

    // Step 9: Set Alarm Codes
    cmsUInt16Number alarmCodes[16];
    cmsGetAlarmCodes(alarmCodes);
    cmsSetAlarmCodes(alarmCodes);

    // Step 10: Release resources
    cmsIT8Free(it8Handle);
    cmsCloseProfile(profile);
    cmsCloseProfile(grayProfile);
    cmsDeleteContext(context);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;

    assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}