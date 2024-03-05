#include "FDSan.h"
#include <lcms2.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
//<ID> 1553
//<Prompt> ["cmsIT8SetDataDbl","cmsCreateNULLProfileTHR","cmsGetProfileInfo","cmsCreateBCHSWabstractProfile","cmsMLUdup","cmsStageData","cmsStageOutputChannels","cmsMLUalloc"]
/*<Combination>: [cmsBool cmsIT8SetDataDbl(cmsHANDLE hIT8, const char * cPatch, const char * cSample, cmsFloat64Number Val),
    cmsHPROFILE cmsCreateNULLProfileTHR(cmsContext ContextID),
    cmsUInt32Number cmsGetProfileInfo(cmsHPROFILE hProfile, cmsInfoType Info, const char LanguageCode[3], const char CountryCode[3], wchar_t * Buffer, cmsUInt32Number BufferSize),
    cmsHPROFILE cmsCreateBCHSWabstractProfile(cmsUInt32Number nLUTPoints, cmsFloat64Number Bright, cmsFloat64Number Contrast, cmsFloat64Number Hue, cmsFloat64Number Saturation, cmsUInt32Number TempSrc, cmsUInt32Number TempDest),
    cmsMLU *cmsMLUdup(const cmsMLU * mlu),
    void *cmsStageData(const cmsStage * mpe),
    cmsUInt32Number cmsStageOutputChannels(const cmsStage * mpe),
    cmsMLU *cmsMLUalloc(cmsContext ContextID, cmsUInt32Number nItems)
*/
//<score> 2.3161764, nr_unique_branch: 2, p_cov: 0.875
//<Quality> {"diversity":17,"density":15,"covered_api":["cmsCreateNULLProfileTHR","cmsGetProfileInfo","cmsCreateBCHSWabstractProfile","cmsMLUdup","cmsStageData","cmsStageOutputChannels","cmsMLUalloc"],"uncovered_api":["cmsIT8SetDataDbl"],"unique_branches":{"cmsMLUdup":[[289,9,289,28,0],[300,9,300,33,0]]},"library_calls":["cmsCreateContext","cmsIT8Alloc","cmsCreateNULLProfileTHR","cmsGetProfileInfo","cmsCreateBCHSWabstractProfile","cmsMLUalloc","cmsMLUdup","cmsStageAllocIdentity","cmsStageData","cmsStageOutputChannels","cmsIT8Free","cmsCloseProfile","cmsCloseProfile","cmsMLUfree","cmsMLUfree","cmsStageFree","cmsDeleteContext"]}
/**/

extern "C" int LLVMFuzzerTestOneInput_10(const uint8_t *data, size_t size) {
	if(size<=0) return 0;

    // Create a context
    cmsContext context = cmsCreateContext(NULL, NULL);
    
    // Create an empty IT8 structure
    cmsHANDLE it8 = cmsIT8Alloc(context);
    
    // Create a NULL profile
    cmsHPROFILE nullProfile = cmsCreateNULLProfileTHR(context);
    
    // Get profile info
    wchar_t buffer[256];
    cmsUInt32Number bufferSize = 256;
    cmsGetProfileInfo(nullProfile, cmsInfoDescription, "en", "US", buffer, bufferSize);
    
    // Create an abstract profile
    cmsHPROFILE abstractProfile = cmsCreateBCHSWabstractProfile(100, 0.5, 0.5, 0.5, 0.5, 5000, 6500);
    
    // Duplicate an MLU
    cmsMLU *mlu = cmsMLUalloc(context, 10);
    cmsMLU *mluDup = cmsMLUdup(mlu);
    
    // Get stage data
    cmsStage *stage = cmsStageAllocIdentity(context, 3);
    void *stageData = cmsStageData(stage);
    
    // Get stage output channels
    cmsUInt32Number outputChannels = cmsStageOutputChannels(stage);
    
    // Release resources
    cmsIT8Free(it8);
    cmsCloseProfile(nullProfile);
    cmsCloseProfile(abstractProfile);
    cmsMLUfree(mlu);
    cmsMLUfree(mluDup);
    cmsStageFree(stage);
    cmsDeleteContext(context);
    
    return 0;
}