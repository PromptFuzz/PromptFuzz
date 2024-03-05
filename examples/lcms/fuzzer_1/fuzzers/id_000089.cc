#include "FDSan.h"
#include "FuzzedDataProvider.h"
#include <lcms2.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
//<ID> 10707
//<Prompt> ["cmsDoTransformStride","cmsIT8GetProperty","cmsCreateLinearizationDeviceLink","cmsCIE2000DeltaE","cmsEvalToneCurveFloat"]
/*<Combination>: [void cmsDoTransformStride(cmsHTRANSFORM Transform, const void * InputBuffer, void * OutputBuffer, cmsUInt32Number Size, cmsUInt32Number Stride),
    const char *cmsIT8GetProperty(cmsHANDLE hIT8, const char * cProp),
    cmsHPROFILE cmsCreateLinearizationDeviceLink(cmsColorSpaceSignature ColorSpace, cmsToneCurve *const * TransferFunctions),
    cmsFloat64Number cmsCIE2000DeltaE(const cmsCIELab * Lab1, const cmsCIELab * Lab2, cmsFloat64Number Kl, cmsFloat64Number Kc, cmsFloat64Number Kh),
    cmsFloat32Number cmsEvalToneCurveFloat(const cmsToneCurve * Curve, cmsFloat32Number v)
*/
//<score> 1.2307693, nr_unique_branch: 3, p_cov: 0.8
//<Quality> {"diversity":13,"density":5,"covered_api":["cmsIT8GetProperty","cmsCreateLinearizationDeviceLink","cmsCIE2000DeltaE","cmsEvalToneCurveFloat"],"uncovered_api":["cmsDoTransformStride"],"unique_branches":{"cmsCIE2000DeltaE":[[620,32,620,64,0],[624,32,624,61,1],[625,29,625,49,1]]},"library_calls":["cmsIT8LoadFromMem","cmsIT8GetProperty","cmsBuildGamma","cmsCreateLinearizationDeviceLink","cmsCIE2000DeltaE","cmsBuildGamma","cmsEvalToneCurveFloat","cmsIT8Free","cmsFreeToneCurve","cmsFreeToneCurve","cmsFreeToneCurve","cmsFreeToneCurve","cmsCloseProfile"]}
/*Here is the implementation of the fuzz driver:

*/


extern "C" int LLVMFuzzerTestOneInput_89(const uint8_t* f_data, size_t f_size) {
	if(f_size<=16) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
	//fuzzer shim end}




    // Create a file with the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Create a file for output
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Read the property from the IT8 handle
    cmsHANDLE hIT8 = cmsIT8LoadFromMem(NULL, data, size);
    if (!hIT8) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    const char *property = cmsIT8GetProperty(hIT8, fuzz_str_1);
    if (property) {
        fwrite(property, sizeof(char), strlen(property), out_file);
    }

    // Create a linearization device link
    cmsColorSpaceSignature colorSpace = cmsSigXYZData;
    cmsToneCurve *transferFunctions[3];
    for (int i = 0; i < 3; i++) {
        transferFunctions[i] = cmsBuildGamma(NULL, 1.0);
    }
    cmsHPROFILE hProfile = cmsCreateLinearizationDeviceLink(colorSpace, transferFunctions);

    // Calculate the CIE2000 Delta E
    cmsCIELab Lab1, Lab2;
    Lab1.L = 50;
    Lab1.a = 20;
    Lab1.b = 10;
    Lab2.L = 60;
    Lab2.a = -10;
    Lab2.b = 30;
    cmsFloat64Number Kl = 1.0;
    cmsFloat64Number Kc = 1.0;
    cmsFloat64Number Kh = 1.0;
    cmsFloat64Number deltaE = cmsCIE2000DeltaE(&Lab1, &Lab2, Kl, Kc, Kh);
    fprintf(out_file, "Delta E: %f\n", deltaE);

    // Evaluate a tone curve
    cmsToneCurve *curve = cmsBuildGamma(NULL, 1.0);
    cmsFloat32Number v = 0.5;
    cmsFloat32Number result = cmsEvalToneCurveFloat(curve, v);
    fprintf(out_file, "Result: %f\n", result);

    // Cleanup resources
    cmsIT8Free(hIT8);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    cmsFreeToneCurve(curve);
    cmsFreeToneCurve(transferFunctions[0]);
    cmsFreeToneCurve(transferFunctions[1]);
    cmsFreeToneCurve(transferFunctions[2]);
    cmsCloseProfile(hProfile);

    assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}