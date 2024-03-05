#include "FDSan.h"
#include <lcms2.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
//<ID> 12407
//<Prompt> ["cmsIT8GetPropertyDbl","cmsxyY2XYZ","cmsIT8GetProperty","cmsCreate_sRGBProfile","cmsIsIntentSupported"]
/*<Combination>: [cmsFloat64Number cmsIT8GetPropertyDbl(cmsHANDLE hIT8, const char * cProp),
    void cmsxyY2XYZ(cmsCIEXYZ * Dest, const cmsCIExyY * Source),
    const char *cmsIT8GetProperty(cmsHANDLE hIT8, const char * cProp),
    cmsHPROFILE cmsCreate_sRGBProfile(),
    cmsBool cmsIsIntentSupported(cmsHPROFILE hProfile, cmsUInt32Number Intent, cmsUInt32Number UsedDirection)
*/
//<score> 0.5555556, nr_unique_branch: 0, p_cov: 1
//<Quality> {"diversity":9,"density":5,"covered_api":["cmsIT8GetPropertyDbl","cmsxyY2XYZ","cmsIT8GetProperty","cmsCreate_sRGBProfile","cmsIsIntentSupported"],"uncovered_api":[],"unique_branches":{},"library_calls":["cmsIT8LoadFromFile","cmsIT8GetPropertyDbl","cmsxyY2XYZ","cmsIT8GetProperty","cmsCreate_sRGBProfile","cmsIsIntentSupported","cmsIT8Free","cmsCloseProfile"]}
/*Here is the completed C++ program that follows the instructions provided:

*/


extern "C" int LLVMFuzzerTestOneInput_102(const uint8_t *data, size_t size) {
	if(size<=0) return 0;

	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);

  // Create a FILE * variable to read the input data
  FILE *in_file = fmemopen((void *)data, size, "rb");
  if (!in_file) {
    assert_file_closed(&in_file);
	return 0;
  }
  // Create a FILE * variable to write output data
  FILE *out_file = fopen("output_file", "wb");
  if (!out_file) {
    assert_file_closed(&in_file);;
    assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
  }
  // Get the file descriptor for reading and writing
  int in_fd = fuzz_fileno(in_file);
  int out_fd = fuzz_fileno(out_file);

  // Example usage of lcms APIs
  cmsHANDLE hIT8 = cmsIT8LoadFromFile(nullptr, "input_file");
  if (!hIT8) {
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    assert_file_name_closed("input_file");
	assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
  }

  cmsFloat64Number propertyDbl = cmsIT8GetPropertyDbl(hIT8, "property");
  printf("Property value: %f\n", propertyDbl);

  cmsCIExyY source_xyY = {0.5, 0.4, 0.6};
  cmsCIEXYZ dest_XYZ;
  cmsxyY2XYZ(&dest_XYZ, &source_xyY);
  printf("XYZ values: %f, %f, %f\n", dest_XYZ.X, dest_XYZ.Y, dest_XYZ.Z);

  const char *property = cmsIT8GetProperty(hIT8, "property");
  printf("Property: %s\n", property);

  cmsHPROFILE sRGBProfile = cmsCreate_sRGBProfile();
  if (!sRGBProfile) {
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    cmsIT8Free(hIT8);
    assert_file_name_closed("input_file");
	assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
  }

  cmsBool isIntentSupported = cmsIsIntentSupported(sRGBProfile, 1, 2);
  printf("Is Intent Supported: %d\n", isIntentSupported);

  // Release all allocated resources
  assert_file_closed(&in_file);;
  assert_file_closed(&out_file);;
  cmsIT8Free(hIT8);
  cmsCloseProfile(sRGBProfile);

  assert_file_name_closed("input_file");
	assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}