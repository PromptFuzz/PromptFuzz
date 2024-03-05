#include "FDSan.h"
#include "FuzzedDataProvider.h"
#include <lcms2.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
//<ID> 2666
//<Prompt> ["cmsOpenProfileFromMem","cmsGetProfileInfo","cmsCIECAM02Done","cmsGetToneCurveParametricType","cmsIT8LoadFromFile","cmsMLUgetASCII"]
/*<Combination>: [cmsHPROFILE cmsOpenProfileFromMem(const void * MemPtr, cmsUInt32Number dwSize),
    cmsUInt32Number cmsGetProfileInfo(cmsHPROFILE hProfile, cmsInfoType Info, const char LanguageCode[3], const char CountryCode[3], wchar_t * Buffer, cmsUInt32Number BufferSize),
    void cmsCIECAM02Done(cmsHANDLE hModel),
    cmsInt32Number cmsGetToneCurveParametricType(const cmsToneCurve * t),
    cmsHANDLE cmsIT8LoadFromFile(cmsContext ContextID, const char * cFileName),
    cmsUInt32Number cmsMLUgetASCII(const cmsMLU * mlu, const char LanguageCode[3], const char CountryCode[3], char * Buffer, cmsUInt32Number BufferSize)
*/
//<score> 3.3333335, nr_unique_branch: 12, p_cov: 0.8333333
//<Quality> {"diversity":13,"density":4,"covered_api":["cmsOpenProfileFromMem","cmsGetProfileInfo","cmsGetToneCurveParametricType","cmsIT8LoadFromFile","cmsMLUgetASCII"],"uncovered_api":["cmsCIECAM02Done"],"unique_branches":{"_cmsCallocDefaultFn":[[158,9,158,37,0]],"Type_Text_Description_Read":[[983,9,983,75,1]],"Type_MLU_Read":[[1542,13,1542,46,1],[1548,9,1548,23,0],[1561,13,1561,55,0]],"_cmsMLUgetWide":[[363,9,363,19,1]],"cmsMLUgetWide":[[439,9,439,21,0]],"convert_utf16_to_utf32":[[156,13,156,30,1],[164,17,164,48,1],[167,17,167,38,0],[167,17,167,38,1],[167,42,167,63,1]]},"library_calls":["cmsOpenProfileFromMem","cmsGetProfileInfo","cmsBuildGamma","cmsGetToneCurveParametricType","cmsCreateContext","cmsIT8LoadFromFile","cmsMLUdup","cmsMLUgetASCII","cmsCloseProfile","cmsFreeToneCurve","cmsIT8Free","cmsMLUfree","cmsDeleteContext"]}
/**/


extern "C" int LLVMFuzzerTestOneInput_22(const uint8_t* f_data, size_t f_size) {
	if(f_size<=20) return 0;

	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(f_data, sizeof(uint8_t), f_size, input_file_ptr);
	fclose(input_file_ptr);

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
	//fuzzer shim end}





    // Open the profile from memory
    cmsHPROFILE hProfile = cmsOpenProfileFromMem(data, size);
    if (!hProfile) {
        return 0;
    }

    // Get the profile info
    wchar_t buffer[256];
    cmsUInt32Number bufferSize = sizeof(buffer);
    cmsGetProfileInfo(hProfile, cmsInfoDescription, fuzz_str_1, fuzz_str_2, buffer, bufferSize);

    // Get the parametric type of the tone curve
    cmsToneCurve *toneCurve = cmsBuildGamma(NULL, 2.2);
    int parametricType = cmsGetToneCurveParametricType(toneCurve);

    // Load IT8 file
    cmsContext context = cmsCreateContext(NULL, NULL);
    cmsHANDLE hIT8 = cmsIT8LoadFromFile(context, "input_file");

    // Get ASCII data from MLU
    cmsMLU *mlu = cmsMLUdup(NULL);
    char asciiBuffer[256];
    cmsUInt32Number asciiBufferSize = sizeof(asciiBuffer);
    cmsMLUgetASCII(mlu, fuzz_str_3, fuzz_str_4, asciiBuffer, asciiBufferSize);

    // Clean up resources
    cmsCloseProfile(hProfile);
    cmsFreeToneCurve(toneCurve);
    cmsIT8Free(hIT8);
    cmsMLUfree(mlu);
    cmsDeleteContext(context);

    assert_file_name_closed("input_file");
	return 0;
}