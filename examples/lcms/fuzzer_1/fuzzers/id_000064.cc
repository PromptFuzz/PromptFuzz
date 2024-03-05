#include "FDSan.h"
#include "FuzzedDataProvider.h"
#include <lcms2.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
//<ID> 8000
//<Prompt> ["cmsGetSupportedIntentsTHR","cmsCreateGrayProfileTHR","cmsDetectDestinationBlackPoint","cmsGetHeaderFlags","cmsOpenProfileFromFile","cmsWhitePointFromTemp","cmsGetHeaderAttributes"]
/*<Combination>: [cmsUInt32Number cmsGetSupportedIntentsTHR(cmsContext ContextID, cmsUInt32Number nMax, cmsUInt32Number * Codes, char ** Descriptions),
    cmsHPROFILE cmsCreateGrayProfileTHR(cmsContext ContextID, const cmsCIExyY * WhitePoint, const cmsToneCurve * TransferFunction),
    cmsBool cmsDetectDestinationBlackPoint(cmsCIEXYZ * BlackPoint, cmsHPROFILE hProfile, cmsUInt32Number Intent, cmsUInt32Number dwFlags),
    cmsUInt32Number cmsGetHeaderFlags(cmsHPROFILE hProfile),
    cmsHPROFILE cmsOpenProfileFromFile(const char * ICCProfile, const char * sAccess),
    cmsBool cmsWhitePointFromTemp(cmsCIExyY * WhitePoint, cmsFloat64Number TempK),
    void cmsGetHeaderAttributes(cmsHPROFILE hProfile, cmsUInt64Number * Flags)
*/
//<score> 7.714286, nr_unique_branch: 8, p_cov: 0.85714287
//<Quality> {"diversity":11,"density":11,"covered_api":["cmsGetSupportedIntentsTHR","cmsCreateGrayProfileTHR","cmsDetectDestinationBlackPoint","cmsGetHeaderFlags","cmsWhitePointFromTemp","cmsGetHeaderAttributes"],"uncovered_api":["cmsOpenProfileFromFile"],"unique_branches":{"DecideCurveType":[[1249,9,1249,31,0]],"BuildGrayInputMatrixPipeline":[[163,9,163,24,1],[166,9,166,20,1],[169,9,169,45,1],[196,13,196,101,1],[197,13,197,114,1]],"cmsCreateGrayProfileTHR":[[265,9,265,25,0],[267,13,267,75,1]]},"library_calls":["cmsCreateContext","cmsGetSupportedIntentsTHR","cmsWhitePointFromTemp","cmsBuildTabulatedToneCurve16","cmsCreateGrayProfileTHR","cmsDetectDestinationBlackPoint","cmsGetHeaderFlags","cmsGetHeaderAttributes","cmsCloseProfile","cmsFreeToneCurve","cmsDeleteContext"]}
/*Here is the C++ program that achieves the desired event using the lcms library APIs:

*/


extern "C" int LLVMFuzzerTestOneInput_64(const uint8_t* f_data, size_t f_size) {
	if(f_size<=0) return 0;

	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(f_data, sizeof(uint8_t), f_size, input_file_ptr);
	fclose(input_file_ptr);

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Step 4: Create a FILE pointer for reading the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");

    // Step 5: Get the file descriptor for reading
    int in_fd = fuzz_fileno(in_file);

    // Step 6: Set the input file name
    const char *in_file_name = "input_file";

    // Step 7: Allocate memory for the output file name
    const char *out_file_name = "output_file";

    // Step 8: Create a FILE pointer for writing the output data
    FILE *out_file = fopen(out_file_name, "wb");

    // Step 9: Get the file descriptor for writing
    int out_fd = fuzz_fileno(out_file);

    // Step 10: Initialize the lcms context
    cmsContext ctx = cmsCreateContext(NULL, NULL);

    // Step 11: Call the lcms APIs
    cmsUInt32Number intents[100];
    char *descriptions[100];
    cmsUInt32Number numIntents = cmsGetSupportedIntentsTHR(ctx, 100, intents, descriptions);
    cmsCIExyY whitePoint;
    cmsFloat64Number tempK = 6500.0; // Assuming a temperature of 6500 Kelvin
    cmsBool whitePointSuccess = cmsWhitePointFromTemp(&whitePoint, tempK);
    cmsToneCurve *transferFunction = cmsBuildTabulatedToneCurve16(ctx, 256, NULL);
    cmsHPROFILE grayProfile = cmsCreateGrayProfileTHR(ctx, &whitePoint, transferFunction);
    cmsCIEXYZ blackPoint;
    cmsBool blackPointSuccess = cmsDetectDestinationBlackPoint(&blackPoint, grayProfile, intents[0], cmsGetHeaderFlags(grayProfile));
    cmsUInt64Number headerFlags;
    cmsGetHeaderAttributes(grayProfile, &headerFlags);

    // Step 12: Clean up and release resources
    cmsCloseProfile(grayProfile);
    cmsFreeToneCurve(transferFunction);
    cmsDeleteContext(ctx);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;

    assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}