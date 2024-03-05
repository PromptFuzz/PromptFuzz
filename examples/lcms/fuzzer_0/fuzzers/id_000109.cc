#include "FDSan.h"
#include <lcms2.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
//<ID> 13721
//<Prompt> ["cmsEvalToneCurveFloat","cmsReadTag","cmsMLUtranslationsCodes","cmsAllocProfileSequenceDescription","cmsChangeBuffersFormat"]
/*<Combination>: [cmsFloat32Number cmsEvalToneCurveFloat(const cmsToneCurve * Curve, cmsFloat32Number v),
    void *cmsReadTag(cmsHPROFILE hProfile, cmsTagSignature sig),
    cmsBool cmsMLUtranslationsCodes(const cmsMLU * mlu, cmsUInt32Number idx, char LanguageCode[3], char CountryCode[3]),
    cmsSEQ *cmsAllocProfileSequenceDescription(cmsContext ContextID, cmsUInt32Number n),
    cmsBool cmsChangeBuffersFormat(cmsHTRANSFORM hTransform, cmsUInt32Number InputFormat, cmsUInt32Number OutputFormat)
*/
//<score> 3.25, nr_unique_branch: 3, p_cov: 1
//<Quality> {"diversity":16,"density":13,"covered_api":["cmsEvalToneCurveFloat","cmsReadTag","cmsMLUtranslationsCodes","cmsAllocProfileSequenceDescription","cmsChangeBuffersFormat"],"uncovered_api":[],"unique_branches":{"cmsChangeBuffersFormat":[[1438,9,1438,67,1],[1447,9,1447,26,1],[1447,30,1447,46,1]]},"library_calls":["cmsCreateContext","cmsCreate_sRGBProfileTHR","cmsReadTag","cmsMLUalloc","cmsMLUtranslationsCodes","cmsAllocProfileSequenceDescription","cmsCreateTransformTHR","cmsChangeBuffersFormat","cmsBuildGamma","cmsEvalToneCurveFloat","cmsCloseProfile","cmsFreeToneCurve","cmsFreeProfileSequenceDescription","cmsMLUfree","cmsDeleteTransform","cmsDeleteContext"]}
/**/

extern "C" int LLVMFuzzerTestOneInput_109(const uint8_t *data, size_t size) {
	if(size<=0) return 0;

  // Create a context
  cmsContext context = cmsCreateContext(NULL, NULL);

  // Create a profile
  cmsHPROFILE profile = cmsCreate_sRGBProfileTHR(context);

  // Read a tag from the profile
  void *tag = cmsReadTag(profile, cmsSigAToB0Tag);

  // Allocate an MLU
  cmsMLU *mlu = cmsMLUalloc(NULL, 16);

  // Get translations codes from the MLU
  char languageCode[3];
  char countryCode[3];
  cmsBool translations = cmsMLUtranslationsCodes(mlu, 0, languageCode, countryCode);

  // Allocate a profile sequence description
  cmsSEQ *seq = cmsAllocProfileSequenceDescription(context, 3);

  // Change input and output format buffers of a transform
  cmsHTRANSFORM transform = cmsCreateTransformTHR(context, profile, TYPE_RGB_8, profile, TYPE_RGB_FLT, INTENT_RELATIVE_COLORIMETRIC, 0);
  cmsBool formatChanged = cmsChangeBuffersFormat(transform, TYPE_RGB_8, TYPE_RGB_FLT);

  // Evaluate a tone curve
  cmsToneCurve *curve = cmsBuildGamma(context, 2.2);
  cmsFloat32Number evaluatedValue = cmsEvalToneCurveFloat(curve, 0.5);

  // Release resources
  cmsCloseProfile(profile);
  cmsFreeToneCurve(curve);
  cmsFreeProfileSequenceDescription(seq);
  cmsMLUfree(mlu);
  cmsDeleteTransform(transform);
  cmsDeleteContext(context);

  return 0;
}