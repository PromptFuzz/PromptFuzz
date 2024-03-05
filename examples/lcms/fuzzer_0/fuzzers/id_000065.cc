#include "FDSan.h"
#include <lcms2.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
//<ID> 7590
//<Prompt> ["cmsSetDeviceClass","cmsGetEncodedICCversion","cmsGetSupportedIntentsTHR","cmsSetLogErrorHandler","cmsxyY2XYZ","cmsNamedColorCount","cmsDictGetEntryList"]
/*<Combination>: [void cmsSetDeviceClass(cmsHPROFILE hProfile, cmsProfileClassSignature sig),
    cmsUInt32Number cmsGetEncodedICCversion(cmsHPROFILE hProfile),
    cmsUInt32Number cmsGetSupportedIntentsTHR(cmsContext ContextID, cmsUInt32Number nMax, cmsUInt32Number * Codes, char ** Descriptions),
    void cmsSetLogErrorHandler(cmsLogErrorHandlerFunction Fn),
    void cmsxyY2XYZ(cmsCIEXYZ * Dest, const cmsCIExyY * Source),
    cmsUInt32Number cmsNamedColorCount(const cmsNAMEDCOLORLIST * v),
    const cmsDICTentry *cmsDictGetEntryList(cmsHANDLE hDict)
*/
//<score> 0.85714287, nr_unique_branch: 1, p_cov: 1
//<Quality> {"diversity":14,"density":6,"covered_api":["cmsSetDeviceClass","cmsGetEncodedICCversion","cmsGetSupportedIntentsTHR","cmsSetLogErrorHandler","cmsxyY2XYZ","cmsNamedColorCount","cmsDictGetEntryList"],"uncovered_api":[],"unique_branches":{"cmsGetNamedColorList":[[769,9,769,23,0]]},"library_calls":["cmsCreate_sRGBProfile","cmsSetDeviceClass","cmsGetEncodedICCversion","cmsCreateContext","cmsGetSupportedIntentsTHR","cmsSetLogErrorHandler","cmsxyY2XYZ","cmsGetNamedColorList","cmsNamedColorCount","cmsDictAlloc","cmsDictGetEntryList","cmsCloseProfile","cmsDeleteContext","cmsDictFree"]}
/**/


extern "C" int LLVMFuzzerTestOneInput_65(const uint8_t *data, size_t size) {
	if(size<=0) return 0;

    // Create profile
    cmsHPROFILE hProfile = cmsCreate_sRGBProfile();

    // Set device class
    cmsSetDeviceClass(hProfile, cmsSigDisplayClass);

    // Get encoded ICC version
    cmsUInt32Number version = cmsGetEncodedICCversion(hProfile);

    // Get supported intents
    cmsContext contextID = cmsCreateContext(NULL, NULL);
    cmsUInt32Number maxIntents = 10;
    cmsUInt32Number* intents = (cmsUInt32Number*)malloc(maxIntents * sizeof(cmsUInt32Number));
    char** descriptions = (char**)malloc(maxIntents * sizeof(char*));
    cmsUInt32Number numIntents = cmsGetSupportedIntentsTHR(contextID, maxIntents, intents, descriptions);

    // Set log error handler
    cmsSetLogErrorHandler([](cmsContext context, cmsUInt32Number code, const char* text) {
        printf("Error code: %d\n", code);
        printf("Error message: %s\n", text);
    });

    // Convert xyY to XYZ
    cmsCIExyY xyY;
    cmsCIEXYZ XYZ;
    xyY.x = 0.3127;
    xyY.y = 0.3290;
    xyY.Y = 1.0;
    cmsxyY2XYZ(&XYZ, &xyY);

    // Named Color count
    cmsNAMEDCOLORLIST* namedColorList = cmsGetNamedColorList(hProfile);
    cmsUInt32Number colorCount = cmsNamedColorCount(namedColorList);

    // Dictionary entry list
    cmsHANDLE hDict = cmsDictAlloc(NULL);
    const cmsDICTentry* dictEntries = cmsDictGetEntryList(hDict);

    // Free resources
    cmsCloseProfile(hProfile);
    cmsDeleteContext(contextID);
    free(intents);
    free(descriptions);
    cmsDictFree(hDict);

    return 0;
}