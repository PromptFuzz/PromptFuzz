#include "FDSan.h"
#include "FuzzedDataProvider.h"
#include <lcms2.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
//<ID> 8912
//<Prompt> ["cmsCreateLab4ProfileTHR","cmsFloat2LabEncodedV2","cmsIT8SetTableByLabel","cmsCMCdeltaE","_cmsICCcolorSpace"]
/*<Combination>: [cmsHPROFILE cmsCreateLab4ProfileTHR(cmsContext ContextID, const cmsCIExyY * WhitePoint),
    void cmsFloat2LabEncodedV2(cmsUInt16Number wLab[3], const cmsCIELab * Lab),
    int cmsIT8SetTableByLabel(cmsHANDLE hIT8, const char * cSet, const char * cField, const char * ExpectedType),
    cmsFloat64Number cmsCMCdeltaE(const cmsCIELab * Lab1, const cmsCIELab * Lab2, cmsFloat64Number l, cmsFloat64Number c),
    cmsColorSpaceSignature _cmsICCcolorSpace(int OurNotation)
*/
//<score> 2.8, nr_unique_branch: 3, p_cov: 1
//<Quality> {"diversity":10,"density":7,"covered_api":["cmsCreateLab4ProfileTHR","cmsFloat2LabEncodedV2","cmsIT8SetTableByLabel","cmsCMCdeltaE","_cmsICCcolorSpace"],"uncovered_api":[],"unique_branches":{"cmsCMCdeltaE":[[553,24,553,37,1],[564,7,564,32,0],[577,8,577,18,0]]},"library_calls":["cmsCreateContext","cmsCreateLab4ProfileTHR","cmsFloat2LabEncodedV2","cmsIT8Alloc","cmsIT8SetTableByLabel","cmsCMCdeltaE","_cmsICCcolorSpace","cmsCloseProfile","cmsIT8Free","cmsDeleteContext"]}
/**/


// Include lcms headers
extern "C" {
}

// Define the maximum number of API calls
#define MAX_API_CALLS 10

extern "C" int LLVMFuzzerTestOneInput_72(const uint8_t* f_data, size_t f_size) {
	if(f_size<=0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Create a context
    cmsContext context = cmsCreateContext(NULL, NULL);
    
    // Create a Lab profile
    cmsCIExyY whitePoint = {0.3457, 0.3585, 1.0000};
    cmsHPROFILE labProfile = cmsCreateLab4ProfileTHR(context, &whitePoint);
    
    // Convert Lab values to encoded values
    cmsCIELab lab = {50.0, 0.0, 0.0};
    cmsUInt16Number encodedLab[3];
    cmsFloat2LabEncodedV2(encodedLab, &lab);
    
    // Set a table by label
    cmsHANDLE it8 = cmsIT8Alloc(context);
    const char *set = "set";
    const char *field = "field";
    const char *expectedType = "type";
    int result = cmsIT8SetTableByLabel(it8, set, field, expectedType);
    
    // Calculate CMC delta E
    cmsCIELab lab1 = {0.0, 0.0, 0.0};
    cmsCIELab lab2 = {100.0, 50.0, -50.0};
    cmsFloat64Number l = 2.0;
    cmsFloat64Number c = 1.0;
    cmsFloat64Number deltaE = cmsCMCdeltaE(&lab1, &lab2, l, c);
    
    // Get ICC color space
    int ourNotation = 12345;
    cmsColorSpaceSignature colorSpace = _cmsICCcolorSpace(ourNotation);
    
    // Release resources
    cmsCloseProfile(labProfile);
    cmsIT8Free(it8);
    cmsDeleteContext(context);
    
    return 0;
}