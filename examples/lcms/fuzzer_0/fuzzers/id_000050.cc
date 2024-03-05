#include "FDSan.h"
#include <lcms2.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
//<ID> 5799
//<Prompt> ["cmsMD5computeID","cmsCreateLab2ProfileTHR","cmsIT8LoadFromMem","cmsGetProfileContextID","cmsIT8SaveToFile","cmsCreate_sRGBProfileTHR"]
/*<Combination>: [cmsBool cmsMD5computeID(cmsHPROFILE hProfile),
    cmsHPROFILE cmsCreateLab2ProfileTHR(cmsContext ContextID, const cmsCIExyY * WhitePoint),
    cmsHANDLE cmsIT8LoadFromMem(cmsContext ContextID, const void * Ptr, cmsUInt32Number len),
    cmsContext cmsGetProfileContextID(cmsHPROFILE hProfile),
    cmsBool cmsIT8SaveToFile(cmsHANDLE hIT8, const char * cFileName),
    cmsHPROFILE cmsCreate_sRGBProfileTHR(cmsContext ContextID)
*/
//<score> 1, nr_unique_branch: 0, p_cov: 1
//<Quality> {"diversity":29,"density":29,"covered_api":["cmsMD5computeID","cmsCreateLab2ProfileTHR","cmsIT8LoadFromMem","cmsGetProfileContextID","cmsIT8SaveToFile","cmsCreate_sRGBProfileTHR"],"uncovered_api":[],"unique_branches":{},"library_calls":["cmsCreateContext","cmsOpenProfileFromMem","cmsMD5computeID","cmsCreateLab2ProfileTHR","cmsIT8LoadFromMem","cmsGetProfileContextID","cmsIT8SaveToFile","cmsCreate_sRGBProfileTHR","cmsIT8Free","cmsCloseProfile","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"]}
/*Here is the C++ program that achieves the event described above:

*/


extern "C" int LLVMFuzzerTestOneInput_50(const uint8_t *data, size_t size)
{
	if(size<=0) return 0;

    // Create a context
    cmsContext context = cmsCreateContext(NULL, NULL);

    // Load the profile from memory
    cmsHPROFILE hProfile = cmsOpenProfileFromMem(data, (cmsUInt32Number)size);

    if (hProfile == NULL)
    {
        // Profile loading failed, release resources and return
        cmsDeleteContext(context);
        return 0;
    }

    // Calculate the MD5 hash of the profile
    cmsBool result = cmsMD5computeID(hProfile);

    if (!result)
    {
        // MD5 hash computation failed, release resources and return
        cmsCloseProfile(hProfile);
        cmsDeleteContext(context);
        return 0;
    }

    // Create a Lab profile
    cmsCIExyY whitePoint = {0.3127, 0.329};
    cmsHPROFILE labProfile = cmsCreateLab2ProfileTHR(context, &whitePoint);

    if (labProfile == NULL)
    {
        // Lab profile creation failed, release resources and return
        cmsCloseProfile(hProfile);
        cmsDeleteContext(context);
        return 0;
    }

    // Load IT8 data from memory
    cmsHANDLE it8Handle = cmsIT8LoadFromMem(context, data, (cmsUInt32Number)size);

    if (it8Handle == NULL)
    {
        // IT8 data loading failed, release resources and return
        cmsCloseProfile(hProfile);
        cmsCloseProfile(labProfile);
        cmsDeleteContext(context);
        return 0;
    }

    // Get the context ID of the profile
    cmsContext profileContext = cmsGetProfileContextID(hProfile);

    // Save IT8 data to a file
    result = cmsIT8SaveToFile(it8Handle, "output_file");

    if (!result)
    {
        // IT8 data saving failed, release resources and return
        cmsIT8Free(it8Handle);
        cmsCloseProfile(hProfile);
        cmsCloseProfile(labProfile);
        cmsDeleteContext(context);
        assert_file_name_closed("output_file");
	return 0;
    }

    // Create an sRGB profile
    cmsHPROFILE sRGBProfile = cmsCreate_sRGBProfileTHR(context);

    if (sRGBProfile == NULL)
    {
        // sRGB profile creation failed, release resources and return
        cmsIT8Free(it8Handle);
        cmsCloseProfile(hProfile);
        cmsCloseProfile(labProfile);
        cmsDeleteContext(context);
        assert_file_name_closed("output_file");
	return 0;
    }

    // Release resources
    cmsIT8Free(it8Handle);
    cmsCloseProfile(hProfile);
    cmsCloseProfile(labProfile);
    cmsCloseProfile(sRGBProfile);
    cmsDeleteContext(context);

    assert_file_name_closed("output_file");
	return 0;
}