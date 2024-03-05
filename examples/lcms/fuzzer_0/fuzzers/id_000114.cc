#include "FDSan.h"
#include <lcms2.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
//<ID> 14486
//<Prompt> ["cmsBFDdeltaE","cmsGetHeaderCreator","cmsCreateRGBProfileTHR","cmsStageInputChannels","cmsDetectRGBProfileGamma"]
/*<Combination>: [cmsFloat64Number cmsBFDdeltaE(const cmsCIELab * Lab1, const cmsCIELab * Lab2),
    cmsUInt32Number cmsGetHeaderCreator(cmsHPROFILE hProfile),
    cmsHPROFILE cmsCreateRGBProfileTHR(cmsContext ContextID, const cmsCIExyY * WhitePoint, const cmsCIExyYTRIPLE * Primaries, cmsToneCurve *const TransferFunction[3]),
    cmsUInt32Number cmsStageInputChannels(const cmsStage * mpe),
    cmsFloat64Number cmsDetectRGBProfileGamma(cmsHPROFILE hProfile, cmsFloat64Number threshold)
*/
//<score> 1.2800001, nr_unique_branch: 3, p_cov: 0.4
//<Quality> {"diversity":5,"density":4,"covered_api":["cmsBFDdeltaE","cmsCreateRGBProfileTHR"],"uncovered_api":["cmsGetHeaderCreator","cmsStageInputChannels","cmsDetectRGBProfileGamma"],"unique_branches":{"cmsBFDdeltaE":[[517,9,517,51,0]],"ComputeLBFD":[[486,7,486,24,0]],"atan2deg":[[325,18,325,24,1]]},"library_calls":["cmsCreateContext","cmsCreateRGBProfileTHR","cmsBFDdeltaE","cmsCloseProfile","cmsDeleteContext"]}
/**/

extern "C" int LLVMFuzzerTestOneInput_114(const uint8_t *data, size_t size) {
	if(size<=0) return 0;

  // Create a context
  cmsContext ctx = cmsCreateContext(NULL, NULL);

  // Create a RGB profile
  cmsCIExyY whitePoint = {0.3127, 0.3290, 1.0};
  cmsCIExyYTRIPLE primaries = {{0.64, 0.33}, {0.3, 0.6}, {0.15, 0.06}};
  cmsToneCurve* transferFunction[3] = {NULL, NULL, NULL};
  cmsHPROFILE hProfile = cmsCreateRGBProfileTHR(ctx, &whitePoint, &primaries, transferFunction);

  // Read Lab1 and Lab2 from input data
  cmsCIELab Lab1, Lab2;
  if (size < sizeof(cmsCIELab) * 2) {
    return 0;
  }
  memcpy(&Lab1, data, sizeof(cmsCIELab));
  memcpy(&Lab2, data + sizeof(cmsCIELab), sizeof(cmsCIELab));

  // Calculate deltaE
  cmsFloat64Number deltaE = cmsBFDdeltaE(&Lab1, &Lab2);

  // Create output file
  FILE* out_file = fopen("output_file", "wb");
  if (!out_file) {
    assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	return 0;
  }

  // Write deltaE to output file
  fwrite(&deltaE, sizeof(cmsFloat64Number), 1, out_file);

  // Close the output file
  assert_file_closed(&out_file);;

  // Release resources
  cmsCloseProfile(hProfile);
  cmsDeleteContext(ctx);

  assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	return 0;
}