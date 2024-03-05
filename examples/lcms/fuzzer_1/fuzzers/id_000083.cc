#include "FDSan.h"
#include "FuzzedDataProvider.h"
#include <lcms2.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
//<ID> 9622
//<Prompt> ["cmsIT8SetPropertyStr","cmsJoinToneCurve","cmsGetContextUserData","cmsIT8GetData","cmsDupProfileSequenceDescription","cmsIT8SetComment"]
/*<Combination>: [cmsBool cmsIT8SetPropertyStr(cmsHANDLE hIT8, const char * cProp, const char * Str),
    cmsToneCurve *cmsJoinToneCurve(cmsContext ContextID, const cmsToneCurve * X, const cmsToneCurve * Y, cmsUInt32Number nPoints),
    void *cmsGetContextUserData(cmsContext ContextID),
    const char *cmsIT8GetData(cmsHANDLE hIT8, const char * cPatch, const char * cSample),
    cmsSEQ *cmsDupProfileSequenceDescription(const cmsSEQ * pseq),
    cmsBool cmsIT8SetComment(cmsHANDLE hIT8, const char * cComment)
*/
//<score> 3.9375, nr_unique_branch: 6, p_cov: 1
//<Quality> {"diversity":16,"density":9,"covered_api":["cmsIT8SetPropertyStr","cmsJoinToneCurve","cmsGetContextUserData","cmsIT8GetData","cmsDupProfileSequenceDescription","cmsIT8SetComment"],"uncovered_api":[],"unique_branches":{"cmsJoinToneCurve":[[991,9,991,26,1],[994,9,994,20,1],[997,15,997,36,0],[997,15,997,36,1],[1009,9,1009,20,0],[1010,9,1010,26,0]]},"library_calls":["cmsCreateContext","cmsIT8Alloc","cmsIT8SetComment","cmsIT8SetPropertyStr","cmsBuildGamma","cmsBuildGamma","cmsJoinToneCurve","cmsGetContextUserData","cmsIT8GetData","cmsDupProfileSequenceDescription","cmsIT8Free","cmsFreeToneCurve","cmsFreeToneCurve","cmsFreeToneCurve","cmsFreeProfileSequenceDescription","cmsDeleteContext"]}
/*Here is the C++ program that accomplishes the given requirements:

*/


extern "C" int LLVMFuzzerTestOneInput_83(const uint8_t* f_data, size_t f_size) {
	if(f_size<=4) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_1, fdp);
	//fuzzer shim end}




    // Create a lcms context
    cmsContext ctx = cmsCreateContext(NULL, NULL);

    // Create an IT8 handle
    cmsHANDLE hIT8 = cmsIT8Alloc(NULL);

    // Set the comment for the IT8 handle
    const char* comment = "This is a comment";
    cmsIT8SetComment(hIT8, comment);

    // Set the property for the IT8 handle
    const char* property = "Property";
    const char* value = "Value";
    cmsIT8SetPropertyStr(hIT8, property, value);

    // Join two tone curves
    cmsToneCurve* toneCurve1 = cmsBuildGamma(NULL, 1.0);
    cmsToneCurve* toneCurve2 = cmsBuildGamma(NULL, 1.5);
    cmsToneCurve* joinedToneCurve = cmsJoinToneCurve(ctx, toneCurve1, toneCurve2, fuzz_uint32_t_1);

    // Get the user data associated with the lcms context
    void* userData = cmsGetContextUserData(ctx);

    // Get the data from the IT8 handle
    const char* patch = "Patch";
    const char* sample = "Sample";
    const char* dataFromIT8 = cmsIT8GetData(hIT8, patch, sample);

    // Duplicate the profile sequence description
    cmsSEQ* profileSeq = cmsDupProfileSequenceDescription(NULL);

    // Release all allocated resources
    cmsIT8Free(hIT8);
    cmsFreeToneCurve(joinedToneCurve);
    cmsFreeToneCurve(toneCurve1);
    cmsFreeToneCurve(toneCurve2);
    cmsFreeProfileSequenceDescription(profileSeq);
    cmsDeleteContext(ctx);

    return 0;
}