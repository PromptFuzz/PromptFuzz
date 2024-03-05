#include "FDSan.h"
#include <lcms2.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
//<ID> 12417
//<Prompt> ["cmsIT8GetPropertyDbl","cmsPipelineGetPtrToFirstStage","cmsxyY2XYZ","cmsIT8GetProperty","cmsCreate_sRGBProfile","cmsIsIntentSupported"]
/*<Combination>: [cmsFloat64Number cmsIT8GetPropertyDbl(cmsHANDLE hIT8, const char * cProp),
    cmsStage *cmsPipelineGetPtrToFirstStage(const cmsPipeline * lut),
    void cmsxyY2XYZ(cmsCIEXYZ * Dest, const cmsCIExyY * Source),
    const char *cmsIT8GetProperty(cmsHANDLE hIT8, const char * cProp),
    cmsHPROFILE cmsCreate_sRGBProfile(),
    cmsBool cmsIsIntentSupported(cmsHPROFILE hProfile, cmsUInt32Number Intent, cmsUInt32Number UsedDirection)
*/
//<score> 0.42857143, nr_unique_branch: 0, p_cov: 1
//<Quality> {"diversity":14,"density":6,"covered_api":["cmsIT8GetPropertyDbl","cmsPipelineGetPtrToFirstStage","cmsxyY2XYZ","cmsIT8GetProperty","cmsCreate_sRGBProfile","cmsIsIntentSupported"],"uncovered_api":[],"unique_branches":{},"library_calls":["cmsIT8LoadFromMem","cmsIT8GetProperty","cmsIT8GetPropertyDbl","cmsCreate_sRGBProfile","cmsIsIntentSupported","cmsPipelineAlloc","cmsPipelineGetPtrToFirstStage","cmsxyY2XYZ","cmsIT8Free","cmsCloseProfile","cmsPipelineFree"]}
/**/



extern "C" int LLVMFuzzerTestOneInput_103(const uint8_t *data, size_t size) {
	if(size<=0) return 0;

    // Create an IT8 handle
    cmsHANDLE hIT8 = cmsIT8LoadFromMem(NULL, data, size);
    if (!hIT8) {
        return 0;
    }
    
    // Get a property value
    const char *prop = "property_name";
    const char *propValue = cmsIT8GetProperty(hIT8, prop);
    if (propValue) {
        // Use the property value
        printf("Property value: %s\n", propValue);
    }
    
    // Get a double property value
    const char *propDbl = "property_name_double";
    cmsFloat64Number propDblValue = cmsIT8GetPropertyDbl(hIT8, propDbl);
    // Use the double property value
    
    // Create an sRGB profile
    cmsHPROFILE sRGBProfile = cmsCreate_sRGBProfile();
    if (!sRGBProfile) {
        cmsIT8Free(hIT8);
        return 0;
    }
    
    // Check if an intent is supported
    cmsUInt32Number intent = 0; // Set the desired intent value
    cmsUInt32Number usedDirection = 0; // Set the used direction value
    cmsBool isSupported = cmsIsIntentSupported(sRGBProfile, intent, usedDirection);
    // Use the isSupported value
    
    // Get a pointer to the first stage of a pipeline
    cmsPipeline *pipeline = cmsPipelineAlloc(NULL, 3, 1);
    if (!pipeline) {
        cmsIT8Free(hIT8);
        cmsCloseProfile(sRGBProfile);
        return 0;
    }
    
    cmsStage *firstStage = cmsPipelineGetPtrToFirstStage(pipeline);
    // Use the first stage
    
    // Convert xyY to XYZ
    cmsCIExyY xyY;
    cmsCIEXYZ XYZ;
    cmsxyY2XYZ(&XYZ, &xyY);
    
    // Release resources
    cmsIT8Free(hIT8);
    cmsCloseProfile(sRGBProfile);
    cmsPipelineFree(pipeline);
    
    return 0;
}