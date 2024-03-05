#include "FDSan.h"
#include <lcms2.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
//<ID> 7454
//<Prompt> ["cmsSetDeviceClass","cmsGetEncodedICCversion","cmsGetSupportedIntentsTHR","cmsxyY2XYZ","cmsDictGetEntryList","cmsCreateTransformTHR"]
/*<Combination>: [void cmsSetDeviceClass(cmsHPROFILE hProfile, cmsProfileClassSignature sig),
    cmsUInt32Number cmsGetEncodedICCversion(cmsHPROFILE hProfile),
    cmsUInt32Number cmsGetSupportedIntentsTHR(cmsContext ContextID, cmsUInt32Number nMax, cmsUInt32Number * Codes, char ** Descriptions),
    void cmsxyY2XYZ(cmsCIEXYZ * Dest, const cmsCIExyY * Source),
    const cmsDICTentry *cmsDictGetEntryList(cmsHANDLE hDict),
    cmsHTRANSFORM cmsCreateTransformTHR(cmsContext ContextID, cmsHPROFILE Input, cmsUInt32Number InputFormat, cmsHPROFILE Output, cmsUInt32Number OutputFormat, cmsUInt32Number Intent, cmsUInt32Number dwFlags)
*/
//<score> 2.2916667, nr_unique_branch: 2, p_cov: 0.8333333
//<Quality> {"diversity":12,"density":11,"covered_api":["cmsSetDeviceClass","cmsGetEncodedICCversion","cmsGetSupportedIntentsTHR","cmsxyY2XYZ","cmsCreateTransformTHR"],"uncovered_api":["cmsDictGetEntryList"],"unique_branches":{"_cmsICCcolorSpace":[[778,8,778,19,0]],"FixWhiteMisalignment":[[574,9,575,38,0]]},"library_calls":["cmsCreateContext","cmsCreate_sRGBProfileTHR","cmsCreateXYZProfileTHR","cmsSetDeviceClass","cmsGetEncodedICCversion","cmsGetSupportedIntentsTHR","cmsxyY2XYZ","cmsCreateTransformTHR","cmsDeleteTransform","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"]}
/*Here is the code that follows the instructions provided:

*/


extern "C" int LLVMFuzzerTestOneInput_64(const uint8_t *data, size_t size) {
	if(size<=0) return 0;

    // Initialize the context
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    
    // Create an input profile
    cmsHPROFILE inputProfile = cmsCreate_sRGBProfileTHR(ctx);
    
    // Create an output profile
    cmsHPROFILE outputProfile = cmsCreateXYZProfileTHR(ctx);
    
    // Set the device class of the input profile
    cmsSetDeviceClass(inputProfile, cmsSigDisplayClass);
    
    // Get the encoded ICC version of the input profile
    cmsUInt32Number encodedVersion = cmsGetEncodedICCversion(inputProfile);
    
    // Get the supported intents
    cmsUInt32Number supportedIntents[10];
    char* intentDescriptions[10];
    cmsUInt32Number numIntents = cmsGetSupportedIntentsTHR(ctx, 10, supportedIntents, intentDescriptions);
    
    // Convert xyY to XYZ
    cmsCIExyY xyY;
    cmsCIEXYZ XYZ;
    cmsxyY2XYZ(&XYZ, &xyY);
    
    // Create a transform
    cmsHTRANSFORM transform = cmsCreateTransformTHR(ctx, inputProfile, TYPE_RGB_8, outputProfile, TYPE_XYZ_16, INTENT_PERCEPTUAL, 0);
    
    // Release all allocated resources
    cmsDeleteTransform(transform);
    cmsCloseProfile(inputProfile);
    cmsCloseProfile(outputProfile);
    cmsDeleteContext(ctx);

    return 0;
}