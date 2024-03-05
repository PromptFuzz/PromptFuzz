#include "FDSan.h"
#include <lcms2.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
//<ID> 14355
//<Prompt> ["cmsIT8SaveToMem","cmsAllocNamedColorList","cmsGetToneCurveParametricType","cmsCreate_sRGBProfile","cmsCreateExtendedTransform","cmsCreateContext"]
/*<Combination>: [cmsBool cmsIT8SaveToMem(cmsHANDLE hIT8, void * MemPtr, cmsUInt32Number * BytesNeeded),
    cmsNAMEDCOLORLIST *cmsAllocNamedColorList(cmsContext ContextID, cmsUInt32Number n, cmsUInt32Number ColorantCount, const char * Prefix, const char * Suffix),
    cmsInt32Number cmsGetToneCurveParametricType(const cmsToneCurve * t),
    cmsHPROFILE cmsCreate_sRGBProfile(),
    cmsHTRANSFORM cmsCreateExtendedTransform(cmsContext ContextID, cmsUInt32Number nProfiles, cmsHPROFILE * hProfiles, cmsBool * BPC, cmsUInt32Number * Intents, cmsFloat64Number * AdaptationStates, cmsHPROFILE hGamutProfile, cmsUInt32Number nGamutPCSposition, cmsUInt32Number InputFormat, cmsUInt32Number OutputFormat, cmsUInt32Number dwFlags),
    cmsContext cmsCreateContext(void * Plugin, void * UserData)
*/
//<score> 2.5, nr_unique_branch: 2, p_cov: 0.8333333
//<Quality> {"diversity":18,"density":18,"covered_api":["cmsIT8SaveToMem","cmsAllocNamedColorList","cmsCreate_sRGBProfile","cmsCreateExtendedTransform","cmsCreateContext"],"uncovered_api":["cmsGetToneCurveParametricType"],"unique_branches":{"cmsCreateExtendedTransform":[[1210,9,1210,30,0],[1210,34,1210,65,1]]},"library_calls":["cmsCreateContext","cmsIT8LoadFromMem","cmsIT8TableCount","cmsAllocNamedColorList","cmsIT8SaveToMem","cmsCreate_sRGBProfile","cmsCreateExtendedTransform","cmsDeleteTransform","cmsCloseProfile","cmsIT8Free","cmsFreeNamedColorList","cmsDeleteContext"]}
/**/



extern "C" int LLVMFuzzerTestOneInput_113(const uint8_t *data, size_t size) {
	if(size<=0) return 0;

    // Create a context
    cmsContext context = cmsCreateContext(nullptr, nullptr);
    
    // Read the input data from memory
    cmsHANDLE it8Handle = cmsIT8LoadFromMem(context, data, (cmsUInt32Number)size);
    if (it8Handle == nullptr) {
        // Release resources
        cmsDeleteContext(context);
        return 0;
    }
    
    // Get the patch count
    cmsUInt32Number patchCount = cmsIT8TableCount(it8Handle);
    
    // Create a named color list
    cmsNAMEDCOLORLIST* namedColorList = cmsAllocNamedColorList(context, patchCount, 0, "Prefix", "Suffix");
    if (namedColorList == nullptr) {
        // Release resources
        cmsIT8Free(it8Handle);
        cmsDeleteContext(context);
        return 0;
    }
    
    // Save the named color list to memory
    cmsUInt32Number bytesNeeded;
    cmsBool result = cmsIT8SaveToMem(it8Handle, nullptr, &bytesNeeded);
    if (result == 0) {
        // Release resources
        cmsIT8Free(it8Handle);
        cmsFreeNamedColorList(namedColorList);
        cmsDeleteContext(context);
        return 0;
    }
    
    // Create an sRGB profile
    cmsHPROFILE profile = cmsCreate_sRGBProfile();
    
    // Create an extended transform
    cmsHPROFILE hProfiles[2] = {profile, profile};
    cmsBool BPC[2] = {0, 0};
    cmsUInt32Number Intents[2] = {0, 1};
    cmsFloat64Number AdaptationStates[2] = {0.0, 0.0};
    cmsHTRANSFORM transform = cmsCreateExtendedTransform(context, 2, hProfiles, BPC, Intents, AdaptationStates, profile, 0, TYPE_RGB_8, TYPE_RGB_8, 0);
    
    // Cleanup
    cmsDeleteTransform(transform);
    cmsCloseProfile(profile);
    cmsIT8Free(it8Handle);
    cmsFreeNamedColorList(namedColorList);
    cmsDeleteContext(context);
    
    return 0;
}