#include "FDSan.h"
#include <lcms2.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
//<ID> 7371
//<Prompt> ["cmsMLUtranslationsCount","cmsXYZ2Lab","cmsPipelineEvalReverseFloat","cmsGetSupportedIntentsTHR","cmsIsCLUT"]
/*<Combination>: [cmsUInt32Number cmsMLUtranslationsCount(const cmsMLU * mlu),
    void cmsXYZ2Lab(const cmsCIEXYZ * WhitePoint, cmsCIELab * Lab, const cmsCIEXYZ * xyz),
    cmsBool cmsPipelineEvalReverseFloat(cmsFloat32Number * Target, cmsFloat32Number * Result, cmsFloat32Number * Hint, const cmsPipeline * lut),
    cmsUInt32Number cmsGetSupportedIntentsTHR(cmsContext ContextID, cmsUInt32Number nMax, cmsUInt32Number * Codes, char ** Descriptions),
    cmsBool cmsIsCLUT(cmsHPROFILE hProfile, cmsUInt32Number Intent, cmsUInt32Number UsedDirection)
*/
//<score> 24.375, nr_unique_branch: 25, p_cov: 1
//<Quality> {"diversity":16,"density":15,"covered_api":["cmsMLUtranslationsCount","cmsXYZ2Lab","cmsPipelineEvalReverseFloat","cmsGetSupportedIntentsTHR","cmsIsCLUT"],"uncovered_api":[],"unique_branches":{"cmsGetSupportedIntentsTHR":[[1181,17,1181,30,1]],"cmsMLUtranslationsCount":[[487,9,487,20,1]],"cmsPipelineEvalReverseFloat":[[1759,9,1759,33,1],[1760,9,1760,34,1],[1763,9,1763,21,1],[1771,19,1771,24,0],[1771,19,1771,24,1],[1776,9,1776,33,1],[1783,17,1783,45,0],[1792,13,1792,31,1],[1797,19,1797,42,0],[1797,19,1797,42,1],[1801,13,1801,23,0],[1801,13,1801,23,1],[1805,21,1805,26,0],[1805,21,1805,26,1],[1826,13,1826,51,1],[1835,19,1835,24,0],[1835,19,1835,24,1],[1836,17,1836,25,1],[1838,21,1838,31,1]],"EuclideanDistance":[[1729,15,1729,20,0],[1729,15,1729,20,1]],"_cmsMAT3solve":[[162,9,162,35,1]],"IncDelta":[[1711,9,1711,40,0]]},"library_calls":["cmsCreateContext","cmsSetAdaptationStateTHR","cmsMLUalloc","cmsMLUtranslationsCount","cmsMLUtranslationsCodes","cmsXYZ2Lab","cmsPipelineAlloc","cmsPipelineEvalReverseFloat","cmsGetSupportedIntentsTHR","cmsGetSupportedIntentsTHR","cmsCreate_sRGBProfileTHR","cmsIsCLUT","cmsMLUfree","cmsPipelineFree","cmsCloseProfile","cmsDeleteContext"]}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_62(const uint8_t *data, size_t size) {
	if(size<=0) return 0;

	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);

    // Step 4: Create FILE pointer for reading input data
    FILE *in_file = fmemopen((void *)data, size, "rb");

    // Step 5: Get file descriptor for reading
    int in_fd = fuzz_fileno(in_file);

    // Step 6: Set input file name
    const char *input_file = "input_file";

    // Initialize LCMS context
    cmsContext context = cmsCreateContext(NULL, NULL);
    cmsSetAdaptationStateTHR(context, 0.5);  // Set adaptation state

    // Use some APIs from lcms
    // Step 1: Call cmsMLUtranslationsCount
    cmsMLU *mlu = cmsMLUalloc(context, 10);  // Allocate MLU structure
    cmsUInt32Number count = cmsMLUtranslationsCount(mlu);
    cmsMLUtranslationsCodes(mlu, 0, NULL, NULL);  // Get translation codes

    // Step 2: Call cmsXYZ2Lab
    cmsCIEXYZ whitePoint = {0.9505, 1.0, 1.0890};
    cmsCIELab Lab;
    cmsXYZ2Lab(&whitePoint, &Lab, &whitePoint);

    // Step 3: Call cmsPipelineEvalReverseFloat
    cmsPipeline *pipeline = cmsPipelineAlloc(context, 3, 3);
    cmsFloat32Number target[3] = {0.5f, 0.5f, 0.5f};
    cmsFloat32Number result[3] = {0.0f, 0.0f, 0.0f};
    cmsFloat32Number hint[3] = {0.0f, 0.0f, 0.0f};
    cmsPipelineEvalReverseFloat(target, result, hint, pipeline);

    // Step 4: Create FILE pointer for writing output data
    FILE *out_file = fopen("output_file", "wb");

    // Step 5: Get file descriptor for writing
    int out_fd = fuzz_fileno(out_file);

    // Step 6: Set output file name
    const char *output_file = "output_file";

    // Step 7: Call cmsGetSupportedIntentsTHR
    cmsUInt32Number max_intents = cmsGetSupportedIntentsTHR(context, 10, NULL, NULL);
    cmsUInt32Number *codes = new cmsUInt32Number[max_intents];
    char **descriptions = new char*[max_intents];
    cmsGetSupportedIntentsTHR(context, max_intents, codes, descriptions);

    // Step 8: Call cmsIsCLUT
    cmsHPROFILE hProfile = cmsCreate_sRGBProfileTHR(context);
    cmsUInt32Number intent = 0;
    cmsUInt32Number usedDirection = 0;
    cmsIsCLUT(hProfile, intent, usedDirection);

    // Step 9: Release all allocated resources
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    delete[] codes;
    delete[] descriptions;
    cmsMLUfree(mlu);
    cmsPipelineFree(pipeline);
    cmsCloseProfile(hProfile);
    cmsDeleteContext(context);

    assert_file_name_closed("output_file");
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}