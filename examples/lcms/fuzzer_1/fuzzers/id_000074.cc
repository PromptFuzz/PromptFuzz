#include "FDSan.h"
#include "FuzzedDataProvider.h"
#include <lcms2.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
//<ID> 8958
//<Prompt> ["cmsDoTransformLineStride","cmsCreateLab4ProfileTHR","cmsFloat2LabEncodedV2","cmsFormatterForPCSOfProfile","cmsIT8SetTableByLabel","_cmsICCcolorSpace"]
/*<Combination>: [void cmsDoTransformLineStride(cmsHTRANSFORM Transform, const void * InputBuffer, void * OutputBuffer, cmsUInt32Number PixelsPerLine, cmsUInt32Number LineCount, cmsUInt32Number BytesPerLineIn, cmsUInt32Number BytesPerLineOut, cmsUInt32Number BytesPerPlaneIn, cmsUInt32Number BytesPerPlaneOut),
    cmsHPROFILE cmsCreateLab4ProfileTHR(cmsContext ContextID, const cmsCIExyY * WhitePoint),
    void cmsFloat2LabEncodedV2(cmsUInt16Number wLab[3], const cmsCIELab * Lab),
    cmsUInt32Number cmsFormatterForPCSOfProfile(cmsHPROFILE hProfile, cmsUInt32Number nBytes, cmsBool lIsFloat),
    int cmsIT8SetTableByLabel(cmsHANDLE hIT8, const char * cSet, const char * cField, const char * ExpectedType),
    cmsColorSpaceSignature _cmsICCcolorSpace(int OurNotation)
*/
//<score> 3, nr_unique_branch: 5, p_cov: 0.5
//<Quality> {"diversity":10,"density":10,"covered_api":["cmsDoTransformLineStride","cmsCreateLab4ProfileTHR","cmsFormatterForPCSOfProfile"],"uncovered_api":["cmsFloat2LabEncodedV2","cmsIT8SetTableByLabel","_cmsICCcolorSpace"],"unique_branches":{"UnrollLabFloatToFloat":[[709,31,709,44,1]],"PackLabFloatFromFloat":[[709,31,709,44,1]],"cmsGetTransformInputFormat":[[1416,9,1416,22,1]],"cmsFormatterForPCSOfProfile":[[3865,29,3865,37,0],[3868,9,3868,25,1]]},"library_calls":["cmsCreateContext","cmsCreateLab4ProfileTHR","cmsFormatterForPCSOfProfile","cmsCreateTransform","cmsGetTransformInputFormat","cmsGetTransformOutputFormat","cmsDoTransformLineStride","cmsDeleteTransform","cmsCloseProfile","cmsDeleteContext"]}
/**/

extern "C" int LLVMFuzzerTestOneInput_74(const uint8_t* f_data, size_t f_size) {
	if(f_size<=0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Set up input and output file descriptors
    FILE* in_file = fmemopen((void*)data, size, "rb");
    FILE* out_file = fopen("output_file", "wb");
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);

    // Create a lcms context
    cmsContext context = cmsCreateContext(nullptr, nullptr);
    if (!context) {
        // Handle error
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }

    // Create a LAB profile
    cmsCIExyY whitePoint = {0.3457, 0.3585, 1.0};
    cmsHPROFILE labProfile = cmsCreateLab4ProfileTHR(context, &whitePoint);

    // Convert LAB profile to encoded format
    cmsUInt32Number encodedFormat = cmsFormatterForPCSOfProfile(labProfile, 4, true);

    // Perform color transformation using the LAB profile
    cmsHTRANSFORM transform = cmsCreateTransform(
        labProfile, encodedFormat, labProfile, encodedFormat, INTENT_PERCEPTUAL, 0
    );
  
    // Calculate the required buffer sizes
    cmsUInt32Number inputBufferSize = cmsGetTransformInputFormat(transform);
    cmsUInt32Number outputBufferSize = cmsGetTransformOutputFormat(transform);

    // Allocate input and output buffers
    void* inputBuffer = malloc(inputBufferSize);
    void* outputBuffer = malloc(outputBufferSize);

    // Read input data from file
    fread(inputBuffer, 1, inputBufferSize, in_file);

    // Perform color transformation
    cmsDoTransformLineStride(
        transform, inputBuffer, outputBuffer,
        1, 1, inputBufferSize, outputBufferSize,
        inputBufferSize, outputBufferSize
    );

    // Write output data to file
    fwrite(outputBuffer, 1, outputBufferSize, out_file);

    // Clean up resources
    free(inputBuffer);
    free(outputBuffer);
    cmsDeleteTransform(transform);
    cmsCloseProfile(labProfile);
    cmsDeleteContext(context);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;

    assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}