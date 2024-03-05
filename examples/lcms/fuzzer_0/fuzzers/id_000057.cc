#include "FDSan.h"
#include <lcms2.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
//<ID> 6842
//<Prompt> ["cmsPipelineStageCount","cmsTempFromWhitePoint","cmsIT8EnumProperties","cmsCreate_OkLabProfile","cmsD50_XYZ"]
/*<Combination>: [cmsUInt32Number cmsPipelineStageCount(const cmsPipeline * lut),
    cmsBool cmsTempFromWhitePoint(cmsFloat64Number * TempK, const cmsCIExyY * WhitePoint),
    cmsUInt32Number cmsIT8EnumProperties(cmsHANDLE hIT8, char *** PropertyNames),
    cmsHPROFILE cmsCreate_OkLabProfile(cmsContext ctx),
    const cmsCIEXYZ *cmsD50_XYZ()
*/
//<score> 0.64, nr_unique_branch: 0, p_cov: 1
//<Quality> {"diversity":25,"density":16,"covered_api":["cmsPipelineStageCount","cmsTempFromWhitePoint","cmsIT8EnumProperties","cmsCreate_OkLabProfile","cmsD50_XYZ"],"uncovered_api":[],"unique_branches":{},"library_calls":["cmsCreateContext","cmsOpenProfileFromMem","cmsCreate_OkLabProfile","cmsPipelineAlloc","cmsStageAllocIdentity","cmsStageAllocToneCurves","cmsPipelineGetPtrToLastStage","cmsPipelineInsertStage","cmsPipelineInsertStage","cmsPipelineStageCount","cmsTempFromWhitePoint","cmsIT8Alloc","cmsIT8EnumProperties","cmsD50_XYZ","cmsIT8Free","cmsPipelineFree","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"]}
/**/


extern "C" int LLVMFuzzerTestOneInput_57(const uint8_t *data, size_t size) {
	if(size<=0) return 0;

  // Create a context
  cmsContext ctx = cmsCreateContext(NULL, NULL);

  // Open profile from memory
  cmsHPROFILE inputProfile = cmsOpenProfileFromMem(data, size);
  if (!inputProfile) {
    // Error handling
    cmsDeleteContext(ctx);
    return 0;
  }

  // Create an OkLab profile
  cmsHPROFILE okLabProfile = cmsCreate_OkLabProfile(ctx);
  if (!okLabProfile) {
    // Error handling
    cmsCloseProfile(inputProfile);
    cmsDeleteContext(ctx);
    return 0;
  }

  // Create a pipeline
  cmsPipeline *pipeline = cmsPipelineAlloc(ctx, 3, 3);
  if (!pipeline) {
    // Error handling
    cmsCloseProfile(inputProfile);
    cmsCloseProfile(okLabProfile);
    cmsDeleteContext(ctx);
    return 0;
  }

  // Append stages to the pipeline
  cmsStage *stage1 = cmsStageAllocIdentity(ctx, 3);
  cmsStage *stage2 = cmsStageAllocToneCurves(ctx, 3, NULL);
  cmsStage *stage3 = cmsPipelineGetPtrToLastStage(pipeline);

  cmsPipelineInsertStage(pipeline, cmsAT_BEGIN, stage1);
  cmsPipelineInsertStage(pipeline, cmsAT_END, stage2);

  cmsPipelineStageCount(pipeline);  // Call this API

  // Calculate temperature from white point
  cmsCIExyY whitePoint;
  cmsFloat64Number tempK;
  cmsTempFromWhitePoint(&tempK, &whitePoint);  // Call this API

  // Enumerate IT8 properties
  cmsHANDLE hIT8 = cmsIT8Alloc(NULL);
  char **propertyNames;
  cmsUInt32Number numProperties = cmsIT8EnumProperties(hIT8, &propertyNames);  // Call this API

  // Get D50 XYZ
  const cmsCIEXYZ *D50_XYZ = cmsD50_XYZ();  // Call this API

  // Cleanup
  cmsIT8Free(hIT8);
  cmsPipelineFree(pipeline);
  cmsCloseProfile(inputProfile);
  cmsCloseProfile(okLabProfile);
  cmsDeleteContext(ctx);

  return 0;
}