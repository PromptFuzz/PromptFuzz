#include "FDSan.h"
#include <lcms2.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
//<ID> 12946
//<Prompt> ["cmsIT8GetPatchName","cmsGetHeaderManufacturer","cmsPipelineGetPtrToFirstStage","cmsIT8GetProperty","cmsCreate_sRGBProfile","cmsIsIntentSupported"]
/*<Combination>: [const char *cmsIT8GetPatchName(cmsHANDLE hIT8, int nPatch, char * buffer),
    cmsUInt32Number cmsGetHeaderManufacturer(cmsHPROFILE hProfile),
    cmsStage *cmsPipelineGetPtrToFirstStage(const cmsPipeline * lut),
    const char *cmsIT8GetProperty(cmsHANDLE hIT8, const char * cProp),
    cmsHPROFILE cmsCreate_sRGBProfile(),
    cmsBool cmsIsIntentSupported(cmsHPROFILE hProfile, cmsUInt32Number Intent, cmsUInt32Number UsedDirection)
*/
//<score> 0.85714287, nr_unique_branch: 1, p_cov: 1
//<Quality> {"diversity":14,"density":6,"covered_api":["cmsIT8GetPatchName","cmsGetHeaderManufacturer","cmsPipelineGetPtrToFirstStage","cmsIT8GetProperty","cmsCreate_sRGBProfile","cmsIsIntentSupported"],"uncovered_api":[],"unique_branches":{"cmsIT8GetPatchName":[[2868,9,2868,14,0]]},"library_calls":["cmsCreate_sRGBProfile","cmsGetHeaderManufacturer","cmsPipelineAlloc","cmsPipelineGetPtrToFirstStage","cmsIT8LoadFromFile","cmsIT8GetPatchName","cmsIT8GetProperty","cmsIsIntentSupported","cmsCloseProfile","cmsPipelineFree","cmsIT8Free"]}
/*Here is the C++ program that follows the given instructions:

*/


extern "C" int LLVMFuzzerTestOneInput_105(const uint8_t *data, size_t size) {
	if(size<=0) return 0;

	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);

    // Create an input file stream
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file == nullptr) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Create an output file stream
    FILE *out_file = fopen("output_file", "wb");
    if (out_file == nullptr) {
        assert_file_closed(&in_file);;
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Create a color profile
    cmsHPROFILE profile = cmsCreate_sRGBProfile();
    if (profile == nullptr) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Get the manufacturer of the profile
    cmsUInt32Number manufacturer = cmsGetHeaderManufacturer(profile);

    // Create a color transformation pipeline
    cmsPipeline *pipeline = cmsPipelineAlloc(nullptr, 3, 3);
    if (pipeline == nullptr) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        cmsCloseProfile(profile);
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Get the first stage of the pipeline
    cmsStage *firstStage = cmsPipelineGetPtrToFirstStage(pipeline);

    // Load an IT8 file
     cmsHANDLE it8Handle = cmsIT8LoadFromFile(nullptr, "input_file");
    if (it8Handle == nullptr) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        cmsCloseProfile(profile);
        cmsPipelineFree(pipeline);
        assert_file_name_closed("input_file");
	assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Get a patch name from IT8 data
    char patchName[256];
    const char *patchNameResult = cmsIT8GetPatchName(it8Handle, 1, patchName);

    // Get a property from IT8 data
    const char *propertyValue = cmsIT8GetProperty(it8Handle, "property_name");

    // Check if a specific intent is supported by the profile
    cmsBool isIntentSupported = cmsIsIntentSupported(profile, INTENT_PERCEPTUAL, 0);

    // Cleanup
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    cmsCloseProfile(profile);
    cmsPipelineFree(pipeline);
    cmsIT8Free(it8Handle);

    assert_file_name_closed("input_file");
	assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}