#include "FDSan.h"
#include <lcms2.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
//<ID> 3133
//<Prompt> ["cmsxyY2XYZ","cmsStageAllocIdentity","cmsSetHeaderModel","cmsGDBAddPoint","cmsGetProfileContextID"]
/*<Combination>: [void cmsxyY2XYZ(cmsCIEXYZ * Dest, const cmsCIExyY * Source),
    cmsStage *cmsStageAllocIdentity(cmsContext ContextID, cmsUInt32Number nChannels),
    void cmsSetHeaderModel(cmsHPROFILE hProfile, cmsUInt32Number model),
    cmsBool cmsGDBAddPoint(cmsHANDLE hGBD, const cmsCIELab * Lab),
    cmsContext cmsGetProfileContextID(cmsHPROFILE hProfile)
*/
//<score> 7.2, nr_unique_branch: 7, p_cov: 1
//<Quality> {"diversity":10,"density":9,"covered_api":["cmsxyY2XYZ","cmsStageAllocIdentity","cmsSetHeaderModel","cmsGDBAddPoint","cmsGetProfileContextID"],"uncovered_api":[],"unique_branches":{"cmsCreateXYZProfileTHR":[[573,9,573,25,1],[581,9,581,57,1],[585,9,585,20,1],[587,9,587,94,1],[590,9,590,52,1]],"cmsGDBAddPoint":[[367,9,367,20,1],[370,9,370,31,0]]},"library_calls":["cmsCreateXYZProfile","cmsGetProfileContextID","cmsxyY2XYZ","cmsStageAllocIdentity","cmsSetHeaderModel","cmsGBDAlloc","cmsGDBAddPoint","cmsCloseProfile","cmsStageFree","cmsGBDFree"]}
/**/


extern "C" int LLVMFuzzerTestOneInput_26(const uint8_t *data, size_t size) {
	if(size<=0) return 0;

    // Step 1: Declare variables
    cmsCIEXYZ DestXYZ;
    cmsCIExyY SourcexyY;
    cmsStage *IdentityStage;
    cmsHPROFILE hProfile;
    cmsBool result;
    cmsHANDLE hGBD;
    cmsCIELab Lab;
    cmsContext ContextID;

    // Step 2: Create and initialize a profile
    hProfile = cmsCreateXYZProfile();
    
    // Step 3: Create a context
    ContextID = cmsGetProfileContextID(hProfile);
    
    // Step 4: Read input data
    FILE *in_file = fmemopen((void *)data, size, "rb");

    // Step 5: Get file descriptor
    int in_fd = fuzz_fileno(in_file);

    // Step 6: Create output file
    FILE *out_file = fopen("output_file", "wb");

    // Step 7: Get file descriptor
    int out_fd = fuzz_fileno(out_file);

    // Step 8: Convert xyY to XYZ
    fread(&SourcexyY, sizeof(SourcexyY), 1, in_file);
    cmsxyY2XYZ(&DestXYZ, &SourcexyY);

    // Step 9: Allocate an identity stage
    IdentityStage = cmsStageAllocIdentity(ContextID, 3);

    // Step 10: Set the header model
    cmsSetHeaderModel(hProfile, 1);

    // Step 11: Add point to GBD
    hGBD = cmsGBDAlloc(ContextID);
    fread(&Lab, sizeof(Lab), 1, in_file);
    result = cmsGDBAddPoint(hGBD, &Lab);

    // Step 12: Write output data
    fwrite(&DestXYZ, sizeof(DestXYZ), 1, out_file);
    fwrite(&result, sizeof(result), 1, out_file);

    // Step 13: Release resources
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    cmsCloseProfile(hProfile);
    cmsStageFree(IdentityStage);
    cmsGBDFree(hGBD);

    assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}