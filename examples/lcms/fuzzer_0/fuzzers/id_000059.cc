#include "FDSan.h"
#include <lcms2.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
//<ID> 6946
//<Prompt> ["cmsPipelineStageCount","cmsTempFromWhitePoint","cmsIT8EnumProperties","cmsCIE94DeltaE","cmsCreate_OkLabProfile","cmsEvalToneCurve16","cmsD50_XYZ"]
/*<Combination>: [cmsUInt32Number cmsPipelineStageCount(const cmsPipeline * lut),
    cmsBool cmsTempFromWhitePoint(cmsFloat64Number * TempK, const cmsCIExyY * WhitePoint),
    cmsUInt32Number cmsIT8EnumProperties(cmsHANDLE hIT8, char *** PropertyNames),
    cmsFloat64Number cmsCIE94DeltaE(const cmsCIELab * Lab1, const cmsCIELab * Lab2),
    cmsHPROFILE cmsCreate_OkLabProfile(cmsContext ctx),
    cmsUInt16Number cmsEvalToneCurve16(const cmsToneCurve * Curve, cmsUInt16Number v),
    const cmsCIEXYZ *cmsD50_XYZ()
*/
//<score> 0.8125, nr_unique_branch: 0, p_cov: 1
//<Quality> {"diversity":16,"density":13,"covered_api":["cmsPipelineStageCount","cmsTempFromWhitePoint","cmsIT8EnumProperties","cmsCIE94DeltaE","cmsCreate_OkLabProfile","cmsEvalToneCurve16","cmsD50_XYZ"],"uncovered_api":[],"unique_branches":{},"library_calls":["cmsCreateContext","cmsPipelineAlloc","cmsPipelineStageCount","cmsTempFromWhitePoint","cmsIT8Alloc","cmsIT8EnumProperties","cmsCIE94DeltaE","cmsCreate_OkLabProfile","cmsBuildGamma","cmsEvalToneCurve16","cmsD50_XYZ","cmsPipelineFree","cmsIT8Free","cmsCloseProfile","cmsFreeToneCurve","cmsDeleteContext"]}
/*Here is the C++ program that implements the steps mentioned above:

*/


extern "C" int LLVMFuzzerTestOneInput_59(const uint8_t *data, size_t size) {
	if(size<=0) return 0;

	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);

    // Step 4: Create FILE pointers for input and output
    FILE* in_file = fmemopen((void*)data, size, "rb");
    FILE* out_file = fopen("output_file", "wb");

    if (in_file == nullptr || out_file == nullptr) {
        // Handle error
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Step 5: Get file descriptor
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);

    // Step 6: Get file name
    const char* in_filename = "input_file";
    const char* out_filename = "output_file";

    // Step 2: Use lcms APIs
    cmsContext context = cmsCreateContext(nullptr, nullptr);

    cmsPipeline* pipeline = cmsPipelineAlloc(context, 3, 3);
    cmsUInt32Number stages = cmsPipelineStageCount(pipeline);

    cmsFloat64Number tempK;
    cmsCIExyY whitePoint;
    cmsTempFromWhitePoint(&tempK, &whitePoint);

    cmsHANDLE hIT8 = cmsIT8Alloc(context);
    char** propertyNames;
    cmsUInt32Number numProperties = cmsIT8EnumProperties(hIT8, &propertyNames);

    cmsCIELab lab1, lab2;
    cmsFloat64Number deltaE = cmsCIE94DeltaE(&lab1, &lab2);

    cmsHPROFILE okLabProfile = cmsCreate_OkLabProfile(context);

    cmsToneCurve* curve = cmsBuildGamma(context, 2.2);
    cmsUInt16Number eval = cmsEvalToneCurve16(curve, 128);

    const cmsCIEXYZ* d50_xyz = cmsD50_XYZ();

    // Step 7: Release allocated resources
    cmsPipelineFree(pipeline);
    cmsIT8Free(hIT8);
    cmsCloseProfile(okLabProfile);
    cmsFreeToneCurve(curve);
    cmsDeleteContext(context);

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