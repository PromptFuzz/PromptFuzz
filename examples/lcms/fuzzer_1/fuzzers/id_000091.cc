#include "FDSan.h"
#include "FuzzedDataProvider.h"
#include <lcms2.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
//<ID> 11386
//<Prompt> ["cmsFloat2LabEncodedV2","cmsNamedColorInfo","cmsIT8SetTable","cmsEstimateGamma","cmsPipelineOutputChannels","cmsIT8SaveToFile"]
/*<Combination>: [void cmsFloat2LabEncodedV2(cmsUInt16Number wLab[3], const cmsCIELab * Lab),
    cmsBool cmsNamedColorInfo(const cmsNAMEDCOLORLIST * NamedColorList, cmsUInt32Number nColor, char * Name, char * Prefix, char * Suffix, cmsUInt16Number * PCS, cmsUInt16Number * Colorant),
    cmsInt32Number cmsIT8SetTable(cmsHANDLE hIT8, cmsUInt32Number nTable),
    cmsFloat64Number cmsEstimateGamma(const cmsToneCurve * t, cmsFloat64Number Precision),
    cmsUInt32Number cmsPipelineOutputChannels(const cmsPipeline * lut),
    cmsBool cmsIT8SaveToFile(cmsHANDLE hIT8, const char * cFileName)
*/
//<score> 0.8571429, nr_unique_branch: 2, p_cov: 1
//<Quality> {"diversity":14,"density":4,"covered_api":["cmsFloat2LabEncodedV2","cmsNamedColorInfo","cmsIT8SetTable","cmsEstimateGamma","cmsPipelineOutputChannels","cmsIT8SaveToFile"],"uncovered_api":[],"unique_branches":{"cmsNamedColorInfo":[[658,9,658,31,1],[660,9,660,53,0]]},"library_calls":["cmsFloat2LabEncodedV2","cmsAllocNamedColorList","cmsNamedColorInfo","cmsIT8Alloc","cmsIT8SetTable","cmsBuildGamma","cmsEstimateGamma","cmsPipelineAlloc","cmsPipelineOutputChannels","cmsIT8SaveToFile","cmsIT8Free","cmsFreeToneCurve","cmsPipelineFree","cmsFreeNamedColorList"]}
/**/

extern "C"
int LLVMFuzzerTestOneInput_91(const uint8_t* f_data, size_t f_size) {
	if(f_size<=18) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
	//fuzzer shim end}




    FILE *in_file = fmemopen((void *)data, size, "rb");
    FILE *out_file = fopen("output_file", "wb");
    
    if (in_file == NULL || out_file == NULL) {
        if (in_file != NULL) assert_file_closed(&in_file);;
        if (out_file != NULL) assert_file_closed(&out_file);;
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Read the input data using file stream
    // ...

    cmsUInt16Number wLab[3];
    cmsCIELab Lab;
    cmsFloat2LabEncodedV2(wLab, &Lab);
    
    cmsNAMEDCOLORLIST *namedColorList = cmsAllocNamedColorList(NULL, 10, 3, fuzz_str_1, fuzz_str_2);
    char name[256], prefix[256], suffix[256];
    cmsUInt16Number pcs, colorant;
    cmsNamedColorInfo(namedColorList, 5, name, prefix, suffix, &pcs, &colorant);
    
    cmsHANDLE it8Handle = cmsIT8Alloc(NULL);
    cmsIT8SetTable(it8Handle, 2);
    
    cmsToneCurve *toneCurve = cmsBuildGamma(NULL, 2.2);
    cmsFloat64Number gamma = cmsEstimateGamma(toneCurve, 0.001);
    
    cmsPipeline *pipeline = cmsPipelineAlloc(NULL, 1, 1);
    cmsUInt32Number outputChannels = cmsPipelineOutputChannels(pipeline);
    
    cmsIT8SaveToFile(it8Handle, "output_file");
    
    // Write the output data using file stream
    // ...

    cmsIT8Free(it8Handle);
    cmsFreeToneCurve(toneCurve);
    cmsPipelineFree(pipeline);
    cmsFreeNamedColorList(namedColorList);
    
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_name_closed("output_file");
	assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}