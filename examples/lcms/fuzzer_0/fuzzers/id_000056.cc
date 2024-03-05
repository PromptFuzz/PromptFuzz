#include "FDSan.h"
#include <lcms2.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
//<ID> 6396
//<Prompt> ["cmsIT8SetTableByLabel","cmsFloat2XYZEncoded","cmsSetAlarmCodes","cmsStageAllocCLut16bit","cmsIsIntentSupported"]
/*<Combination>: [int cmsIT8SetTableByLabel(cmsHANDLE hIT8, const char * cSet, const char * cField, const char * ExpectedType),
    void cmsFloat2XYZEncoded(cmsUInt16Number XYZ[3], const cmsCIEXYZ * fXYZ),
    void cmsSetAlarmCodes(const cmsUInt16Number NewAlarm[16]),
    cmsStage *cmsStageAllocCLut16bit(cmsContext ContextID, cmsUInt32Number nGridPoints, cmsUInt32Number inputChan, cmsUInt32Number outputChan, const cmsUInt16Number * Table),
    cmsBool cmsIsIntentSupported(cmsHPROFILE hProfile, cmsUInt32Number Intent, cmsUInt32Number UsedDirection)
*/
//<score> 0.8, nr_unique_branch: 1, p_cov: 0.8
//<Quality> {"diversity":12,"density":6,"covered_api":["cmsIT8SetTableByLabel","cmsFloat2XYZEncoded","cmsStageAllocCLut16bit","cmsIsIntentSupported"],"uncovered_api":["cmsSetAlarmCodes"],"unique_branches":{"cmsFloat2XYZEncoded":[[383,9,383,19,1]]},"library_calls":["cmsCreateContext","cmsIT8LoadFromMem","cmsIT8SetTableByLabel","cmsFloat2XYZEncoded","cmsGetAlarmCodes","cmsStageAllocCLut16bit","cmsCreate_sRGBProfile","cmsIsIntentSupported","cmsStageFree","cmsCloseProfile","cmsIT8Free","cmsDeleteContext"]}
/*Here's an example program that uses the lcms library APIs to achieve the event described:

*/


extern "C" int LLVMFuzzerTestOneInput_56(const uint8_t *data, size_t size) {
	if(size<=0) return 0;

    // Create a context
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    
    // Create an IT8 handle
    cmsHANDLE hIT8 = cmsIT8LoadFromMem(ctx, data, size);
    
    if (hIT8) {
        // Set the table by label
        cmsIT8SetTableByLabel(hIT8, "set", "field", "type");
    }
    
    // Create XYZ array
    cmsUInt16Number XYZ[3];
    
    // Create a sample XYZ value
    cmsCIEXYZ sampleXYZ;
    sampleXYZ.X = 1.0;
    sampleXYZ.Y = 1.0;
    sampleXYZ.Z = 1.0;
    
    // Convert XYZ to encoded format
    cmsFloat2XYZEncoded(XYZ, &sampleXYZ);
    
    // Set alarm codes
    cmsUInt16Number alarmCodes[16];
    cmsGetAlarmCodes(alarmCodes);
    
    // Create a CLut stage
    cmsStage *clutStage = cmsStageAllocCLut16bit(ctx, 0, 0, 0, NULL);
    
    // Check if intent is supported in a profile
    cmsHPROFILE hProfile = cmsCreate_sRGBProfile();
    cmsBool isIntentSupported = cmsIsIntentSupported(hProfile, 0, 0);
    
    // Clean up
    if (clutStage) cmsStageFree(clutStage);
    if (hProfile) cmsCloseProfile(hProfile);
    if (hIT8) cmsIT8Free(hIT8);
    if (ctx) cmsDeleteContext(ctx);
    
    return 0;
}