#include "FDSan.h"
#include <lcms2.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
//<ID> 8918
//<Prompt> ["cmsCreateLab4ProfileTHR","cmsFloat2LabEncodedV2","cmsIT8SetTableByLabel","cmsCMCdeltaE","_cmsICCcolorSpace"]
/*<Combination>: [cmsHPROFILE cmsCreateLab4ProfileTHR(cmsContext ContextID, const cmsCIExyY * WhitePoint),
    void cmsFloat2LabEncodedV2(cmsUInt16Number wLab[3], const cmsCIELab * Lab),
    int cmsIT8SetTableByLabel(cmsHANDLE hIT8, const char * cSet, const char * cField, const char * ExpectedType),
    cmsFloat64Number cmsCMCdeltaE(const cmsCIELab * Lab1, const cmsCIELab * Lab2, cmsFloat64Number l, cmsFloat64Number c),
    cmsColorSpaceSignature _cmsICCcolorSpace(int OurNotation)
*/
//<score> 3.2727273, nr_unique_branch: 3, p_cov: 1
//<Quality> {"diversity":11,"density":9,"covered_api":["cmsCreateLab4ProfileTHR","cmsFloat2LabEncodedV2","cmsIT8SetTableByLabel","cmsCMCdeltaE","_cmsICCcolorSpace"],"uncovered_api":[],"unique_branches":{"cmsCMCdeltaE":[[553,7,553,20,1],[564,7,564,32,1],[577,8,577,18,1]]},"library_calls":["cmsCreateContext","cmsCreateLab4ProfileTHR","cmsFloat2LabEncodedV2","cmsIT8Alloc","cmsIT8SetTableByLabel","cmsCMCdeltaE","_cmsICCcolorSpace","cmsGetColorSpace","cmsIT8Free","cmsCloseProfile","cmsDeleteContext"]}
/**/


extern "C" int LLVMFuzzerTestOneInput_76(const uint8_t *data, size_t size) {
	if(size<=0) return 0;

  // Create a context
  cmsContext context = cmsCreateContext(NULL, NULL);
  
  // Create a Lab color profile
  cmsCIExyY whitePoint = {0.3457, 0.3585, 1.0};
  cmsHPROFILE labProfile = cmsCreateLab4ProfileTHR(context, &whitePoint);
  
  // Convert input data to Lab color using the Lab profile
  cmsUInt16Number wLab[3];
  cmsCIELab inputLab;
  cmsFloat64Number l, c, deltaE;
  
  // Read Lab values from input data
  FILE *in_file = fmemopen((void *)data, size, "rb");
  fread(wLab, sizeof(cmsUInt16Number), 3, in_file);
  cmsFloat2LabEncodedV2(wLab, &inputLab);
  assert_file_closed(&in_file);;
  
  // Set up IT8 table by label
  cmsHANDLE it8Handle = cmsIT8Alloc(context);
  cmsIT8SetTableByLabel(it8Handle, "set", "field", "ExpectedType");
  
  // Calculate deltaE using CMC formula
  cmsCIELab refLab = {50.0, 40.0, 30.0};
  l = 2.0;
  c = 1.5;
  deltaE = cmsCMCdeltaE(&refLab, &inputLab, l, c);
  
  // Get ICC color space for Lab profile
  cmsColorSpaceSignature colorSpace = _cmsICCcolorSpace(cmsGetColorSpace(labProfile));
  
  // Print output values
  printf("Lab values: L=%f, a=%f, b=%f\n", inputLab.L, inputLab.a, inputLab.b);
  printf("DeltaE: %f\n", deltaE);
  printf("ICC color space: %d\n", colorSpace);
  
  // Clean up
  cmsIT8Free(it8Handle);
  cmsCloseProfile(labProfile);
  cmsDeleteContext(context);
  
  assert_file_closed(&in_file);
	return 0;
}