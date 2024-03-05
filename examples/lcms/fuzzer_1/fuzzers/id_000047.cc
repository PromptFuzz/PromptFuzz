#include "FDSan.h"
#include "FuzzedDataProvider.h"
#include <lcms2.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
//<ID> 5795
//<Prompt> ["cmsMD5computeID","cmsCreateLab2ProfileTHR","cmsIT8LoadFromMem","cmsGetProfileContextID","cmsIT8SaveToFile","cmsCreate_sRGBProfileTHR"]
/*<Combination>: [cmsBool cmsMD5computeID(cmsHPROFILE hProfile),
    cmsHPROFILE cmsCreateLab2ProfileTHR(cmsContext ContextID, const cmsCIExyY * WhitePoint),
    cmsHANDLE cmsIT8LoadFromMem(cmsContext ContextID, const void * Ptr, cmsUInt32Number len),
    cmsContext cmsGetProfileContextID(cmsHPROFILE hProfile),
    cmsBool cmsIT8SaveToFile(cmsHANDLE hIT8, const char * cFileName),
    cmsHPROFILE cmsCreate_sRGBProfileTHR(cmsContext ContextID)
*/
//<score> 63, nr_unique_branch: 62, p_cov: 1
//<Quality> {"diversity":28,"density":28,"covered_api":["cmsMD5computeID","cmsCreateLab2ProfileTHR","cmsIT8LoadFromMem","cmsGetProfileContextID","cmsIT8SaveToFile","cmsCreate_sRGBProfileTHR"],"uncovered_api":[],"unique_branches":{"cmsMD5finish":[[228,9,228,18,0]],"Type_Text_Description_Write":[[1037,9,1037,17,1],[1045,13,1045,25,1],[1048,13,1048,25,1],[1070,9,1070,45,1],[1071,9,1071,40,1],[1073,9,1073,38,1],[1075,9,1075,45,1],[1077,9,1077,49,1],[1080,9,1080,38,1],[1081,9,1081,37,1],[1083,9,1083,36,1],[1086,8,1086,45,1],[1092,9,1092,13,0],[1093,9,1093,13,0]],"Type_Text_Write":[[802,9,802,18,1],[806,9,806,21,1]],"cmsMLUgetASCII":[[401,9,401,23,0]],"Type_LUT16_Write":[[2215,9,2215,20,0],[2215,24,2215,58,1],[2223,9,2223,20,0],[2223,24,2223,60,1],[2228,9,2228,20,0],[2228,24,2228,56,0],[2233,9,2233,20,1],[2239,9,2239,20,1],[2247,9,2247,21,1],[2252,21,2252,38,0],[2252,21,2252,38,1],[2253,17,2253,56,1],[2260,9,2260,66,1],[2261,9,2261,67,1],[2262,9,2262,63,1],[2263,9,2263,37,1],[2265,9,2265,23,1],[2275,13,2275,45,1],[2276,13,2276,45,1],[2277,13,2277,45,1],[2278,13,2278,45,1],[2279,13,2279,45,1],[2280,13,2280,45,1],[2281,13,2281,45,1],[2282,13,2282,45,1],[2283,13,2283,45,1],[2287,9,2287,23,1],[2290,17,2290,46,1],[2293,9,2293,24,1],[2296,13,2296,42,1],[2302,9,2302,23,1],[2306,19,2306,36,0],[2306,19,2306,36,1],[2308,17,2308,46,1],[2309,17,2309,51,1],[2314,9,2314,41,1],[2315,9,2315,21,0],[2317,13,2317,25,0],[2318,17,2318,65,1],[2323,9,2323,24,1],[2327,19,2327,37,0],[2327,19,2327,37,1],[2329,17,2329,46,1],[2330,17,2330,51,1]]},"library_calls":["cmsCreateContext","cmsIT8LoadFromMem","cmsIT8SaveToFile","cmsCreateLab2ProfileTHR","cmsCreate_sRGBProfileTHR","cmsMD5computeID","cmsGetProfileContextID","cmsIT8Free","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"]}
/**/


extern "C" int LLVMFuzzerTestOneInput_47(const uint8_t* f_data, size_t f_size) {
	if(f_size<=0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




  cmsContext ctx = cmsCreateContext(NULL, NULL);

  FILE *in_file = fmemopen((void *)data, size, "rb");
  if (!in_file) {
    cmsDeleteContext(ctx);
    assert_file_closed(&in_file);
	return 0;
  }

  // Load profile from memory
  cmsHANDLE it8_handle = cmsIT8LoadFromMem(ctx, data, size);
  if (!it8_handle) {
    assert_file_closed(&in_file);;
    cmsDeleteContext(ctx);
    assert_file_closed(&in_file);
	return 0;
  }

  // Save IT8 file to disk
  cmsBool saved = cmsIT8SaveToFile(it8_handle, "output_file");
  if (!saved) {
    assert_file_closed(&in_file);;
    cmsIT8Free(it8_handle);
    cmsDeleteContext(ctx);
    assert_file_name_closed("output_file");
	assert_file_closed(&in_file);
	return 0;
  }

  // Create Lab2 profile
  cmsCIExyY white_point = {0.3457, 0.3585, 1.0}; // Example white point values
  cmsHPROFILE lab2_profile = cmsCreateLab2ProfileTHR(ctx, &white_point);
  if (!lab2_profile) {
    assert_file_closed(&in_file);;
    cmsIT8Free(it8_handle);
    cmsDeleteContext(ctx);
    assert_file_name_closed("output_file");
	assert_file_closed(&in_file);
	return 0;
  }

  // Create sRGB profile
  cmsHPROFILE sRGB_profile = cmsCreate_sRGBProfileTHR(ctx);
  if (!sRGB_profile) {
    assert_file_closed(&in_file);;
    cmsIT8Free(it8_handle);
    cmsCloseProfile(lab2_profile);
    cmsDeleteContext(ctx);
    assert_file_name_closed("output_file");
	assert_file_closed(&in_file);
	return 0;
  }

  // Compute MD5 checksum of the profile
  cmsBool computed = cmsMD5computeID(lab2_profile);
  if (!computed) {
    assert_file_closed(&in_file);;
    cmsIT8Free(it8_handle);
    cmsCloseProfile(lab2_profile);
    cmsCloseProfile(sRGB_profile);
    cmsDeleteContext(ctx);
    assert_file_name_closed("output_file");
	assert_file_closed(&in_file);
	return 0;
  }

  // Get context ID from profile
  cmsContext profile_ctx = cmsGetProfileContextID(lab2_profile);
  if (profile_ctx != ctx) {
    assert_file_closed(&in_file);;
    cmsIT8Free(it8_handle);
    cmsCloseProfile(lab2_profile);
    cmsCloseProfile(sRGB_profile);
    cmsDeleteContext(ctx);
    assert_file_name_closed("output_file");
	assert_file_closed(&in_file);
	return 0;
  }

  // Cleanup
  assert_file_closed(&in_file);;
  cmsIT8Free(it8_handle);
  cmsCloseProfile(lab2_profile);
  cmsCloseProfile(sRGB_profile);
  cmsDeleteContext(ctx);

  assert_file_name_closed("output_file");
	assert_file_closed(&in_file);
	return 0;
}