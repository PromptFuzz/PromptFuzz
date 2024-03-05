#include "FDSan.h"
#include "FuzzedDataProvider.h"
#include <lcms2.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
//<ID> 6232
//<Prompt> ["cmsChannelsOfColorSpace","cmsDictNextEntry","cmsNamedColorIndex","cmsGetTransformOutputFormat","cmsDeltaE"]
/*<Combination>: [cmsInt32Number cmsChannelsOfColorSpace(cmsColorSpaceSignature ColorSpace),
    const cmsDICTentry *cmsDictNextEntry(const cmsDICTentry * e),
    cmsInt32Number cmsNamedColorIndex(const cmsNAMEDCOLORLIST * v, const char * Name),
    cmsUInt32Number cmsGetTransformOutputFormat(cmsHTRANSFORM hTransform),
    cmsFloat64Number cmsDeltaE(const cmsCIELab * Lab1, const cmsCIELab * Lab2)
*/
//<score> 2.4545455, nr_unique_branch: 2, p_cov: 1
//<Quality> {"diversity":11,"density":9,"covered_api":["cmsChannelsOfColorSpace","cmsDictNextEntry","cmsNamedColorIndex","cmsGetTransformOutputFormat","cmsDeltaE"],"uncovered_api":[],"unique_branches":{"cmsNamedColorIndex":[[683,9,683,31,0]],"cmsDictNextEntry":[[995,10,995,19,0]]},"library_calls":["cmsCreate_sRGBProfileTHR","cmsCreateTransformTHR","cmsChannelsOfColorSpace","cmsGetColorSpace","cmsDictNextEntry","cmsGetNamedColorList","cmsNamedColorIndex","cmsGetTransformOutputFormat","cmsDeltaE","cmsDeleteTransform","cmsCloseProfile"]}
/**/

// Include the necessary headers

extern "C" int LLVMFuzzerTestOneInput_53(const uint8_t* f_data, size_t f_size) {
	if(f_size<=6) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
	//fuzzer shim end}




    // Create a color profile
    cmsHPROFILE profile = cmsCreate_sRGBProfileTHR(nullptr);
    
    // Create a handle for the transform
    cmsHTRANSFORM transform = cmsCreateTransformTHR(nullptr, profile, TYPE_RGB_DBL, profile, TYPE_RGB_DBL, INTENT_PERCEPTUAL, 0);
    
    // Get the number of channels for the color space
    cmsInt32Number numChannels = cmsChannelsOfColorSpace(cmsGetColorSpace(profile));
    
    // Iterate through the dictionary entries
    const cmsDICTentry *entry = nullptr;
    while ((entry = cmsDictNextEntry(entry)) != nullptr) {
        // Do something with the dictionary entry
    }
    
    // Get the named color index
    cmsNAMEDCOLORLIST *namedColorList = cmsGetNamedColorList(transform);
    cmsInt32Number namedColorIndex = cmsNamedColorIndex(namedColorList, fuzz_str_1);
    
    // Get the transform output format
    cmsUInt32Number outputFormat = cmsGetTransformOutputFormat(transform);
    
    // Create two CIELab colors
    cmsCIELab lab1 = { 50, 0, 0 };
    cmsCIELab lab2 = { 70, 10, -20 };
    
    // Calculate the Delta E between the two colors
    cmsFloat64Number deltaE = cmsDeltaE(&lab1, &lab2);
    
    // Clean up and release resources
    cmsDeleteTransform(transform);
    cmsCloseProfile(profile);
    
    return 0;
}