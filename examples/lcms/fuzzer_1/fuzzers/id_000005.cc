#include "FDSan.h"
#include "FuzzedDataProvider.h"
#include <lcms2.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
//<ID> 750
//<Prompt> ["cmsGetHeaderModel","cmsFloat2XYZEncoded","cmsIT8SetPropertyUncooked","cmsGetHeaderRenderingIntent","cmsUnregisterPluginsTHR","cmsSetAlarmCodes"]
/*<Combination>: [cmsUInt32Number cmsGetHeaderModel(cmsHPROFILE hProfile),
    void cmsFloat2XYZEncoded(cmsUInt16Number XYZ[3], const cmsCIEXYZ * fXYZ),
    cmsBool cmsIT8SetPropertyUncooked(cmsHANDLE hIT8, const char * Key, const char * Buffer),
    cmsUInt32Number cmsGetHeaderRenderingIntent(cmsHPROFILE hProfile),
    void cmsUnregisterPluginsTHR(cmsContext ContextID),
    void cmsSetAlarmCodes(const cmsUInt16Number NewAlarm[16])
*/
//<score> 3.6363637, nr_unique_branch: 7, p_cov: 0.8333333
//<Quality> {"diversity":11,"density":6,"covered_api":["cmsGetHeaderModel","cmsFloat2XYZEncoded","cmsIT8SetPropertyUncooked","cmsGetHeaderRenderingIntent","cmsUnregisterPluginsTHR"],"uncovered_api":["cmsSetAlarmCodes"],"unique_branches":{"cmsFloat2XYZEncoded":[[383,9,383,19,0],[390,9,390,35,1],[393,9,393,18,1],[396,9,396,35,1],[399,9,399,18,1],[402,9,402,35,1],[405,9,405,18,1]]},"library_calls":["cmsCreateContext","cmsCreateLab4Profile","cmsGetHeaderModel","cmsFloat2XYZEncoded","cmsIT8Alloc","cmsIT8SetPropertyUncooked","cmsGetHeaderRenderingIntent","cmsUnregisterPluginsTHR","cmsDeleteContext","cmsCloseProfile","cmsIT8Free"]}
/**/

// Include lcms headers

// Define the fuzz driver function
extern "C" int LLVMFuzzerTestOneInput_5(const uint8_t* f_data, size_t f_size) {
	if(f_size<=0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Create a context for lcms
    cmsContext context = cmsCreateContext(nullptr, nullptr);

    // Create a dummy profile for testing
    cmsHPROFILE hProfile = cmsCreateLab4Profile(nullptr);

    // Read the input data as a file stream
    FILE *in_file = fmemopen((void *)data, size, "rb");

    // Get the file descriptor for reading
    int in_fd = fuzz_fileno(in_file);

    // Get the model from the profile
    cmsUInt32Number model = cmsGetHeaderModel(hProfile);

    // Convert XYZ color to encoded XYZ
    cmsCIEXYZ xyz;
    // Read XYZ color from the input file
    fread(&xyz, sizeof(cmsCIEXYZ), 1, in_file);
    // Convert XYZ to encoded XYZ
    cmsUInt16Number encodedXYZ[3];
    cmsFloat2XYZEncoded(encodedXYZ, &xyz);

    // Set an uncooked property in IT8 format
    cmsHANDLE hIT8 = cmsIT8Alloc(context);
    const char *key = "PropertyKey";
    const char *value = "PropertyValue";
    cmsBool result = cmsIT8SetPropertyUncooked(hIT8, key, value);

    // Get the rendering intent from the profile
    cmsUInt32Number renderingIntent = cmsGetHeaderRenderingIntent(hProfile);

    // Unregister all plugins
    cmsUnregisterPluginsTHR(context);

    // Write the output data to a file
    FILE *out_file = fopen("output_file", "wb");
    // Get the file descriptor for writing
    int out_fd = fuzz_fileno(out_file);
    // Write the encoded XYZ to the output file
    fwrite(encodedXYZ, sizeof(cmsUInt16Number), 3, out_file);

    // Close the file streams
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;

    // Release the resources
    cmsDeleteContext(context);
    cmsCloseProfile(hProfile);
    cmsIT8Free(hIT8);

    assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}