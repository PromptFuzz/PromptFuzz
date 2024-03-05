#include "FDSan.h"
#include "FuzzedDataProvider.h"
#include <lcms2.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
//<ID> 9106
//<Prompt> ["cmsIT8SetTable","cmsCreateTransform","cmsDeltaE","cmsSetAdaptationStateTHR","cmsMLUtranslationsCodes"]
/*<Combination>: [cmsInt32Number cmsIT8SetTable(cmsHANDLE hIT8, cmsUInt32Number nTable),
    cmsHTRANSFORM cmsCreateTransform(cmsHPROFILE Input, cmsUInt32Number InputFormat, cmsHPROFILE Output, cmsUInt32Number OutputFormat, cmsUInt32Number Intent, cmsUInt32Number dwFlags),
    cmsFloat64Number cmsDeltaE(const cmsCIELab * Lab1, const cmsCIELab * Lab2),
    cmsFloat64Number cmsSetAdaptationStateTHR(cmsContext ContextID, cmsFloat64Number d),
    cmsBool cmsMLUtranslationsCodes(const cmsMLU * mlu, cmsUInt32Number idx, char LanguageCode[3], char CountryCode[3])
*/
//<score> 2.8, nr_unique_branch: 5, p_cov: 1
//<Quality> {"diversity":15,"density":7,"covered_api":["cmsIT8SetTable","cmsCreateTransform","cmsDeltaE","cmsSetAdaptationStateTHR","cmsMLUtranslationsCodes"],"uncovered_api":[],"unique_branches":{"OptimizeByJoiningCurves":[[1395,47,1395,82,0]],"OptimizeByComputingLinearization":[[1051,47,1051,82,0]],"cmsIT8SetTable":[[1359,10,1359,37,0],[1361,14,1361,41,1]],"OptimizeByResampling":[[662,47,662,82,0]]},"library_calls":["cmsIT8LoadFromMem","cmsIT8SetTable","cmsCreate_sRGBProfile","cmsCreateLab2Profile","cmsD50_xyY","cmsCreateTransform","cmsSetAdaptationStateTHR","cmsMLUalloc","cmsMLUtranslationsCodes","cmsDeltaE","cmsMLUfree","cmsCloseProfile","cmsCloseProfile","cmsDeleteTransform","cmsIT8Free"]}
/**/

extern "C" int LLVMFuzzerTestOneInput_77(const uint8_t* f_data, size_t f_size) {
	if(f_size<=4) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_1, fdp);
	//fuzzer shim end}




    // Create a handle for IT8 (International Target) data
    cmsHANDLE hIT8 = cmsIT8LoadFromMem(nullptr, data, size);
    if (hIT8 == nullptr) {
        return 0;
    }

    // Set the table for IT8 data
    cmsInt32Number nTable = 10;
    cmsIT8SetTable(hIT8, nTable);

    // Create input and output profiles
    cmsHPROFILE inputProfile = cmsCreate_sRGBProfile();
    cmsHPROFILE outputProfile = cmsCreateLab2Profile(cmsD50_xyY());

    // Create a transform using the profiles
    cmsHTRANSFORM transform = cmsCreateTransform(inputProfile, TYPE_RGB_8, outputProfile, TYPE_Lab_DBL, INTENT_PERCEPTUAL, 0);
    
    // Get the current adaptation state
    cmsFloat64Number adaptationState = cmsSetAdaptationStateTHR(nullptr, 0.5);

    // Get the translations codes from the MLU (Multilingual Unicode) data
    cmsMLU *mlu = cmsMLUalloc(nullptr, 10);
    char languageCode[3], countryCode[3];
    cmsMLUtranslationsCodes(mlu, fuzz_uint32_t_1, languageCode, countryCode);

    // Use the transform and print the DeltaE between two Lab values
    cmsCIELab lab1 = {50.0, 0.0, 0.0};
    cmsCIELab lab2 = {60.0, 10.0, 20.0};
    cmsFloat64Number deltaE = cmsDeltaE(&lab1, &lab2);
    std::cout << "DeltaE: " << deltaE << std::endl;

    // Free the resources
    cmsMLUfree(mlu);
    cmsCloseProfile(inputProfile);
    cmsCloseProfile(outputProfile);
    cmsDeleteTransform(transform);
    cmsIT8Free(hIT8);

    return 0;
}