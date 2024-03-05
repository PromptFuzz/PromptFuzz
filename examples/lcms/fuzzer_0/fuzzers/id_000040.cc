#include "FDSan.h"
#include <lcms2.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
//<ID> 4690
//<Prompt> ["cmsPipelineEval16","cmsStageAllocToneCurves","cmsBFDdeltaE","cmsCreate_sRGBProfile","cmsGetPostScriptCRD","cmsGetEncodedICCversion","cmsCreateTransform"]
/*<Combination>: [void cmsPipelineEval16(const cmsUInt16Number * In, cmsUInt16Number * Out, const cmsPipeline * lut),
    cmsStage *cmsStageAllocToneCurves(cmsContext ContextID, cmsUInt32Number nChannels, cmsToneCurve *const * Curves),
    cmsFloat64Number cmsBFDdeltaE(const cmsCIELab * Lab1, const cmsCIELab * Lab2),
    cmsHPROFILE cmsCreate_sRGBProfile(),
    cmsUInt32Number cmsGetPostScriptCRD(cmsContext ContextID, cmsHPROFILE hProfile, cmsUInt32Number Intent, cmsUInt32Number dwFlags, void * Buffer, cmsUInt32Number dwBufferLen),
    cmsUInt32Number cmsGetEncodedICCversion(cmsHPROFILE hProfile),
    cmsHTRANSFORM cmsCreateTransform(cmsHPROFILE Input, cmsUInt32Number InputFormat, cmsHPROFILE Output, cmsUInt32Number OutputFormat, cmsUInt32Number Intent, cmsUInt32Number dwFlags)
*/
//<score> 2, nr_unique_branch: 13, p_cov: 0.14285715
//<Quality> {"diversity":9,"density":9,"covered_api":["cmsCreateTransform"],"uncovered_api":["cmsPipelineEval16","cmsStageAllocToneCurves","cmsBFDdeltaE","cmsCreate_sRGBProfile","cmsGetPostScriptCRD","cmsGetEncodedICCversion"],"unique_branches":{"TetrahedralInterp16":[[762,13,762,21,1],[776,20,776,28,0],[776,20,776,28,1],[779,20,779,28,0],[779,20,779,28,1],[793,20,793,28,0],[793,20,793,28,1],[806,13,806,21,0],[809,20,809,28,0],[809,20,809,28,1],[820,20,820,28,1],[837,20,837,28,0],[837,20,837,28,1]]},"library_calls":["cmsCreateContext","cmsCreate_sRGBProfileTHR","cmsCreateLab4ProfileTHR","cmsCreateTransform","cmsDoTransform","cmsDeleteTransform","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"]}
/**/

extern "C" int LLVMFuzzerTestOneInput_40(const uint8_t *data, size_t size) {
	if(size<=0) return 0;

    // Create a context
    cmsContext ctx = cmsCreateContext(NULL, NULL);

    // Create an sRGB profile
    cmsHPROFILE inProfile = cmsCreate_sRGBProfileTHR(ctx);

    // Create a Lab profile
    cmsCIExyY whitePoint = {0.3457, 0.3585, 1.0000};
    cmsHPROFILE outProfile = cmsCreateLab4ProfileTHR(ctx, &whitePoint);

    // Create a transform
    cmsHTRANSFORM transform = cmsCreateTransform(
        inProfile, TYPE_RGB_16, outProfile, TYPE_Lab_16, INTENT_PERCEPTUAL, 0
    );

    // Allocate memory for input and output buffers
    cmsUInt16Number *inBuffer = (cmsUInt16Number*)malloc(sizeof(cmsUInt16Number) * 3);
    cmsUInt16Number *outBuffer = (cmsUInt16Number*)malloc(sizeof(cmsUInt16Number) * 3);

    // Read input data
    if (size >= sizeof(cmsUInt16Number) * 3) {
        memcpy(inBuffer, data, sizeof(cmsUInt16Number) * 3);
    }

    // Perform color transformation
    cmsDoTransform(transform, inBuffer, outBuffer, 1);

    // Clean up
    free(inBuffer);
    free(outBuffer);
    cmsDeleteTransform(transform);
    cmsCloseProfile(inProfile);
    cmsCloseProfile(outProfile);
    cmsDeleteContext(ctx);

    return 0;
}