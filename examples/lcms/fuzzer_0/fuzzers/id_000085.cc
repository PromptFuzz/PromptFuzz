#include "FDSan.h"
#include <lcms2.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
//<ID> 9717
//<Prompt> ["cmsIT8SetPropertyStr","cmsJoinToneCurve","cmsSetHeaderProfileID","cmsIT8GetData","cmsDupProfileSequenceDescription","cmsIT8SetComment"]
/*<Combination>: [cmsBool cmsIT8SetPropertyStr(cmsHANDLE hIT8, const char * cProp, const char * Str),
    cmsToneCurve *cmsJoinToneCurve(cmsContext ContextID, const cmsToneCurve * X, const cmsToneCurve * Y, cmsUInt32Number nPoints),
    void cmsSetHeaderProfileID(cmsHPROFILE hProfile, cmsUInt8Number * ProfileID),
    const char *cmsIT8GetData(cmsHANDLE hIT8, const char * cPatch, const char * cSample),
    cmsSEQ *cmsDupProfileSequenceDescription(const cmsSEQ * pseq),
    cmsBool cmsIT8SetComment(cmsHANDLE hIT8, const char * cComment)
*/
//<score> 0.85, nr_unique_branch: 0, p_cov: 1
//<Quality> {"diversity":20,"density":17,"covered_api":["cmsIT8SetPropertyStr","cmsJoinToneCurve","cmsSetHeaderProfileID","cmsIT8GetData","cmsDupProfileSequenceDescription","cmsIT8SetComment"],"uncovered_api":[],"unique_branches":{},"library_calls":["cmsCreateContext","cmsIT8Alloc","cmsIT8SetPropertyStr","cmsBuildGamma","cmsBuildGamma","cmsJoinToneCurve","cmsCreate_sRGBProfile","cmsSetHeaderProfileID","cmsIT8GetData","cmsAllocProfileSequenceDescription","cmsDupProfileSequenceDescription","cmsIT8SetComment","cmsIT8Free","cmsFreeToneCurve","cmsFreeToneCurve","cmsFreeToneCurve","cmsCloseProfile","cmsFreeProfileSequenceDescription","cmsFreeProfileSequenceDescription","cmsDeleteContext"]}
/**/

extern "C" int LLVMFuzzerTestOneInput_85(const uint8_t *data, size_t size) {
	if(size<=0) return 0;

    // Create a context
    cmsContext context = cmsCreateContext(nullptr, nullptr);

    // Create an input file from the provided data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Create an output file
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Read the input data and perform operations using lcms APIs
    // Set property string
    cmsHANDLE hIT8 = cmsIT8Alloc(context);
    cmsIT8SetPropertyStr(hIT8, "property", "value");

    // Join tone curves
    cmsToneCurve *curveX = cmsBuildGamma(context, 2.2);
    cmsToneCurve *curveY = cmsBuildGamma(context, 2.2);
    cmsUInt32Number nPoints = 256;
    cmsToneCurve *joinedCurve = cmsJoinToneCurve(context, curveX, curveY, nPoints);

    // Set header profile ID
    cmsHPROFILE profile = cmsCreate_sRGBProfile();
    cmsUInt8Number profileID[16];
    memset(profileID, 0, sizeof(profileID));
    cmsSetHeaderProfileID(profile, profileID);

    // Get IT8 data
    const char *patchData = cmsIT8GetData(hIT8, "patch", "data");

    // Duplicate profile sequence description
    cmsSEQ *originalSeq = cmsAllocProfileSequenceDescription(context, 2);
    cmsSEQ *duplicateSeq = cmsDupProfileSequenceDescription(originalSeq);

    // Set IT8 comment
    cmsIT8SetComment(hIT8, "comment");

    // Clean up resources
    cmsIT8Free(hIT8);
    cmsFreeToneCurve(curveX);
    cmsFreeToneCurve(curveY);
    cmsFreeToneCurve(joinedCurve);
    cmsCloseProfile(profile);
    cmsFreeProfileSequenceDescription(originalSeq);
    cmsFreeProfileSequenceDescription(duplicateSeq);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    cmsDeleteContext(context);

    assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}