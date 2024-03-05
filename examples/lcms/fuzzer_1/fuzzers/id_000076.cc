#include "FDSan.h"
#include "FuzzedDataProvider.h"
#include <lcms2.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
//<ID> 9000
//<Prompt> ["cmsDoTransformLineStride","cmsCreateLab4ProfileTHR","cmsFloat2LabEncodedV2","cmsIT8SetTableByLabel","_cmsICCcolorSpace"]
/*<Combination>: [void cmsDoTransformLineStride(cmsHTRANSFORM Transform, const void * InputBuffer, void * OutputBuffer, cmsUInt32Number PixelsPerLine, cmsUInt32Number LineCount, cmsUInt32Number BytesPerLineIn, cmsUInt32Number BytesPerLineOut, cmsUInt32Number BytesPerPlaneIn, cmsUInt32Number BytesPerPlaneOut),
    cmsHPROFILE cmsCreateLab4ProfileTHR(cmsContext ContextID, const cmsCIExyY * WhitePoint),
    void cmsFloat2LabEncodedV2(cmsUInt16Number wLab[3], const cmsCIELab * Lab),
    int cmsIT8SetTableByLabel(cmsHANDLE hIT8, const char * cSet, const char * cField, const char * ExpectedType),
    cmsColorSpaceSignature _cmsICCcolorSpace(int OurNotation)
*/
//<score> 3.2, nr_unique_branch: 3, p_cov: 0.8
//<Quality> {"diversity":11,"density":11,"covered_api":["cmsDoTransformLineStride","cmsCreateLab4ProfileTHR","cmsIT8SetTableByLabel","_cmsICCcolorSpace"],"uncovered_api":["cmsFloat2LabEncodedV2"],"unique_branches":{"AllocEmptyTransform":[[944,13,944,30,0],[944,34,944,52,0]],"IsProperColorSpace":[[1073,9,1073,25,0]]},"library_calls":["cmsCreateContext","cmsCreateLab4ProfileTHR","cmsIT8Alloc","cmsIT8SetTableByLabel","_cmsICCcolorSpace","cmsCreateTransformTHR","cmsDoTransformLineStride","cmsDeleteTransform","cmsIT8Free","cmsCloseProfile","cmsDeleteContext"]}
/**/

extern "C" int LLVMFuzzerTestOneInput_76(const uint8_t* f_data, size_t f_size) {
	if(f_size<=29) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
	//fuzzer shim end}




    // Create a lcms context
    cmsContext context = cmsCreateContext(nullptr, nullptr);

    // Create a LAB profile with a specific white point
    cmsCIExyY whitePoint = {0.3127, 0.3290, 100.0};
    cmsHPROFILE labProfile = cmsCreateLab4ProfileTHR(context, &whitePoint);

    // Create an IT8 handle
    cmsHANDLE it8Handle = cmsIT8Alloc(context);

    // Set a table by label in the IT8 handle
    cmsIT8SetTableByLabel(it8Handle, fuzz_str_1, fuzz_str_2, fuzz_str_3);

    // Get a color space signature from an ICC notation
    cmsColorSpaceSignature colorSpace = _cmsICCcolorSpace(123);

    // Create a transform
    cmsHTRANSFORM transform = cmsCreateTransformTHR(
        context, labProfile, colorSpace, labProfile, colorSpace, INTENT_PERCEPTUAL, 0);

    // Read the input data from a file
    FILE *in_file = fmemopen((void *)data, size, "rb");

    // Write the output data to a file
    FILE *out_file = fopen("output_file", "wb");

    // Read and write data using the transform
    uint8_t inputBuffer[1024];
    uint8_t outputBuffer[1024];

    size_t bytesRead = fread(inputBuffer, 1, sizeof(inputBuffer), in_file);
    size_t bytesToWrite = sizeof(outputBuffer);
    
    while (bytesRead > 0) {
        cmsUInt32Number pixelsPerLine = bytesRead / 3;
        cmsUInt32Number lineCount = 1;
        cmsUInt32Number bytesPerLineIn = bytesRead;
        cmsUInt32Number bytesPerLineOut = bytesToWrite;
        cmsUInt32Number bytesPerPlaneIn = bytesPerLineIn;
        cmsUInt32Number bytesPerPlaneOut = bytesPerLineOut;

        cmsDoTransformLineStride(transform, inputBuffer, outputBuffer, pixelsPerLine, lineCount,
                                 bytesPerLineIn, bytesPerLineOut, bytesPerPlaneIn, bytesPerPlaneOut);

        fwrite(outputBuffer, 1, bytesToWrite, out_file);

        bytesRead = fread(inputBuffer, 1, sizeof(inputBuffer), in_file);
    }

    // Clean up resources
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    cmsDeleteTransform(transform);
    cmsIT8Free(it8Handle);
    cmsCloseProfile(labProfile);
    cmsDeleteContext(context);

    assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}