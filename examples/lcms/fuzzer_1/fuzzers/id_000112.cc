#include "FDSan.h"
#include "FuzzedDataProvider.h"
#include <lcms2.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
//<ID> 15588
//<Prompt> ["cmsIT8SetPropertyUncooked","cmsDupProfileSequenceDescription","cmsBuildTabulatedToneCurveFloat","cmsDesaturateLab","cmsSetEncodedICCversion","cmsChannelsOf"]
/*<Combination>: [cmsBool cmsIT8SetPropertyUncooked(cmsHANDLE hIT8, const char * Key, const char * Buffer),
    cmsSEQ *cmsDupProfileSequenceDescription(const cmsSEQ * pseq),
    cmsToneCurve *cmsBuildTabulatedToneCurveFloat(cmsContext ContextID, cmsUInt32Number nEntries, const cmsFloat32Number * values),
    cmsBool cmsDesaturateLab(cmsCIELab * Lab, double amax, double amin, double bmax, double bmin),
    void cmsSetEncodedICCversion(cmsHPROFILE hProfile, cmsUInt32Number Version),
    cmsUInt32Number cmsChannelsOf(cmsColorSpaceSignature ColorSpace)
*/
//<score> 7, nr_unique_branch: 7, p_cov: 1
//<Quality> {"diversity":16,"density":14,"covered_api":["cmsIT8SetPropertyUncooked","cmsDupProfileSequenceDescription","cmsBuildTabulatedToneCurveFloat","cmsDesaturateLab","cmsSetEncodedICCversion","cmsChannelsOf"],"uncovered_api":[],"unique_branches":{"cmsDesaturateLab":[[534,28,534,41,0],[544,17,544,32,1],[558,18,558,25,0],[558,29,558,36,1],[559,18,559,26,1],[566,21,566,29,0],[566,33,566,41,0]]},"library_calls":["cmsCreateContext","cmsIT8Alloc","cmsIT8SetPropertyUncooked","cmsAllocProfileSequenceDescription","cmsDupProfileSequenceDescription","cmsBuildTabulatedToneCurveFloat","cmsDesaturateLab","cmsCreateProfilePlaceholder","cmsSetEncodedICCversion","cmsChannelsOf","cmsIT8Free","cmsFreeProfileSequenceDescription","cmsFreeProfileSequenceDescription","cmsFreeToneCurve","cmsCloseProfile","cmsDeleteContext"]}
/*Here is the C++ program that follows the given instructions:

*/


// Include the lcms headers
extern "C" {
}

// Define the LLVMFuzzerTestOneInput_112 function
extern "C" int LLVMFuzzerTestOneInput_112(const uint8_t* f_data, size_t f_size) {
	if(f_size<=8) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
	//fuzzer shim end}




    // Create a context
    cmsContext ctx = cmsCreateContext(NULL, NULL);

    // Create an IT8 handle
    cmsHANDLE it8Handle = cmsIT8Alloc(ctx);

    // Set an uncooked property for the IT8 handle
    cmsIT8SetPropertyUncooked(it8Handle, "key", "buffer");

    // Duplicate a profile sequence description
    cmsSEQ *profileSequence = cmsAllocProfileSequenceDescription(ctx, fuzz_uint32_t_1);
    cmsSEQ *duplicatedSequence = cmsDupProfileSequenceDescription(profileSequence);

    // Build a tabulated tone curve
    cmsUInt32Number numEntries = 10;
    cmsFloat32Number values[10] = {0.0f, 0.1f, 0.2f, 0.3f, 0.4f, 0.5f, 0.6f, 0.7f, 0.8f, 0.9f};
    cmsToneCurve *toneCurve = cmsBuildTabulatedToneCurveFloat(ctx, numEntries, values);

    // Create a Lab color
    cmsCIELab lab = {50.0, 1.0, 1.0};

    // Desaturate the Lab color
    cmsDesaturateLab(&lab, 0.9, 0.1, 0.9, 0.1);

    // Create a profile
    cmsHPROFILE profile = cmsCreateProfilePlaceholder(ctx);

    // Set the encoded ICC version
    cmsSetEncodedICCversion(profile, fuzz_uint32_t_2);

    // Get the number of channels for a color space
    cmsColorSpaceSignature colorSpace = cmsSigLabData;
    cmsUInt32Number numChannels = cmsChannelsOf(colorSpace);

    // Open an input file
    FILE *inFile = fmemopen((void *)data, size, "rb");

    // Read from the input file

    // Open an output file
    FILE *outFile = fopen("output_file", "wb");

    // Write to the output file

    // Close the input file
    assert_file_closed(&inFile);;

    // Close the output file
    assert_file_closed(&outFile);;

    // Free allocated resources
    cmsIT8Free(it8Handle);
    cmsFreeProfileSequenceDescription(profileSequence);
    cmsFreeProfileSequenceDescription(duplicatedSequence);
    cmsFreeToneCurve(toneCurve);
    cmsCloseProfile(profile);
    cmsDeleteContext(ctx);

    assert_file_name_closed("output_file");
	assert_file_closed(&outFile);
	assert_file_closed(&inFile);
	return 0;
}