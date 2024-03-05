#include "FDSan.h"
#include "FuzzedDataProvider.h"
#include <lcms2.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
//<ID> 12400
//<Prompt> ["cmsIT8GetPropertyDbl","cmsxyY2XYZ","cmsIT8GetProperty","cmsCreate_sRGBProfile","cmsIsIntentSupported"]
/*<Combination>: [cmsFloat64Number cmsIT8GetPropertyDbl(cmsHANDLE hIT8, const char * cProp),
    void cmsxyY2XYZ(cmsCIEXYZ * Dest, const cmsCIExyY * Source),
    const char *cmsIT8GetProperty(cmsHANDLE hIT8, const char * cProp),
    cmsHPROFILE cmsCreate_sRGBProfile(),
    cmsBool cmsIsIntentSupported(cmsHPROFILE hProfile, cmsUInt32Number Intent, cmsUInt32Number UsedDirection)
*/
//<score> 1, nr_unique_branch: 1, p_cov: 1
//<Quality> {"diversity":8,"density":4,"covered_api":["cmsIT8GetPropertyDbl","cmsxyY2XYZ","cmsIT8GetProperty","cmsCreate_sRGBProfile","cmsIsIntentSupported"],"uncovered_api":[],"unique_branches":{"cmsIT8GetPropertyDbl":[[1518,9,1518,18,0]]},"library_calls":["cmsIT8LoadFromFile","cmsIT8GetPropertyDbl","cmsxyY2XYZ","cmsIT8GetProperty","cmsCreate_sRGBProfile","cmsIsIntentSupported","cmsIT8Free","cmsCloseProfile"]}
/**/


extern "C" int LLVMFuzzerTestOneInput_96(const uint8_t* f_data, size_t f_size) {
	if(f_size<=0) return 0;

	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(f_data, sizeof(uint8_t), f_size, input_file_ptr);
	fclose(input_file_ptr);

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Step 4: Create a FILE* variable for reading the input data
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Step 6: Specify the input file name
    const char* input_file = "input_file";

    // Step 2: Use lcms APIs to achieve the desired event
    cmsHANDLE hIT8 = cmsIT8LoadFromFile(NULL, input_file);
    if (!hIT8) {
        assert_file_name_closed("input_file");
	assert_file_closed(&in_file);
	return 0;
    }

    // Call cmsIT8GetPropertyDbl
    cmsFloat64Number propertyDbl = cmsIT8GetPropertyDbl(hIT8, "property_name");

    // Call cmsxyY2XYZ
    cmsCIEXYZ DestXYZ;
    cmsCIExyY SourcexyY;
    cmsxyY2XYZ(&DestXYZ, &SourcexyY);

    // Call cmsIT8GetProperty
    const char* property = cmsIT8GetProperty(hIT8, "property_name");

    // Call cmsCreate_sRGBProfile
    cmsHPROFILE sRGBProfile = cmsCreate_sRGBProfile();

    // Call cmsIsIntentSupported
    cmsBool isIntentSupported = cmsIsIntentSupported(sRGBProfile, 0, 0);

    // Step 7: Release allocated resources
    cmsIT8Free(hIT8);
    cmsCloseProfile(sRGBProfile);
    assert_file_closed(&in_file);;
    assert_file_name_closed("input_file");
	assert_file_closed(&in_file);
	return 0;
}