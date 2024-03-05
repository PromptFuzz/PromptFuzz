#include "FDSan.h"
#include "FuzzedDataProvider.h"
#include <lcms2.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
//<ID> 10156
//<Prompt> ["cmsGetProfileIOhandler","cmsOpenProfileFromMem","cmsEvalToneCurveFloat","cmsCreateProofingTransform","cmsChannelsOf"]
/*<Combination>: [cmsIOHANDLER *cmsGetProfileIOhandler(cmsHPROFILE hProfile),
    cmsHPROFILE cmsOpenProfileFromMem(const void * MemPtr, cmsUInt32Number dwSize),
    cmsFloat32Number cmsEvalToneCurveFloat(const cmsToneCurve * Curve, cmsFloat32Number v),
    cmsHTRANSFORM cmsCreateProofingTransform(cmsHPROFILE Input, cmsUInt32Number InputFormat, cmsHPROFILE Output, cmsUInt32Number OutputFormat, cmsHPROFILE Proofing, cmsUInt32Number Intent, cmsUInt32Number ProofingIntent, cmsUInt32Number dwFlags),
    cmsUInt32Number cmsChannelsOf(cmsColorSpaceSignature ColorSpace)
*/
//<score> 1.6, nr_unique_branch: 1, p_cov: 0.8
//<Quality> {"diversity":12,"density":12,"covered_api":["cmsGetProfileIOhandler","cmsOpenProfileFromMem","cmsCreateProofingTransform","cmsChannelsOf"],"uncovered_api":["cmsEvalToneCurveFloat"],"unique_branches":{"cmsChannelsOfColorSpace":[[933,5,933,27,0]]},"library_calls":["cmsOpenProfileFromMem","cmsGetProfileIOhandler","cmsChannelsOf","cmsGetColorSpace","cmsCreateProofingTransform","cmsCloseProfile","cmsDeleteTransform"]}
/*Here is an example of a C++ program that uses the lcms library APIs to achieve the event described above:

*/


extern "C" int LLVMFuzzerTestOneInput_86(const uint8_t* f_data, size_t f_size) {
	if(f_size<=8) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
	//fuzzer shim end}




    // Open the profile from memory
    cmsHPROFILE hProfile = cmsOpenProfileFromMem(data, size);
    if (!hProfile) {
        return 0;
    }

    // Get the IO handler for the profile
    cmsIOHANDLER *ioHandler = cmsGetProfileIOhandler(hProfile);

    // Get the number of channels for the color space
    cmsUInt32Number colorSpaceChannels = cmsChannelsOf(cmsGetColorSpace(hProfile));

    // Create a proofing transform
    cmsHTRANSFORM hTransform = cmsCreateProofingTransform(hProfile, TYPE_RGB_8, hProfile, TYPE_RGB_8,
                                                         hProfile, INTENT_PERCEPTUAL, fuzz_uint32_t_1, fuzz_uint32_t_2);
    if (!hTransform) {
        cmsCloseProfile(hProfile);
        return 0;
    }

    // Read and print the profile data
    FILE *profileFile = fmemopen((void *)data, size, "rb");
    if (!profileFile) {
        cmsCloseProfile(hProfile);
        cmsDeleteTransform(hTransform);
        assert_file_closed(&profileFile);
	return 0;
    }

    char profileData[1024];
    fread(profileData, 1, sizeof(profileData), profileFile);
    printf("Profile Data: %s\n", profileData);

    assert_file_closed(&profileFile);;

    // Write the transformed data to an output file
    FILE *outputFile = fopen("output_file", "wb");
    if (!outputFile) {
        cmsCloseProfile(hProfile);
        cmsDeleteTransform(hTransform);
        assert_file_name_closed("output_file");
	assert_file_closed(&outputFile);
	assert_file_closed(&profileFile);
	return 0;
    }

    // Write the transformed data to the output file
    const char *outputData = "Transformed Data";
    fwrite(outputData, 1, strlen(outputData), outputFile);

    assert_file_closed(&outputFile);;

    // Release resources
    cmsCloseProfile(hProfile);
    cmsDeleteTransform(hTransform);

    assert_file_name_closed("output_file");
	assert_file_closed(&outputFile);
	assert_file_closed(&profileFile);
	return 0;
}