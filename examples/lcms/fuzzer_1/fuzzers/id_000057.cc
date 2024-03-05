#include "FDSan.h"
#include "FuzzedDataProvider.h"
#include <lcms2.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
//<ID> 6895
//<Prompt> ["cmsPipelineStageCount","cmsTempFromWhitePoint","cmsIT8EnumProperties","cmsCIE94DeltaE","cmsCreate_OkLabProfile","cmsD50_XYZ"]
/*<Combination>: [cmsUInt32Number cmsPipelineStageCount(const cmsPipeline * lut),
    cmsBool cmsTempFromWhitePoint(cmsFloat64Number * TempK, const cmsCIExyY * WhitePoint),
    cmsUInt32Number cmsIT8EnumProperties(cmsHANDLE hIT8, char *** PropertyNames),
    cmsFloat64Number cmsCIE94DeltaE(const cmsCIELab * Lab1, const cmsCIELab * Lab2),
    cmsHPROFILE cmsCreate_OkLabProfile(cmsContext ctx),
    const cmsCIEXYZ *cmsD50_XYZ()
*/
//<score> 1.5384616, nr_unique_branch: 1, p_cov: 1
//<Quality> {"diversity":13,"density":10,"covered_api":["cmsPipelineStageCount","cmsTempFromWhitePoint","cmsIT8EnumProperties","cmsCIE94DeltaE","cmsCreate_OkLabProfile","cmsD50_XYZ"],"uncovered_api":[],"unique_branches":{"cmsCIE94DeltaE":[[466,9,466,17,1]]},"library_calls":["cmsCreateContext","cmsPipelineAlloc","cmsPipelineStageCount","cmsTempFromWhitePoint","cmsIT8Alloc","cmsIT8EnumProperties","cmsCIE94DeltaE","cmsCreate_OkLabProfile","cmsD50_XYZ","cmsPipelineFree","cmsIT8Free","cmsCloseProfile","cmsDeleteContext"]}
/**/



extern "C" int LLVMFuzzerTestOneInput_57(const uint8_t* f_data, size_t f_size) {
	if(f_size<=0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Create a new context
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    
    // Create a new pipeline
    cmsPipeline* lut = cmsPipelineAlloc(ctx, 1, 1);
    
    // Get the number of stages in the pipeline
    cmsUInt32Number stageCount = cmsPipelineStageCount(lut);
    
    // Convert the input data to a stream
    FILE *in_file = fmemopen((void *)data, size, "rb");
    
    // Create a white point structure
    cmsCIExyY whitePoint;
    
    // Get the temperature from the white point
    cmsFloat64Number tempK;
    cmsTempFromWhitePoint(&tempK, &whitePoint);
    
    // Create an IT8 handle
    cmsHANDLE it8Handle = cmsIT8Alloc(ctx);
    
    // Enumerate the properties of the IT8 handle
    char **propertyNames;
    cmsUInt32Number propertyCount = cmsIT8EnumProperties(it8Handle, &propertyNames);
    
    // Create Lab colors
    cmsCIELab lab1, lab2;
    
    // Calculate the CIE94 delta E
    cmsFloat64Number deltaE = cmsCIE94DeltaE(&lab1, &lab2);
    
    // Create the OkLab profile
    cmsHPROFILE okLabProfile = cmsCreate_OkLabProfile(ctx);
    
    // Get the D50 XYZ values
    const cmsCIEXYZ *d50XYZ = cmsD50_XYZ();
    
    // Release all allocated resources
    cmsPipelineFree(lut);
    cmsIT8Free(it8Handle);
    cmsCloseProfile(okLabProfile);
    cmsDeleteContext(ctx);
    assert_file_closed(&in_file);;
    
    assert_file_closed(&in_file);
	return 0;
}