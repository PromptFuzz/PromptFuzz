#include "FDSan.h"
#include "FuzzedDataProvider.h"
#include <lcms2.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
//<ID> 8425
//<Prompt> ["cmsSetPCS","cmsCreateXYZProfile","cmsCreateGrayProfile","cmsBuildGamma","cmsSetAlarmCodes","cmsstrcasecmp"]
/*<Combination>: [void cmsSetPCS(cmsHPROFILE hProfile, cmsColorSpaceSignature pcs),
    cmsHPROFILE cmsCreateXYZProfile(),
    cmsHPROFILE cmsCreateGrayProfile(const cmsCIExyY * WhitePoint, const cmsToneCurve * TransferFunction),
    cmsToneCurve *cmsBuildGamma(cmsContext ContextID, cmsFloat64Number Gamma),
    void cmsSetAlarmCodes(const cmsUInt16Number NewAlarm[16]),
    int cmsstrcasecmp(const char * s1, const char * s2)
*/
//<score> 0.6666667, nr_unique_branch: 1, p_cov: 0.8333333
//<Quality> {"diversity":10,"density":4,"covered_api":["cmsSetPCS","cmsCreateXYZProfile","cmsCreateGrayProfile","cmsBuildGamma","cmsSetAlarmCodes"],"uncovered_api":["cmsstrcasecmp"],"unique_branches":{"cmsCreateGrayProfileTHR":[[259,9,259,19,1]]},"library_calls":["cmsCreateXYZProfile","cmsCreateGrayProfile","cmsSetPCS","cmsCreateContext","cmsBuildGamma","cmsSetAlarmCodes","cmsCloseProfile","cmsCloseProfile","cmsFreeToneCurve","cmsDeleteContext"]}
/**/


extern "C" int LLVMFuzzerTestOneInput_67(const uint8_t* f_data, size_t f_size) {
	if(f_size<=0) return 0;

	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(f_data, sizeof(uint8_t), f_size, input_file_ptr);
	fclose(input_file_ptr);

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Step 4: Create FILE * variables
    FILE *in_file = fmemopen((void *)data, size, "rb");
    FILE *out_file = fopen("output_file", "wb");
    
    // Step 5: Get file descriptors
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);
    
    // Step 1: Set PCS and create profiles
    cmsHPROFILE xyzProfile = cmsCreateXYZProfile();
    cmsHPROFILE grayProfile = cmsCreateGrayProfile(NULL, NULL);
    cmsSetPCS(grayProfile, cmsSigXYZData);
    
    // Step 2: Build gamma curve
    cmsContext context = cmsCreateContext(NULL, NULL);
    cmsToneCurve *gammaCurve = cmsBuildGamma(context, 1.0);
    
    // Step 3: Set alarm codes
    cmsUInt16Number alarmCodes[16];
    for (int i = 0; i < 16; i++) {
        alarmCodes[i] = i;
    }
    cmsSetAlarmCodes(alarmCodes);
    
    // Step 6: Directly use file names
    const char *input_file = "input_file";
    const char *output_file = "output_file";
    
    // Step 7: Free allocated resources
    cmsCloseProfile(xyzProfile);
    cmsCloseProfile(grayProfile);
    cmsFreeToneCurve(gammaCurve);
    cmsDeleteContext(context);
    
    // Step 7: Close files
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}