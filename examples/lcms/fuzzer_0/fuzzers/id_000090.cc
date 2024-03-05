#include "FDSan.h"
#include <lcms2.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
//<ID> 10509
//<Prompt> ["cmsIT8GetProperty","cmsSaveProfileToFile","cmsCreateGrayProfileTHR","cmsGetHeaderFlags","cmsEvalToneCurveFloat"]
/*<Combination>: [const char *cmsIT8GetProperty(cmsHANDLE hIT8, const char * cProp),
    cmsBool cmsSaveProfileToFile(cmsHPROFILE hProfile, const char * FileName),
    cmsHPROFILE cmsCreateGrayProfileTHR(cmsContext ContextID, const cmsCIExyY * WhitePoint, const cmsToneCurve * TransferFunction),
    cmsUInt32Number cmsGetHeaderFlags(cmsHPROFILE hProfile),
    cmsFloat32Number cmsEvalToneCurveFloat(const cmsToneCurve * Curve, cmsFloat32Number v)
*/
//<score> 0.7692308, nr_unique_branch: 0, p_cov: 1
//<Quality> {"diversity":13,"density":10,"covered_api":["cmsIT8GetProperty","cmsSaveProfileToFile","cmsCreateGrayProfileTHR","cmsGetHeaderFlags","cmsEvalToneCurveFloat"],"uncovered_api":[],"unique_branches":{},"library_calls":["cmsCreateContext","cmsCreateGrayProfileTHR","cmsSaveProfileToFile","cmsIT8Alloc","cmsIT8GetProperty","cmsIT8Free","cmsGetHeaderFlags","cmsBuildGamma","cmsEvalToneCurveFloat","cmsFreeToneCurve","cmsCloseProfile","cmsDeleteContext"]}
/**/


extern "C" int LLVMFuzzerTestOneInput_90(const uint8_t *data, size_t size) {
	if(size<=0) return 0;

  // Open input file
  FILE *in_file = fmemopen((void *)data, size, "rb");
  if (!in_file) {
    assert_file_closed(&in_file);
	return 0;
  }

  // Create a gray profile
  cmsContext context = cmsCreateContext(NULL, NULL);
  cmsHPROFILE profile = cmsCreateGrayProfileTHR(context, NULL, NULL);
  if (!profile) {
    assert_file_closed(&in_file);;
    cmsDeleteContext(context);
    assert_file_closed(&in_file);
	return 0;
  }

  // Save profile to file
  const char *output_file = "output_file";
  cmsSaveProfileToFile(profile, output_file);

  // Get a property from IT8 handle
  cmsHANDLE it8_handle = cmsIT8Alloc(context);
  const char *prop = cmsIT8GetProperty(it8_handle, "property");
  // Use the property, e.g. print it
  printf("Property: %s\n", prop);
  cmsIT8Free(it8_handle);

  // Get header flags of the profile
  cmsUInt32Number flags = cmsGetHeaderFlags(profile);
  printf("Header Flags: %u\n", flags);

  // Evaluate a tone curve
  cmsToneCurve *curve = cmsBuildGamma(NULL, 2.2);
  cmsFloat32Number value = 0.5;
  cmsFloat32Number eval_result = cmsEvalToneCurveFloat(curve, value);
  printf("Evaluated tone curve result: %f\n", eval_result);
  cmsFreeToneCurve(curve);

  // Close input file and release resources
  assert_file_closed(&in_file);;
  cmsCloseProfile(profile);
  cmsDeleteContext(context);

  assert_file_name_closed("output_file");
	assert_file_closed(&in_file);
	return 0;
}