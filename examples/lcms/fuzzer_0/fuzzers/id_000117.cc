#include "FDSan.h"
#include <lcms2.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
//<ID> 14791
//<Prompt> ["cmsEvalToneCurveFloat","cmsBuildTabulatedToneCurveFloat","cmsCreate_sRGBProfileTHR","cmsSetHeaderProfileID","cmsNamedColorInfo"]
/*<Combination>: [cmsFloat32Number cmsEvalToneCurveFloat(const cmsToneCurve * Curve, cmsFloat32Number v),
    cmsToneCurve *cmsBuildTabulatedToneCurveFloat(cmsContext ContextID, cmsUInt32Number nEntries, const cmsFloat32Number * values),
    cmsHPROFILE cmsCreate_sRGBProfileTHR(cmsContext ContextID),
    void cmsSetHeaderProfileID(cmsHPROFILE hProfile, cmsUInt8Number * ProfileID),
    cmsBool cmsNamedColorInfo(const cmsNAMEDCOLORLIST * NamedColorList, cmsUInt32Number nColor, char * Name, char * Prefix, char * Suffix, cmsUInt16Number * PCS, cmsUInt16Number * Colorant)
*/
//<score> 1, nr_unique_branch: 0, p_cov: 1
//<Quality> {"diversity":12,"density":12,"covered_api":["cmsEvalToneCurveFloat","cmsBuildTabulatedToneCurveFloat","cmsCreate_sRGBProfileTHR","cmsSetHeaderProfileID","cmsNamedColorInfo"],"uncovered_api":[],"unique_branches":{},"library_calls":["cmsCreateContext","cmsSetLogErrorHandlerTHR","cmsCreate_sRGBProfileTHR","cmsSetHeaderProfileID","cmsAllocNamedColorList","cmsNamedColorInfo","cmsBuildTabulatedToneCurveFloat","cmsEvalToneCurveFloat","cmsCloseProfile","cmsFreeNamedColorList","cmsFreeToneCurve","cmsDeleteContext"]}
/**/


extern "C" int LLVMFuzzerTestOneInput_117(const uint8_t *data, size_t size)
{
	if(size<=0) return 0;

    // Create a memory stream from the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");

    // Create an sRGB profile
    cmsContext context = cmsCreateContext(NULL, NULL);
    cmsSetLogErrorHandlerTHR(context, NULL);
    cmsHPROFILE sRGBProfile = cmsCreate_sRGBProfileTHR(context);

    // Set the profile ID
    cmsUInt8Number profileID[16] = {0};
    cmsSetHeaderProfileID(sRGBProfile, profileID);

    // Create a named color list
    cmsNAMEDCOLORLIST *namedColorList = cmsAllocNamedColorList(context, 1, 3, "_", "");

    // Fill the named color list
    char name[256];
    char prefix[256];
    char suffix[256];
    cmsUInt16Number pcs[3];
    cmsUInt16Number colorant[16];
    cmsNamedColorInfo(namedColorList, 0, name, prefix, suffix, pcs, colorant);

    // Build a tone curve
    cmsFloat32Number values[256];
    cmsToneCurve *toneCurve = cmsBuildTabulatedToneCurveFloat(context, 256, values);

    // Evaluate the tone curve
    cmsFloat32Number v = cmsEvalToneCurveFloat(toneCurve, 0.5f);

    // Release resources
    assert_file_closed(&in_file);;
    cmsCloseProfile(sRGBProfile);
    cmsFreeNamedColorList(namedColorList);
    cmsFreeToneCurve(toneCurve);
    cmsDeleteContext(context);

    assert_file_closed(&in_file);
	return 0;
}