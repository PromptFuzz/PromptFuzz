#include "FDSan.h"
#include "FuzzedDataProvider.h"
#include <lcms2.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
//<ID> 10483
//<Prompt> ["cmsStageDup","cmsMLUsetASCII","cmsIT8Alloc","cmsTempFromWhitePoint","cmsGetNamedColorList"]
/*<Combination>: [cmsStage *cmsStageDup(cmsStage * mpe),
    cmsBool cmsMLUsetASCII(cmsMLU * mlu, const char LanguageCode[3], const char CountryCode[3], const char * ASCIIString),
    cmsHANDLE cmsIT8Alloc(cmsContext ContextID),
    cmsBool cmsTempFromWhitePoint(cmsFloat64Number * TempK, const cmsCIExyY * WhitePoint),
    cmsNAMEDCOLORLIST *cmsGetNamedColorList(cmsHTRANSFORM xform)
*/
//<score> 0, nr_unique_branch: 1, p_cov: 0
//<Quality> {"diversity":2,"density":2,"covered_api":[],"uncovered_api":["cmsStageDup","cmsMLUsetASCII","cmsIT8Alloc","cmsTempFromWhitePoint","cmsGetNamedColorList"],"unique_branches":{"cmsGetNamedColorList":[[768,9,768,18,0]]},"library_calls":["cmsCreateContext","cmsDeleteContext"]}
/**/


// Event: Create a named color list from a color transform and duplicate a stage
void  id_000087_CreateNamedColorListAndDuplicateStage(const uint8_t* data, size_t size)
{
    // Create a context
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (ctx == NULL) {
        return;
    }

    // Create a named color list
    cmsNAMEDCOLORLIST* namedColorList = cmsGetNamedColorList(NULL);
    if (namedColorList == NULL) {
        cmsDeleteContext(ctx);
        return;
    }

    // Duplicate a stage
    cmsStage* stage = cmsStageDup(NULL);
    if (stage == NULL) {
        cmsFreeNamedColorList(namedColorList);
        cmsDeleteContext(ctx);
        return;
    }

    // Free resources
    cmsFreeNamedColorList(namedColorList);
    cmsStageFree(stage);
    cmsDeleteContext(ctx);
}

extern "C" int LLVMFuzzerTestOneInput_87(const uint8_t* f_data, size_t f_size)
{
	if(f_size<=0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Create a context
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (ctx == NULL) {
        return 0;
    }

    // Create a named color list and duplicate a stage
     id_000087_CreateNamedColorListAndDuplicateStage(data, size);

    // Free resources
    cmsDeleteContext(ctx);

    return 0;
}