#include "FDSan.h"
#include <lcms2.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
//<ID> 6195
//<Prompt> ["cmsIT8SetComment","cmsDictAddEntry","cmsIT8Alloc","cmsGetPipelineContextID","cmsOpenProfileFromFileTHR"]
/*<Combination>: [cmsBool cmsIT8SetComment(cmsHANDLE hIT8, const char * cComment),
    cmsBool cmsDictAddEntry(cmsHANDLE hDict, const wchar_t * Name, const wchar_t * Value, const cmsMLU * DisplayName, const cmsMLU * DisplayValue),
    cmsHANDLE cmsIT8Alloc(cmsContext ContextID),
    cmsContext cmsGetPipelineContextID(const cmsPipeline * lut),
    cmsHPROFILE cmsOpenProfileFromFileTHR(cmsContext ContextID, const char * ICCProfile, const char * sAccess)
*/
//<score> 6.4, nr_unique_branch: 7, p_cov: 0.8
//<Quality> {"diversity":23,"density":23,"covered_api":["cmsIT8SetComment","cmsDictAddEntry","cmsIT8Alloc","cmsOpenProfileFromFileTHR"],"uncovered_api":["cmsGetPipelineContextID"],"unique_branches":{"DupWcs":[[927,9,927,20,1]],"cmsDictAddEntry":[[941,9,941,22,1]],"cmsDictFree":[[905,12,905,25,0],[907,17,907,45,1],[908,17,908,45,1],[909,17,909,37,0],[910,17,910,38,0]]},"library_calls":["cmsCreateContext","cmsOpenProfileFromFileTHR","cmsIT8Alloc","cmsIT8SetComment","cmsDictAlloc","cmsDictAddEntry","cmsDictFree","cmsIT8Free","cmsCloseProfile","cmsDeleteContext"]}
/**/

extern "C" int LLVMFuzzerTestOneInput_53(const uint8_t *data, size_t size) {
	if(size<=0) return 0;

	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);

    // Create a FILE pointer to read the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Create a context for lcms
    cmsContext context = cmsCreateContext(NULL, NULL);
    if (!context) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // Open a profile from file
    cmsHPROFILE profile = cmsOpenProfileFromFileTHR(context, "input_file", "r");
    if (!profile) {
        cmsDeleteContext(context);
        assert_file_closed(&in_file);;
        assert_file_name_closed("input_file");
	assert_file_closed(&in_file);
	return 0;
    }

    // Allocate an IT8 handle
    cmsHANDLE it8 = cmsIT8Alloc(context);
    if (!it8) {
        cmsCloseProfile(profile);
        cmsDeleteContext(context);
        assert_file_closed(&in_file);;
        assert_file_name_closed("input_file");
	assert_file_closed(&in_file);
	return 0;
    }

    // Set a comment for the IT8 handle
    cmsBool result = cmsIT8SetComment(it8, "This is a comment");
    if (!result) {
        cmsIT8Free(it8);
        cmsCloseProfile(profile);
        cmsDeleteContext(context);
        assert_file_closed(&in_file);;
        assert_file_name_closed("input_file");
	assert_file_closed(&in_file);
	return 0;
    }

    // Create a dictionary handle
    cmsHANDLE dict = cmsDictAlloc(context);
    if (!dict) {
        cmsIT8Free(it8);
        cmsCloseProfile(profile);
        cmsDeleteContext(context);
        assert_file_closed(&in_file);;
        assert_file_name_closed("input_file");
	assert_file_closed(&in_file);
	return 0;
    }

    // Add an entry to the dictionary
    const wchar_t* name = L"entry_name";
    const wchar_t* value = L"entry_value";
    cmsBool addResult = cmsDictAddEntry(dict, name, value, NULL, NULL);
    if (!addResult) {
        cmsDictFree(dict);
        cmsIT8Free(it8);
        cmsCloseProfile(profile);
        cmsDeleteContext(context);
        assert_file_closed(&in_file);;
        assert_file_name_closed("input_file");
	assert_file_closed(&in_file);
	return 0;
    }

    // Clean up resources
    cmsDictFree(dict);
    cmsIT8Free(it8);
    cmsCloseProfile(profile);
    cmsDeleteContext(context);
    assert_file_closed(&in_file);;

    assert_file_name_closed("input_file");
	assert_file_closed(&in_file);
	return 0;
}