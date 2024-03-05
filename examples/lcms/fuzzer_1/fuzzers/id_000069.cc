#include "FDSan.h"
#include "FuzzedDataProvider.h"
#include <lcms2.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
//<ID> 8552
//<Prompt> ["cmsIT8SetPropertyStr","cmsGetTagCount","cmsFreeNamedColorList","cmsDetectRGBProfileGamma","cmsGBDFree"]
/*<Combination>: [cmsBool cmsIT8SetPropertyStr(cmsHANDLE hIT8, const char * cProp, const char * Str),
    cmsInt32Number cmsGetTagCount(cmsHPROFILE hProfile),
    void cmsFreeNamedColorList(cmsNAMEDCOLORLIST * v),
    cmsFloat64Number cmsDetectRGBProfileGamma(cmsHPROFILE hProfile, cmsFloat64Number threshold),
    void cmsGBDFree(cmsHANDLE hGBD)
*/
//<score> 0.72727275, nr_unique_branch: 1, p_cov: 1
//<Quality> {"diversity":11,"density":4,"covered_api":["cmsIT8SetPropertyStr","cmsGetTagCount","cmsFreeNamedColorList","cmsDetectRGBProfileGamma","cmsGBDFree"],"uncovered_api":[],"unique_branches":{"cmsGetTagCount":[[579,9,579,20,1]]},"library_calls":["cmsIT8Alloc","cmsCreateNULLProfile","cmsIT8SetPropertyStr","cmsGetTagCount","cmsAllocNamedColorList","cmsDetectRGBProfileGamma","cmsGBDAlloc","cmsFreeNamedColorList","cmsGBDFree","cmsIT8Free","cmsCloseProfile"]}
/**/


extern "C" int LLVMFuzzerTestOneInput_69(const uint8_t* f_data, size_t f_size) {
	if(f_size<=37) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
	//fuzzer shim end}




    // Create a handle for IT8
    cmsHANDLE hIT8 = cmsIT8Alloc(NULL);

    // Create a profile handle
    cmsHPROFILE hProfile = cmsCreateNULLProfile();

    // Set property string in IT8 handle
    cmsIT8SetPropertyStr(hIT8, fuzz_str_1, fuzz_str_2);

    // Get the number of tags in the profile
    cmsInt32Number tagCount = cmsGetTagCount(hProfile);

    // Create a named color list
    cmsNAMEDCOLORLIST *namedColorList = cmsAllocNamedColorList(NULL, 10, 3, fuzz_str_3, fuzz_str_4);

    // Detect RGB profile gamma
    cmsFloat64Number gamma = cmsDetectRGBProfileGamma(hProfile, 0.5);

    // Create a GBD handle
    cmsHANDLE hGBD = cmsGBDAlloc(NULL);

    // Free the named color list
    cmsFreeNamedColorList(namedColorList);

    // Free the GBD handle
    cmsGBDFree(hGBD);

    // Release the IT8 handle
    cmsIT8Free(hIT8);

    // Release the profile handle
    cmsCloseProfile(hProfile);

    return 0;
}