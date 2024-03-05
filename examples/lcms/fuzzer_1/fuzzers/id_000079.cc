#include "FDSan.h"
#include "FuzzedDataProvider.h"
#include <lcms2.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
//<ID> 9300
//<Prompt> ["cmsCreateMultiprofileTransformTHR","cmsIT8SetSheetType","cmsGetToneCurveParametricType","cmsIT8GetPropertyMulti","cmsstrcasecmp"]
/*<Combination>: [cmsHTRANSFORM cmsCreateMultiprofileTransformTHR(cmsContext ContextID, cmsHPROFILE * hProfiles, cmsUInt32Number nProfiles, cmsUInt32Number InputFormat, cmsUInt32Number OutputFormat, cmsUInt32Number Intent, cmsUInt32Number dwFlags),
    cmsBool cmsIT8SetSheetType(cmsHANDLE hIT8, const char * Type),
    cmsInt32Number cmsGetToneCurveParametricType(const cmsToneCurve * t),
    const char *cmsIT8GetPropertyMulti(cmsHANDLE hIT8, const char * Key, const char * SubKey),
    int cmsstrcasecmp(const char * s1, const char * s2)
*/
//<score> 1.875, nr_unique_branch: 1, p_cov: 1
//<Quality> {"diversity":16,"density":15,"covered_api":["cmsCreateMultiprofileTransformTHR","cmsIT8SetSheetType","cmsGetToneCurveParametricType","cmsIT8GetPropertyMulti","cmsstrcasecmp"],"uncovered_api":[],"unique_branches":{"cmsIT8GetPropertyMulti":[[1528,9,1528,72,1]]},"library_calls":["cmsCreateContext","cmsCreate_sRGBProfileTHR","cmsCreate_sRGBProfileTHR","cmsCreateMultiprofileTransformTHR","cmsIT8Alloc","cmsIT8SetSheetType","cmsBuildGamma","cmsGetToneCurveParametricType","cmsIT8GetPropertyMulti","cmsstrcasecmp","cmsDeleteTransform","cmsCloseProfile","cmsCloseProfile","cmsIT8Free","cmsFreeToneCurve","cmsDeleteContext"]}
/**/


extern "C" int LLVMFuzzerTestOneInput_79(const uint8_t* f_data, size_t f_size) {
	if(f_size<=51) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_5, fuzz_str_sz_5, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_6, fuzz_str_sz_6, fdp);
	//fuzzer shim end}




    // Create a context for lcms
    cmsContext context = cmsCreateContext(NULL, NULL);
    
    // Create an array of profiles
    cmsHPROFILE profiles[2];
    profiles[0] = cmsCreate_sRGBProfileTHR(context);
    profiles[1] = cmsCreate_sRGBProfileTHR(context);
    
    // Create a multiprofile transform
    cmsHTRANSFORM transform = cmsCreateMultiprofileTransformTHR(context, profiles, 2, TYPE_RGB_8, TYPE_RGB_8, INTENT_PERCEPTUAL, fuzz_uint32_t_1);
    
    // Set a sheet type for IT8
    cmsHANDLE it8Handle = cmsIT8Alloc(context);
    cmsIT8SetSheetType(it8Handle, fuzz_str_2);
    
    // Get the parametric type of a tone curve
    cmsToneCurve* toneCurve = cmsBuildGamma(context, 2.2);
    cmsInt32Number parametricType = cmsGetToneCurveParametricType(toneCurve);
    
    // Get a property from an IT8 handle
    const char* property = cmsIT8GetPropertyMulti(it8Handle, fuzz_str_3, fuzz_str_4);
    
    // Perform case-insensitive string comparison
    int result = cmsstrcasecmp(fuzz_str_5, fuzz_str_6);
    
    // Release all allocated resources
    cmsDeleteTransform(transform);
    cmsCloseProfile(profiles[0]);
    cmsCloseProfile(profiles[1]);
    cmsIT8Free(it8Handle);
    cmsFreeToneCurve(toneCurve);
    cmsDeleteContext(context);
    
    return 0;
}