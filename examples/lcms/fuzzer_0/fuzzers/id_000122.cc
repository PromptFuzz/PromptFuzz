#include "FDSan.h"
#include <lcms2.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
//<ID> 15660
//<Prompt> ["cmsIT8SetPropertyUncooked","cmsDupProfileSequenceDescription","cmsBuildTabulatedToneCurveFloat","cmsDesaturateLab","cmsSetEncodedICCversion","cmsChannelsOf"]
/*<Combination>: [cmsBool cmsIT8SetPropertyUncooked(cmsHANDLE hIT8, const char * Key, const char * Buffer),
    cmsSEQ *cmsDupProfileSequenceDescription(const cmsSEQ * pseq),
    cmsToneCurve *cmsBuildTabulatedToneCurveFloat(cmsContext ContextID, cmsUInt32Number nEntries, const cmsFloat32Number * values),
    cmsBool cmsDesaturateLab(cmsCIELab * Lab, double amax, double amin, double bmax, double bmin),
    void cmsSetEncodedICCversion(cmsHPROFILE hProfile, cmsUInt32Number Version),
    cmsUInt32Number cmsChannelsOf(cmsColorSpaceSignature ColorSpace)
*/
//<score> 0.46666667, nr_unique_branch: 0, p_cov: 1
//<Quality> {"diversity":15,"density":7,"covered_api":["cmsIT8SetPropertyUncooked","cmsDupProfileSequenceDescription","cmsBuildTabulatedToneCurveFloat","cmsDesaturateLab","cmsSetEncodedICCversion","cmsChannelsOf"],"uncovered_api":[],"unique_branches":{},"library_calls":["cmsCreateContext","cmsIT8Alloc","cmsIT8SetPropertyUncooked","cmsDupProfileSequenceDescription","cmsBuildTabulatedToneCurveFloat","cmsDesaturateLab","cmsCreate_sRGBProfile","cmsSetEncodedICCversion","cmsGetColorSpace","cmsChannelsOf","cmsDeleteContext","cmsIT8Free","cmsFreeProfileSequenceDescription","cmsFreeToneCurve","cmsCloseProfile"]}
/*Here is the C++ program that achieves the event mentioned above:

*/


extern "C" int LLVMFuzzerTestOneInput_122(const uint8_t *data, size_t size) {
	if(size<=0) return 0;

    // Create a context
    cmsContext ctx = cmsCreateContext(NULL, NULL);

    // Create an IT8 handle
    cmsHANDLE hIT8 = cmsIT8Alloc(ctx);

    // Set an uncooked property
    cmsBool success = cmsIT8SetPropertyUncooked(hIT8, "key", "value");

    // Duplicate profile sequence description
    cmsSEQ *sequence = cmsDupProfileSequenceDescription(NULL);

    // Build a tabulated tone curve
    cmsToneCurve *toneCurve = cmsBuildTabulatedToneCurveFloat(ctx, 5, NULL);

    // Desaturate Lab values
    cmsCIELab Lab;
    double amax = 1.0, amin = 0.0, bmax = 1.0, bmin = 0.0;
    cmsDesaturateLab(&Lab, amax, amin, bmax, bmin);

    // Set encoded ICC version
    cmsHPROFILE hProfile = cmsCreate_sRGBProfile();
    cmsSetEncodedICCversion(hProfile, 2);

    // Get the number of channels of a color space
    cmsColorSpaceSignature colorSpace = cmsGetColorSpace(hProfile);
    cmsUInt32Number channels = cmsChannelsOf(colorSpace);

    // Release all allocated resources
    cmsDeleteContext(ctx);
    cmsIT8Free(hIT8);
    cmsFreeProfileSequenceDescription(sequence);
    cmsFreeToneCurve(toneCurve);
    cmsCloseProfile(hProfile);

    return 0;
}