#include "FDSan.h"
#include "FuzzedDataProvider.h"
#include <lcms2.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
//<ID> 7647
//<Prompt> ["cmsCreateXYZProfileTHR","cmsSetDeviceClass","cmsGetEncodedICCversion","cmsGetSupportedIntentsTHR","cmsPipelineEvalFloat","cmsAllocProfileSequenceDescription","cmsSetLogErrorHandler","cmsxyY2XYZ","cmsNamedColorCount","cmsDictGetEntryList"]
/*<Combination>: [cmsHPROFILE cmsCreateXYZProfileTHR(cmsContext ContextID),
    void cmsSetDeviceClass(cmsHPROFILE hProfile, cmsProfileClassSignature sig),
    cmsUInt32Number cmsGetEncodedICCversion(cmsHPROFILE hProfile),
    cmsUInt32Number cmsGetSupportedIntentsTHR(cmsContext ContextID, cmsUInt32Number nMax, cmsUInt32Number * Codes, char ** Descriptions),
    void cmsPipelineEvalFloat(const cmsFloat32Number * In, cmsFloat32Number * Out, const cmsPipeline * lut),
    cmsSEQ *cmsAllocProfileSequenceDescription(cmsContext ContextID, cmsUInt32Number n),
    void cmsSetLogErrorHandler(cmsLogErrorHandlerFunction Fn),
    void cmsxyY2XYZ(cmsCIEXYZ * Dest, const cmsCIExyY * Source),
    cmsUInt32Number cmsNamedColorCount(const cmsNAMEDCOLORLIST * v),
    const cmsDICTentry *cmsDictGetEntryList(cmsHANDLE hDict)
*/
//<score> 0.5538462, nr_unique_branch: 1, p_cov: 0.9
//<Quality> {"diversity":13,"density":4,"covered_api":["cmsCreateXYZProfileTHR","cmsSetDeviceClass","cmsGetEncodedICCversion","cmsPipelineEvalFloat","cmsAllocProfileSequenceDescription","cmsSetLogErrorHandler","cmsxyY2XYZ","cmsNamedColorCount","cmsDictGetEntryList"],"uncovered_api":["cmsGetSupportedIntentsTHR"],"unique_branches":{"cmsDictGetEntryList":[[988,9,988,21,0]]},"library_calls":["cmsCreateXYZProfileTHR","cmsSetDeviceClass","cmsGetEncodedICCversion","cmsPipelineAlloc","cmsPipelineEvalFloat","cmsAllocProfileSequenceDescription","cmsSetLogErrorHandler","cmsxyY2XYZ","cmsNamedColorCount","cmsDictGetEntryList","cmsCloseProfile","cmsPipelineFree","cmsFreeProfileSequenceDescription"]}
/**/


extern "C" int LLVMFuzzerTestOneInput_63(const uint8_t* f_data, size_t f_size) {
	if(f_size<=0) return 0;

	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(f_data, sizeof(uint8_t), f_size, input_file_ptr);
	fclose(input_file_ptr);

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Step 4: Open input file
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file == NULL) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Step 6: Define input file name
    const char *input_file = "input_file";

    // Step 1: Create XYZ profile
    cmsHPROFILE xyz_profile = cmsCreateXYZProfileTHR(NULL);
    if (xyz_profile == NULL) {
        assert_file_closed(&in_file);;
        assert_file_name_closed("input_file");
	assert_file_closed(&in_file);
	return 0;
    }

    // Step 2: Set profile class
    cmsProfileClassSignature class_signature = cmsSigDisplayClass;
    cmsSetDeviceClass(xyz_profile, class_signature);

    // Step 3: Get encoded ICC version
    cmsUInt32Number version = cmsGetEncodedICCversion(xyz_profile);

    // Step 5: Create color transform pipeline
    cmsPipeline *pipeline = cmsPipelineAlloc(NULL, 3, 3);
    if (pipeline == NULL) {
        assert_file_closed(&in_file);;
        assert_file_name_closed("input_file");
	assert_file_closed(&in_file);
	return 0;
    }

    // Step 7: Evaluate color transform pipeline
    cmsFloat32Number in[3] = {0.5f, 0.5f, 0.5f};
    cmsFloat32Number out[3] = {0};
    cmsPipelineEvalFloat(in, out, pipeline);

    // Step 8: Allocate profile sequence description
    cmsUInt32Number seq_size = 2;
    cmsSEQ *sequence = cmsAllocProfileSequenceDescription(NULL, seq_size);
    if (sequence == NULL) {
        assert_file_closed(&in_file);;
        assert_file_name_closed("input_file");
	assert_file_closed(&in_file);
	return 0;
    }

    // Step 9: Set log error handler
    cmsSetLogErrorHandler(NULL);

    // Step 10: Convert xyY to XYZ
    cmsCIEXYZ xyz;
    cmsCIExyY xyY = {0.3, 0.4, 1.0};
    cmsxyY2XYZ(&xyz, &xyY);

    // Step 11: Get named color count
    cmsNAMEDCOLORLIST *named_color_list = NULL;
    cmsUInt32Number named_color_count = cmsNamedColorCount(named_color_list);

    // Step 12: Get dictionary entry list
    cmsHANDLE hdict = NULL;
    const cmsDICTentry *entry_list = cmsDictGetEntryList(hdict);

    // Step 13: Close input file
    assert_file_closed(&in_file);;

    // Step 14: Define output file name
    const char *output_file = "output_file";

    // Step 15: Open output file
    FILE *out_file = fopen(output_file, "wb");
    if (out_file == NULL) {
        assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Step 16: Release allocated resources
    cmsCloseProfile(xyz_profile);
    cmsPipelineFree(pipeline);
    cmsFreeProfileSequenceDescription(sequence);
    assert_file_closed(&out_file);;

    assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}