#include "FDSan.h"
#include "FuzzedDataProvider.h"
#include <lcms2.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
//<ID> 2688
//<Prompt> ["cmsOpenProfileFromMem","cmsGetProfileInfo","cmsGetToneCurveParametricType","cmsIT8LoadFromFile","cmsGetSupportedIntentsTHR","cmsMLUgetASCII"]
/*<Combination>: [cmsHPROFILE cmsOpenProfileFromMem(const void * MemPtr, cmsUInt32Number dwSize),
    cmsUInt32Number cmsGetProfileInfo(cmsHPROFILE hProfile, cmsInfoType Info, const char LanguageCode[3], const char CountryCode[3], wchar_t * Buffer, cmsUInt32Number BufferSize),
    cmsInt32Number cmsGetToneCurveParametricType(const cmsToneCurve * t),
    cmsHANDLE cmsIT8LoadFromFile(cmsContext ContextID, const char * cFileName),
    cmsUInt32Number cmsGetSupportedIntentsTHR(cmsContext ContextID, cmsUInt32Number nMax, cmsUInt32Number * Codes, char ** Descriptions),
    cmsUInt32Number cmsMLUgetASCII(const cmsMLU * mlu, const char LanguageCode[3], const char CountryCode[3], char * Buffer, cmsUInt32Number BufferSize)
*/
//<score> 2.25, nr_unique_branch: 8, p_cov: 1
//<Quality> {"diversity":12,"density":3,"covered_api":["cmsOpenProfileFromMem","cmsGetProfileInfo","cmsGetToneCurveParametricType","cmsIT8LoadFromFile","cmsGetSupportedIntentsTHR","cmsMLUgetASCII"],"uncovered_api":[],"unique_branches":{"cmsMLUgetASCII":[[396,9,396,21,0]],"cmsGetSupportedIntentsTHR":[[1165,41,1165,51,1],[1178,43,1178,53,0],[1178,43,1178,53,1],[1180,13,1180,28,0],[1181,17,1181,30,0],[1184,17,1184,37,1]],"IsMyFile":[[2440,8,2440,11,0]]},"library_calls":["cmsOpenProfileFromMem","cmsGetProfileInfo","cmsBuildGamma","cmsGetToneCurveParametricType","cmsIT8LoadFromFile","cmsGetSupportedIntentsTHR","cmsMLUalloc","cmsMLUgetASCII","cmsCloseProfile","cmsFreeToneCurve","cmsIT8Free","cmsMLUfree"]}
/*Here is the C++ program that achieves the event using the provided lcms APIs:

*/


extern "C" int LLVMFuzzerTestOneInput_23(const uint8_t* f_data, size_t f_size) {
	if(f_size<=20) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
	//fuzzer shim end}




    cmsContext ctx = NULL;

    // Step 1: Open profile from memory
    cmsHPROFILE hProfile = cmsOpenProfileFromMem(data, size);
    if (hProfile == NULL) {
        return 0;
    }

    // Step 2: Get profile info
    wchar_t buffer[2048];
    cmsUInt32Number bufferSize = sizeof(buffer);
    cmsGetProfileInfo(hProfile, cmsInfoDescription, fuzz_str_1, fuzz_str_2, buffer, bufferSize);

    // Step 3: Get tone curve parametric type
    cmsToneCurve* toneCurve = cmsBuildGamma(NULL, 2.2);
    cmsInt32Number parametricType = cmsGetToneCurveParametricType(toneCurve);

    // Step 4: Load IT8 file from file system
    cmsContext it8Ctx = NULL;
    cmsHANDLE it8Handle = cmsIT8LoadFromFile(it8Ctx, "it8_file");

    // Step 5: Get supported intents
    cmsUInt32Number intents[10];
    cmsUInt32Number numIntents = cmsGetSupportedIntentsTHR(ctx, 10, intents, NULL);

    // Step 6: Get ASCII string from MLU
    cmsMLU* mlu = cmsMLUalloc(NULL, 100);
    char asciiBuffer[256];
    cmsMLUgetASCII(mlu, fuzz_str_3, fuzz_str_4, asciiBuffer, sizeof(asciiBuffer));

    // Clean up resources
    cmsCloseProfile(hProfile);
    cmsFreeToneCurve(toneCurve);
    cmsIT8Free(it8Handle);
    cmsMLUfree(mlu);

    return 0;
}