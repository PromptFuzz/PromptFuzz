#include "FDSan.h"
#include "FuzzedDataProvider.h"
#include <lcms2.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
//<ID> 12757
//<Prompt> ["cmsGetHeaderManufacturer","cmsPipelineGetPtrToFirstStage","cmsIT8GetProperty","cmsCreate_sRGBProfile","cmsIsIntentSupported"]
/*<Combination>: [cmsUInt32Number cmsGetHeaderManufacturer(cmsHPROFILE hProfile),
    cmsStage *cmsPipelineGetPtrToFirstStage(const cmsPipeline * lut),
    const char *cmsIT8GetProperty(cmsHANDLE hIT8, const char * cProp),
    cmsHPROFILE cmsCreate_sRGBProfile(),
    cmsBool cmsIsIntentSupported(cmsHPROFILE hProfile, cmsUInt32Number Intent, cmsUInt32Number UsedDirection)
*/
//<score> 1.3333334, nr_unique_branch: 1, p_cov: 1
//<Quality> {"diversity":9,"density":6,"covered_api":["cmsGetHeaderManufacturer","cmsPipelineGetPtrToFirstStage","cmsIT8GetProperty","cmsCreate_sRGBProfile","cmsIsIntentSupported"],"uncovered_api":[],"unique_branches":{"cmsPipelineFree":[[1424,9,1424,20,0]]},"library_calls":["cmsCreate_sRGBProfile","cmsGetHeaderManufacturer","cmsPipelineGetPtrToFirstStage","cmsIT8Alloc","cmsIT8GetProperty","cmsIsIntentSupported","cmsIT8Free","cmsPipelineFree","cmsCloseProfile"]}
/**/



extern "C" int LLVMFuzzerTestOneInput_97(const uint8_t* f_data, size_t f_size) {
	if(f_size<=16) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
	//fuzzer shim end}




    // Create a file stream for reading the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
  
    // Create a sRGB profile
    cmsHPROFILE hProfile = cmsCreate_sRGBProfile();
  
    // Get the manufacturer of the input profile
    cmsUInt32Number manufacturer = cmsGetHeaderManufacturer(hProfile);
  
    // Get the first stage of the pipeline
    cmsPipeline *lut = (cmsPipeline *)cmsPipelineGetPtrToFirstStage((const cmsPipeline *)hProfile);

    // Create an IT8 handle
    cmsHANDLE hIT8 = cmsIT8Alloc(NULL);
  
    // Get the property from the IT8 handle
    const char *property = cmsIT8GetProperty(hIT8, fuzz_str_1);
  
    // Check if a specific intent is supported by the profile
    cmsBool isIntentSupported = cmsIsIntentSupported(hProfile, 0, 0);
  
    // Release the resources
    cmsIT8Free(hIT8);
    cmsPipelineFree(lut);
    cmsCloseProfile(hProfile);
    assert_file_closed(&in_file);;
  
    assert_file_closed(&in_file);
	return 0;
}