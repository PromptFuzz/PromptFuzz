#include "FDSan.h"
#include <lcms2.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
//<ID> 9107
//<Prompt> ["cmsIT8SetTable","cmsCreateTransform","cmsDeltaE","cmsSetAdaptationStateTHR","cmsMLUtranslationsCodes"]
/*<Combination>: [cmsInt32Number cmsIT8SetTable(cmsHANDLE hIT8, cmsUInt32Number nTable),
    cmsHTRANSFORM cmsCreateTransform(cmsHPROFILE Input, cmsUInt32Number InputFormat, cmsHPROFILE Output, cmsUInt32Number OutputFormat, cmsUInt32Number Intent, cmsUInt32Number dwFlags),
    cmsFloat64Number cmsDeltaE(const cmsCIELab * Lab1, const cmsCIELab * Lab2),
    cmsFloat64Number cmsSetAdaptationStateTHR(cmsContext ContextID, cmsFloat64Number d),
    cmsBool cmsMLUtranslationsCodes(const cmsMLU * mlu, cmsUInt32Number idx, char LanguageCode[3], char CountryCode[3])
*/
//<score> 1.4, nr_unique_branch: 1, p_cov: 0.8
//<Quality> {"diversity":16,"density":14,"covered_api":["cmsIT8SetTable","cmsCreateTransform","cmsSetAdaptationStateTHR","cmsMLUtranslationsCodes"],"uncovered_api":["cmsDeltaE"],"unique_branches":{"cmsMLUtranslationsCodes":[[499,9,499,20,0]]},"library_calls":["cmsCreateContext","cmsIT8LoadFromMem","cmsIT8SetTable","cmsCreate_sRGBProfileTHR","cmsCreateLab2Profile","cmsCreateTransform","cmsSetAdaptationStateTHR","cmsMLUtranslationsCodes","cmsDeleteTransform","cmsCloseProfile","cmsCloseProfile","cmsIT8Free","cmsDeleteContext"]}
/**/

extern "C" int LLVMFuzzerTestOneInput_82(const uint8_t *data, size_t size) {
	if(size<=0) return 0;

    // Create an lcms context
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    
    // Create an lcms IT8 handler
    cmsHANDLE hIT8 = cmsIT8LoadFromMem(ctx, data, size);
    if (!hIT8) {
        // Error handling
        cmsDeleteContext(ctx);
        return 0;
    }
    
    // Set table in IT8 handler
    cmsInt32Number table = 10;
    cmsIT8SetTable(hIT8, table);
    
    // Create input profile
    cmsHPROFILE inputProfile = cmsCreate_sRGBProfileTHR(ctx);
    
    // Create output profile
    cmsHPROFILE outputProfile = cmsCreateLab2Profile(NULL);
    
    // Create the transform
    cmsHTRANSFORM transform = cmsCreateTransform(inputProfile, TYPE_RGB_8, outputProfile, TYPE_Lab_8, INTENT_PERCEPTUAL, 0);
    if (!transform) {
        // Error handling
        cmsMLUtranslationsCodes(NULL, 0, NULL, NULL);
        cmsDeleteContext(ctx);
        return 0;
    }
    
    // Set adaptation state
    cmsSetAdaptationStateTHR(ctx, 0.5);
    
    // Translate MLU codes
    char languageCode[3];
    char countryCode[3];
    cmsMLUtranslationsCodes(NULL, 0, languageCode, countryCode);
    
    // Cleanup
    cmsDeleteTransform(transform);
    cmsCloseProfile(outputProfile);
    cmsCloseProfile(inputProfile);
    cmsIT8Free(hIT8);
    cmsDeleteContext(ctx);
    
    return 0;
}