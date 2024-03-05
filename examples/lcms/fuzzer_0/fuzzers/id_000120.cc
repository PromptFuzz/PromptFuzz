#include "FDSan.h"
#include <lcms2.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
//<ID> 15475
//<Prompt> ["cmsIT8SetPropertyUncooked","cmsDupProfileSequenceDescription","cmsBuildTabulatedToneCurveFloat","cmsIT8GetDataRowColDbl","cmsSetEncodedICCversion","cmsChannelsOf"]
/*<Combination>: [cmsBool cmsIT8SetPropertyUncooked(cmsHANDLE hIT8, const char * Key, const char * Buffer),
    cmsSEQ *cmsDupProfileSequenceDescription(const cmsSEQ * pseq),
    cmsToneCurve *cmsBuildTabulatedToneCurveFloat(cmsContext ContextID, cmsUInt32Number nEntries, const cmsFloat32Number * values),
    cmsFloat64Number cmsIT8GetDataRowColDbl(cmsHANDLE hIT8, int row, int col),
    void cmsSetEncodedICCversion(cmsHPROFILE hProfile, cmsUInt32Number Version),
    cmsUInt32Number cmsChannelsOf(cmsColorSpaceSignature ColorSpace)
*/
//<score> 0.33333334, nr_unique_branch: 0, p_cov: 1
//<Quality> {"diversity":12,"density":4,"covered_api":["cmsIT8SetPropertyUncooked","cmsDupProfileSequenceDescription","cmsBuildTabulatedToneCurveFloat","cmsIT8GetDataRowColDbl","cmsSetEncodedICCversion","cmsChannelsOf"],"uncovered_api":[],"unique_branches":{},"library_calls":["cmsIT8Alloc","cmsIT8SetPropertyUncooked","cmsDupProfileSequenceDescription","cmsBuildTabulatedToneCurveFloat","cmsIT8GetDataRowColDbl","cmsCreateXYZProfile","cmsSetEncodedICCversion","cmsChannelsOf","cmsIT8Free","cmsFreeProfileSequenceDescription","cmsFreeToneCurve","cmsCloseProfile"]}
/**/

extern "C" int LLVMFuzzerTestOneInput_120(const uint8_t *data, size_t size) {
	if(size<=0) return 0;

    // Step 4: Create a FILE pointer for reading the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file == nullptr) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Step 5: Get the file descriptor for reading
    int in_fd = fuzz_fileno(in_file);

    // Step 1: Call the lcms APIs to achieve the desired event
    // Set custom property
    cmsHANDLE it8Handle = cmsIT8Alloc(nullptr);
    cmsIT8SetPropertyUncooked(it8Handle, "PropertyA", "ValueA");

    // Duplicate profile sequence description
    cmsSEQ *seq = nullptr;
    cmsSEQ *dupSeq = cmsDupProfileSequenceDescription(seq);

    // Build tabulated tone curve
    cmsFloat32Number values[256];
    for (int i = 0; i < 256; i++) {
        values[i] = i / 255.0f;
    }
    cmsToneCurve *toneCurve = cmsBuildTabulatedToneCurveFloat(nullptr, 256, values);

    // Get data from IT8 handle
    cmsFloat64Number dataValue = cmsIT8GetDataRowColDbl(it8Handle, 0, 0);

    // Set encoded ICC version
    cmsHPROFILE profile = cmsCreateXYZProfile();
    cmsSetEncodedICCversion(profile, 0x02030100);

    // Get the number of channels of a color space
    cmsUInt32Number channels = cmsChannelsOf(cmsSigLabData);

    // Step 7: Release allocated resources
    cmsIT8Free(it8Handle);
    cmsFreeProfileSequenceDescription(dupSeq);
    cmsFreeToneCurve(toneCurve);
    cmsCloseProfile(profile);

    // Step 6: Release the input FILE pointer
    assert_file_closed(&in_file);;

    assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
}