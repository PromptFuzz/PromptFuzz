#include "FDSan.h"
#include <lcms2.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
//<ID> 2777
//<Prompt> ["cmsOpenProfileFromMem","cmsGetProfileInfo","cmsGetToneCurveParametricType","cmsIT8LoadFromFile","cmsDupToneCurve","cmsGetSupportedIntentsTHR","cmsIT8SetTableByLabel","cmsMLUgetASCII"]
/*<Combination>: [cmsHPROFILE cmsOpenProfileFromMem(const void * MemPtr, cmsUInt32Number dwSize),
    cmsUInt32Number cmsGetProfileInfo(cmsHPROFILE hProfile, cmsInfoType Info, const char LanguageCode[3], const char CountryCode[3], wchar_t * Buffer, cmsUInt32Number BufferSize),
    cmsInt32Number cmsGetToneCurveParametricType(const cmsToneCurve * t),
    cmsHANDLE cmsIT8LoadFromFile(cmsContext ContextID, const char * cFileName),
    cmsToneCurve *cmsDupToneCurve(const cmsToneCurve * Src),
    cmsUInt32Number cmsGetSupportedIntentsTHR(cmsContext ContextID, cmsUInt32Number nMax, cmsUInt32Number * Codes, char ** Descriptions),
    int cmsIT8SetTableByLabel(cmsHANDLE hIT8, const char * cSet, const char * cField, const char * ExpectedType),
    cmsUInt32Number cmsMLUgetASCII(const cmsMLU * mlu, const char LanguageCode[3], const char CountryCode[3], char * Buffer, cmsUInt32Number BufferSize)
*/
//<score> 3.75, nr_unique_branch: 5, p_cov: 0.875
//<Quality> {"diversity":28,"density":20,"covered_api":["cmsOpenProfileFromMem","cmsGetProfileInfo","cmsGetToneCurveParametricType","cmsIT8LoadFromFile","cmsGetSupportedIntentsTHR","cmsIT8SetTableByLabel","cmsMLUgetASCII"],"uncovered_api":["cmsDupToneCurve"],"unique_branches":{"cmsIT8GetData":[[2772,9,2772,19,0]],"cmsIT8SetTableByLabel":[[2903,9,2903,23,0],[2903,27,2903,39,1],[2906,9,2906,23,1],[2910,9,2910,19,0]]},"library_calls":["cmsCreateContext","cmsOpenProfileFromMem","cmsGetProfileInfo","cmsBuildGamma","cmsGetToneCurveParametricType","cmsFreeToneCurve","cmsIT8LoadFromFile","cmsIT8SetTableByLabel","cmsGetSupportedIntentsTHR","cmsMLUalloc","cmsMLUgetASCII","cmsMLUfree","cmsIT8Free","cmsCloseProfile","cmsDeleteContext"]}
/**/


extern "C" int LLVMFuzzerTestOneInput_25(const uint8_t *data, size_t size) {
	if(size<=0) return 0;

	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);


    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (ctx == NULL) {
        return 0;
    }

    cmsHPROFILE hProfile = NULL;
    cmsHANDLE hIT8 = NULL;

    // Open profile from memory
    hProfile = cmsOpenProfileFromMem(data, size);
    if (hProfile == NULL) {
        cmsDeleteContext(ctx);
        return 0;
    }

    // Get profile information
    cmsUInt32Number bufferSize = 100;
    wchar_t* buffer = (wchar_t*)malloc(bufferSize * sizeof(wchar_t));
    if (buffer == NULL) {
        cmsCloseProfile(hProfile);
        cmsDeleteContext(ctx);
        return 0;
    }
    cmsGetProfileInfo(hProfile, cmsInfoDescription, "en", "US", buffer, bufferSize);
    free(buffer);

    // Get tone curve parametric type
    cmsToneCurve* toneCurve = cmsBuildGamma(ctx, 2.2);
    if (toneCurve == NULL) {
        cmsCloseProfile(hProfile);
        cmsDeleteContext(ctx);
        return 0;
    }
    cmsInt32Number parametricType = cmsGetToneCurveParametricType(toneCurve);
    cmsFreeToneCurve(toneCurve);

    // Load IT8 data from file
    hIT8 = cmsIT8LoadFromFile(ctx, "input_file");
    if (hIT8 == NULL) {
        cmsCloseProfile(hProfile);
        cmsDeleteContext(ctx);
        assert_file_name_closed("input_file");
	return 0;
    }

    // Set table by label
    cmsIT8SetTableByLabel(hIT8, "Set", "Field", "ExpectedType");

    // Get supported intents
    cmsUInt32Number maxIntents = 10;
    cmsUInt32Number numIntents;
    cmsUInt32Number* intents = (cmsUInt32Number*)malloc(maxIntents * sizeof(cmsUInt32Number));
    char** descriptions = (char**)malloc(maxIntents * sizeof(char*));
    if (intents == NULL || descriptions == NULL) {
        cmsIT8Free(hIT8);
        cmsCloseProfile(hProfile);
        cmsDeleteContext(ctx);
        assert_file_name_closed("input_file");
	return 0;
    }
    numIntents = cmsGetSupportedIntentsTHR(ctx, maxIntents, intents, descriptions);
    free(intents);
    free(descriptions);

    // Get ASCII from MLU
    cmsMLU* mlu = cmsMLUalloc(ctx, 1);
    char* asciiBuffer = (char*)malloc(100);
    if (mlu == NULL || asciiBuffer == NULL) {
        cmsIT8Free(hIT8);
        cmsCloseProfile(hProfile);
        cmsDeleteContext(ctx);
        assert_file_name_closed("input_file");
	return 0;
    }
    cmsMLUgetASCII(mlu, "en", "US", asciiBuffer, 100);
    free(asciiBuffer);
    cmsMLUfree(mlu);

    // Clean up
    cmsIT8Free(hIT8);
    cmsCloseProfile(hProfile);
    cmsDeleteContext(ctx);

    assert_file_name_closed("input_file");
	return 0;
}