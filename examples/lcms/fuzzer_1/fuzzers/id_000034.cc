#include "FDSan.h"
#include "FuzzedDataProvider.h"
#include <lcms2.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
//<ID> 4187
//<Prompt> ["cmsIT8EnumDataFormat","cmsDupContext","cmsIT8EnumPropertyMulti","cmsSmoothToneCurve","cmsTagLinkedTo"]
/*<Combination>: [int cmsIT8EnumDataFormat(cmsHANDLE hIT8, char *** SampleNames),
    cmsContext cmsDupContext(cmsContext ContextID, void * NewUserData),
    cmsUInt32Number cmsIT8EnumPropertyMulti(cmsHANDLE hIT8, const char * cProp, const char *** SubpropertyNames),
    cmsBool cmsSmoothToneCurve(cmsToneCurve * Tab, cmsFloat64Number lambda),
    cmsTagSignature cmsTagLinkedTo(cmsHPROFILE hProfile, cmsTagSignature sig)
*/
//<score> 1.5714285, nr_unique_branch: 1, p_cov: 1
//<Quality> {"diversity":14,"density":11,"covered_api":["cmsIT8EnumDataFormat","cmsDupContext","cmsIT8EnumPropertyMulti","cmsSmoothToneCurve","cmsTagLinkedTo"],"uncovered_api":[],"unique_branches":{"cmsIT8EnumPropertyMulti":[[2615,8,2615,58,0]]},"library_calls":["cmsCreateContext","cmsIT8Alloc","cmsIT8EnumDataFormat","cmsDupContext","cmsIT8EnumPropertyMulti","cmsCreate_sRGBProfile","cmsTagLinkedTo","cmsBuildGamma","cmsSmoothToneCurve","cmsFreeToneCurve","cmsCloseProfile","cmsIT8Free","cmsDeleteContext","cmsDeleteContext"]}
/**/


// Function to complete
extern "C" int LLVMFuzzerTestOneInput_34(const uint8_t* f_data, size_t f_size) {
	if(f_size<=11) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
	//fuzzer shim end}




    // Create a file handle for reading the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    
    // Create a context for the lcms library
    cmsContext context = cmsCreateContext(NULL, NULL);
    
    // Create a handle for the IT8 object
    cmsHANDLE it8 = cmsIT8Alloc(context);
    
    // Enumerate the data format using the IT8 handle
    char **sampleNames;
    int numSampleNames = cmsIT8EnumDataFormat(it8, &sampleNames);
    for (int i = 0; i < numSampleNames; i++) {
        printf("Sample Name: %s\n", sampleNames[i]);
    }
    
    // Duplicate the context
    cmsContext newContext = cmsDupContext(context, NULL);
    
    // Enumerate the property multi using the IT8 handle
    const char **subpropertyNames;
    int numSubpropNames = cmsIT8EnumPropertyMulti(it8, fuzz_str_1, &subpropertyNames);
    for (int i = 0; i < numSubpropNames; i++) {
        printf("Subproperty Name: %s\n", subpropertyNames[i]);
    }
    
    // Create a profile handle
    cmsHPROFILE profile = cmsCreate_sRGBProfile();
    
    // Get the linked tag of the profile
    cmsTagSignature linkedTag = cmsTagLinkedTo(profile, cmsSigProfileDescriptionTag);
    printf("Linked Tag: 0x%X\n", linkedTag);
    
    // Create a tone curve
    cmsToneCurve *toneCurve = cmsBuildGamma(context, 2.2);
    
    // Smooth the tone curve
    cmsSmoothToneCurve(toneCurve, 1.0);
    
    // Release resources
    cmsFreeToneCurve(toneCurve);
    cmsCloseProfile(profile);
    cmsIT8Free(it8);
    cmsDeleteContext(context);
    cmsDeleteContext(newContext);
    assert_file_closed(&in_file);;
    
    assert_file_closed(&in_file);
	return 0;
}