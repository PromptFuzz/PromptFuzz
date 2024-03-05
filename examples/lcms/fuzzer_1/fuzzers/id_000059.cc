#include "FDSan.h"
#include "FuzzedDataProvider.h"
#include <lcms2.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
//<ID> 7355
//<Prompt> ["cmsOpenProfileFromMemTHR","cmsCreateProofingTransformTHR","cmsCreateInkLimitingDeviceLink","cmsSetHeaderModel","cmsCreateLab4Profile"]
/*<Combination>: [cmsHPROFILE cmsOpenProfileFromMemTHR(cmsContext ContextID, const void * MemPtr, cmsUInt32Number dwSize),
    cmsHTRANSFORM cmsCreateProofingTransformTHR(cmsContext ContextID, cmsHPROFILE Input, cmsUInt32Number InputFormat, cmsHPROFILE Output, cmsUInt32Number OutputFormat, cmsHPROFILE Proofing, cmsUInt32Number Intent, cmsUInt32Number ProofingIntent, cmsUInt32Number dwFlags),
    cmsHPROFILE cmsCreateInkLimitingDeviceLink(cmsColorSpaceSignature ColorSpace, cmsFloat64Number Limit),
    void cmsSetHeaderModel(cmsHPROFILE hProfile, cmsUInt32Number model),
    cmsHPROFILE cmsCreateLab4Profile(const cmsCIExyY * WhitePoint)
*/
//<score> 2.4, nr_unique_branch: 2, p_cov: 0.8
//<Quality> {"diversity":17,"density":17,"covered_api":["cmsOpenProfileFromMemTHR","cmsCreateProofingTransformTHR","cmsSetHeaderModel","cmsCreateLab4Profile"],"uncovered_api":["cmsCreateInkLimitingDeviceLink"],"unique_branches":{"FastEvaluateCurves8":[[1344,15,1344,33,0],[1344,15,1344,33,1]]},"library_calls":["cmsCreateContext","cmsOpenProfileFromMemTHR","cmsCreateLab4Profile","cmsD50_xyY","cmsSetHeaderModel","cmsCreateProofingTransformTHR","cmsDoTransform","cmsDeleteTransform","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"]}
/**/

extern "C" int LLVMFuzzerTestOneInput_59(const uint8_t* f_data, size_t f_size) {
	if(f_size<=8) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
	//fuzzer shim end}




    // Create a context
    cmsContext ctx = cmsCreateContext(NULL, NULL);

    // Open the profile from the input data
    cmsHPROFILE inputProfile = cmsOpenProfileFromMemTHR(ctx, data, size);
    if (inputProfile == NULL) {
        // Clean up and return
        cmsDeleteContext(ctx);
        return 0;
    }

    // Create the proofing profile
    cmsHPROFILE proofingProfile = cmsCreateLab4Profile(cmsD50_xyY());
    if (proofingProfile == NULL) {
        // Clean up and return
        cmsCloseProfile(inputProfile);
        cmsDeleteContext(ctx);
        return 0;
    }

    // Set the header model
    cmsSetHeaderModel(inputProfile, 1);

    // Create the proofing transform
    cmsHTRANSFORM transform = cmsCreateProofingTransformTHR(ctx, inputProfile, TYPE_BGR_8,
        inputProfile, TYPE_BGR_8, proofingProfile, fuzz_uint32_t_1,
        fuzz_uint32_t_2, 0);
    if (transform == NULL) {
        // Clean up and return
        cmsCloseProfile(inputProfile);
        cmsCloseProfile(proofingProfile);
        cmsDeleteContext(ctx);
        return 0;
    }

    // Convert color data using the transform
    cmsUInt8Number inputData[3] = {255, 0, 0};
    cmsUInt8Number outputData[3] = {0, 0, 0};
    cmsDoTransform(transform, inputData, outputData, 1);

    // Print the output color data
    std::cout << "Output color: " << (int)outputData[0] << ", " << (int)outputData[1] << ", " << (int)outputData[2] << std::endl;

    // Clean up
    cmsDeleteTransform(transform);
    cmsCloseProfile(inputProfile);
    cmsCloseProfile(proofingProfile);
    cmsDeleteContext(ctx);

    return 0;
}