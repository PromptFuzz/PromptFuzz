#include "FDSan.h"
#include <lcms2.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
//<ID> 7622
//<Prompt> ["cmsCreateXYZProfileTHR","cmsSetDeviceClass","cmsGetEncodedICCversion","cmsGetSupportedIntentsTHR","cmsPipelineEvalFloat","cmsSetLogErrorHandler","cmsxyY2XYZ","cmsNamedColorCount","cmsDictGetEntryList"]
/*<Combination>: [cmsHPROFILE cmsCreateXYZProfileTHR(cmsContext ContextID),
    void cmsSetDeviceClass(cmsHPROFILE hProfile, cmsProfileClassSignature sig),
    cmsUInt32Number cmsGetEncodedICCversion(cmsHPROFILE hProfile),
    cmsUInt32Number cmsGetSupportedIntentsTHR(cmsContext ContextID, cmsUInt32Number nMax, cmsUInt32Number * Codes, char ** Descriptions),
    void cmsPipelineEvalFloat(const cmsFloat32Number * In, cmsFloat32Number * Out, const cmsPipeline * lut),
    void cmsSetLogErrorHandler(cmsLogErrorHandlerFunction Fn),
    void cmsxyY2XYZ(cmsCIEXYZ * Dest, const cmsCIExyY * Source),
    cmsUInt32Number cmsNamedColorCount(const cmsNAMEDCOLORLIST * v),
    const cmsDICTentry *cmsDictGetEntryList(cmsHANDLE hDict)
*/
//<score> 0.8888889, nr_unique_branch: 0, p_cov: 1
//<Quality> {"diversity":18,"density":16,"covered_api":["cmsCreateXYZProfileTHR","cmsSetDeviceClass","cmsGetEncodedICCversion","cmsGetSupportedIntentsTHR","cmsPipelineEvalFloat","cmsSetLogErrorHandler","cmsxyY2XYZ","cmsNamedColorCount","cmsDictGetEntryList"],"uncovered_api":[],"unique_branches":{},"library_calls":["cmsCreateContext","cmsCreateXYZProfileTHR","cmsSetDeviceClass","cmsGetEncodedICCversion","cmsGetSupportedIntentsTHR","cmsPipelineAlloc","cmsPipelineEvalFloat","cmsSetLogErrorHandler","cmsxyY2XYZ","cmsAllocNamedColorList","cmsNamedColorCount","cmsDictAlloc","cmsDictGetEntryList","cmsPipelineFree","cmsFreeNamedColorList","cmsDictFree","cmsCloseProfile","cmsDeleteContext"]}
/**/

// Include the lcms headers

// Declare the LLVMFuzzerTestOneInput_66 function
extern "C" int LLVMFuzzerTestOneInput_66(const uint8_t *data, size_t size) {
	if(size<=0) return 0;

    // Create a lcms context
    cmsContext context = cmsCreateContext(nullptr, nullptr);
    
    // Create an XYZ profile
    cmsHPROFILE profile = cmsCreateXYZProfileTHR(context);
    
    // Set the device class of the profile
    cmsSetDeviceClass(profile, cmsSigDisplayClass);
    
    // Get the encoded ICC version of the profile
    cmsUInt32Number encodedVersion = cmsGetEncodedICCversion(profile);
    
    // Get the supported intents
    cmsUInt32Number supportedIntents[5];
    char *intentDescriptions[5];
    cmsUInt32Number numIntents = cmsGetSupportedIntentsTHR(context, 5, supportedIntents, intentDescriptions);
    
    // Create a pipeline
    cmsPipeline *pipeline = cmsPipelineAlloc(context, 3, 3);
    
    // Evaluate the pipeline with float data
    cmsFloat32Number input[3] = {1.0f, 1.0f, 1.0f};
    cmsFloat32Number output[3];
    cmsPipelineEvalFloat(input, output, pipeline);
    
    // Set the log error handler
    cmsSetLogErrorHandler([](cmsContext, cmsUInt32Number, const char *) {});
    
    // Convert xyY to XYZ
    cmsCIEXYZ destXYZ;
    cmsCIExyY sourceXY;
    sourceXY.x = 0.3127;
    sourceXY.y = 0.3290;
    sourceXY.Y = 1.0;
    cmsxyY2XYZ(&destXYZ, &sourceXY);
    
    // Get the named color count
    cmsNAMEDCOLORLIST *namedColorList = cmsAllocNamedColorList(context, 10, 3, "Prefix", "Suffix");
    cmsUInt32Number namedColorCount = cmsNamedColorCount(namedColorList);
    
    // Get the entry list from a dictionary
    cmsHANDLE dict = cmsDictAlloc(context);
    const cmsDICTentry *entryList = cmsDictGetEntryList(dict);
    
    // Clean up resources
    cmsPipelineFree(pipeline);
    cmsFreeNamedColorList(namedColorList);
    cmsDictFree(dict);
    cmsCloseProfile(profile);
    cmsDeleteContext(context);
    
    return 0;
}