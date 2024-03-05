#include "FDSan.h"
#include <lcms2.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
//<ID> 11609
//<Prompt> ["cmsGDBAddPoint","cmsPipelineInputChannels","cmsIT8SetPropertyMulti","cmsGetPostScriptCRD","cmsCIECAM02Done"]
/*<Combination>: [cmsBool cmsGDBAddPoint(cmsHANDLE hGBD, const cmsCIELab * Lab),
    cmsUInt32Number cmsPipelineInputChannels(const cmsPipeline * lut),
    cmsBool cmsIT8SetPropertyMulti(cmsHANDLE hIT8, const char * Key, const char * SubKey, const char * Buffer),
    cmsUInt32Number cmsGetPostScriptCRD(cmsContext ContextID, cmsHPROFILE hProfile, cmsUInt32Number Intent, cmsUInt32Number dwFlags, void * Buffer, cmsUInt32Number dwBufferLen),
    void cmsCIECAM02Done(cmsHANDLE hModel)
*/
//<score> 3.75, nr_unique_branch: 3, p_cov: 1
//<Quality> {"diversity":16,"density":15,"covered_api":["cmsGDBAddPoint","cmsPipelineInputChannels","cmsIT8SetPropertyMulti","cmsGetPostScriptCRD","cmsCIECAM02Done"],"uncovered_api":[],"unique_branches":{"AddToList":[[1287,21,1287,37,1]],"cmsCIECAM02Done":[[432,9,432,14,1]],"_cmsAtan2":[[106,21,106,29,1]]},"library_calls":["cmsCreateContext","cmsGBDAlloc","cmsGDBAddPoint","cmsPipelineAlloc","cmsPipelineInputChannels","cmsIT8Alloc","cmsIT8SetPropertyMulti","cmsCreate_sRGBProfile","cmsGetPostScriptCRD","cmsGetPostScriptCRD","cmsCIECAM02Done","cmsGBDFree","cmsIT8Free","cmsPipelineFree","cmsCloseProfile","cmsDeleteContext"]}
/**/


extern "C" int LLVMFuzzerTestOneInput_96(const uint8_t *data, size_t size) {
	if(size<=0) return 0;

    cmsContext ctx = cmsCreateContext(NULL, NULL);
  
    // Create a GBD handle
    cmsHANDLE hGBD = cmsGBDAlloc(ctx);

    // Create a CIE LAB color
    cmsCIELab Lab;
    // Fill the Lab color with data from the input buffer
    if (size >= sizeof(cmsCIELab)) {
        memcpy(&Lab, data, sizeof(cmsCIELab));
        data += sizeof(cmsCIELab);
        size -= sizeof(cmsCIELab);
    }
    
    // Add a new point to the GBD
    cmsBool result = cmsGDBAddPoint(hGBD, &Lab);
    
    // Create a pipeline
    cmsPipeline* lut = cmsPipelineAlloc(ctx, 1, 1);
    
    // Get the number of input channels of the pipeline
    cmsUInt32Number numInputChannels = cmsPipelineInputChannels(lut);
    
    // Set a property in an IT8 handle
    cmsHANDLE hIT8 = cmsIT8Alloc(ctx);
    const char* key = "my_key";
    const char* subkey = "my_subkey";
    const char* buffer = "my_buffer";
    result = cmsIT8SetPropertyMulti(hIT8, key, subkey, buffer);
    
    // Create a buffer to hold PostScript CRD
    void* crdBuffer = NULL;
    cmsUInt32Number bufferSize = 0;
    cmsHPROFILE hProfile = cmsCreate_sRGBProfile();
    cmsUInt32Number intent = 0;
    cmsUInt32Number flags = 0;
    bufferSize = cmsGetPostScriptCRD(ctx, hProfile, intent, flags, NULL, 0);
    crdBuffer = malloc(bufferSize);
    result = cmsGetPostScriptCRD(ctx, hProfile, intent, flags, crdBuffer, bufferSize);
    
    // Release the model used for CIECAM02
    cmsHANDLE hModel = NULL;
    cmsCIECAM02Done(hModel);

    // Cleanup and free resources
    cmsGBDFree(hGBD);
    cmsIT8Free(hIT8);
    cmsPipelineFree(lut);
    cmsCloseProfile(hProfile);
    free(crdBuffer);
    cmsDeleteContext(ctx);

    return 0;
}