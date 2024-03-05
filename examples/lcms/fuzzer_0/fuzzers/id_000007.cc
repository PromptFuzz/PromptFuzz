#include "FDSan.h"
#include <lcms2.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
//<ID> 1469
//<Prompt> ["cmsCreateNULLProfileTHR","cmsGetProfileInfo","cmsReadRawTag","cmsCreateBCHSWabstractProfile","cmsStageOutputChannels","cmsMLUalloc"]
/*<Combination>: [cmsHPROFILE cmsCreateNULLProfileTHR(cmsContext ContextID),
    cmsUInt32Number cmsGetProfileInfo(cmsHPROFILE hProfile, cmsInfoType Info, const char LanguageCode[3], const char CountryCode[3], wchar_t * Buffer, cmsUInt32Number BufferSize),
    cmsUInt32Number cmsReadRawTag(cmsHPROFILE hProfile, cmsTagSignature sig, void * Buffer, cmsUInt32Number BufferSize),
    cmsHPROFILE cmsCreateBCHSWabstractProfile(cmsUInt32Number nLUTPoints, cmsFloat64Number Bright, cmsFloat64Number Contrast, cmsFloat64Number Hue, cmsFloat64Number Saturation, cmsUInt32Number TempSrc, cmsUInt32Number TempDest),
    cmsUInt32Number cmsStageOutputChannels(const cmsStage * mpe),
    cmsMLU *cmsMLUalloc(cmsContext ContextID, cmsUInt32Number nItems)
*/
//<score> 0.75, nr_unique_branch: 1, p_cov: 0.5
//<Quality> {"diversity":8,"density":6,"covered_api":["cmsCreateNULLProfileTHR","cmsCreateBCHSWabstractProfile","cmsMLUalloc"],"uncovered_api":["cmsGetProfileInfo","cmsReadRawTag","cmsStageOutputChannels"],"unique_branches":{"GetInfo":[[993,5,993,22,0]]},"library_calls":["cmsCreateContext","cmsCreateNULLProfileTHR","cmsCreateBCHSWabstractProfile","cmsMLUalloc","cmsMLUfree","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"]}
/**/

// Function to read profile information
void  id_000007_readProfileInfo(cmsHPROFILE hProfile) {
    wchar_t buffer[256];
    cmsUInt32Number bufferSize = sizeof(buffer) / sizeof(buffer[0]);

    // Get profile description
    cmsGetProfileInfo(hProfile, cmsInfoDescription, "en", "US", buffer, bufferSize);
    printf("Profile Description: %ls\n", buffer);

    // Get profile manufacturer
    cmsGetProfileInfo(hProfile, cmsInfoManufacturer, "en", "US", buffer, bufferSize);
    printf("Profile Manufacturer: %ls\n", buffer);

    // Get profile model
    cmsGetProfileInfo(hProfile, cmsInfoModel, "en", "US", buffer, bufferSize);
    printf("Profile Model: %ls\n", buffer);
}

extern "C" int LLVMFuzzerTestOneInput_7(const uint8_t *data, size_t size) {
	if(size<=0) return 0;

    // Create a context
    cmsContext context = cmsCreateContext(NULL, NULL);
    
    // Create a NULL profile
    cmsHPROFILE profile = cmsCreateNULLProfileTHR(context);

    // Read profile information
     id_000007_readProfileInfo(profile);

    // Create a BC HSW abstract profile
    cmsHPROFILE abstractProfile = cmsCreateBCHSWabstractProfile(
        16, 1.0, 1.0, 0.0, 1.0, 6500, 5000);

    // Read abstract profile information
     id_000007_readProfileInfo(abstractProfile);

    // Allocate a MLU
    cmsMLU *mlu = cmsMLUalloc(context, 3);

    // Free the MLU
    cmsMLUfree(mlu);

    // Free the profiles and context
    cmsCloseProfile(profile);
    cmsCloseProfile(abstractProfile);
    cmsDeleteContext(context);

    return 0;
}