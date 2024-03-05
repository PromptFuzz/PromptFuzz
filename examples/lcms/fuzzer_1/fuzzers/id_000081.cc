#include "FDSan.h"
#include "FuzzedDataProvider.h"
#include <lcms2.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
//<ID> 9601
//<Prompt> ["cmsIT8SetPropertyStr","cmsIT8GetData","cmsCreateLab2Profile","cmsDupProfileSequenceDescription","cmsIT8SetComment"]
/*<Combination>: [cmsBool cmsIT8SetPropertyStr(cmsHANDLE hIT8, const char * cProp, const char * Str),
    const char *cmsIT8GetData(cmsHANDLE hIT8, const char * cPatch, const char * cSample),
    cmsHPROFILE cmsCreateLab2Profile(const cmsCIExyY * WhitePoint),
    cmsSEQ *cmsDupProfileSequenceDescription(const cmsSEQ * pseq),
    cmsBool cmsIT8SetComment(cmsHANDLE hIT8, const char * cComment)
*/
//<score> 1.309091, nr_unique_branch: 1, p_cov: 0.8
//<Quality> {"diversity":11,"density":9,"covered_api":["cmsIT8SetPropertyStr","cmsIT8GetData","cmsDupProfileSequenceDescription","cmsIT8SetComment"],"uncovered_api":["cmsCreateLab2Profile"],"unique_branches":{"IsAvailableOnList":[[1224,13,1224,24,1]]},"library_calls":["cmsCreateContext","cmsCreateLab2ProfileTHR","cmsIT8Alloc","cmsIT8SetPropertyStr","cmsIT8GetData","cmsDupProfileSequenceDescription","cmsIT8SetComment","cmsIT8Free","cmsFreeProfileSequenceDescription","cmsCloseProfile","cmsDeleteContext"]}
/**/


extern "C" int LLVMFuzzerTestOneInput_81(const uint8_t* f_data, size_t f_size)
{
	if(f_size<=29) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
	//fuzzer shim end}




    // Create an lcms context
    cmsContext context = cmsCreateContext(NULL, NULL);

    // Create an lCMS profile
    cmsHPROFILE profile = cmsCreateLab2ProfileTHR(context, NULL);

    // Create an lCMS handle
    cmsHANDLE handle = cmsIT8Alloc(context);

    // Set a property for IT8 handle
    cmsIT8SetPropertyStr(handle, fuzz_str_1, fuzz_str_2);

    // Get data from IT8 handle
    const char *patch = "patch";
    const char *sample = "sample";
    const char *dataValue = cmsIT8GetData(handle, patch, sample);

    // Duplicate a profile sequence description
    cmsSEQ *sequence = cmsDupProfileSequenceDescription(NULL);

    // Set a comment for IT8 handle
    cmsIT8SetComment(handle, fuzz_str_3);

    // Release all allocated resources
    cmsIT8Free(handle);
    cmsFreeProfileSequenceDescription(sequence);
    cmsCloseProfile(profile);
    cmsDeleteContext(context);

    return 0;
}