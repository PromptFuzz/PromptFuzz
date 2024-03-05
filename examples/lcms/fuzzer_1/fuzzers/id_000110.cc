#include "FDSan.h"
#include "FuzzedDataProvider.h"
#include <lcms2.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
//<ID> 15279
//<Prompt> ["cmsIT8SetPropertyUncooked","cmsDupProfileSequenceDescription","cmsBuildTabulatedToneCurveFloat","cmsCIECAM02Init","cmsSetEncodedICCversion","cmsChannelsOf"]
/*<Combination>: [cmsBool cmsIT8SetPropertyUncooked(cmsHANDLE hIT8, const char * Key, const char * Buffer),
    cmsSEQ *cmsDupProfileSequenceDescription(const cmsSEQ * pseq),
    cmsToneCurve *cmsBuildTabulatedToneCurveFloat(cmsContext ContextID, cmsUInt32Number nEntries, const cmsFloat32Number * values),
    cmsHANDLE cmsCIECAM02Init(cmsContext ContextID, const cmsViewingConditions * pVC),
    void cmsSetEncodedICCversion(cmsHPROFILE hProfile, cmsUInt32Number Version),
    cmsUInt32Number cmsChannelsOf(cmsColorSpaceSignature ColorSpace)
*/
//<score> 0.6923077, nr_unique_branch: 2, p_cov: 1
//<Quality> {"diversity":13,"density":3,"covered_api":["cmsIT8SetPropertyUncooked","cmsDupProfileSequenceDescription","cmsBuildTabulatedToneCurveFloat","cmsCIECAM02Init","cmsSetEncodedICCversion","cmsChannelsOf"],"uncovered_api":[],"unique_branches":{"cmsCreateRGBProfileTHR":[[143,9,143,19,1],[154,9,154,19,1]]},"library_calls":["cmsIT8Alloc","cmsIT8SetPropertyUncooked","cmsDupProfileSequenceDescription","cmsBuildTabulatedToneCurveFloat","cmsCIECAM02Init","cmsCreateRGBProfile","cmsSetEncodedICCversion","cmsChannelsOf","cmsIT8Free","cmsFreeProfileSequenceDescription","cmsFreeToneCurve","cmsCIECAM02Done","cmsCloseProfile"]}
/**/


extern "C" int LLVMFuzzerTestOneInput_110(const uint8_t* f_data, size_t f_size)
{
	if(f_size<=4) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_1, fdp);
	//fuzzer shim end}




    // Step 4: Create a FILE pointer for reading input data
    FILE *in_file = fmemopen((void *)data, size, "rb");

    // Step 6: Directly use "output_file" as the file name for writing output data
    FILE *out_file = fopen("output_file", "wb");

    if (in_file == NULL || out_file == NULL) {
        // Error handling
        if (in_file != NULL) assert_file_closed(&in_file);;
        if (out_file != NULL) assert_file_closed(&out_file);;
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Step 5: Get file descriptors for reading and writing
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);

    // Step 2: Call lcms library APIs
    cmsHANDLE hIT8 = cmsIT8Alloc(NULL);
    cmsIT8SetPropertyUncooked(hIT8, "key", "buffer");

    cmsSEQ *pseq = cmsDupProfileSequenceDescription(NULL);

    const cmsFloat32Number values[3] = {0.0f, 0.5f, 1.0f};
    cmsToneCurve *curve = cmsBuildTabulatedToneCurveFloat(NULL, 3, values);

    const cmsViewingConditions vc = {/* initialize viewing conditions */};
    cmsHANDLE hCIECAM02 = cmsCIECAM02Init(NULL, &vc);

    cmsHPROFILE hProfile = cmsCreateRGBProfile(NULL, NULL, NULL);

    cmsSetEncodedICCversion(hProfile, fuzz_uint32_t_1);

    int channels = cmsChannelsOf(cmsSigLabData);

    // Step 7: Release allocated resources
    cmsIT8Free(hIT8);
    cmsFreeProfileSequenceDescription(pseq);
    cmsFreeToneCurve(curve);
    cmsCIECAM02Done(hCIECAM02);
    cmsCloseProfile(hProfile);

    // Step 3: Close the files
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;

    assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}