#include "FDSan.h"
#include <lcms2.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
//<ID> 6894
//<Prompt> ["cmsPipelineStageCount","cmsTempFromWhitePoint","cmsIT8EnumProperties","cmsCIE94DeltaE","cmsCreate_OkLabProfile","cmsD50_XYZ"]
/*<Combination>: [cmsUInt32Number cmsPipelineStageCount(const cmsPipeline * lut),
    cmsBool cmsTempFromWhitePoint(cmsFloat64Number * TempK, const cmsCIExyY * WhitePoint),
    cmsUInt32Number cmsIT8EnumProperties(cmsHANDLE hIT8, char *** PropertyNames),
    cmsFloat64Number cmsCIE94DeltaE(const cmsCIELab * Lab1, const cmsCIELab * Lab2),
    cmsHPROFILE cmsCreate_OkLabProfile(cmsContext ctx),
    const cmsCIEXYZ *cmsD50_XYZ()
*/
//<score> 1.5384616, nr_unique_branch: 1, p_cov: 1
//<Quality> {"diversity":13,"density":10,"covered_api":["cmsPipelineStageCount","cmsTempFromWhitePoint","cmsIT8EnumProperties","cmsCIE94DeltaE","cmsCreate_OkLabProfile","cmsD50_XYZ"],"uncovered_api":[],"unique_branches":{"cmsCIE94DeltaE":[[466,9,466,17,0]]},"library_calls":["cmsCreateContext","cmsPipelineAlloc","cmsPipelineStageCount","cmsTempFromWhitePoint","cmsIT8Alloc","cmsIT8EnumProperties","cmsCIE94DeltaE","cmsCreate_OkLabProfile","cmsD50_XYZ","cmsPipelineFree","cmsIT8Free","cmsCloseProfile","cmsDeleteContext"]}
/**/


// Include lcms headers here

extern "C" int LLVMFuzzerTestOneInput_58(const uint8_t *data, size_t size)
{
	if(size<=0) return 0;

    // Create a lcms context
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    
    // Create an empty lcms pipeline
    cmsPipeline *lut = cmsPipelineAlloc(ctx, 3, 3);
    
    // Get the number of stages in the pipeline
    cmsUInt32Number stageCount = cmsPipelineStageCount(lut);
    
    // Get temperature from a white point
    cmsFloat64Number tempK;
    const cmsCIExyY whitePoint = {0.3457, 0.3585, 1.0};
    cmsTempFromWhitePoint(&tempK, &whitePoint);
    
    // Enumerate properties of an IT8 handle
    cmsHANDLE it8Handle = cmsIT8Alloc(ctx);
    char **propertyNames;
    cmsUInt32Number propertyCount = cmsIT8EnumProperties(it8Handle, &propertyNames);
    
    // Calculate CIE94 DeltaE between two Lab colors
    cmsCIELab lab1 = {79.0, -20.0, -60.0};
    cmsCIELab lab2 = {50.0, -10.0, -30.0};
    cmsFloat64Number deltaE = cmsCIE94DeltaE(&lab1, &lab2);
    
    // Create an OkLab profile
    cmsHPROFILE okLabProfile = cmsCreate_OkLabProfile(ctx);
    
    // Get the D50 XYZ values
    const cmsCIEXYZ *d50xyz = cmsD50_XYZ();
    
    // Release all allocated resources
    cmsPipelineFree(lut);
    cmsIT8Free(it8Handle);
    cmsCloseProfile(okLabProfile);
    cmsDeleteContext(ctx);
    
    return 0;
}