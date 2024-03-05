#include "FDSan.h"
#include "FuzzedDataProvider.h"
#include <lcms2.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
//<ID> 1575
//<Prompt> ["cmsCreateNULLProfileTHR","cmsGetProfileInfo","cmsCreateBCHSWabstractProfile","cmsGetTransformInputFormat","cmsMLUdup","cmsStageData","cmsStageOutputChannels","cmsMLUalloc"]
/*<Combination>: [cmsHPROFILE cmsCreateNULLProfileTHR(cmsContext ContextID),
    cmsUInt32Number cmsGetProfileInfo(cmsHPROFILE hProfile, cmsInfoType Info, const char LanguageCode[3], const char CountryCode[3], wchar_t * Buffer, cmsUInt32Number BufferSize),
    cmsHPROFILE cmsCreateBCHSWabstractProfile(cmsUInt32Number nLUTPoints, cmsFloat64Number Bright, cmsFloat64Number Contrast, cmsFloat64Number Hue, cmsFloat64Number Saturation, cmsUInt32Number TempSrc, cmsUInt32Number TempDest),
    cmsUInt32Number cmsGetTransformInputFormat(cmsHTRANSFORM hTransform),
    cmsMLU *cmsMLUdup(const cmsMLU * mlu),
    void *cmsStageData(const cmsStage * mpe),
    cmsUInt32Number cmsStageOutputChannels(const cmsStage * mpe),
    cmsMLU *cmsMLUalloc(cmsContext ContextID, cmsUInt32Number nItems)
*/
//<score> 1.625, nr_unique_branch: 1, p_cov: 1
//<Quality> {"diversity":16,"density":13,"covered_api":["cmsCreateNULLProfileTHR","cmsGetProfileInfo","cmsCreateBCHSWabstractProfile","cmsGetTransformInputFormat","cmsMLUdup","cmsStageData","cmsStageOutputChannels","cmsMLUalloc"],"uncovered_api":[],"unique_branches":{"cmsGetTransformInputFormat":[[1416,9,1416,22,0]]},"library_calls":["cmsCreateContext","cmsCreateNULLProfileTHR","cmsGetProfileInfo","cmsCreateBCHSWabstractProfile","cmsGetTransformInputFormat","cmsMLUalloc","cmsMLUdup","cmsStageAllocIdentity","cmsStageData","cmsStageOutputChannels","cmsMLUfree","cmsMLUfree","cmsStageFree","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"]}
/*Here is an example program that demonstrates the usage of the lcms library APIs to achieve the desired event:

*/


extern "C" int LLVMFuzzerTestOneInput_11(const uint8_t* f_data, size_t f_size) {
	if(f_size<=10) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
	//fuzzer shim end}




    // Create a context
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    
    // Create a NULL profile
    cmsHPROFILE hNullProfile = cmsCreateNULLProfileTHR(ctx);
    
    // Get profile info
    wchar_t buffer[256];
    cmsUInt32Number bufferSize = sizeof(buffer);
    cmsGetProfileInfo(hNullProfile, cmsInfoDescription, fuzz_str_1, fuzz_str_2, buffer, bufferSize);
    
    // Create a BCHSW abstract profile
    cmsUInt32Number nLUTPoints = 256;
    cmsFloat64Number Bright = 0.5;
    cmsFloat64Number Contrast = 1.0;
    cmsFloat64Number Hue = 0.0;
    cmsFloat64Number Saturation = 1.0;
    cmsUInt32Number TempSrc = 5000;
    cmsUInt32Number TempDest = 6500;
    cmsHPROFILE hBCHSWProfile = cmsCreateBCHSWabstractProfile(nLUTPoints, Bright, Contrast, Hue, Saturation, TempSrc, TempDest);
    
    // Get the input format of a transform
    cmsUInt32Number inputFormat = cmsGetTransformInputFormat(NULL);
    
    // Duplicate an MLU
    cmsMLU *mlu = cmsMLUalloc(ctx, 10);
    cmsMLU *mluCopy = cmsMLUdup(mlu);
    
    // Get data from a stage
    cmsStage *stage = cmsStageAllocIdentity(ctx, 3);
    void *stageData = cmsStageData(stage);
    
    // Get number of output channels in a stage
    cmsUInt32Number numOutputChannels = cmsStageOutputChannels(stage);
    
    // Clean up
    cmsMLUfree(mlu);
    cmsMLUfree(mluCopy);
    cmsStageFree(stage);
    cmsCloseProfile(hNullProfile);
    cmsCloseProfile(hBCHSWProfile);
    cmsDeleteContext(ctx);
    
    return 0;
}