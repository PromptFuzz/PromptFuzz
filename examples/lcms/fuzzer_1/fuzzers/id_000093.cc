#include "FDSan.h"
#include "FuzzedDataProvider.h"
#include <lcms2.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
//<ID> 11844
//<Prompt> ["cmsOpenProfileFromFile","cmsCreateBCHSWabstractProfile","cmsCreateInkLimitingDeviceLinkTHR","cmsIT8LoadFromFile","cmsLabEncoded2FloatV2"]
/*<Combination>: [cmsHPROFILE cmsOpenProfileFromFile(const char * ICCProfile, const char * sAccess),
    cmsHPROFILE cmsCreateBCHSWabstractProfile(cmsUInt32Number nLUTPoints, cmsFloat64Number Bright, cmsFloat64Number Contrast, cmsFloat64Number Hue, cmsFloat64Number Saturation, cmsUInt32Number TempSrc, cmsUInt32Number TempDest),
    cmsHPROFILE cmsCreateInkLimitingDeviceLinkTHR(cmsContext ContextID, cmsColorSpaceSignature ColorSpace, cmsFloat64Number Limit),
    cmsHANDLE cmsIT8LoadFromFile(cmsContext ContextID, const char * cFileName),
    void cmsLabEncoded2FloatV2(cmsCIELab * Lab, const cmsUInt16Number wLab[3])
*/
//<score> 2.7272727, nr_unique_branch: 4, p_cov: 1
//<Quality> {"diversity":11,"density":6,"covered_api":["cmsOpenProfileFromFile","cmsCreateBCHSWabstractProfile","cmsCreateInkLimitingDeviceLinkTHR","cmsIT8LoadFromFile","cmsLabEncoded2FloatV2"],"uncovered_api":[],"unique_branches":{"cmsWhitePointFromTemp":[[62,23,62,33,1],[69,13,69,23,0],[69,27,69,39,0]],"cmsOpenIOhandlerFromFile":[[410,9,410,16,0]]},"library_calls":["cmsCreateContext","cmsOpenProfileFromFile","cmsCreateBCHSWabstractProfile","cmsCreateInkLimitingDeviceLinkTHR","cmsIT8LoadFromFile","cmsLabEncoded2FloatV2","cmsCloseProfile","cmsCloseProfile","cmsCloseProfile","cmsIT8Free","cmsDeleteContext"]}
/*Here is a step-by-step implementation of the fuzz driver using the lcms library APIs:

*/


extern "C" int LLVMFuzzerTestOneInput_93(const uint8_t* f_data, size_t f_size) {
	if(f_size<=0) return 0;

	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(f_data, sizeof(uint8_t), f_size, input_file_ptr);
	fclose(input_file_ptr);

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    cmsContext ctx = cmsCreateContext(NULL, NULL);
    cmsHPROFILE hProfile = NULL;
    cmsHANDLE hIT8 = NULL;
    
    FILE *in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Open profile from file
    const char* profileFileName = "input_file";
    const char* accessMode = "rb";
    hProfile = cmsOpenProfileFromFile(profileFileName, accessMode);
    
    // Create abstract profile
    cmsUInt32Number nLUTPoints = 100;
    cmsFloat64Number Bright = 1.0;
    cmsFloat64Number Contrast = 1.0;
    cmsFloat64Number Hue = 1.0;
    cmsFloat64Number Saturation = 1.0;
    cmsUInt32Number TempSrc = 5000;
    cmsUInt32Number TempDest = 8000;
    cmsHPROFILE abstractProfile = cmsCreateBCHSWabstractProfile(nLUTPoints, Bright, Contrast, Hue, Saturation, TempSrc, TempDest);
    
    // Create ink limiting device link
    cmsColorSpaceSignature ColorSpace = cmsSigXYZData;
    cmsFloat64Number Limit = 0.5;
    cmsHPROFILE inkLimitingProfile = cmsCreateInkLimitingDeviceLinkTHR(ctx, ColorSpace, Limit);
    
    // Load IT8 data from file
    const char* fileName = "input_file";
    hIT8 = cmsIT8LoadFromFile(ctx, fileName);
    
    // Convert LAB encoded values to float
    cmsCIELab Lab;
    cmsUInt16Number wLab[3];
    cmsLabEncoded2FloatV2(&Lab, wLab);
    
    // Clean up resources
    cmsCloseProfile(hProfile);
    cmsCloseProfile(abstractProfile);
    cmsCloseProfile(inkLimitingProfile);
    cmsIT8Free(hIT8);
    cmsDeleteContext(ctx);
    assert_file_closed(&in_file);;
    
    assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	assert_file_closed(&in_file);
	return 0;
}