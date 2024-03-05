#include "FDSan.h"
#include <lcms2.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
//<ID> 14539
//<Prompt> ["cmsGetProfileIOhandler","cmsGDBCompute","cmsIT8EnumDataFormat","cmsCreateGrayProfile","cmsGetHeaderFlags"]
/*<Combination>: [cmsIOHANDLER *cmsGetProfileIOhandler(cmsHPROFILE hProfile),
    cmsBool cmsGDBCompute(cmsHANDLE hGDB, cmsUInt32Number dwFlags),
    int cmsIT8EnumDataFormat(cmsHANDLE hIT8, char *** SampleNames),
    cmsHPROFILE cmsCreateGrayProfile(const cmsCIExyY * WhitePoint, const cmsToneCurve * TransferFunction),
    cmsUInt32Number cmsGetHeaderFlags(cmsHPROFILE hProfile)
*/
//<score> 0.2, nr_unique_branch: 0, p_cov: 0.2
//<Quality> {"diversity":3,"density":3,"covered_api":["cmsGetProfileIOhandler"],"uncovered_api":["cmsGDBCompute","cmsIT8EnumDataFormat","cmsCreateGrayProfile","cmsGetHeaderFlags"],"unique_branches":{},"library_calls":["cmsOpenProfileFromFile","cmsGetProfileIOhandler","cmsCloseProfile"]}
/**/

extern "C" int LLVMFuzzerTestOneInput_115(const uint8_t *data, size_t size) {
	if(size<=0) return 0;

	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);

    // Step 4: Open input file
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Step 5: Get file descriptor
    int in_fd = fuzz_fileno(in_file);
    
    // Step 6: Get file name
    const char *in_file_name = "input_file";
    
    // Step 1: Get profile IO handler
    cmsHPROFILE hProfile = cmsOpenProfileFromFile(in_file_name, "r");
    cmsIOHANDLER *profileIOhandler = cmsGetProfileIOhandler(hProfile);
    
    // Step 7: Release resources
    assert_file_closed(&in_file);;
    cmsCloseProfile(hProfile);

    assert_file_name_closed("input_file");
	assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
}