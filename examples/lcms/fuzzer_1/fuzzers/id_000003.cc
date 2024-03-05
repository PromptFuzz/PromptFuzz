#include "FDSan.h"
#include "FuzzedDataProvider.h"
#include <lcms2.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
//<ID> 469
//<Prompt> ["cmsAppendNamedColor","cmsTagLinkedTo","cmsGDBCheckPoint","cmsCreate_OkLabProfile","cmsGetAlarmCodesTHR"]
/*<Combination>: [cmsBool cmsAppendNamedColor(cmsNAMEDCOLORLIST * v, const char * Name, cmsUInt16Number PCS[3], cmsUInt16Number Colorant[16]),
    cmsTagSignature cmsTagLinkedTo(cmsHPROFILE hProfile, cmsTagSignature sig),
    cmsBool cmsGDBCheckPoint(cmsHANDLE hGBD, const cmsCIELab * Lab),
    cmsHPROFILE cmsCreate_OkLabProfile(cmsContext ctx),
    void cmsGetAlarmCodesTHR(cmsContext ContextID, cmsUInt16Number AlarmCodes[16])
*/
//<score> 7, nr_unique_branch: 6, p_cov: 1
//<Quality> {"diversity":12,"density":12,"covered_api":["cmsAppendNamedColor","cmsTagLinkedTo","cmsGDBCheckPoint","cmsCreate_OkLabProfile","cmsGetAlarmCodesTHR"],"uncovered_api":[],"unique_branches":{"_cmsAtan2":[[106,9,106,17,0],[106,9,106,17,1],[106,21,106,29,0],[110,12,110,17,1]],"ToSpherical":[[130,8,130,19,1]],"QuantizeToSector":[[175,9,175,26,0]]},"library_calls":["cmsCreateContext","cmsAllocNamedColorList","cmsAppendNamedColor","cmsCreate_OkLabProfile","cmsTagLinkedTo","cmsGBDAlloc","cmsGDBCheckPoint","cmsGetAlarmCodesTHR","cmsFreeNamedColorList","cmsCloseProfile","cmsGBDFree","cmsDeleteContext"]}
/**/

extern "C" int LLVMFuzzerTestOneInput_3(const uint8_t* f_data, size_t f_size) {
	if(f_size<=29) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
	//fuzzer shim end}




    cmsContext ctx = cmsCreateContext(NULL, NULL);
    cmsBool result;
    cmsUInt16Number PCS[3] = {0};
    cmsUInt16Number Colorant[16] = {0};
    cmsHPROFILE hProfile;
    cmsTagSignature tagSig;
    cmsHANDLE hGBD;
    cmsCIELab Lab;
    cmsUInt16Number AlarmCodes[16] = {0};
    
    FILE *in_file = fmemopen((void *)data, size, "rb");
    
    // Call lcms APIs
    cmsNAMEDCOLORLIST *namedColorList = cmsAllocNamedColorList(ctx, 10, 16, fuzz_str_1, fuzz_str_2);
    result = cmsAppendNamedColor(namedColorList, fuzz_str_3, PCS, Colorant);
    
    hProfile = cmsCreate_OkLabProfile(ctx);
    tagSig = cmsTagLinkedTo(hProfile, cmsSigLuminanceTag);
    
    hGBD = cmsGBDAlloc(ctx);
    result = cmsGDBCheckPoint(hGBD, &Lab);
    
    cmsGetAlarmCodesTHR(ctx, AlarmCodes);
    
    assert_file_closed(&in_file);;
    cmsFreeNamedColorList(namedColorList);
    cmsCloseProfile(hProfile);
    cmsGBDFree(hGBD);
    cmsDeleteContext(ctx);
    
    assert_file_closed(&in_file);
	return 0;
}