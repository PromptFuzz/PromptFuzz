#include "FDSan.h"
#include <lcms2.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
//<ID> 5152
//<Prompt> ["cmsGetEncodedCMMversion","cmsSaveProfileToFile","cmsFloat2LabEncoded","cmsOpenIOhandlerFromFile","cmsDeltaE","cmsCreateLab4ProfileTHR","cmsSliceSpaceFloat"]
/*<Combination>: [int cmsGetEncodedCMMversion(),
    cmsBool cmsSaveProfileToFile(cmsHPROFILE hProfile, const char * FileName),
    void cmsFloat2LabEncoded(cmsUInt16Number wLab[3], const cmsCIELab * Lab),
    cmsIOHANDLER *cmsOpenIOhandlerFromFile(cmsContext ContextID, const char * FileName, const char * AccessMode),
    cmsFloat64Number cmsDeltaE(const cmsCIELab * Lab1, const cmsCIELab * Lab2),
    cmsHPROFILE cmsCreateLab4ProfileTHR(cmsContext ContextID, const cmsCIExyY * WhitePoint),
    cmsBool cmsSliceSpaceFloat(cmsUInt32Number nInputs, const cmsUInt32Number * clutPoints, cmsSAMPLERFLOAT Sampler, void * Cargo)
*/
//<score> 3.3061225, nr_unique_branch: 8, p_cov: 0.85714287
//<Quality> {"diversity":7,"density":3,"covered_api":["cmsGetEncodedCMMversion","cmsSaveProfileToFile","cmsFloat2LabEncoded","cmsDeltaE","cmsCreateLab4ProfileTHR","cmsSliceSpaceFloat"],"uncovered_api":["cmsOpenIOhandlerFromFile"],"unique_branches":{"FileTell":[[345,9,345,17,1]],"cmsSaveProfileToFile":[[1503,9,1503,19,1],[1508,9,1508,20,1]],"FileWrite":[[357,9,357,18,1]],"cmsOpenIOhandlerFromFile":[[392,9,392,17,0],[421,5,421,13,1],[439,5,439,13,0],[441,13,441,23,1]]},"library_calls":["cmsGetEncodedCMMversion","cmsCreateLab4ProfileTHR","cmsSaveProfileToFile","cmsFloat2LabEncoded","cmsDeltaE","cmsSliceSpaceFloat","cmsCloseProfile"]}
/**/


extern "C" int LLVMFuzzerTestOneInput_43(const uint8_t *data, size_t size) {
	if(size<=0) return 0;

	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);

    // Step 4: Open input file
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Step 5: Get file descriptor
    int in_fd = fuzz_fileno(in_file);

    // Step 6: Define input and output file names
    const char *input_file = "input_file";
    const char *output_file = "output_file";

    // Step 1: Get encoded CMM version
    int cmm_version = cmsGetEncodedCMMversion();
    printf("CMM version: %d\n", cmm_version);

    // Step 2: Save profile to file
    cmsHPROFILE hProfile = cmsCreateLab4ProfileTHR(NULL, NULL);
    cmsBool save_result = cmsSaveProfileToFile(hProfile, output_file);
    if (!save_result) {
        printf("Failed to save profile to file\n");
    }

    // Step 3: Convert float to Lab encoded
    cmsCIELab Lab = {0, 0, 0};
    cmsUInt16Number wLab[3];
    cmsFloat2LabEncoded(wLab, &Lab);
    printf("Lab: %u %u %u\n", wLab[0], wLab[1], wLab[2]);

    // Step 4: Open output file
    FILE *out_file = fopen(output_file, "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
    }

    // Step 5: Get file descriptor
    int out_fd = fuzz_fileno(out_file);

    // Step 6: Define input and output file names
    input_file = "input_file";
    output_file = "output_file";

    // Step 7: DeltaE calculation
    cmsCIELab Lab1 = {0, 0, 0};
    cmsCIELab Lab2 = {1, 1, 1};
    cmsFloat64Number deltaE = cmsDeltaE(&Lab1, &Lab2);
    printf("DeltaE: %f\n", deltaE);

    // Step 8: Slice Space Float
    cmsUInt32Number clutPoints[3] = {0, 0, 0};
    cmsSAMPLERFLOAT sampler = nullptr;
    void *cargo = nullptr;
    cmsBool slice_result = cmsSliceSpaceFloat(3, clutPoints, sampler, cargo);
    if (!slice_result) {
        printf("Slice space float failed\n");
    }

    // Step 5: Close input and output file
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;

    // Step 7: Release allocated resources
    cmsCloseProfile(hProfile);

    assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}