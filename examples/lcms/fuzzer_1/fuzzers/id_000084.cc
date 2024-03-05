#include "FDSan.h"
#include "FuzzedDataProvider.h"
#include <lcms2.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
//<ID> 9790
//<Prompt> ["cmsStageInputChannels","cmsIT8SetPropertyStr","cmsJoinToneCurve","cmsCreateRGBProfileTHR","cmsSetHeaderProfileID","cmsIT8SetPropertyHex","cmsIT8GetData","cmsIT8GetProperty","cmsDupProfileSequenceDescription"]
/*<Combination>: [cmsUInt32Number cmsStageInputChannels(const cmsStage * mpe),
    cmsBool cmsIT8SetPropertyStr(cmsHANDLE hIT8, const char * cProp, const char * Str),
    cmsToneCurve *cmsJoinToneCurve(cmsContext ContextID, const cmsToneCurve * X, const cmsToneCurve * Y, cmsUInt32Number nPoints),
    cmsHPROFILE cmsCreateRGBProfileTHR(cmsContext ContextID, const cmsCIExyY * WhitePoint, const cmsCIExyYTRIPLE * Primaries, cmsToneCurve *const TransferFunction[3]),
    void cmsSetHeaderProfileID(cmsHPROFILE hProfile, cmsUInt8Number * ProfileID),
    cmsBool cmsIT8SetPropertyHex(cmsHANDLE hIT8, const char * cProp, cmsUInt32Number Val),
    const char *cmsIT8GetData(cmsHANDLE hIT8, const char * cPatch, const char * cSample),
    const char *cmsIT8GetProperty(cmsHANDLE hIT8, const char * cProp),
    cmsSEQ *cmsDupProfileSequenceDescription(const cmsSEQ * pseq)
*/
//<score> 0.49122807, nr_unique_branch: 1, p_cov: 0.6666667
//<Quality> {"diversity":19,"density":7,"covered_api":["cmsStageInputChannels","cmsIT8SetPropertyStr","cmsJoinToneCurve","cmsIT8GetData","cmsIT8GetProperty","cmsDupProfileSequenceDescription"],"uncovered_api":["cmsCreateRGBProfileTHR","cmsSetHeaderProfileID","cmsIT8SetPropertyHex"],"unique_branches":{"cmsIT8GetProperty":[[1506,9,1506,70,0]]},"library_calls":["cmsStageAllocCLutFloat","cmsCreateContext","cmsStageInputChannels","cmsIT8Alloc","cmsCreateContext","cmsIT8SetPropertyStr","cmsCreateContext","cmsBuildGamma","cmsBuildGamma","cmsJoinToneCurve","cmsIT8GetData","cmsIT8GetProperty","cmsDupProfileSequenceDescription","cmsFreeToneCurve","cmsFreeToneCurve","cmsFreeToneCurve","cmsFreeProfileSequenceDescription","cmsIT8Free","cmsStageFree"]}
/**/


extern "C" int LLVMFuzzerTestOneInput_84(const uint8_t* f_data, size_t f_size) {
	if(f_size<=19) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
	//fuzzer shim end}




    // Step 4: Create input file
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Step 5: Get file descriptor for reading
    int in_fd = fuzz_fileno(in_file);

    // Step 6: Define output file name
    const char *out_file_name = "output_file";

    // Step 1: Call cmsStageInputChannels to get input channels
    cmsStage *stage = cmsStageAllocCLutFloat(cmsCreateContext(NULL, NULL), 3, 3, 3, nullptr);
    cmsUInt32Number inputChannels = cmsStageInputChannels(stage);

    // Step 2: Call cmsIT8SetPropertyStr to set a property string
    cmsHANDLE hIT8 = cmsIT8Alloc(cmsCreateContext(NULL, NULL));
    cmsBool propSet = cmsIT8SetPropertyStr(hIT8, fuzz_str_1, fuzz_str_2);

    // Step 3: Call cmsJoinToneCurve to join two tone curves
    cmsContext context = cmsCreateContext(NULL, NULL);
    cmsToneCurve *curve1 = cmsBuildGamma(context, 1.0);
    cmsToneCurve *curve2 = cmsBuildGamma(context, 2.0);
    cmsUInt32Number nPoints = 256;
    cmsToneCurve *joinedCurve = cmsJoinToneCurve(context, curve1, curve2, nPoints);

    // Step 7: Call cmsIT8GetData to get data from IT8 handle
    const char *patch = "patch";
    const char *sample = "sample";
    const char *dataValue = cmsIT8GetData(hIT8, patch, sample);

    // Step 7: Call cmsIT8GetProperty to get a property from IT8 handle
    const char *property = "property";
    const char *propValue = cmsIT8GetProperty(hIT8, property);

    // Step 7: Call cmsDupProfileSequenceDescription to duplicate profile sequence description
    cmsSEQ *profileSeq = cmsDupProfileSequenceDescription(nullptr);

    // Step 5: Create output file
    FILE *out_file = fopen(out_file_name, "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
    }

    // Step 5: Get file descriptor for writing
    int out_fd = fuzz_fileno(out_file);

    // Do something with input and output files

    // Step 4: Release input file resource
    assert_file_closed(&in_file);;

    // Step 4: Release output file resource
    assert_file_closed(&out_file);;

    // Step 7: Free all allocated resources
    cmsFreeToneCurve(curve1);
    cmsFreeToneCurve(curve2);
    cmsFreeToneCurve(joinedCurve);
    cmsFreeProfileSequenceDescription(profileSeq);
    cmsIT8Free(hIT8);
    cmsStageFree(stage);

    assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}