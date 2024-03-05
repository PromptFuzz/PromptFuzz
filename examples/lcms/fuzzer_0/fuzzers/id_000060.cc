#include "FDSan.h"
#include <lcms2.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
//<ID> 7152
//<Prompt> ["cmsIT8LoadFromMem","cmsDetectBlackPoint","cmsPipelineStageCount","cmsTempFromWhitePoint","cmsWriteRawTag","cmsEvalToneCurve16"]
/*<Combination>: [cmsHANDLE cmsIT8LoadFromMem(cmsContext ContextID, const void * Ptr, cmsUInt32Number len),
    cmsBool cmsDetectBlackPoint(cmsCIEXYZ * BlackPoint, cmsHPROFILE hProfile, cmsUInt32Number Intent, cmsUInt32Number dwFlags),
    cmsUInt32Number cmsPipelineStageCount(const cmsPipeline * lut),
    cmsBool cmsTempFromWhitePoint(cmsFloat64Number * TempK, const cmsCIExyY * WhitePoint),
    cmsBool cmsWriteRawTag(cmsHPROFILE hProfile, cmsTagSignature sig, const void * data, cmsUInt32Number Size),
    cmsUInt16Number cmsEvalToneCurve16(const cmsToneCurve * Curve, cmsUInt16Number v)
*/
//<score> 0.625, nr_unique_branch: 0, p_cov: 1
//<Quality> {"diversity":16,"density":10,"covered_api":["cmsIT8LoadFromMem","cmsDetectBlackPoint","cmsPipelineStageCount","cmsTempFromWhitePoint","cmsWriteRawTag","cmsEvalToneCurve16"],"uncovered_api":[],"unique_branches":{},"library_calls":["cmsCreateContext","cmsIT8LoadFromMem","cmsCreateXYZProfile","cmsDetectBlackPoint","cmsD50_xyY","cmsTempFromWhitePoint","cmsWriteRawTag","cmsBuildGamma","cmsEvalToneCurve16","cmsPipelineAlloc","cmsPipelineStageCount","cmsIT8Free","cmsCloseProfile","cmsFreeToneCurve","cmsPipelineFree","cmsDeleteContext"]}
/*Here is the implementation of the fuzz driver using the lcms library APIs:

*/


extern "C" int LLVMFuzzerTestOneInput_60(const uint8_t *data, size_t size) {
	if(size<=0) return 0;

    // Create a lcms context
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    
    // Load the input data into an lcms handle
    cmsHANDLE it8Handle = cmsIT8LoadFromMem(ctx, data, (cmsUInt32Number)size);
    
    // Create an lcms profile
    cmsHPROFILE profile = cmsCreateXYZProfile();
    
    // Detect the black point of the profile
    cmsCIEXYZ blackPoint;
    cmsBool blackPointDetected = cmsDetectBlackPoint(&blackPoint, profile, 0, 0);
    
    // Get the temp from white point
    cmsFloat64Number tempK;
    cmsCIExyY whitePoint = *cmsD50_xyY();
    cmsBool tempFromWhitePoint = cmsTempFromWhitePoint(&tempK, &whitePoint);
    
    // Write raw tag to the profile
    cmsTagSignature tagSig = cmsSigLuminanceTag;
    cmsUInt16Number tagData = 255;
    cmsBool rawTagWritten = cmsWriteRawTag(profile, tagSig, &tagData, sizeof(tagData));
    
    // Evaluate a tone curve
    cmsToneCurve* toneCurve = cmsBuildGamma(ctx, 2.2);
    cmsUInt16Number evalResult = cmsEvalToneCurve16(toneCurve, 128);
    
    // Get the stage count of a pipeline
    cmsPipeline* pipeline = cmsPipelineAlloc(ctx, 1, 1);
    cmsUInt32Number stageCount = cmsPipelineStageCount(pipeline);
    
    // Free all allocated resources
    cmsIT8Free(it8Handle);
    cmsCloseProfile(profile);
    cmsFreeToneCurve(toneCurve);
    cmsPipelineFree(pipeline);
    cmsDeleteContext(ctx);
    
    return 0;
}