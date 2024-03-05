#include "FDSan.h"
#include "FuzzedDataProvider.h"
#include <lcms2.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
//<ID> 1519
//<Prompt> ["cmsIT8SetDataDbl","cmsCreateNULLProfileTHR","cmsGetProfileInfo","cmsCreateBCHSWabstractProfile","cmsStageData","cmsStageOutputChannels","cmsMLUalloc"]
/*<Combination>: [cmsBool cmsIT8SetDataDbl(cmsHANDLE hIT8, const char * cPatch, const char * cSample, cmsFloat64Number Val),
    cmsHPROFILE cmsCreateNULLProfileTHR(cmsContext ContextID),
    cmsUInt32Number cmsGetProfileInfo(cmsHPROFILE hProfile, cmsInfoType Info, const char LanguageCode[3], const char CountryCode[3], wchar_t * Buffer, cmsUInt32Number BufferSize),
    cmsHPROFILE cmsCreateBCHSWabstractProfile(cmsUInt32Number nLUTPoints, cmsFloat64Number Bright, cmsFloat64Number Contrast, cmsFloat64Number Hue, cmsFloat64Number Saturation, cmsUInt32Number TempSrc, cmsUInt32Number TempDest),
    void *cmsStageData(const cmsStage * mpe),
    cmsUInt32Number cmsStageOutputChannels(const cmsStage * mpe),
    cmsMLU *cmsMLUalloc(cmsContext ContextID, cmsUInt32Number nItems)
*/
//<score> 1.2941177, nr_unique_branch: 1, p_cov: 1
//<Quality> {"diversity":17,"density":11,"covered_api":["cmsIT8SetDataDbl","cmsCreateNULLProfileTHR","cmsGetProfileInfo","cmsCreateBCHSWabstractProfile","cmsStageData","cmsStageOutputChannels","cmsMLUalloc"],"uncovered_api":[],"unique_branches":{"strTo16":[[190,9,190,20,0]]},"library_calls":["cmsIT8Alloc","cmsCreateContext","cmsIT8SetDataDbl","cmsCreateContext","cmsCreateNULLProfileTHR","cmsGetProfileInfo","cmsGetProfileInfo","cmsCreateBCHSWabstractProfile","cmsStageAllocToneCurves","cmsStageData","cmsStageOutputChannels","cmsMLUalloc","cmsIT8Free","cmsCloseProfile","cmsCloseProfile","cmsStageFree","cmsMLUfree"]}
/**/


extern "C" int LLVMFuzzerTestOneInput_9(const uint8_t* f_data, size_t f_size) {
	if(f_size<=33) return 0;

	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(f_data, sizeof(uint8_t), f_size, input_file_ptr);
	fclose(input_file_ptr);

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_4, fdp);
	//fuzzer shim end}




    // Step 4: Create FILE * variables for input and output
    FILE *in_file = fmemopen((void *)data, size, "rb");
    FILE *out_file = fopen("output_file", "wb");
    
    // Step 5: Get file descriptors
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);
    
    // Step 6: File names
    const char *in_file_name = "input_file";
    const char *out_file_name = "output_file";
    
    // Step 2: Use lcms APIs
    cmsHANDLE hIT8 = cmsIT8Alloc(cmsCreateContext(NULL, NULL));
    
    cmsBool result = cmsIT8SetDataDbl(hIT8, fuzz_str_1, fuzz_str_2, 1.0);
    if (!result) {
        // Handle error
    }
    
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    cmsHPROFILE nullProfile = cmsCreateNULLProfileTHR(ctx);
    
    cmsUInt32Number profileInfoSize = cmsGetProfileInfo(nullProfile, cmsInfoDescription, NULL, NULL, NULL, 0);
    wchar_t *profileInfoBuffer = new wchar_t[profileInfoSize];
    cmsGetProfileInfo(nullProfile, cmsInfoDescription, NULL, NULL, profileInfoBuffer, profileInfoSize);
    delete[] profileInfoBuffer;
    
    cmsHPROFILE bchswProfile = cmsCreateBCHSWabstractProfile(256, 0.5, 0.5, 0.0, 1.0, fuzz_uint32_t_3, fuzz_uint32_t_4);
    
    const cmsStage *stage = cmsStageAllocToneCurves(ctx, 3, NULL);
    void *stageData = cmsStageData(stage);
    cmsUInt32Number stageOutputChannels = cmsStageOutputChannels(stage);
    
    cmsMLU *mlu = cmsMLUalloc(ctx, 10);
    
    // Step 7: Release resources
    cmsIT8Free(hIT8);
    cmsCloseProfile(nullProfile);
    cmsCloseProfile(bchswProfile);
    cmsStageFree((cmsStage *)stage);
    cmsMLUfree(mlu);
    
    // Close FILEs
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