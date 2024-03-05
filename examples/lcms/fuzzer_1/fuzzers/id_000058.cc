#include "FDSan.h"
#include "FuzzedDataProvider.h"
#include <lcms2.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
//<ID> 6968
//<Prompt> ["cmsPipelineStageCount","cmsTempFromWhitePoint","cmsIT8EnumProperties","cmsWriteRawTag","cmsCIE94DeltaE","cmsCreate_OkLabProfile","cmsEvalToneCurve16","cmsD50_XYZ"]
/*<Combination>: [cmsUInt32Number cmsPipelineStageCount(const cmsPipeline * lut),
    cmsBool cmsTempFromWhitePoint(cmsFloat64Number * TempK, const cmsCIExyY * WhitePoint),
    cmsUInt32Number cmsIT8EnumProperties(cmsHANDLE hIT8, char *** PropertyNames),
    cmsBool cmsWriteRawTag(cmsHPROFILE hProfile, cmsTagSignature sig, const void * data, cmsUInt32Number Size),
    cmsFloat64Number cmsCIE94DeltaE(const cmsCIELab * Lab1, const cmsCIELab * Lab2),
    cmsHPROFILE cmsCreate_OkLabProfile(cmsContext ctx),
    cmsUInt16Number cmsEvalToneCurve16(const cmsToneCurve * Curve, cmsUInt16Number v),
    const cmsCIEXYZ *cmsD50_XYZ()
*/
//<score> 2.5, nr_unique_branch: 4, p_cov: 0.5
//<Quality> {"diversity":7,"density":7,"covered_api":["cmsWriteRawTag","cmsCreate_OkLabProfile","cmsEvalToneCurve16","cmsD50_XYZ"],"uncovered_api":["cmsPipelineStageCount","cmsTempFromWhitePoint","cmsIT8EnumProperties","cmsCIE94DeltaE"],"unique_branches":{"freeOneTag":[[1564,13,1564,32,1]],"cmsWriteRawTag":[[2034,9,2034,55,1],[2036,9,2036,34,1],[2052,9,2052,32,1]]},"library_calls":["cmsCreate_OkLabProfile","cmsBuildGamma","cmsEvalToneCurve16","cmsD50_XYZ","cmsWriteRawTag","cmsCloseProfile","cmsFreeToneCurve"]}
/*To achieve the event using the provided lcms APIs, you can write the following C++ program:

*/


extern "C" int LLVMFuzzerTestOneInput_58(const uint8_t* f_data, size_t f_size)
{
	if(f_size<=2) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_1, fdp);
	//fuzzer shim end}




    // Create a color profile
    cmsContext ctx = NULL;
    cmsHPROFILE hProfile = cmsCreate_OkLabProfile(ctx);

    // Read the input data into a FILE *
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file == NULL) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Create a FILE * to write the output data
    FILE *out_file = fopen("output_file", "wb");
    if (out_file == NULL) {
        assert_file_closed(&in_file);;
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Get the file descriptor for reading
    int in_fd = fuzz_fileno(in_file);
    if (in_fd == -1) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
    }

    // Get the file descriptor for writing
    int out_fd = fuzz_fileno(out_file);
    if (out_fd == -1) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }

    // Create a tone curve
    cmsToneCurve *curve = cmsBuildGamma(ctx, 2.2);

    // Evaluate the tone curve
    cmsUInt16Number result = cmsEvalToneCurve16(curve, fuzz_uint16_t_1);

    // Get the D50 XYZ values
    const cmsCIEXYZ *d50_xyz = cmsD50_XYZ();

    // Write raw tag data
    cmsWriteRawTag(hProfile, cmsSigRedColorantTag, d50_xyz, sizeof(cmsCIEXYZ));

    // Close the profile
    cmsCloseProfile(hProfile);

    // Cleanup
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    cmsFreeToneCurve(curve);

    assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}