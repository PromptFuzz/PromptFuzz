#include "FDSan.h"
#include <lcms2.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
//<ID> 13955
//<Prompt> ["cmsMLUgetTranslation","_cmsLCMScolorSpace","cmsReadRawTag","cmsIT8GetPropertyMulti","cmsPipelineFree","cmsIT8EnumDataFormat"]
/*<Combination>: [cmsBool cmsMLUgetTranslation(const cmsMLU * mlu, const char LanguageCode[3], const char CountryCode[3], char ObtainedLanguage[3], char ObtainedCountry[3]),
    int _cmsLCMScolorSpace(cmsColorSpaceSignature ProfileSpace),
    cmsUInt32Number cmsReadRawTag(cmsHPROFILE hProfile, cmsTagSignature sig, void * Buffer, cmsUInt32Number BufferSize),
    const char *cmsIT8GetPropertyMulti(cmsHANDLE hIT8, const char * Key, const char * SubKey),
    void cmsPipelineFree(cmsPipeline * lut),
    int cmsIT8EnumDataFormat(cmsHANDLE hIT8, char *** SampleNames)
*/
//<score> 1.6666667, nr_unique_branch: 5, p_cov: 0.8333333
//<Quality> {"diversity":12,"density":4,"covered_api":["cmsMLUgetTranslation","cmsReadRawTag","cmsIT8GetPropertyMulti","cmsPipelineFree","cmsIT8EnumDataFormat"],"uncovered_api":["_cmsLCMScolorSpace"],"unique_branches":{"_cmsMLUgetWide":[[368,9,368,33,0],[369,9,369,33,0],[371,9,371,20,1]],"cmsMLUgetTranslation":[[470,9,470,20,1],[473,9,473,21,0]]},"library_calls":["cmsCreateXYZProfile","cmsReadRawTag","cmsMLUalloc","cmsMLUgetTranslation","cmsIT8Alloc","cmsIT8EnumDataFormat","cmsIT8GetPropertyMulti","cmsPipelineAlloc","cmsPipelineFree","cmsIT8Free","cmsMLUfree","cmsCloseProfile"]}
/**/


extern "C" int LLVMFuzzerTestOneInput_110(const uint8_t *data, size_t size) {
	if(size<=0) return 0;

  // Create a profile object
  cmsHPROFILE hProfile = cmsCreateXYZProfile();

  // Read raw tag
  cmsTagSignature tag = cmsSigMediaBlackPointTag;
  cmsUInt32Number bufferSize = 1024;
  void *buffer = malloc(bufferSize);
  cmsUInt32Number bytesRead = cmsReadRawTag(hProfile, tag, buffer, bufferSize);
  free(buffer);

  // Create an MLU object
  cmsMLU *mlu = cmsMLUalloc(NULL, 1);
  
  // Get translation from MLU
  const char languageCode[3] = "en";
  const char countryCode[3] = "US";
  char obtainedLanguage[3];
  char obtainedCountry[3];
  cmsBool translation = cmsMLUgetTranslation(mlu, languageCode, countryCode, obtainedLanguage, obtainedCountry);

  // Create an IT8 handle
  cmsHANDLE it8Handle = cmsIT8Alloc(NULL);
  
  // Enumerate data format in IT8
  char **sampleNames;
  int numSampleNames = cmsIT8EnumDataFormat(it8Handle, &sampleNames);

  // Get property from IT8
  const char *key = "Key";
  const char *subKey = "SubKey";
  const char *propertyValue = cmsIT8GetPropertyMulti(it8Handle, key, subKey);

  // Create a pipeline object
  cmsPipeline *lut = cmsPipelineAlloc(NULL, 3, 3);
  
  // Free the pipeline object
  cmsPipelineFree(lut);

  // Free the IT8 handle
  cmsIT8Free(it8Handle);
  
  // Destroy the MLU object
  cmsMLUfree(mlu);
  
  // Destroy the profile object
  cmsCloseProfile(hProfile);

  return 0;
}