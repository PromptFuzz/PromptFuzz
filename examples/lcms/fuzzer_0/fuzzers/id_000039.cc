#include "FDSan.h"
#include <lcms2.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
//<ID> 4682
//<Prompt> ["cmsPipelineEval16","cmsStageAllocToneCurves","cmsCreate_sRGBProfile","cmsGetPostScriptCRD","cmsGetEncodedICCversion","cmsCreateTransform"]
/*<Combination>: [void cmsPipelineEval16(const cmsUInt16Number * In, cmsUInt16Number * Out, const cmsPipeline * lut),
    cmsStage *cmsStageAllocToneCurves(cmsContext ContextID, cmsUInt32Number nChannels, cmsToneCurve *const * Curves),
    cmsHPROFILE cmsCreate_sRGBProfile(),
    cmsUInt32Number cmsGetPostScriptCRD(cmsContext ContextID, cmsHPROFILE hProfile, cmsUInt32Number Intent, cmsUInt32Number dwFlags, void * Buffer, cmsUInt32Number dwBufferLen),
    cmsUInt32Number cmsGetEncodedICCversion(cmsHPROFILE hProfile),
    cmsHTRANSFORM cmsCreateTransform(cmsHPROFILE Input, cmsUInt32Number InputFormat, cmsHPROFILE Output, cmsUInt32Number OutputFormat, cmsUInt32Number Intent, cmsUInt32Number dwFlags)
*/
//<score> 2, nr_unique_branch: 1, p_cov: 1
//<Quality> {"diversity":20,"density":20,"covered_api":["cmsPipelineEval16","cmsStageAllocToneCurves","cmsCreate_sRGBProfile","cmsGetPostScriptCRD","cmsGetEncodedICCversion","cmsCreateTransform"],"uncovered_api":[],"unique_branches":{"_LUTeval16":[[1332,10,1332,21,0]]},"library_calls":["cmsCreateContext","cmsCreate_sRGBProfile","cmsCreate_sRGBProfile","cmsCreateTransform","cmsGetEncodedICCversion","cmsGetEncodedICCversion","cmsGetPostScriptCRD","cmsPipelineAlloc","cmsBuildTabulatedToneCurve16","cmsStageAllocToneCurves","cmsPipelineInsertStage","cmsPipelineEval16","cmsPipelineFree","cmsFreeToneCurve","cmsFreeToneCurve","cmsFreeToneCurve","cmsDeleteTransform","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"]}
/**/

extern "C" int LLVMFuzzerTestOneInput_39(const uint8_t *data, size_t size) {
	if(size<=0) return 0;

    FILE *in_file = fmemopen((void *)data, size, "rb");
    FILE *out_file = fopen("output_file", "wb");

    cmsContext context = cmsCreateContext(NULL, NULL); // Create a context

    // Use the lcms APIs to achieve the desired event
    cmsHPROFILE inputProfile = cmsCreate_sRGBProfile();
    cmsHPROFILE outputProfile = cmsCreate_sRGBProfile();
    cmsHTRANSFORM transform = cmsCreateTransform(inputProfile, TYPE_RGB_8, outputProfile, TYPE_RGB_8, INTENT_PERCEPTUAL, 0);
    
    cmsUInt32Number inputSize = cmsGetEncodedICCversion(inputProfile);
    cmsUInt32Number outputSize = cmsGetEncodedICCversion(outputProfile);
    cmsUInt32Number bufferSize = inputSize > outputSize ? inputSize : outputSize;
    void *buffer = malloc(bufferSize);

    cmsUInt32Number crdSize = cmsGetPostScriptCRD(context, inputProfile, INTENT_PERCEPTUAL, 0, buffer, bufferSize);
    
    cmsPipeline *pipeline = cmsPipelineAlloc(context, 3, 3);
    cmsToneCurve *curves[3];

    // Allocate tone curves
    for (int i = 0; i < 3; i++) {
        curves[i] = cmsBuildTabulatedToneCurve16(context, 256, NULL);
    }

    // Allocate stage using the tone curves
    cmsStage *stage = cmsStageAllocToneCurves(context, 3, curves);

    // Add stage to the pipeline
    cmsPipelineInsertStage(pipeline, cmsAT_BEGIN, stage);
    
    cmsUInt16Number *input = (cmsUInt16Number *)malloc(3 * sizeof(cmsUInt16Number));
    cmsUInt16Number *output = (cmsUInt16Number *)malloc(3 * sizeof(cmsUInt16Number));

    // Read input data from the file
    fread(input, sizeof(cmsUInt16Number), 3, in_file);
    
    // Evaluate the pipeline using the input data
    cmsPipelineEval16(input, output, pipeline);

    // Write output data to the file
    fwrite(output, sizeof(cmsUInt16Number), 3, out_file);

    // Cleanup and release resources
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    free(input);
    free(output);
    free(buffer);
    cmsPipelineFree(pipeline);
    cmsFreeToneCurve(curves[0]);
    cmsFreeToneCurve(curves[1]);
    cmsFreeToneCurve(curves[2]);
    cmsDeleteTransform(transform);
    cmsCloseProfile(inputProfile);
    cmsCloseProfile(outputProfile);
    cmsDeleteContext(context);

    assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}