#include "FDSan.h"
#include <lcms2.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
//<ID> 10009
//<Prompt> ["cmsDetectTAC","cmsIT8SetPropertyStr","cmsSetHeaderProfileID","cmsIT8GetData","cmsDupProfileSequenceDescription"]
/*<Combination>: [cmsFloat64Number cmsDetectTAC(cmsHPROFILE hProfile),
    cmsBool cmsIT8SetPropertyStr(cmsHANDLE hIT8, const char * cProp, const char * Str),
    void cmsSetHeaderProfileID(cmsHPROFILE hProfile, cmsUInt8Number * ProfileID),
    const char *cmsIT8GetData(cmsHANDLE hIT8, const char * cPatch, const char * cSample),
    cmsSEQ *cmsDupProfileSequenceDescription(const cmsSEQ * pseq)
*/
//<score> 23.5, nr_unique_branch: 46, p_cov: 1
//<Quality> {"diversity":14,"density":7,"covered_api":["cmsDetectTAC","cmsIT8SetPropertyStr","cmsSetHeaderProfileID","cmsIT8GetData","cmsDupProfileSequenceDescription"],"uncovered_api":[],"unique_branches":{"TrilinearInterp16":[[580,16,580,35,0],[583,16,583,35,0],[586,16,586,35,0]],"PackFloatsFromFloat":[[2967,35,2967,65,0],[2974,12,2974,22,1],[2977,20,2977,29,0],[2977,20,2977,29,1],[2979,39,2979,45,1],[2983,19,2983,26,1],[2986,19,2986,25,1],[2993,12,2993,22,0],[2993,26,2993,35,1],[709,31,709,44,1]],"IsInkSpace":[[1062,6,1062,17,1],[1063,6,1063,18,0],[1064,6,1064,18,1],[1065,6,1065,18,1],[1066,6,1066,18,1],[1067,6,1067,18,1],[1068,6,1068,18,1],[1069,6,1069,19,1],[1070,6,1070,19,1],[1071,6,1071,19,1],[1072,6,1072,19,1],[1073,6,1073,19,1],[1074,6,1074,19,1],[1076,6,1076,13,1]],"cmsSliceSpace16":[[885,17,885,39,1],[897,13,897,38,1]],"cmsFormatterForColorspaceOfProfile":[[3848,46,3848,54,0]],"EstimateTAC":[[439,22,439,43,0],[439,22,439,43,1],[443,9,443,26,0],[443,9,443,26,1],[447,23,447,44,0],[447,23,447,44,1]],"BuildGrayOutputPipeline":[[418,9,418,24,0]],"cmsDetectTAC":[[468,9,468,57,0],[468,9,468,57,1],[476,9,476,25,0],[476,9,476,25,1],[482,9,482,42,1],[485,9,485,21,1],[491,9,491,30,0],[491,9,491,30,1],[499,9,499,58,1]]},"library_calls":["cmsCreateContext","cmsOpenProfileFromMem","cmsDetectTAC","cmsIT8Alloc","cmsIT8SetPropertyStr","cmsSetHeaderProfileID","cmsIT8GetData","cmsDupProfileSequenceDescription","cmsCloseProfile","cmsDeleteContext","cmsIT8Free","cmsFreeProfileSequenceDescription"]}
/**/


extern "C" int LLVMFuzzerTestOneInput_86(const uint8_t *data, size_t size) {
	if(size<=0) return 0;

    // Create lcms context
    cmsContext context = cmsCreateContext(NULL, NULL);
    
    // Open the profile from memory
    cmsHPROFILE profile = cmsOpenProfileFromMem(data, size);
    
    if (profile == NULL) {
        // Error handling
        cmsCloseProfile(profile);
        cmsDeleteContext(context);
        return 0;
    }
    
    // Detect TAC
    cmsFloat64Number tac = cmsDetectTAC(profile);
    
    // Create an IT8 handle
    cmsHANDLE it8Handle = cmsIT8Alloc(context);
    
    // Set a property for the IT8 handle
    cmsIT8SetPropertyStr(it8Handle, "PropName", "PropValue");
    
    // Set the profile ID
    cmsUInt8Number profileID[16];
    memset(profileID, 0, sizeof(profileID));
    cmsSetHeaderProfileID(profile, profileID);
    
    // Get data from the IT8 handle
    const char *it8Data = cmsIT8GetData(it8Handle, "Patch", "Sample");
    
    // Duplicate profile sequence description
    cmsSEQ *seq = cmsDupProfileSequenceDescription(NULL);
    
    // Clean up
    cmsCloseProfile(profile);
    cmsDeleteContext(context);
    cmsIT8Free(it8Handle);
    cmsFreeProfileSequenceDescription(seq);
    
    return 0;
}