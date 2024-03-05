#include "FDSan.h"
#include "FuzzedDataProvider.h"
#include <lcms2.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
//<ID> 4270
//<Prompt> ["cmsMLUtranslationsCodes","cmsIT8EnumDataFormat","cmsDupContext","cmsIT8EnumPropertyMulti","cmsSmoothToneCurve","cmsTagLinkedTo"]
/*<Combination>: [cmsBool cmsMLUtranslationsCodes(const cmsMLU * mlu, cmsUInt32Number idx, char LanguageCode[3], char CountryCode[3]),
    int cmsIT8EnumDataFormat(cmsHANDLE hIT8, char *** SampleNames),
    cmsContext cmsDupContext(cmsContext ContextID, void * NewUserData),
    cmsUInt32Number cmsIT8EnumPropertyMulti(cmsHANDLE hIT8, const char * cProp, const char *** SubpropertyNames),
    cmsBool cmsSmoothToneCurve(cmsToneCurve * Tab, cmsFloat64Number lambda),
    cmsTagSignature cmsTagLinkedTo(cmsHPROFILE hProfile, cmsTagSignature sig)
*/
//<score> 0.6, nr_unique_branch: 2, p_cov: 0.33333334
//<Quality> {"diversity":15,"density":9,"covered_api":["cmsSmoothToneCurve","cmsTagLinkedTo"],"uncovered_api":["cmsMLUtranslationsCodes","cmsIT8EnumDataFormat","cmsDupContext","cmsIT8EnumPropertyMulti"],"unique_branches":{"cmsWriteTag":[[1880,9,1880,33,0]],"cmsMLUdup":[[283,37,283,58,0]]},"library_calls":["cmsCreateContext","cmsOpenProfileFromMem","cmsGetColorSpace","cmsBuildGamma","cmsIsToneCurveLinear","cmsSmoothToneCurve","cmsTagLinkedTo","cmsWriteTag","cmsFreeToneCurve","cmsCloseProfile","cmsDeleteContext"]}
/**/

extern "C" int LLVMFuzzerTestOneInput_35(const uint8_t* f_data, size_t f_size) {
	if(f_size<=0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




  // Create a context
  cmsContext ctx = cmsCreateContext(nullptr, nullptr);
  
  // Open a profile from memory
  cmsHPROFILE profile = cmsOpenProfileFromMem(data, size);
  if (!profile) {
    // Clean up and return
    cmsDeleteContext(ctx);
    return 0;
  }
  
  // Get the color space of the profile
  cmsColorSpaceSignature colorSpace = cmsGetColorSpace(profile);
  
  // Create a tone curve
  cmsToneCurve *toneCurve = cmsBuildGamma(ctx, 2.2);
  
  // Check if the tone curve is linear
  cmsBool isLinear = cmsIsToneCurveLinear(toneCurve);
  
  // Smooth the tone curve
  cmsBool smoothed = cmsSmoothToneCurve(toneCurve, 0.5);
  
  // Get the tag linked to the profile
  cmsTagSignature linkedTag = cmsTagLinkedTo(profile, cmsSigProfileDescriptionTag);
  
  // Create a file to write the output
  FILE *outFile = fopen("output_file", "wb");
  if (!outFile) {
    // Clean up and return
    cmsFreeToneCurve(toneCurve);
    cmsCloseProfile(profile);
    cmsDeleteContext(ctx);
    assert_file_name_closed("output_file");
	assert_file_closed(&outFile);
	return 0;
  }
  
  // Write the profile to the output file
  cmsBool writeSuccess = cmsWriteTag(profile, cmsSigProfileDescriptionTag, outFile);
  
  // Close the output file
  assert_file_closed(&outFile);;
  
  // Clean up
  cmsFreeToneCurve(toneCurve);
  cmsCloseProfile(profile);
  cmsDeleteContext(ctx);

  assert_file_name_closed("output_file");
	assert_file_closed(&outFile);
	return 0;
}