#include "FDSan.h"
#include <lcms2.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
//<ID> 75
//<Prompt> ["cmsFreeToneCurve","cmsCreateProofingTransform","cmsChangeBuffersFormat","cmsGetProfileInfoASCII","cmsFloat2LabEncodedV2"]
/*<Combination>: [void cmsFreeToneCurve(cmsToneCurve * Curve),
    cmsHTRANSFORM cmsCreateProofingTransform(cmsHPROFILE Input, cmsUInt32Number InputFormat, cmsHPROFILE Output, cmsUInt32Number OutputFormat, cmsHPROFILE Proofing, cmsUInt32Number Intent, cmsUInt32Number ProofingIntent, cmsUInt32Number dwFlags),
    cmsBool cmsChangeBuffersFormat(cmsHTRANSFORM hTransform, cmsUInt32Number InputFormat, cmsUInt32Number OutputFormat),
    cmsUInt32Number cmsGetProfileInfoASCII(cmsHPROFILE hProfile, cmsInfoType Info, const char LanguageCode[3], const char CountryCode[3], char * Buffer, cmsUInt32Number BufferSize),
    void cmsFloat2LabEncodedV2(cmsUInt16Number wLab[3], const cmsCIELab * Lab)
*/
//<score> 1.5384616, nr_unique_branch: 1, p_cov: 1
//<Quality> {"diversity":13,"density":10,"covered_api":["cmsFreeToneCurve","cmsCreateProofingTransform","cmsChangeBuffersFormat","cmsGetProfileInfoASCII","cmsFloat2LabEncodedV2"],"uncovered_api":[],"unique_branches":{"cmsGetProfileInfoASCII":[[1026,9,1026,20,0]]},"library_calls":["cmsCreate_sRGBProfileTHR","cmsCreateLab4ProfileTHR","cmsOpenProfileFromFile","cmsCreateProofingTransform","cmsChangeBuffersFormat","cmsGetProfileInfoASCII","cmsFloat2LabEncodedV2","cmsBuildTabulatedToneCurve16","cmsFreeToneCurve","cmsCloseProfile","cmsCloseProfile","cmsCloseProfile","cmsDeleteTransform"]}
/**/

extern "C" int LLVMFuzzerTestOneInput_0(const uint8_t *data, size_t size) {
	if(size<=0) return 0;

  // Step 4: Create a FILE * variable to read the input data
  FILE *in_file = fmemopen((void *)data, size, "rb");

  // Step 6: Define the output file name
  const char *output_file = "output_file";

  // Step 7: Open the output file for writing
  FILE *out_file = fopen(output_file, "wb");

  // Step 2: Call lcms APIs to achieve the desired event
  // Call cmsCreateProofingTransform
  cmsHPROFILE inputProfile = cmsCreate_sRGBProfileTHR(nullptr);
  cmsHPROFILE outputProfile = cmsCreateLab4ProfileTHR(nullptr, nullptr);
  cmsHPROFILE proofingProfile = cmsOpenProfileFromFile("proofing.icc", "r");
  cmsHTRANSFORM transform = cmsCreateProofingTransform(inputProfile, TYPE_BGR_8, outputProfile, TYPE_Lab_16, proofingProfile, INTENT_PERCEPTUAL, INTENT_ABSOLUTE_COLORIMETRIC, 0);

  // Call cmsChangeBuffersFormat
  cmsChangeBuffersFormat(transform, TYPE_BGR_8, TYPE_Lab_16);

  // Call cmsGetProfileInfoASCII
  char infoBuffer[256];
  cmsGetProfileInfoASCII(inputProfile, cmsInfoManufacturer, "en", "US", infoBuffer, sizeof(infoBuffer));

  // Call cmsFloat2LabEncodedV2
  cmsCIELab labValues;
  cmsUInt16Number encodedLab[3];
  cmsFloat2LabEncodedV2(encodedLab, &labValues);

  // Call cmsFreeToneCurve
  cmsToneCurve *toneCurve = cmsBuildTabulatedToneCurve16(nullptr, 256, nullptr);
  cmsFreeToneCurve(toneCurve);

  // Step 5: Get the file descriptors
  int in_fd = fuzz_fileno(in_file);
  int out_fd = fuzz_fileno(out_file);

  // Perform operations using the file descriptors

  // Step 7: Release allocated resources before return
  assert_file_closed(&out_file);;
  assert_file_closed(&in_file);;
  cmsCloseProfile(inputProfile);
  cmsCloseProfile(outputProfile);
  cmsCloseProfile(proofingProfile);
  cmsDeleteTransform(transform);

  assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}