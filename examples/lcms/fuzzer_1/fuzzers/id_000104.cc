#include "FDSan.h"
#include "FuzzedDataProvider.h"
#include <lcms2.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
//<ID> 14001
//<Prompt> ["_cmsLCMScolorSpace","cmsReadRawTag","cmsIT8GetPropertyMulti","cmsPipelineFree","cmsIT8EnumDataFormat"]
/*<Combination>: [int _cmsLCMScolorSpace(cmsColorSpaceSignature ProfileSpace),
    cmsUInt32Number cmsReadRawTag(cmsHPROFILE hProfile, cmsTagSignature sig, void * Buffer, cmsUInt32Number BufferSize),
    const char *cmsIT8GetPropertyMulti(cmsHANDLE hIT8, const char * Key, const char * SubKey),
    void cmsPipelineFree(cmsPipeline * lut),
    int cmsIT8EnumDataFormat(cmsHANDLE hIT8, char *** SampleNames)
*/
//<score> 3.2, nr_unique_branch: 3, p_cov: 0.8
//<Quality> {"diversity":15,"density":15,"covered_api":["_cmsLCMScolorSpace","cmsReadRawTag","cmsIT8GetPropertyMulti","cmsPipelineFree"],"uncovered_api":["cmsIT8EnumDataFormat"],"unique_branches":{"_cmsLCMScolorSpace":[[822,5,822,24,0],[858,5,858,27,0],[870,5,870,27,0]]},"library_calls":["cmsCreateContext","cmsOpenProfileFromMem","cmsReadRawTag","cmsReadRawTag","_cmsLCMScolorSpace","cmsGetPCS","cmsIT8LoadFromMem","cmsIT8GetPropertyMulti","cmsGetColorSpace","cmsPipelineAlloc","cmsPipelineFree","cmsCloseProfile","cmsIT8Free","cmsDeleteContext"]}
/**/


extern "C" int LLVMFuzzerTestOneInput_104(const uint8_t* f_data, size_t f_size) {
	if(f_size<=15) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
	//fuzzer shim end}




  cmsContext context = cmsCreateContext(NULL, NULL);
  cmsHPROFILE hProfile = NULL;
  cmsHANDLE hIT8 = NULL;
  cmsPipeline *lut = NULL;

  // Open profile from memory
  hProfile = cmsOpenProfileFromMem(data, size);
  if (hProfile == NULL) {
    cmsDeleteContext(context);
    return 0;
  }

  // Read raw tags
  cmsUInt32Number tagSize = cmsReadRawTag(hProfile, cmsSigProfileDescriptionTag,
                                          NULL, 0);
  if (tagSize > 0) {
    void *tagBuffer = malloc(tagSize);
    if (tagBuffer != NULL) {
      cmsReadRawTag(hProfile, cmsSigProfileDescriptionTag, tagBuffer, tagSize);
      free(tagBuffer);
    }
  }

  // Check the profile color space
  int profileSpace = _cmsLCMScolorSpace(cmsGetPCS(hProfile));

  // Open IT8 handle
  hIT8 = cmsIT8LoadFromMem(context, data, size);
  if (hIT8 != NULL) {
    // Get property value
    const char *propertyValue = cmsIT8GetPropertyMulti(hIT8, fuzz_str_1, fuzz_str_2);
  }

  // Create a color transform pipeline
  cmsUInt32Number inputFormat = cmsGetColorSpace(hProfile);
  cmsUInt32Number outputFormat = cmsSigLabData;
  lut = cmsPipelineAlloc(context, 3, 3);
  cmsPipelineFree(lut);

  cmsCloseProfile(hProfile);
  cmsIT8Free(hIT8);
  cmsDeleteContext(context);

  return 0;
}