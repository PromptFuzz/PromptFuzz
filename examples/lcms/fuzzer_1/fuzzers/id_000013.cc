#include "FDSan.h"
#include "FuzzedDataProvider.h"
#include <lcms2.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
//<ID> 1715
//<Prompt> ["cmsLinkTag","cmsWhitePointFromTemp","cmsDetectBlackPoint","cmsCreateLinearizationDeviceLinkTHR","cmsSetColorSpace","cmsIsCLUT"]
/*<Combination>: [cmsBool cmsLinkTag(cmsHPROFILE hProfile, cmsTagSignature sig, cmsTagSignature dest),
    cmsBool cmsWhitePointFromTemp(cmsCIExyY * WhitePoint, cmsFloat64Number TempK),
    cmsBool cmsDetectBlackPoint(cmsCIEXYZ * BlackPoint, cmsHPROFILE hProfile, cmsUInt32Number Intent, cmsUInt32Number dwFlags),
    cmsHPROFILE cmsCreateLinearizationDeviceLinkTHR(cmsContext ContextID, cmsColorSpaceSignature ColorSpace, cmsToneCurve *const * TransferFunctions),
    void cmsSetColorSpace(cmsHPROFILE hProfile, cmsColorSpaceSignature sig),
    cmsBool cmsIsCLUT(cmsHPROFILE hProfile, cmsUInt32Number Intent, cmsUInt32Number UsedDirection)
*/
//<score> 58.18182, nr_unique_branch: 127, p_cov: 1
//<Quality> {"diversity":22,"density":10,"covered_api":["cmsLinkTag","cmsWhitePointFromTemp","cmsDetectBlackPoint","cmsCreateLinearizationDeviceLinkTHR","cmsSetColorSpace","cmsIsCLUT"],"uncovered_api":[],"unique_branches":{"ColorSpaceIsCompatible":[[503,33,503,53,1]],"_cmsReadInputLUT":[[351,13,351,39,1],[355,13,355,38,0],[361,17,361,28,0],[361,17,361,28,1],[370,17,370,48,1],[370,52,370,88,1],[374,17,374,60,1],[379,17,379,92,1]],"cmsDetectBlackPoint":[[197,9,197,36,0],[199,9,199,42,0],[218,17,218,44,1],[267,9,267,47,1]],"Eval4Inputs":[[889,16,889,35,0],[892,16,892,35,0],[892,16,892,35,1],[895,16,895,35,0],[895,16,895,35,1],[898,16,898,35,0],[898,16,898,35,1],[903,21,903,46,0],[903,21,903,46,1],[907,13,907,21,0],[907,13,907,21,1],[907,25,907,33,0],[915,17,915,25,0],[915,17,915,25,1],[915,29,915,37,1],[923,21,923,29,0],[923,21,923,29,1],[923,33,923,41,1],[931,25,931,33,0],[931,37,931,45,0],[931,37,931,45,1],[939,29,939,37,0],[939,29,939,37,1],[939,41,939,49,0],[947,33,947,41,0],[947,45,947,53,0],[967,21,967,46,0],[967,21,967,46,1],[971,13,971,21,0],[971,13,971,21,1],[971,25,971,33,0],[979,17,979,25,0],[979,17,979,25,1],[979,29,979,37,1],[987,21,987,29,0],[987,21,987,29,1],[987,33,987,41,1],[995,25,995,33,0],[995,37,995,45,0],[995,37,995,45,1],[1003,29,1003,37,0],[1003,29,1003,37,1],[1003,41,1003,49,0],[1011,33,1011,41,0],[1011,45,1011,53,0],[1029,15,1029,34,0],[1029,15,1029,34,1]],"BlackPointAsDarkerColorant":[[80,9,80,66,0],[93,9,93,63,0],[116,9,116,22,0]],"_cmsEndPointsBySpace":[[742,8,742,27,0],[742,33,742,38,1],[743,32,743,37,0],[744,32,744,40,0]],"Read16bitTables":[[2049,9,2049,22,0],[2049,9,2049,22,1],[2052,9,2052,21,0],[2052,9,2052,21,1],[2053,9,2053,35,1],[2058,15,2058,28,0],[2058,15,2058,28,1],[2061,13,2061,30,1],[2063,13,2063,67,0],[2063,13,2063,67,1],[2068,9,2068,103,1],[2071,15,2071,28,0],[2071,15,2071,28,1],[2077,15,2077,28,0],[2077,15,2077,28,1],[2078,13,2078,22,0],[2078,13,2078,22,1]],"cmsIsMatrixShaper":[[817,17,817,59,1],[818,17,818,58,1],[819,17,819,52,1],[820,17,820,54,1]],"Type_LUT16_Read":[[2120,9,2120,49,1],[2121,9,2121,50,1],[2122,9,2122,46,1],[2125,9,2125,39,1],[2128,9,2128,27,1],[2128,31,2128,61,0],[2128,31,2128,61,1],[2129,9,2129,28,1],[2129,32,2129,63,1],[2133,9,2133,23,1],[2136,9,2136,50,1],[2137,9,2137,50,1],[2138,9,2138,50,1],[2139,9,2139,50,1],[2140,9,2140,50,1],[2141,9,2141,50,1],[2142,9,2142,50,1],[2143,9,2143,50,1],[2144,9,2144,50,1],[2148,9,2148,29,1],[2154,9,2154,49,1],[2155,9,2155,50,1],[2157,9,2157,30,0],[2157,9,2157,30,1],[2157,34,2157,56,0],[2157,34,2157,56,1],[2158,9,2158,24,1],[2161,9,2161,85,0],[2161,9,2161,85,1],[2165,9,2165,41,1],[2166,9,2166,21,0],[2171,13,2171,23,1],[2173,13,2173,50,0],[2173,13,2173,50,1],[2178,13,2178,143,1],[2187,9,2187,87,0],[2187,9,2187,87,1],[2193,9,2193,23,0],[2193,9,2193,23,1]]},"library_calls":["cmsOpenProfileFromFile","cmsLinkTag","cmsWhitePointFromTemp","cmsDetectBlackPoint","cmsBuildGamma","cmsBuildGamma","cmsBuildGamma","cmsCreateLinearizationDeviceLinkTHR","cmsSetColorSpace","cmsIsCLUT","cmsCloseProfile","cmsFreeToneCurve","cmsFreeToneCurve","cmsFreeToneCurve","cmsCloseProfile"]}
/**/

extern "C" int LLVMFuzzerTestOneInput_13(const uint8_t* f_data, size_t f_size) {
	if(f_size<=8) return 0;

	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(f_data, sizeof(uint8_t), f_size, input_file_ptr);
	fclose(input_file_ptr);

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
	//fuzzer shim end}




    // Create a FILE * variable for reading the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file == nullptr) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Create a file descriptor for reading the input data
    int in_fd = fuzz_fileno(in_file);
    
    // Create the output file
    FILE *out_file = fopen("output_file", "wb");
    if (out_file == nullptr) {
        assert_file_closed(&in_file);;
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Create a file descriptor for writing the output data
    int out_fd = fuzz_fileno(out_file);
    
    // Load the input profile from the input file
    cmsHPROFILE input_profile = cmsOpenProfileFromFile("input_file", fuzz_str_1);
    if (input_profile == nullptr) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_name_closed("input_file");
	assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Link a tag in the input profile
    cmsBool link_result = cmsLinkTag(input_profile, cmsSigProfileDescriptionTag, cmsSigChromaticityTag);
    if (link_result == 0) {
        cmsCloseProfile(input_profile);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_name_closed("input_file");
	assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Get the white point from a temperature value
    cmsCIExyY white_point;
    cmsBool white_point_result = cmsWhitePointFromTemp(&white_point, 6500);
    if (white_point_result == 0) {
        cmsCloseProfile(input_profile);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_name_closed("input_file");
	assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Detect the black point in the input profile
    cmsCIEXYZ black_point;
    cmsBool detect_black_point_result = cmsDetectBlackPoint(&black_point, input_profile, 0, 0);
    if (detect_black_point_result == 0) {
        cmsCloseProfile(input_profile);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_name_closed("input_file");
	assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Create a linearization device link with specified transfer functions
    cmsToneCurve *transfer_functions[3];
    transfer_functions[0] = cmsBuildGamma(nullptr, 1.0);
    transfer_functions[1] = cmsBuildGamma(nullptr, 1.0);
    transfer_functions[2] = cmsBuildGamma(nullptr, 1.0);
    
    cmsHPROFILE linearization_profile = cmsCreateLinearizationDeviceLinkTHR(nullptr, cmsSigRgbData, transfer_functions);
    if (linearization_profile == nullptr) {
        cmsFreeToneCurve(transfer_functions[0]);
        cmsFreeToneCurve(transfer_functions[1]);
        cmsFreeToneCurve(transfer_functions[2]);
        cmsCloseProfile(input_profile);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_name_closed("input_file");
	assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Set the color space of the input profile
    cmsSetColorSpace(input_profile, cmsSigRgbData);
    
    // Check if the input profile is a CLUT profile
    cmsBool is_clut = cmsIsCLUT(input_profile, 0, fuzz_uint32_t_2);
    
    // Release all allocated resources
    cmsCloseProfile(input_profile);
    cmsFreeToneCurve(transfer_functions[0]);
    cmsFreeToneCurve(transfer_functions[1]);
    cmsFreeToneCurve(transfer_functions[2]);
    cmsCloseProfile(linearization_profile);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_name_closed("input_file");
	assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}