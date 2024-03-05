#include "FDSan.h"
#include "FuzzedDataProvider.h"
#include <lcms2.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
//<ID> 12978
//<Prompt> ["cmsIT8GetPatchName","cmsGetHeaderManufacturer","cmsPipelineGetPtrToFirstStage","cmsIsToneCurveMonotonic","cmsBuildSegmentedToneCurve","cmsIT8GetProperty","cmsCreate_sRGBProfile","cmsIsIntentSupported"]
/*<Combination>: [const char *cmsIT8GetPatchName(cmsHANDLE hIT8, int nPatch, char * buffer),
    cmsUInt32Number cmsGetHeaderManufacturer(cmsHPROFILE hProfile),
    cmsStage *cmsPipelineGetPtrToFirstStage(const cmsPipeline * lut),
    cmsBool cmsIsToneCurveMonotonic(const cmsToneCurve * t),
    cmsToneCurve *cmsBuildSegmentedToneCurve(cmsContext ContextID, cmsUInt32Number nSegments, const cmsCurveSegment * Segments),
    const char *cmsIT8GetProperty(cmsHANDLE hIT8, const char * cProp),
    cmsHPROFILE cmsCreate_sRGBProfile(),
    cmsBool cmsIsIntentSupported(cmsHPROFILE hProfile, cmsUInt32Number Intent, cmsUInt32Number UsedDirection)
*/
//<score> 2.6666667, nr_unique_branch: 7, p_cov: 1
//<Quality> {"diversity":15,"density":5,"covered_api":["cmsIT8GetPatchName","cmsGetHeaderManufacturer","cmsPipelineGetPtrToFirstStage","cmsIsToneCurveMonotonic","cmsBuildSegmentedToneCurve","cmsIT8GetProperty","cmsCreate_sRGBProfile","cmsIsIntentSupported"],"uncovered_api":[],"unique_branches":{"cmsIsToneCurveMonotonic":[[1352,9,1352,14,1],[1357,9,1357,20,1],[1374,31,1374,37,0],[1374,31,1374,37,1],[1376,17,1376,42,1]],"AllocateToneCurveStruct":[[286,42,286,75,1]],"EvalSegmentedFn":[[730,40,730,64,1]]},"library_calls":["cmsCreate_sRGBProfile","cmsGetHeaderManufacturer","cmsIT8Alloc","cmsIT8SetDataFormat","cmsIT8GetProperty","cmsIT8GetPatchName","cmsBuildSegmentedToneCurve","cmsIsToneCurveMonotonic","cmsPipelineAlloc","cmsPipelineGetPtrToFirstStage","cmsIsIntentSupported","cmsPipelineFree","cmsCloseProfile","cmsIT8Free","cmsFreeToneCurve"]}
/**/


extern "C" int LLVMFuzzerTestOneInput_100(const uint8_t* f_data, size_t f_size) {
	if(f_size<=31) return 0;

	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(f_data, sizeof(uint8_t), f_size, input_file_ptr);
	fclose(input_file_ptr);

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
	//fuzzer shim end}




    // Create profile handle
    cmsHPROFILE hProfile = cmsCreate_sRGBProfile();
    
    // Get header manufacturer
    cmsUInt32Number manufacturer = cmsGetHeaderManufacturer(hProfile);
    
    // Create IT8 handle
    cmsHANDLE hIT8 = cmsIT8Alloc(NULL);
    
    // Set IT8 data format
    cmsIT8SetDataFormat(hIT8, fuzz_int32_t_2, fuzz_str_1);
    
    // Get IT8 property
    const char *property = cmsIT8GetProperty(hIT8, fuzz_str_3);
    
    // Get patch name
    char buffer[256];
    const char *patchName = cmsIT8GetPatchName(hIT8, 0, buffer);
    
    // Create segmented tone curve
    cmsCurveSegment segments[2];
    cmsToneCurve *toneCurve = cmsBuildSegmentedToneCurve(NULL, 2, segments);
    
    // Check if tone curve is monotonic
    cmsBool isMonotonic = cmsIsToneCurveMonotonic(toneCurve);
    
    // Create pipeline
    cmsPipeline *pipeline = cmsPipelineAlloc(NULL, 3, 3);
    
    // Get pointer to first stage in pipeline
    cmsStage *firstStage = cmsPipelineGetPtrToFirstStage(pipeline);
    
    // Check if intent is supported
    cmsBool isIntentSupported = cmsIsIntentSupported(hProfile, 0, 0);
    
    // Use the data and size parameters passed to LLVMFuzzerTestOneInput_100
    FILE *in_file = fmemopen((void *)data, size, "rb");
    FILE *out_file = fopen("output_file", "wb");
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);
    const char *input_file = "input_file";
    const char *output_file = "output_file";
    
    // Release resources
    assert_file_closed(&out_file);;
    assert_file_closed(&in_file);;
    cmsPipelineFree(pipeline);
    cmsCloseProfile(hProfile);
    cmsIT8Free(hIT8);
    cmsFreeToneCurve(toneCurve);
    
    assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}