#include "FDSan.h"
#include <lcms2.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
//<ID> 8922
//<Prompt> ["cmsCreateLab4ProfileTHR","cmsFloat2LabEncodedV2","cmsFormatterForPCSOfProfile","cmsIT8SetTableByLabel","cmsCMCdeltaE","_cmsICCcolorSpace"]
/*<Combination>: [cmsHPROFILE cmsCreateLab4ProfileTHR(cmsContext ContextID, const cmsCIExyY * WhitePoint),
    void cmsFloat2LabEncodedV2(cmsUInt16Number wLab[3], const cmsCIELab * Lab),
    cmsUInt32Number cmsFormatterForPCSOfProfile(cmsHPROFILE hProfile, cmsUInt32Number nBytes, cmsBool lIsFloat),
    int cmsIT8SetTableByLabel(cmsHANDLE hIT8, const char * cSet, const char * cField, const char * ExpectedType),
    cmsFloat64Number cmsCMCdeltaE(const cmsCIELab * Lab1, const cmsCIELab * Lab2, cmsFloat64Number l, cmsFloat64Number c),
    cmsColorSpaceSignature _cmsICCcolorSpace(int OurNotation)
*/
//<score> 0.26666668, nr_unique_branch: 0, p_cov: 0.33333334
//<Quality> {"diversity":5,"density":4,"covered_api":["cmsCreateLab4ProfileTHR","cmsFloat2LabEncodedV2"],"uncovered_api":["cmsFormatterForPCSOfProfile","cmsIT8SetTableByLabel","cmsCMCdeltaE","_cmsICCcolorSpace"],"unique_branches":{},"library_calls":["cmsCreateContext","cmsCreateLab4ProfileTHR","cmsFloat2LabEncodedV2","cmsCloseProfile","cmsDeleteContext"]}
/**/


extern "C" int LLVMFuzzerTestOneInput_77(const uint8_t* data, size_t size) {
	if(size<=0) return 0;

    // Step 4: Open input file
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Step 1: Create a Lab color profile
    cmsContext context = cmsCreateContext(nullptr, nullptr);
    cmsHPROFILE labProfile = cmsCreateLab4ProfileTHR(context, nullptr);
    if (!labProfile) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // Step 5: Get the file descriptor for input file
    int in_fd = fuzz_fileno(in_file);

    // Step 2: Read Lab color from input file
    cmsCIELab labColor;
    fread(&labColor, sizeof(labColor), 1, in_file);

    // Step 3: Convert Lab color to encoded Lab color
    cmsUInt16Number encodedLab[3];
    cmsFloat2LabEncodedV2(encodedLab, &labColor);

    // Step 7: Close input file and release resources
    assert_file_closed(&in_file);;
    cmsCloseProfile(labProfile);
    cmsDeleteContext(context);

    assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
}