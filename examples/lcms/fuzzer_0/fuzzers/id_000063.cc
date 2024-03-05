#include "FDSan.h"
#include <lcms2.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
//<ID> 7428
//<Prompt> ["cmsSetDeviceClass","cmsGetEncodedICCversion","cmsGetSupportedIntentsTHR","cmsxyY2XYZ","cmsCreateTransformTHR"]
/*<Combination>: [void cmsSetDeviceClass(cmsHPROFILE hProfile, cmsProfileClassSignature sig),
    cmsUInt32Number cmsGetEncodedICCversion(cmsHPROFILE hProfile),
    cmsUInt32Number cmsGetSupportedIntentsTHR(cmsContext ContextID, cmsUInt32Number nMax, cmsUInt32Number * Codes, char ** Descriptions),
    void cmsxyY2XYZ(cmsCIEXYZ * Dest, const cmsCIExyY * Source),
    cmsHTRANSFORM cmsCreateTransformTHR(cmsContext ContextID, cmsHPROFILE Input, cmsUInt32Number InputFormat, cmsHPROFILE Output, cmsUInt32Number OutputFormat, cmsUInt32Number Intent, cmsUInt32Number dwFlags)
*/
//<score> 0.9166667, nr_unique_branch: 0, p_cov: 1
//<Quality> {"diversity":12,"density":11,"covered_api":["cmsSetDeviceClass","cmsGetEncodedICCversion","cmsGetSupportedIntentsTHR","cmsxyY2XYZ","cmsCreateTransformTHR"],"uncovered_api":[],"unique_branches":{},"library_calls":["cmsCreateContext","cmsCreate_sRGBProfile","cmsCreateLab4Profile","cmsSetDeviceClass","cmsGetEncodedICCversion","cmsGetSupportedIntentsTHR","cmsxyY2XYZ","cmsCreateTransformTHR","cmsDeleteTransform","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"]}
/**/


extern "C" int LLVMFuzzerTestOneInput_63(const uint8_t *data, size_t size) {
	if(size<=0) return 0;

  // Initialize the lcms context
  cmsContext context = cmsCreateContext(NULL, NULL);
  
  // Create input and output profiles
  cmsHPROFILE inputProfile = cmsCreate_sRGBProfile();
  cmsHPROFILE outputProfile = cmsCreateLab4Profile(NULL);

  // Set the device class of the input profile to display
  cmsSetDeviceClass(inputProfile, cmsSigDisplayClass);
  
  // Get the encoded ICC version of the input profile
  cmsUInt32Number encodedICCversion = cmsGetEncodedICCversion(inputProfile);
  
  // Get the list of supported intents
  cmsUInt32Number maxIntents = 10;
  cmsUInt32Number supportedIntents[maxIntents];
  char *intentDescriptions[maxIntents];
  cmsUInt32Number numSupportedIntents = cmsGetSupportedIntentsTHR(context, maxIntents, supportedIntents, intentDescriptions);
  
  // Convert xyY color to XYZ color
  cmsCIExyY xyYColor = {0.3, 0.4, 1.0};
  cmsCIEXYZ xyzColor;
  cmsxyY2XYZ(&xyzColor, &xyYColor);
  
  // Create a transform from input profile to output profile
  cmsHTRANSFORM transform = cmsCreateTransformTHR(context, inputProfile, TYPE_RGB_8, outputProfile, TYPE_Lab_16, INTENT_PERCEPTUAL, 0);

  // Cleanup
  cmsDeleteTransform(transform);
  cmsCloseProfile(inputProfile);
  cmsCloseProfile(outputProfile);
  cmsDeleteContext(context);

  return 0;
}