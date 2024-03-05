#include "FDSan.h"
#include <lcms2.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
//<ID> 3383
//<Prompt> ["cmsSetLogErrorHandlerTHR","cmsGetPipelineContextID","cmsCreateLab4ProfileTHR","cmsIT8SaveToMem","cmsSliceSpace16"]
/*<Combination>: [void cmsSetLogErrorHandlerTHR(cmsContext ContextID, cmsLogErrorHandlerFunction Fn),
    cmsContext cmsGetPipelineContextID(const cmsPipeline * lut),
    cmsHPROFILE cmsCreateLab4ProfileTHR(cmsContext ContextID, const cmsCIExyY * WhitePoint),
    cmsBool cmsIT8SaveToMem(cmsHANDLE hIT8, void * MemPtr, cmsUInt32Number * BytesNeeded),
    cmsBool cmsSliceSpace16(cmsUInt32Number nInputs, const cmsUInt32Number * clutPoints, cmsSAMPLER16 Sampler, void * Cargo)
*/
//<score> 1.4222223, nr_unique_branch: 1, p_cov: 0.8
//<Quality> {"diversity":9,"density":8,"covered_api":["cmsSetLogErrorHandlerTHR","cmsCreateLab4ProfileTHR","cmsIT8SaveToMem","cmsSliceSpace16"],"uncovered_api":["cmsGetPipelineContextID"],"unique_branches":{"WriteStr":[[1735,17,1735,35,0]]},"library_calls":["cmsCreateContext","cmsSetLogErrorHandlerTHR","cmsCreateLab4ProfileTHR","cmsIT8Alloc","cmsIT8SaveToMem","cmsSliceSpace16","cmsCloseProfile","cmsIT8Free","cmsDeleteContext"]}
/**/


extern "C" int LLVMFuzzerTestOneInput_28(const uint8_t *data, size_t size) {
	if(size<=0) return 0;

    // Create a lcms context
    cmsContext context = cmsCreateContext(NULL, NULL);
    
    // Set log error handler
    cmsSetLogErrorHandlerTHR(context, NULL);
    
    // Create Lab profile
    cmsCIExyY whitePoint = {0.3127, 0.3290, 1.0};
    cmsHPROFILE labProfile = cmsCreateLab4ProfileTHR(context, &whitePoint);

    // Create IT8 handle
    cmsHANDLE it8Handle = cmsIT8Alloc(context);
    
    // Save IT8 data to memory
    void *memPtr = malloc(size);
    cmsUInt32Number bytesNeeded = size;
    cmsBool saveResult = cmsIT8SaveToMem(it8Handle, memPtr, &bytesNeeded);
    
    // Slice space 16
    cmsUInt32Number clutPoints[3] = {0};
    cmsSAMPLER16 sampler = NULL;
    void *cargo = NULL;
    cmsBool sliceResult = cmsSliceSpace16(3, clutPoints, sampler, cargo);

    // Release resources
    cmsCloseProfile(labProfile);
    free(memPtr);
    cmsIT8Free(it8Handle);
    cmsDeleteContext(context);
    
    return 0;
}