#include "FDSan.h"
#include <lcms2.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
//<ID> 11110
//<Prompt> ["cmsGetContextUserData","cmsSetHeaderModel","cmsIT8LoadFromMem","cmsCreateTransform","cmsJoinToneCurve"]
/*<Combination>: [void *cmsGetContextUserData(cmsContext ContextID),
    void cmsSetHeaderModel(cmsHPROFILE hProfile, cmsUInt32Number model),
    cmsHANDLE cmsIT8LoadFromMem(cmsContext ContextID, const void * Ptr, cmsUInt32Number len),
    cmsHTRANSFORM cmsCreateTransform(cmsHPROFILE Input, cmsUInt32Number InputFormat, cmsHPROFILE Output, cmsUInt32Number OutputFormat, cmsUInt32Number Intent, cmsUInt32Number dwFlags),
    cmsToneCurve *cmsJoinToneCurve(cmsContext ContextID, const cmsToneCurve * X, const cmsToneCurve * Y, cmsUInt32Number nPoints)
*/
//<score> 0.6875, nr_unique_branch: 0, p_cov: 1
//<Quality> {"diversity":16,"density":11,"covered_api":["cmsGetContextUserData","cmsSetHeaderModel","cmsIT8LoadFromMem","cmsCreateTransform","cmsJoinToneCurve"],"uncovered_api":[],"unique_branches":{},"library_calls":["cmsCreateContext","cmsCreateLab2Profile","cmsSetHeaderModel","cmsIT8LoadFromMem","cmsCreateTransform","cmsBuildGamma","cmsBuildGamma","cmsJoinToneCurve","cmsGetContextUserData","cmsCloseProfile","cmsIT8Free","cmsDeleteTransform","cmsFreeToneCurve","cmsFreeToneCurve","cmsFreeToneCurve","cmsDeleteContext"]}
/**/


extern "C" int LLVMFuzzerTestOneInput_94(const uint8_t *data, size_t size) {
	if(size<=0) return 0;

  // Step 1: Initialize the lcms context
  cmsContext context = cmsCreateContext(NULL, NULL);
  
  // Step 2: Set a model for the profile
  cmsHPROFILE profile = cmsCreateLab2Profile(NULL);
  cmsSetHeaderModel(profile, 1234);
  
  // Step 3: Load IT8 data from the input stream
  cmsHANDLE it8 = cmsIT8LoadFromMem(context, data, size);
  
  // Step 4: Create a transform from the input profile to the output profile
  cmsHTRANSFORM transform = cmsCreateTransform(profile, TYPE_Lab_DBL, profile, TYPE_Lab_DBL, INTENT_PERCEPTUAL, 0);
  
  // Step 5: Join two tone curves
  cmsToneCurve* curve1 = cmsBuildGamma(context, 2.2);
  cmsToneCurve* curve2 = cmsBuildGamma(context, 1.8);
  cmsToneCurve* joinedCurve = cmsJoinToneCurve(context, curve1, curve2, 256);
  
  // Step 6: Get the user data from the context
  void* userData = cmsGetContextUserData(context);
  
  // Step 7: Cleanup
  cmsCloseProfile(profile);
  cmsIT8Free(it8);
  cmsDeleteTransform(transform);
  cmsFreeToneCurve(curve1);
  cmsFreeToneCurve(curve2);
  cmsFreeToneCurve(joinedCurve);
  cmsDeleteContext(context);
  
  return 0;
}