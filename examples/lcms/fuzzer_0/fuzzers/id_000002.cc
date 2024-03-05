#include "FDSan.h"
#include <lcms2.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
//<ID> 461
//<Prompt> ["cmsAppendNamedColor","cmsTagLinkedTo","cmsGDBCheckPoint","cmsCreate_OkLabProfile","cmsGetAlarmCodesTHR"]
/*<Combination>: [cmsBool cmsAppendNamedColor(cmsNAMEDCOLORLIST * v, const char * Name, cmsUInt16Number PCS[3], cmsUInt16Number Colorant[16]),
    cmsTagSignature cmsTagLinkedTo(cmsHPROFILE hProfile, cmsTagSignature sig),
    cmsBool cmsGDBCheckPoint(cmsHANDLE hGBD, const cmsCIELab * Lab),
    cmsHPROFILE cmsCreate_OkLabProfile(cmsContext ctx),
    void cmsGetAlarmCodesTHR(cmsContext ContextID, cmsUInt16Number AlarmCodes[16])
*/
//<score> 2, nr_unique_branch: 1, p_cov: 1
//<Quality> {"diversity":12,"density":12,"covered_api":["cmsAppendNamedColor","cmsTagLinkedTo","cmsGDBCheckPoint","cmsCreate_OkLabProfile","cmsGetAlarmCodesTHR"],"uncovered_api":[],"unique_branches":{"ToSpherical":[[130,8,130,19,0]]},"library_calls":["cmsCreateContext","cmsAllocNamedColorList","cmsAppendNamedColor","cmsCreate_OkLabProfile","cmsTagLinkedTo","cmsGBDAlloc","cmsGDBCheckPoint","cmsGetAlarmCodesTHR","cmsFreeNamedColorList","cmsCloseProfile","cmsGBDFree","cmsDeleteContext"]}
/**/


extern "C" int LLVMFuzzerTestOneInput_2(const uint8_t *data, size_t size) {
	if(size<=0) return 0;


    // Step 4: Open input and output files
    FILE *in_file = fmemopen((void *)data, size, "rb");
    FILE *out_file = fopen("output_file", "wb");

    // Step 5: Get file descriptors
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);

    // Step 2: Use lcms APIs to achieve the desired event
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    cmsNAMEDCOLORLIST *color_list = cmsAllocNamedColorList(ctx, 10, 3, "", "");
    cmsUInt16Number PCS[3] = {0xFFFF, 0xFFFF, 0xFFFF};
    cmsUInt16Number Colorant[16] = {0xFFFF};
    cmsAppendNamedColor(color_list, "Color", PCS, Colorant);

    cmsHPROFILE profile = cmsCreate_OkLabProfile(ctx);
    cmsTagSignature linked_tag = cmsTagLinkedTo(profile, cmsSigColorantTableTag);
    printf("Linked tag: %x\n", linked_tag);

    cmsHANDLE gdb = cmsGBDAlloc(ctx);
    cmsCIELab lab = {50, 0, 0};
    cmsBool is_valid_point = cmsGDBCheckPoint(gdb, &lab);
    printf("Is valid point: %d\n", is_valid_point);

    cmsUInt16Number alarm_codes[16];
    cmsGetAlarmCodesTHR(ctx, alarm_codes);
    for (int i = 0; i < 16; i++) {
        printf("Alarm code %d: %d\n", i, alarm_codes[i]);
    }

    // Step 7: Release resources
    cmsFreeNamedColorList(color_list);
    cmsCloseProfile(profile);
    cmsGBDFree(gdb);
    cmsDeleteContext(ctx);

    // Step 6: Release file resources
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;

    assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}