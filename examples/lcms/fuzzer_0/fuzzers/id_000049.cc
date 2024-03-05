#include "FDSan.h"
#include <lcms2.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
//<ID> 5796
//<Prompt> ["cmsMD5computeID","cmsCreateLab2ProfileTHR","cmsIT8LoadFromMem","cmsGetProfileContextID","cmsIT8SaveToFile","cmsCreate_sRGBProfileTHR"]
/*<Combination>: [cmsBool cmsMD5computeID(cmsHPROFILE hProfile),
    cmsHPROFILE cmsCreateLab2ProfileTHR(cmsContext ContextID, const cmsCIExyY * WhitePoint),
    cmsHANDLE cmsIT8LoadFromMem(cmsContext ContextID, const void * Ptr, cmsUInt32Number len),
    cmsContext cmsGetProfileContextID(cmsHPROFILE hProfile),
    cmsBool cmsIT8SaveToFile(cmsHANDLE hIT8, const char * cFileName),
    cmsHPROFILE cmsCreate_sRGBProfileTHR(cmsContext ContextID)
*/
//<score> 9, nr_unique_branch: 8, p_cov: 1
//<Quality> {"diversity":11,"density":11,"covered_api":["cmsMD5computeID","cmsCreateLab2ProfileTHR","cmsIT8LoadFromMem","cmsGetProfileContextID","cmsIT8SaveToFile","cmsCreate_sRGBProfileTHR"],"uncovered_api":[],"unique_branches":{"Type_Chromaticity_Write":[[468,9,468,38,1],[469,9,469,38,1],[471,9,471,65,1],[472,9,472,67,1],[473,9,473,66,1]],"cmsMD5finish":[[228,9,228,18,1]],"SaveOneChromaticity":[[457,9,457,79,1],[458,9,458,79,1]]},"library_calls":["cmsCreateContext","cmsCreate_sRGBProfileTHR","cmsCreateLab2ProfileTHR","cmsIT8LoadFromMem","cmsIT8SaveToFile","cmsIT8Free","cmsGetProfileContextID","cmsMD5computeID","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"]}
/**/


extern "C" int LLVMFuzzerTestOneInput_49(const uint8_t *data, size_t size) {
	if(size<=0) return 0;

  // Create a context
  cmsContext ctx = cmsCreateContext(nullptr, nullptr);

  // Create an sRGB profile
  cmsHPROFILE sRGBProfile = cmsCreate_sRGBProfileTHR(ctx);

  // Create a Lab profile with a white point
  cmsCIExyY whitePoint = {0.3127, 0.3290, 1.0};
  cmsHPROFILE labProfile = cmsCreateLab2ProfileTHR(ctx, &whitePoint);

  // Create an IT8 handle
  cmsHANDLE it8Handle = cmsIT8LoadFromMem(ctx, data, static_cast<cmsUInt32Number>(size));

  if (it8Handle != nullptr) {
    // Save IT8 data to a file
    const char* fileName = "output_file.it8";
    cmsIT8SaveToFile(it8Handle, fileName);

    // Close IT8 handle
    cmsIT8Free(it8Handle);
  }

  // Get the profile context ID
  cmsContext profileCtx = cmsGetProfileContextID(sRGBProfile);

  // Compute MD5 ID of a profile
  cmsBool success = cmsMD5computeID(sRGBProfile);

  // Release resources
  cmsCloseProfile(sRGBProfile);
  cmsCloseProfile(labProfile);
  cmsDeleteContext(ctx);

  return 0;
}