#include "FDSan.h"
#include <lcms2.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
//<ID> 1483
//<Prompt> ["cmsCreateNULLProfileTHR","cmsGetProfileInfo","cmsCreateBCHSWabstractProfile","cmsStageOutputChannels","cmsMLUalloc"]
/*<Combination>: [cmsHPROFILE cmsCreateNULLProfileTHR(cmsContext ContextID),
    cmsUInt32Number cmsGetProfileInfo(cmsHPROFILE hProfile, cmsInfoType Info, const char LanguageCode[3], const char CountryCode[3], wchar_t * Buffer, cmsUInt32Number BufferSize),
    cmsHPROFILE cmsCreateBCHSWabstractProfile(cmsUInt32Number nLUTPoints, cmsFloat64Number Bright, cmsFloat64Number Contrast, cmsFloat64Number Hue, cmsFloat64Number Saturation, cmsUInt32Number TempSrc, cmsUInt32Number TempDest),
    cmsUInt32Number cmsStageOutputChannels(const cmsStage * mpe),
    cmsMLU *cmsMLUalloc(cmsContext ContextID, cmsUInt32Number nItems)
*/
//<score> 1.6666666, nr_unique_branch: 1, p_cov: 1
//<Quality> {"diversity":12,"density":10,"covered_api":["cmsCreateNULLProfileTHR","cmsGetProfileInfo","cmsCreateBCHSWabstractProfile","cmsStageOutputChannels","cmsMLUalloc"],"uncovered_api":[],"unique_branches":{"cmsCreateBCHSWabstractProfileTHR":[[870,9,870,28,0]]},"library_calls":["cmsCreateContext","cmsCreateNULLProfileTHR","cmsGetProfileInfo","cmsCreateBCHSWabstractProfile","cmsStageAllocIdentity","cmsStageOutputChannels","cmsMLUalloc","cmsDeleteContext","cmsCloseProfile","cmsCloseProfile","cmsStageFree","cmsMLUfree"]}
/**/


extern "C" int LLVMFuzzerTestOneInput_8(const uint8_t *data, size_t size) {
	if(size<=0) return 0;

    // Create a LCMS context
    cmsContext context = cmsCreateContext(NULL, NULL);
    
    // Create a NULL profile
    cmsHPROFILE nullProfile = cmsCreateNULLProfileTHR(context);
    
    // Get the profile info
    wchar_t infoBuffer[256];
    cmsUInt32Number infoSize = cmsGetProfileInfo(nullProfile, cmsInfoDescription, "en", "US", infoBuffer, sizeof(infoBuffer));
    
    // Create a BCHSW abstract profile
    cmsHPROFILE abstractProfile = cmsCreateBCHSWabstractProfile(256, 1.0, 1.0, 0.0, 1.0, 6500, 6500);
    
    // Get the output channels of a stage
    cmsStage *stage = cmsStageAllocIdentity(context, 3);
    cmsUInt32Number outputChannels = cmsStageOutputChannels(stage);
    
    // Allocate a MLU structure
    cmsMLU *mlu = cmsMLUalloc(context, 10);
    
    // Clean up
    cmsDeleteContext(context);
    cmsCloseProfile(nullProfile);
    cmsCloseProfile(abstractProfile);
    cmsStageFree(stage);
    cmsMLUfree(mlu);
    
    return 0;
}