#include "FDSan.h"
#include "FuzzedDataProvider.h"
#include <lcms2.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
//<ID> 171
//<Prompt> ["cmsFreeToneCurve","cmsCreateProofingTransform","cmsChangeBuffersFormat","cmsGetProfileInfoASCII","cmsFloat2LabEncodedV2"]
/*<Combination>: [void cmsFreeToneCurve(cmsToneCurve * Curve),
    cmsHTRANSFORM cmsCreateProofingTransform(cmsHPROFILE Input, cmsUInt32Number InputFormat, cmsHPROFILE Output, cmsUInt32Number OutputFormat, cmsHPROFILE Proofing, cmsUInt32Number Intent, cmsUInt32Number ProofingIntent, cmsUInt32Number dwFlags),
    cmsBool cmsChangeBuffersFormat(cmsHTRANSFORM hTransform, cmsUInt32Number InputFormat, cmsUInt32Number OutputFormat),
    cmsUInt32Number cmsGetProfileInfoASCII(cmsHPROFILE hProfile, cmsInfoType Info, const char LanguageCode[3], const char CountryCode[3], char * Buffer, cmsUInt32Number BufferSize),
    void cmsFloat2LabEncodedV2(cmsUInt16Number wLab[3], const cmsCIELab * Lab)
*/
//<score> 1.4545455, nr_unique_branch: 1, p_cov: 1
//<Quality> {"diversity":11,"density":8,"covered_api":["cmsFreeToneCurve","cmsCreateProofingTransform","cmsChangeBuffersFormat","cmsGetProfileInfoASCII","cmsFloat2LabEncodedV2"],"uncovered_api":[],"unique_branches":{"cmsGetProfileInfoASCII":[[1026,9,1026,20,1]]},"library_calls":["cmsBuildGamma","cmsCreate_sRGBProfile","cmsCreateLab2ProfileTHR","cmsCreateProofingTransform","cmsChangeBuffersFormat","cmsGetProfileInfoASCII","cmsFloat2LabEncodedV2","cmsFreeToneCurve","cmsCloseProfile","cmsCloseProfile","cmsDeleteTransform"]}
/*Here is the completed C++ code that achieves the event described using the lcms library APIs:

*/


extern "C" int LLVMFuzzerTestOneInput_1(const uint8_t* f_data, size_t f_size) {
	if(f_size<=14) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
	//fuzzer shim end}




    FILE *in_file = fmemopen((void *)data, size, "rb");
    FILE *out_file = fopen("output_file", "wb");

    if (!in_file || !out_file) {
        // Failed to open files, return
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Read the input file using the file descriptor
    int in_fd = fuzz_fileno(in_file);
    // Write the output file using the file descriptor
    int out_fd = fuzz_fileno(out_file);

    // Use lcms library APIs to achieve the desired event
    cmsToneCurve *curve = cmsBuildGamma(NULL, 2.2);
    cmsHPROFILE inputProfile = cmsCreate_sRGBProfile();
    cmsHPROFILE outputProfile = cmsCreateLab2ProfileTHR(NULL, NULL);
    cmsHTRANSFORM transform = cmsCreateProofingTransform(inputProfile, TYPE_RGB_8, outputProfile, TYPE_Lab_16, NULL, INTENT_PERCEPTUAL, fuzz_uint32_t_1, 0);
    
    // Change the format of buffers
    cmsChangeBuffersFormat(transform, TYPE_RGB_8, TYPE_Lab_16);
    
    // Get profile info and store it in a buffer
    char buffer[256];
    cmsGetProfileInfoASCII(inputProfile, cmsInfoDescription, fuzz_str_2, fuzz_str_3, buffer, sizeof(buffer));
    
    // Convert Lab values to encoded values
    cmsCIELab Lab = {50, 0, 0};
    cmsUInt16Number wLab[3];
    cmsFloat2LabEncodedV2(wLab, &Lab);
    
    // Free allocated resources
    cmsFreeToneCurve(curve);
    cmsCloseProfile(inputProfile);
    cmsCloseProfile(outputProfile);
    cmsDeleteTransform(transform);
    
    // Clean up: close files
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;

    assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}