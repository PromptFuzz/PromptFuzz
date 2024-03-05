#include "FDSan.h"
#include <lcms2.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
//<ID> 4436
//<Prompt> ["cmsStageAllocToneCurves","cmsGetPostScriptCRD","cmsGetEncodedICCversion","cmsTransform2DeviceLink","cmsIT8SetDataRowCol"]
/*<Combination>: [cmsStage *cmsStageAllocToneCurves(cmsContext ContextID, cmsUInt32Number nChannels, cmsToneCurve *const * Curves),
    cmsUInt32Number cmsGetPostScriptCRD(cmsContext ContextID, cmsHPROFILE hProfile, cmsUInt32Number Intent, cmsUInt32Number dwFlags, void * Buffer, cmsUInt32Number dwBufferLen),
    cmsUInt32Number cmsGetEncodedICCversion(cmsHPROFILE hProfile),
    cmsHPROFILE cmsTransform2DeviceLink(cmsHTRANSFORM hTransform, cmsFloat64Number Version, cmsUInt32Number dwFlags),
    cmsBool cmsIT8SetDataRowCol(cmsHANDLE hIT8, int row, int col, const char * Val)
*/
//<score> 42, nr_unique_branch: 41, p_cov: 1
//<Quality> {"diversity":17,"density":17,"covered_api":["cmsStageAllocToneCurves","cmsGetPostScriptCRD","cmsGetEncodedICCversion","cmsTransform2DeviceLink","cmsIT8SetDataRowCol"],"uncovered_api":[],"unique_branches":{"cmsTransform2DeviceLink":[[1203,9,1203,20,0],[1205,13,1205,58,1],[1212,9,1212,20,1],[1215,9,1215,51,1],[1222,9,1222,50,1],[1231,9,1231,18,1],[1250,10,1250,42,1],[1256,9,1256,38,1],[1261,9,1261,27,0],[1270,9,1270,27,0],[1280,13,1280,31,0],[1280,35,1280,78,0],[1281,18,1281,109,1],[1285,13,1285,30,0],[1285,34,1285,76,0],[1286,18,1286,110,1],[1293,9,1293,27,1],[1298,9,1298,44,1],[1302,9,1302,46,1],[1305,9,1305,52,1],[1308,9,1308,39,1],[1312,9,1312,40,1],[1316,9,1316,41,0],[1316,45,1316,71,1],[1321,9,1321,40,1],[1325,14,1325,87,1]],"cmsGetTransformContextID":[[1407,9,1407,22,1]],"FixColorSpaces":[[1027,9,1027,44,1]],"FindCombination":[[1168,15,1168,38,0],[1168,15,1168,38,1],[1172,13,1172,31,0],[1172,13,1172,31,1],[1173,13,1173,37,1],[1175,13,1175,31,0],[1175,13,1175,31,1]],"OptimizeByResampling":[[674,9,674,41,0]],"CheckOne":[[1153,37,1153,48,0],[1153,37,1153,48,1],[1155,13,1155,29,1],[1156,13,1156,51,0],[1156,13,1156,51,1]]},"library_calls":["cmsCreateContext","cmsBuildGamma","cmsStageAllocToneCurves","cmsCreate_sRGBProfile","cmsGetPostScriptCRD","cmsGetEncodedICCversion","cmsCreateTransform","cmsTransform2DeviceLink","cmsIT8Alloc","cmsIT8SetDataRowCol","cmsStageFree","cmsFreeToneCurve","cmsCloseProfile","cmsCloseProfile","cmsIT8Free","cmsDeleteTransform","cmsDeleteContext"]}
/**/

extern "C" int LLVMFuzzerTestOneInput_36(const uint8_t *data, size_t size) {
	if(size<=0) return 0;

    // Create a context
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    
    // Allocate tone curves for each channel
    cmsUInt32Number nChannels = 3;
    cmsToneCurve* curves[3];
    for (int i = 0; i < nChannels; i++) {
        curves[i] = cmsBuildGamma(ctx, 1.0);
    }
    
    // Allocate a stage with the tone curves
    cmsStage* stage = cmsStageAllocToneCurves(ctx, nChannels, curves);
    
    // Get the PostScript CRD
    cmsHPROFILE profile = cmsCreate_sRGBProfile();
    cmsUInt32Number intent = INTENT_PERCEPTUAL;
    cmsUInt32Number flags = 0;
    void* buffer = malloc(1024);
    cmsUInt32Number crdSize = cmsGetPostScriptCRD(ctx, profile, intent, flags, buffer, 1024);
    
    // Get the encoded ICC version
    cmsUInt32Number version = cmsGetEncodedICCversion(profile);
    
    // Transform to a device link profile
    cmsHTRANSFORM transform = cmsCreateTransform(profile, TYPE_RGB_8, profile, TYPE_RGB_8, intent, flags);
    cmsFloat64Number transformVersion = 2.0;
    cmsUInt32Number transformFlags = 0;
    cmsHPROFILE deviceLinkProfile = cmsTransform2DeviceLink(transform, transformVersion, transformFlags);

    // Create an IT8 handle
    cmsHANDLE it8Handle = cmsIT8Alloc(ctx);
    
    // Set a data row col value
    int row = 1;
    int col = 2;
    const char* val = "test";
    cmsBool success = cmsIT8SetDataRowCol(it8Handle, row, col, val);
    
    // Clean up resources
    cmsStageFree(stage);
    for (int i = 0; i < nChannels; i++) {
        cmsFreeToneCurve(curves[i]);
    }
    free(buffer);
    cmsCloseProfile(profile);
    cmsCloseProfile(deviceLinkProfile);
    cmsIT8Free(it8Handle);
    cmsDeleteTransform(transform);
    cmsDeleteContext(ctx);
    
    return 0;
}