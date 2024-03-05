#include "FDSan.h"
#include <lcms2.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
//<ID> 10538
//<Prompt> ["cmsDoTransformStride","cmsIT8GetProperty","cmsSaveProfileToFile","cmsCreateGrayProfileTHR","cmsGetHeaderFlags","cmsEvalToneCurveFloat"]
/*<Combination>: [void cmsDoTransformStride(cmsHTRANSFORM Transform, const void * InputBuffer, void * OutputBuffer, cmsUInt32Number Size, cmsUInt32Number Stride),
    const char *cmsIT8GetProperty(cmsHANDLE hIT8, const char * cProp),
    cmsBool cmsSaveProfileToFile(cmsHPROFILE hProfile, const char * FileName),
    cmsHPROFILE cmsCreateGrayProfileTHR(cmsContext ContextID, const cmsCIExyY * WhitePoint, const cmsToneCurve * TransferFunction),
    cmsUInt32Number cmsGetHeaderFlags(cmsHPROFILE hProfile),
    cmsFloat32Number cmsEvalToneCurveFloat(const cmsToneCurve * Curve, cmsFloat32Number v)
*/
//<score> 4.3333335, nr_unique_branch: 12, p_cov: 0.33333334
//<Quality> {"diversity":9,"density":9,"covered_api":["cmsDoTransformStride","cmsCreateGrayProfileTHR"],"uncovered_api":["cmsIT8GetProperty","cmsSaveProfileToFile","cmsGetHeaderFlags","cmsEvalToneCurveFloat"],"unique_branches":{"OptimizeByComputingLinearization":[[1054,9,1054,46,0]],"_cmsICCcolorSpace":[[769,8,769,20,0]],"OptimizeMatrixShaper":[[1676,12,1676,41,0]],"_cmsEndPointsBySpace":[[727,33,727,38,0],[728,32,728,37,1]],"_MultiplyMatrix":[[212,29,212,61,0]],"BuildGrayOutputPipeline":[[418,9,418,24,1],[421,9,421,27,1],[424,9,424,20,1],[429,9,429,45,1],[435,13,435,110,1],[439,9,439,100,1]]},"library_calls":["cmsCreateContext","cmsBuildGamma","cmsCreateGrayProfileTHR","cmsCreateTransform","cmsDoTransformStride","cmsDeleteTransform","cmsFreeToneCurve","cmsCloseProfile","cmsDeleteContext"]}
/**/



extern "C" int LLVMFuzzerTestOneInput_91(const uint8_t* data, size_t size) {
	if(size<=0) return 0;

    // Create a lcms context
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    
    // Create a gray profile
    cmsCIExyY whitePoint = {0.3127, 0.3290, 1.0};
    cmsToneCurve* transferFunction = cmsBuildGamma(ctx, 2.2);
    cmsHPROFILE grayProfile = cmsCreateGrayProfileTHR(ctx, &whitePoint, transferFunction);

    // Create a transform between the gray profile and gray profile
    cmsHTRANSFORM transform = cmsCreateTransform(
        grayProfile, TYPE_GRAY_8, grayProfile, TYPE_GRAY_8, INTENT_PERCEPTUAL, 0);
    
    // Create a file to read input data
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
        // Failed to create file
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Create a file to write output data
    FILE* out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Use the transform to process the input data and write the output to the file
    fseek(in_file, 0, SEEK_END);
    long file_size = ftell(in_file);
    fseek(in_file, 0, SEEK_SET);
    
    void* input_buffer = malloc(file_size);
    void* output_buffer = malloc(file_size);
    
    fread(input_buffer, 1, file_size, in_file);
    
    cmsDoTransformStride(transform, input_buffer, output_buffer, file_size, 1);
    
    fwrite(output_buffer, 1, file_size, out_file);
    
    // Free resources
    free(input_buffer);
    free(output_buffer);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    cmsDeleteTransform(transform);
    cmsFreeToneCurve(transferFunction);
    cmsCloseProfile(grayProfile);
    cmsDeleteContext(ctx);
    
    assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}