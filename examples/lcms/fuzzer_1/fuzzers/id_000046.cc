#include "FDSan.h"
#include "FuzzedDataProvider.h"
#include <lcms2.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
//<ID> 5631
//<Prompt> ["cmsCreateLab2ProfileTHR","cmsStageAllocCLutFloat","cmsIT8LoadFromMem","cmsGetProfileContextID","cmsIT8SaveToFile","cmsCreate_sRGBProfileTHR"]
/*<Combination>: [cmsHPROFILE cmsCreateLab2ProfileTHR(cmsContext ContextID, const cmsCIExyY * WhitePoint),
    cmsStage *cmsStageAllocCLutFloat(cmsContext ContextID, cmsUInt32Number nGridPoints, cmsUInt32Number inputChan, cmsUInt32Number outputChan, const cmsFloat32Number * Table),
    cmsHANDLE cmsIT8LoadFromMem(cmsContext ContextID, const void * Ptr, cmsUInt32Number len),
    cmsContext cmsGetProfileContextID(cmsHPROFILE hProfile),
    cmsBool cmsIT8SaveToFile(cmsHANDLE hIT8, const char * cFileName),
    cmsHPROFILE cmsCreate_sRGBProfileTHR(cmsContext ContextID)
*/
//<score> 4, nr_unique_branch: 3, p_cov: 1
//<Quality> {"diversity":30,"density":30,"covered_api":["cmsCreateLab2ProfileTHR","cmsStageAllocCLutFloat","cmsIT8LoadFromMem","cmsGetProfileContextID","cmsIT8SaveToFile","cmsCreate_sRGBProfileTHR"],"uncovered_api":[],"unique_branches":{"cmsStageAllocCLutFloatGranular":[[683,9,683,22,0],[684,19,684,24,0],[684,19,684,24,1]]},"library_calls":["cmsCreateContext","cmsCreateLab2ProfileTHR","cmsStageAllocCLutFloat","cmsIT8LoadFromMem","cmsGetProfileContextID","cmsIT8SaveToFile","cmsCreate_sRGBProfileTHR","cmsCloseProfile","cmsStageFree","cmsIT8Free","cmsCloseProfile","cmsDeleteContext"]}
/**/


extern "C" int LLVMFuzzerTestOneInput_46(const uint8_t* f_data, size_t f_size) {
	if(f_size<=0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    cmsContext ctx = cmsCreateContext(nullptr, nullptr); // Create a context

    // Create Lab2 profile
    cmsCIExyY whitePoint = {0.3457, 0.3585, 1.0};
    cmsHPROFILE labProfile = cmsCreateLab2ProfileTHR(ctx, &whitePoint);
    if (labProfile == nullptr) {
        cmsDeleteContext(ctx);
        return 0;
    }

    // Create CLUT stage
    cmsUInt32Number nGridPoints = 8;
    cmsUInt32Number inputChan = 3;
    cmsUInt32Number outputChan = 3;
    cmsFloat32Number clutTable[nGridPoints * nGridPoints * nGridPoints * inputChan * outputChan];
    cmsStage* clutStage = cmsStageAllocCLutFloat(ctx, nGridPoints, inputChan, outputChan, clutTable);
    if (clutStage == nullptr) {
        cmsCloseProfile(labProfile);
        cmsDeleteContext(ctx);
        return 0;
    }

    // Load IT8 data
    cmsHANDLE it8Handle = cmsIT8LoadFromMem(ctx, data, static_cast<cmsUInt32Number>(size));
    if (it8Handle == nullptr) {
        cmsStageFree(clutStage);
        cmsCloseProfile(labProfile);
        cmsDeleteContext(ctx);
        return 0;
    }

    // Get the context of the profile
    cmsContext profileCtx = cmsGetProfileContextID(labProfile);
    if (profileCtx == nullptr) {
        cmsIT8Free(it8Handle);
        cmsStageFree(clutStage);
        cmsCloseProfile(labProfile);
        cmsDeleteContext(ctx);
        return 0;
    }

    // Save IT8 data to file
    const char* fileName = "output_file";
    cmsBool saved = cmsIT8SaveToFile(it8Handle, fileName);
    if (!saved) {
        cmsIT8Free(it8Handle);
        cmsStageFree(clutStage);
        cmsCloseProfile(labProfile);
        cmsDeleteContext(ctx);
        assert_file_name_closed("output_file");
	return 0;
    }

    // Create sRGB profile
    cmsHPROFILE sRGBProfile = cmsCreate_sRGBProfileTHR(profileCtx);
    if (sRGBProfile == nullptr) {
        cmsIT8Free(it8Handle);
        cmsStageFree(clutStage);
        cmsCloseProfile(labProfile);
        cmsDeleteContext(ctx);
        assert_file_name_closed("output_file");
	return 0;
    }

    // Clean up
    cmsCloseProfile(labProfile);
    cmsStageFree(clutStage);
    cmsIT8Free(it8Handle);
    cmsCloseProfile(sRGBProfile);
    cmsDeleteContext(ctx);

    assert_file_name_closed("output_file");
	return 0;
}