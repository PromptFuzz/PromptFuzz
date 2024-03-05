#include "FDSan.h"
#include <lcms2.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
//<ID> 8614
//<Prompt> ["cmsIT8SetPropertyStr","cmsGetTagCount","cmsFreeNamedColorList","cmsDetectRGBProfileGamma","cmsGetHeaderCreationDateTime","cmsGBDFree"]
/*<Combination>: [cmsBool cmsIT8SetPropertyStr(cmsHANDLE hIT8, const char * cProp, const char * Str),
    cmsInt32Number cmsGetTagCount(cmsHPROFILE hProfile),
    void cmsFreeNamedColorList(cmsNAMEDCOLORLIST * v),
    cmsFloat64Number cmsDetectRGBProfileGamma(cmsHPROFILE hProfile, cmsFloat64Number threshold),
    cmsBool cmsGetHeaderCreationDateTime(cmsHPROFILE hProfile, struct tm * Dest),
    void cmsGBDFree(cmsHANDLE hGBD)
*/
//<score> 0.41666666, nr_unique_branch: 0, p_cov: 1
//<Quality> {"diversity":12,"density":5,"covered_api":["cmsIT8SetPropertyStr","cmsGetTagCount","cmsFreeNamedColorList","cmsDetectRGBProfileGamma","cmsGetHeaderCreationDateTime","cmsGBDFree"],"uncovered_api":[],"unique_branches":{},"library_calls":["cmsIT8Alloc","cmsIT8SetPropertyStr","cmsCreate_sRGBProfile","cmsGetTagCount","cmsAllocNamedColorList","cmsDetectRGBProfileGamma","cmsGetHeaderCreationDateTime","cmsGBDAlloc","cmsGBDFree","cmsFreeNamedColorList","cmsCloseProfile","cmsIT8Free"]}
/**/


extern "C" int LLVMFuzzerTestOneInput_72(const uint8_t* data, size_t size) {
	if(size<=0) return 0;

    // Create a HANDLE for IT8
    cmsHANDLE hIT8 = cmsIT8Alloc(NULL);

    // Set a property with a string value
    cmsIT8SetPropertyStr(hIT8, "property", "value");

    // Create an HPROFILE
    cmsHPROFILE hProfile = cmsCreate_sRGBProfile();

    // Get the count of tags in the profile
    cmsInt32Number tagCount = cmsGetTagCount(hProfile);

    // Create a NAMEDCOLORLIST
    cmsNAMEDCOLORLIST* namedColorList = cmsAllocNamedColorList(NULL, 10, 3, "Prefix", "Suffix");

    // Detect the gamma of the RGB profile
    cmsFloat64Number threshold = 0.5;
    cmsFloat64Number gamma = cmsDetectRGBProfileGamma(hProfile, threshold);

    // Get the creation date and time of the profile
    struct tm creationDateTime;
    cmsGetHeaderCreationDateTime(hProfile, &creationDateTime);

    // Create a HANDLE for GBD
    cmsHANDLE hGBD = cmsGBDAlloc(NULL);

    // Free the GBD handle
    cmsGBDFree(hGBD);

    // Release the allocated resources
    cmsFreeNamedColorList(namedColorList);
    cmsCloseProfile(hProfile);
    cmsIT8Free(hIT8);

    return 0;
}