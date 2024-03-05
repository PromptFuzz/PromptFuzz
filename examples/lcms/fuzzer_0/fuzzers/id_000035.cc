#include "FDSan.h"
#include <lcms2.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
//<ID> 4304
//<Prompt> ["cmsIT8EnumDataFormat","cmsDupContext","cmsIT8EnumPropertyMulti","cmsGetEncodedICCversion","cmsSmoothToneCurve","cmsTagLinkedTo"]
/*<Combination>: [int cmsIT8EnumDataFormat(cmsHANDLE hIT8, char *** SampleNames),
    cmsContext cmsDupContext(cmsContext ContextID, void * NewUserData),
    cmsUInt32Number cmsIT8EnumPropertyMulti(cmsHANDLE hIT8, const char * cProp, const char *** SubpropertyNames),
    cmsUInt32Number cmsGetEncodedICCversion(cmsHPROFILE hProfile),
    cmsBool cmsSmoothToneCurve(cmsToneCurve * Tab, cmsFloat64Number lambda),
    cmsTagSignature cmsTagLinkedTo(cmsHPROFILE hProfile, cmsTagSignature sig)
*/
//<score> 1.3333334, nr_unique_branch: 1, p_cov: 0.6666667
//<Quality> {"diversity":12,"density":12,"covered_api":["cmsIT8EnumDataFormat","cmsGetEncodedICCversion","cmsSmoothToneCurve","cmsTagLinkedTo"],"uncovered_api":["cmsDupContext","cmsIT8EnumPropertyMulti"],"unique_branches":{"cmsSmoothToneCurve":[[1215,9,1215,20,1]]},"library_calls":["cmsCreateContext","cmsOpenProfileFromMemTHR","cmsGetEncodedICCversion","cmsTagLinkedTo","cmsBuildTabulatedToneCurveFloat","cmsSmoothToneCurve","cmsIT8LoadFromFile","cmsIT8EnumDataFormat","cmsIT8Free","cmsFreeToneCurve","cmsCloseProfile","cmsDeleteContext"]}
/**/


extern "C" int LLVMFuzzerTestOneInput_35(const uint8_t* data, size_t size) {
	if(size<=0) return 0;

	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);

    // Create a context
    cmsContext ctx = cmsCreateContext(NULL, NULL);

    // Open the profile from memory
    cmsHPROFILE profile = cmsOpenProfileFromMemTHR(ctx, data, size);
    if (profile != NULL) {
        // Get the encoded ICC version
        cmsUInt32Number version = cmsGetEncodedICCversion(profile);

        // Check if the profile has a linked tag
        cmsTagSignature linkedTag = cmsTagLinkedTo(profile, cmsSigAToB0Tag);

        // Create a tone curve
        cmsToneCurve* curve = cmsBuildTabulatedToneCurveFloat(ctx, 256, NULL);

        // Smooth the tone curve
        cmsSmoothToneCurve(curve, 1.0);

        // Enumerate and print sample names from an IT8 handle
        cmsHANDLE it8Handle = cmsIT8LoadFromFile(ctx, "input_file");
        if (it8Handle != NULL) {
            char** sampleNames;
            int numSampleNames = cmsIT8EnumDataFormat(it8Handle, &sampleNames);
            for (int i = 0; i < numSampleNames; i++) {
                printf("Sample Name: %s\n", sampleNames[i]);
            }
        }

        // Cleanup
        cmsIT8Free(it8Handle);
        cmsFreeToneCurve(curve);
        cmsCloseProfile(profile);
    }

    // Delete the context
    cmsDeleteContext(ctx);

    return 0;
}