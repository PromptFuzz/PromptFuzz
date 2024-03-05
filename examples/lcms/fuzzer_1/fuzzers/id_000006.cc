#include "FDSan.h"
#include "FuzzedDataProvider.h"
#include <lcms2.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
//<ID> 1041
//<Prompt> ["cmsGetProfileVersion","cmsGetColorSpace","cmsTagLinkedTo","cmsPipelineInputChannels","cmsCreateProofingTransformTHR","cmsstrcasecmp","cmsGetPostScriptCSA"]
/*<Combination>: [cmsFloat64Number cmsGetProfileVersion(cmsHPROFILE hProfile),
    cmsColorSpaceSignature cmsGetColorSpace(cmsHPROFILE hProfile),
    cmsTagSignature cmsTagLinkedTo(cmsHPROFILE hProfile, cmsTagSignature sig),
    cmsUInt32Number cmsPipelineInputChannels(const cmsPipeline * lut),
    cmsHTRANSFORM cmsCreateProofingTransformTHR(cmsContext ContextID, cmsHPROFILE Input, cmsUInt32Number InputFormat, cmsHPROFILE Output, cmsUInt32Number OutputFormat, cmsHPROFILE Proofing, cmsUInt32Number Intent, cmsUInt32Number ProofingIntent, cmsUInt32Number dwFlags),
    int cmsstrcasecmp(const char * s1, const char * s2),
    cmsUInt32Number cmsGetPostScriptCSA(cmsContext ContextID, cmsHPROFILE hProfile, cmsUInt32Number Intent, cmsUInt32Number dwFlags, void * Buffer, cmsUInt32Number dwBufferLen)
*/
//<score> 44.8, nr_unique_branch: 47, p_cov: 1
//<Quality> {"diversity":15,"density":14,"covered_api":["cmsGetProfileVersion","cmsGetColorSpace","cmsTagLinkedTo","cmsPipelineInputChannels","cmsCreateProofingTransformTHR","cmsstrcasecmp","cmsGetPostScriptCSA"],"uncovered_api":[],"unique_branches":{"WriteInputMatrixShaper":[[983,9,983,37,1],[990,13,990,40,0],[997,25,997,30,0],[997,25,997,30,1],[998,29,998,34,0],[998,29,998,34,1]],"EmitSafeGuardEnd":[[445,9,445,18,0],[445,9,445,18,1]],"cmsGetPostScriptColorResource":[[1555,9,1555,32,0],[1560,9,1560,32,1]],"cmsGetPostScriptCSA":[[1607,9,1607,23,1],[1612,9,1612,13,1]],"EmitNGamma":[[550,15,550,20,0],[550,15,550,20,1],[552,13,552,25,1],[554,13,554,18,0],[554,13,554,18,1],[554,22,554,104,0],[554,22,554,104,1]],"Emit1Gamma":[[460,9,460,22,1],[462,9,462,30,1],[465,9,465,36,0],[465,9,465,36,1],[472,10,472,19,0],[472,10,472,19,1],[480,15,480,34,0],[480,15,480,34,1],[481,9,481,20,0],[481,9,481,20,1]],"GammaTableEquals":[[537,9,537,19,0],[537,9,537,19,1]],"GenerateCSA":[[1081,9,1081,61,1],[1092,13,1092,40,1],[1102,13,1102,24,1],[1106,13,1106,118,0],[1108,17,1108,71,1],[1122,9,1122,20,0]],"BlackPointAsDarkerColorant":[[128,9,128,19,0]],"cmsDetectBlackPoint":[[205,9,205,36,0],[206,9,206,47,1],[214,9,214,57,1],[215,10,215,37,1],[215,41,215,68,1],[267,9,267,47,0],[268,9,268,59,1]],"EmitCIEBasedABC":[[754,15,754,20,0],[754,15,754,20,1]]},"library_calls":["cmsOpenProfileFromMem","cmsGetProfileVersion","cmsGetColorSpace","cmsTagLinkedTo","cmsCreateProofingTransformTHR","cmsPipelineInputChannels","cmsstrcasecmp","cmsCreateContext","cmsGetPostScriptCSA","cmsCloseProfile","cmsDeleteTransform","cmsDeleteContext"]}
/**/

extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_6(const uint8_t* f_data, size_t f_size) {
	if(f_size<=16) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_4, fdp);
	//fuzzer shim end}




    // Create a memory buffer from the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Retrieve the size of the input file
    fseek(in_file, 0, SEEK_END);
    long in_file_size = ftell(in_file);
    rewind(in_file);

    // Read the input data into a buffer
    uint8_t *in_buffer = new uint8_t[in_file_size];
    fread(in_buffer, 1, in_file_size, in_file);

    // Create a profile from the input data
    cmsHPROFILE hProfile = cmsOpenProfileFromMem(in_buffer, in_file_size);
    delete[] in_buffer;

    if (!hProfile) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // Get the profile version
    cmsFloat64Number profile_version = cmsGetProfileVersion(hProfile);
    std::cout << "Profile Version: " << profile_version << std::endl;

    // Get the color space of the profile
    cmsColorSpaceSignature color_space = cmsGetColorSpace(hProfile);
    std::cout << "Color Space: " << color_space << std::endl;

    // Get the linked tag
    cmsTagSignature linked_tag = cmsTagLinkedTo(hProfile, cmsSigAToB0Tag);
    std::cout << "Linked Tag: " << linked_tag << std::endl;

    // Create a proofing transform
    cmsHTRANSFORM hTransform = cmsCreateProofingTransformTHR(NULL, hProfile, TYPE_RGB_8, hProfile, TYPE_RGB_8, NULL, fuzz_uint32_t_1, fuzz_uint32_t_2, 0);
    if (!hTransform) {
        assert_file_closed(&in_file);;
        cmsCloseProfile(hProfile);
        assert_file_closed(&in_file);
	return 0;
    }

    // Get the number of input channels of the transform
    cmsUInt32Number input_channels = cmsPipelineInputChannels((cmsPipeline *)hTransform);
    std::cout << "Input Channels: " << input_channels << std::endl;

    // Perform some string comparison
    const char *s1 = "abc";
    const char *s2 = "abc";
    int result = cmsstrcasecmp(s1, s2);
    std::cout << "String Comparison Result: " << result << std::endl;

    // Get the PostScript CSA
    cmsContext context = cmsCreateContext(NULL, NULL);
    if (!context) {
        assert_file_closed(&in_file);;
        cmsCloseProfile(hProfile);
        cmsDeleteTransform(hTransform);
        assert_file_closed(&in_file);
	return 0;
    }
    cmsUInt32Number buffer_len = 1024;
    void *buffer = new uint8_t[buffer_len];
    cmsUInt32Number postscript_csa = cmsGetPostScriptCSA(context, hProfile, fuzz_uint32_t_3, fuzz_uint32_t_4, buffer, buffer_len);
    std::cout << "PostScript CSA: " << postscript_csa << std::endl;

    // Clean up resources
    assert_file_closed(&in_file);;
    cmsCloseProfile(hProfile);
    cmsDeleteTransform(hTransform);
    cmsDeleteContext(context);
    delete[] buffer;

    assert_file_closed(&in_file);
	return 0;
}