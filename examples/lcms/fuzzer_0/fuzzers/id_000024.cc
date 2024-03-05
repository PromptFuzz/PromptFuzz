#include "FDSan.h"
#include <lcms2.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
//<ID> 2745
//<Prompt> ["cmsOpenProfileFromMem","cmsGetProfileInfo","cmsGetToneCurveParametricType","cmsIT8LoadFromFile","cmsDupToneCurve","cmsGetSupportedIntentsTHR","cmsMLUgetASCII"]
/*<Combination>: [cmsHPROFILE cmsOpenProfileFromMem(const void * MemPtr, cmsUInt32Number dwSize),
    cmsUInt32Number cmsGetProfileInfo(cmsHPROFILE hProfile, cmsInfoType Info, const char LanguageCode[3], const char CountryCode[3], wchar_t * Buffer, cmsUInt32Number BufferSize),
    cmsInt32Number cmsGetToneCurveParametricType(const cmsToneCurve * t),
    cmsHANDLE cmsIT8LoadFromFile(cmsContext ContextID, const char * cFileName),
    cmsToneCurve *cmsDupToneCurve(const cmsToneCurve * Src),
    cmsUInt32Number cmsGetSupportedIntentsTHR(cmsContext ContextID, cmsUInt32Number nMax, cmsUInt32Number * Codes, char ** Descriptions),
    cmsUInt32Number cmsMLUgetASCII(const cmsMLU * mlu, const char LanguageCode[3], const char CountryCode[3], char * Buffer, cmsUInt32Number BufferSize)
*/
//<score> 1.0714285, nr_unique_branch: 2, p_cov: 1
//<Quality> {"diversity":14,"density":5,"covered_api":["cmsOpenProfileFromMem","cmsGetProfileInfo","cmsGetToneCurveParametricType","cmsIT8LoadFromFile","cmsDupToneCurve","cmsGetSupportedIntentsTHR","cmsMLUgetASCII"],"uncovered_api":[],"unique_branches":{"cmsGetSupportedIntentsTHR":[[1180,13,1180,28,1],[1184,17,1184,37,0]]},"library_calls":["cmsOpenProfileFromMem","cmsGetProfileInfo","cmsBuildGamma","cmsGetToneCurveParametricType","cmsDupToneCurve","cmsFreeToneCurve","cmsFreeToneCurve","cmsIT8LoadFromFile","cmsGetSupportedIntentsTHR","cmsMLUalloc","cmsMLUgetASCII","cmsCloseProfile","cmsIT8Free","cmsMLUfree"]}
/**/

// Define the maximum buffer size
#define MAX_BUFFER_SIZE 1024

// Function to read a file into a buffer
int  id_000024_readFileToBuffer(const char *fileName, unsigned char *buffer, int bufferSize)
{
    FILE *file = fopen(fileName, "rb");
    if (file == NULL)
    {
        return 0;
    }

    size_t bytesRead = fread(buffer, 1, bufferSize, file);
    assert_file_closed(&file);;

    return bytesRead;
}

// Function to write a buffer to a file
void  id_000024_writeBufferToFile(const char *fileName, unsigned char *buffer, int bufferSize)
{
    FILE *file = fopen(fileName, "wb");
    if (file == NULL)
    {
        return;
    }

    fwrite(buffer, 1, bufferSize, file);
    assert_file_closed(&file);;
}

// Fuzz driver function
extern "C" int LLVMFuzzerTestOneInput_24(const uint8_t *data, size_t size)
{
	if(size<=0) return 0;

    // Create a profile from the input data
    cmsHPROFILE hProfile = cmsOpenProfileFromMem(data, size);
    if (hProfile == NULL)
    {
        return 0;
    }

    // Get profile information
    wchar_t buffer[MAX_BUFFER_SIZE];
    cmsGetProfileInfo(hProfile, cmsInfoDescription, "en", "US", buffer, MAX_BUFFER_SIZE);

    // Get the tone curve parametric type
    cmsToneCurve *toneCurve = cmsBuildGamma(NULL, 2.2);
    if (toneCurve != NULL)
    {
        cmsInt32Number parametricType = cmsGetToneCurveParametricType(toneCurve);
        cmsToneCurve *dupToneCurve = cmsDupToneCurve(toneCurve);
        cmsFreeToneCurve(dupToneCurve);
        cmsFreeToneCurve(toneCurve);
    }

    // Load an IT8 data file
    cmsHANDLE hIT8 = cmsIT8LoadFromFile(NULL, "test.it8");

    // Get supported intents
    cmsUInt32Number intents[8];
    char *descriptions[8];
    cmsUInt32Number numIntents = cmsGetSupportedIntentsTHR(NULL, 8, intents, descriptions);

    // Get the ASCII translation from an MLU object
    cmsMLU *mlu = cmsMLUalloc(NULL, 0);
    char asciiBuffer[MAX_BUFFER_SIZE];
    cmsUInt32Number asciiSize = cmsMLUgetASCII(mlu, "en", "US", asciiBuffer, MAX_BUFFER_SIZE);

    // Cleanup
    cmsCloseProfile(hProfile);
    cmsIT8Free(hIT8);
    cmsMLUfree(mlu);

    return 0;
}