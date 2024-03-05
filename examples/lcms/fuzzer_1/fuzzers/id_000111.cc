#include "FDSan.h"
#include "FuzzedDataProvider.h"
#include <lcms2.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
//<ID> 15572
//<Prompt> ["cmsIT8SetPropertyUncooked","cmsDupProfileSequenceDescription","cmsBuildTabulatedToneCurveFloat","cmsIT8GetDataRowColDbl","cmsDesaturateLab","cmsSetEncodedICCversion","cmsChannelsOf"]
/*<Combination>: [cmsBool cmsIT8SetPropertyUncooked(cmsHANDLE hIT8, const char * Key, const char * Buffer),
    cmsSEQ *cmsDupProfileSequenceDescription(const cmsSEQ * pseq),
    cmsToneCurve *cmsBuildTabulatedToneCurveFloat(cmsContext ContextID, cmsUInt32Number nEntries, const cmsFloat32Number * values),
    cmsFloat64Number cmsIT8GetDataRowColDbl(cmsHANDLE hIT8, int row, int col),
    cmsBool cmsDesaturateLab(cmsCIELab * Lab, double amax, double amin, double bmax, double bmin),
    void cmsSetEncodedICCversion(cmsHPROFILE hProfile, cmsUInt32Number Version),
    cmsUInt32Number cmsChannelsOf(cmsColorSpaceSignature ColorSpace)
*/
//<score> 3.1836734, nr_unique_branch: 3, p_cov: 0.85714287
//<Quality> {"diversity":14,"density":13,"covered_api":["cmsIT8SetPropertyUncooked","cmsBuildTabulatedToneCurveFloat","cmsIT8GetDataRowColDbl","cmsDesaturateLab","cmsSetEncodedICCversion","cmsChannelsOf"],"uncovered_api":["cmsDupProfileSequenceDescription"],"unique_branches":{"cmsDesaturateLab":[[534,28,534,41,1],[535,9,535,24,1],[535,28,535,41,1]]},"library_calls":["cmsCreateContext","cmsCreateProfilePlaceholder","cmsSetEncodedICCversion","cmsChannelsOf","cmsGetColorSpace","cmsBuildTabulatedToneCurveFloat","cmsIT8Alloc","cmsIT8SetPropertyUncooked","cmsIT8GetDataRowColDbl","cmsDesaturateLab","cmsCloseProfile","cmsIT8Free","cmsFreeToneCurve","cmsDeleteContext"]}
/**/



extern "C" int LLVMFuzzerTestOneInput_111(const uint8_t* f_data, size_t f_size) {
	if(f_size<=12) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
	//fuzzer shim end}




    cmsContext context = cmsCreateContext(NULL, NULL);
    
    // Create a FILE pointer for reading the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    
    // Create a placeholder profile
    cmsHPROFILE hProfile = cmsCreateProfilePlaceholder(context);
    
    // Set the encoded ICC version
    cmsSetEncodedICCversion(hProfile, fuzz_uint32_t_1);
    
    // Get the color space channels
    cmsUInt32Number channels = cmsChannelsOf(cmsGetColorSpace(hProfile));
    
    // Create a buffer for the values
    cmsFloat32Number* values = (cmsFloat32Number*)malloc(channels * sizeof(cmsFloat32Number));
    
    // Read the values from the input file
    fread(values, sizeof(cmsFloat32Number), channels, in_file);
    
    // Build the tabulated tone curve
    cmsToneCurve* curve = cmsBuildTabulatedToneCurveFloat(context, channels, values);
    
    // Create a handle for IT8 data
    cmsHANDLE hIT8 = cmsIT8Alloc(context);
    
    // Set a property in the IT8 data
    cmsIT8SetPropertyUncooked(hIT8, "Key", "Value");
    
    // Get a value from a specific row and column in the IT8 data
    cmsFloat64Number value = cmsIT8GetDataRowColDbl(hIT8, fuzz_int32_t_2, fuzz_int32_t_3);
    
    // Create Lab color
    cmsCIELab Lab = { 0.5, 0.2, -0.1 };
    
    // Desaturate the Lab color
    cmsDesaturateLab(&Lab, 0.8, 0.2, 0.0, -0.2);
    
    // Create a FILE pointer for writing the output data
    FILE *out_file = fopen("output_file", "wb");
    
    // Write the output data
    fwrite(&value, sizeof(cmsFloat64Number), 1, out_file);
    
    // Close the input and output files
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Release the allocated resources
    free(values);
    cmsCloseProfile(hProfile);
    cmsIT8Free(hIT8);
    cmsFreeToneCurve(curve);
    cmsDeleteContext(context);

    assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}