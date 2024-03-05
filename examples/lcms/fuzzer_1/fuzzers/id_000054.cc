#include "FDSan.h"
#include "FuzzedDataProvider.h"
#include <lcms2.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
//<ID> 6362
//<Prompt> ["cmsIT8SetTableByLabel","cmsFloat2XYZEncoded","cmsSetAlarmCodes","cmsStageAllocCLut16bit","cmsIsIntentSupported"]
/*<Combination>: [int cmsIT8SetTableByLabel(cmsHANDLE hIT8, const char * cSet, const char * cField, const char * ExpectedType),
    void cmsFloat2XYZEncoded(cmsUInt16Number XYZ[3], const cmsCIEXYZ * fXYZ),
    void cmsSetAlarmCodes(const cmsUInt16Number NewAlarm[16]),
    cmsStage *cmsStageAllocCLut16bit(cmsContext ContextID, cmsUInt32Number nGridPoints, cmsUInt32Number inputChan, cmsUInt32Number outputChan, const cmsUInt16Number * Table),
    cmsBool cmsIsIntentSupported(cmsHPROFILE hProfile, cmsUInt32Number Intent, cmsUInt32Number UsedDirection)
*/
//<score> 2.3333333, nr_unique_branch: 3, p_cov: 1
//<Quality> {"diversity":12,"density":7,"covered_api":["cmsIT8SetTableByLabel","cmsFloat2XYZEncoded","cmsSetAlarmCodes","cmsStageAllocCLut16bit","cmsIsIntentSupported"],"uncovered_api":[],"unique_branches":{"cmsIsCLUT":[[845,8,845,31,0],[846,19,846,77,0],[847,19,847,100,0]]},"library_calls":["cmsCreateContext","cmsIT8Alloc","cmsIT8SetTableByLabel","cmsFloat2XYZEncoded","cmsSetAlarmCodes","cmsStageAllocCLut16bit","cmsCreate_sRGBProfile","cmsIsIntentSupported","cmsCloseProfile","cmsStageFree","cmsIT8Free","cmsDeleteContext"]}
/**/


extern "C" int LLVMFuzzerTestOneInput_54(const uint8_t* f_data, size_t f_size) {
	if(f_size<=27) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
	//fuzzer shim end}




    // Create a context
    cmsContext context = cmsCreateContext(NULL, NULL);
    
    // Create an IT8 handle
    cmsHANDLE hIT8 = cmsIT8Alloc(context);
    
    // Set the table by label
    cmsIT8SetTableByLabel(hIT8, fuzz_str_1, fuzz_str_2, fuzz_str_3);
    
    // Convert input data to XYZ
    cmsUInt16Number XYZ[3];
    cmsCIEXYZ fXYZ;
    cmsFloat2XYZEncoded(XYZ, &fXYZ);
    
    // Set alarm codes
    cmsUInt16Number alarmCodes[16] = {0};
    cmsSetAlarmCodes(alarmCodes);
    
    // Allocate a CLUT stage
    cmsUInt16Number clutTable[16] = {0};
    cmsStage *clutStage = cmsStageAllocCLut16bit(context, 4, 1, 1, clutTable);
    
    // Check if intent is supported
    cmsHPROFILE hProfile = cmsCreate_sRGBProfile();
    cmsBool intentSupported = cmsIsIntentSupported(hProfile, 1, 2);
    
    // Clean up
    cmsCloseProfile(hProfile);
    cmsStageFree(clutStage);
    cmsIT8Free(hIT8);
    cmsDeleteContext(context);
    
    return 0;
}