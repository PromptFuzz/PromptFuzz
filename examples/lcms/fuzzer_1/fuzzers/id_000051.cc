#include "FDSan.h"
#include "FuzzedDataProvider.h"
#include <lcms2.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
//<ID> 6120
//<Prompt> ["cmsIT8SetComment","cmsIT8Alloc","cmsOpenProfileFromFileTHR","cmsMLUsetWide","cmsGetProfileContextID"]
/*<Combination>: [cmsBool cmsIT8SetComment(cmsHANDLE hIT8, const char * cComment),
    cmsHANDLE cmsIT8Alloc(cmsContext ContextID),
    cmsHPROFILE cmsOpenProfileFromFileTHR(cmsContext ContextID, const char * ICCProfile, const char * sAccess),
    cmsBool cmsMLUsetWide(cmsMLU * mlu, const char LanguageCode[3], const char CountryCode[3], const wchar_t * WideString),
    cmsContext cmsGetProfileContextID(cmsHPROFILE hProfile)
*/
//<score> 1.4, nr_unique_branch: 1, p_cov: 1
//<Quality> {"diversity":10,"density":7,"covered_api":["cmsIT8SetComment","cmsIT8Alloc","cmsOpenProfileFromFileTHR","cmsMLUsetWide","cmsGetProfileContextID"],"uncovered_api":[],"unique_branches":{"cmsGetProfileContextID":[[570,9,570,20,0]]},"library_calls":["cmsIT8Alloc","cmsIT8SetComment","cmsGetProfileContextID","cmsOpenProfileFromFileTHR","cmsGetProfileContextID","cmsMLUalloc","cmsMLUsetWide","cmsMLUfree","cmsCloseProfile","cmsIT8Free"]}
/**/


extern "C" int LLVMFuzzerTestOneInput_51(const uint8_t* f_data, size_t f_size) {
	if(f_size<=34) return 0;

	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(f_data, sizeof(uint8_t), f_size, input_file_ptr);
	fclose(input_file_ptr);

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
	//fuzzer shim end}




    // Step 4: Open the input file
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Step 6: Specify the input file name
    const char *input_file = "input_file";

    // Step 7: Open the output file
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Step 6: Specify the output file name
    const char *output_file = "output_file";

    // Step 1: Set a comment for the IT8 handle
    cmsHANDLE hIT8 = cmsIT8Alloc(NULL);
    cmsIT8SetComment(hIT8, fuzz_str_1);

    // Step 2: Open a profile from the input file
    cmsContext ContextID = cmsGetProfileContextID(NULL);
    cmsHPROFILE hProfile = cmsOpenProfileFromFileTHR(ContextID, input_file, fuzz_str_2);

    // Step 5: Get the profile context ID
    cmsContext profileContextID = cmsGetProfileContextID(hProfile);

    // Step 3: Create a MLU handle and set a wide string
    cmsMLU *mlu = cmsMLUalloc(profileContextID, 1);
    cmsBool mluSetWideResult = cmsMLUsetWide(mlu, fuzz_str_3, fuzz_str_4, L"Hello");

    // Release all allocated resources
    cmsMLUfree(mlu);
    cmsCloseProfile(hProfile);
    cmsIT8Free(hIT8);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;

    assert_file_name_closed("output_file");
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}