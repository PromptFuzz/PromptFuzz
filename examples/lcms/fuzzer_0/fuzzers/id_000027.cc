#include "FDSan.h"
#include <lcms2.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
//<ID> 3378
//<Prompt> ["cmsxyY2XYZ","cmsStageAllocIdentity","cmsSetHeaderModel","cmsDictDup","cmsGDBAddPoint"]
/*<Combination>: [void cmsxyY2XYZ(cmsCIEXYZ * Dest, const cmsCIExyY * Source),
    cmsStage *cmsStageAllocIdentity(cmsContext ContextID, cmsUInt32Number nChannels),
    void cmsSetHeaderModel(cmsHPROFILE hProfile, cmsUInt32Number model),
    cmsHANDLE cmsDictDup(cmsHANDLE hDict),
    cmsBool cmsGDBAddPoint(cmsHANDLE hGBD, const cmsCIELab * Lab)
*/
//<score> 4.8, nr_unique_branch: 5, p_cov: 0.8
//<Quality> {"diversity":38,"density":38,"covered_api":["cmsStageAllocIdentity","cmsSetHeaderModel","cmsDictDup","cmsGDBAddPoint"],"uncovered_api":["cmsxyY2XYZ"],"unique_branches":{"cmsChannelsOfColorSpace":[[900,5,900,24,0],[930,5,930,27,0],[936,5,936,27,0]],"cmsDictDup":[[965,9,965,21,1],[969,12,969,25,1]]},"library_calls":["cmsCreateContext","cmsOpenProfileFromMem","cmsStageAllocIdentity","cmsChannelsOf","cmsGetPCS","cmsSetHeaderModel","cmsGetHeaderModel","cmsDictAlloc","cmsDictDup","cmsGBDAlloc","cmsGDBAddPoint","cmsGBDFree","cmsDictFree","cmsDictFree","cmsStageFree","cmsCloseProfile","cmsDeleteContext"]}
/*Here is the complete C++ program that achieves the given event using the lcms library APIs:

*/


extern "C" int LLVMFuzzerTestOneInput_27(const uint8_t *data, size_t size) {
	if(size<=0) return 0;

    // Create a context
    cmsContext context = cmsCreateContext(NULL, NULL);
    if (context == NULL) {
        return 0;
    }

    // Create an input profile from memory
    cmsHPROFILE inputProfile = cmsOpenProfileFromMem(data, size);
    if (inputProfile == NULL) {
        cmsDeleteContext(context);
        return 0;
    }

    // Create an identity transform
    cmsStage *identityTransform = cmsStageAllocIdentity(context, cmsChannelsOf(cmsGetPCS(inputProfile)));
    if (identityTransform == NULL) {
        cmsCloseProfile(inputProfile);
        cmsDeleteContext(context);
        return 0;
    }

    // Set the header model of the profile
    cmsSetHeaderModel(inputProfile, cmsGetHeaderModel(inputProfile));

    // Duplicate a dictionary handle
    cmsHANDLE dictHandle = cmsDictAlloc(context);
    if (dictHandle == NULL) {
        cmsStageFree(identityTransform);
        cmsCloseProfile(inputProfile);
        cmsDeleteContext(context);
        return 0;
    }

    cmsHANDLE dictDupHandle = cmsDictDup(dictHandle);
    if (dictDupHandle == NULL) {
        cmsDictFree(dictHandle);
        cmsStageFree(identityTransform);
        cmsCloseProfile(inputProfile);
        cmsDeleteContext(context);
        return 0;
    }

    // Add a point to a Gamut Boundary Descriptor
    cmsHANDLE gdbHandle = cmsGBDAlloc(context);
    if (gdbHandle == NULL) {
        cmsDictFree(dictHandle);
        cmsDictFree(dictDupHandle);
        cmsStageFree(identityTransform);
        cmsCloseProfile(inputProfile);
        cmsDeleteContext(context);
        return 0;
    }

    cmsCIELab lab = {50.0, 0.0, 0.0};
    cmsBool addPointResult = cmsGDBAddPoint(gdbHandle, &lab);
    if (!addPointResult) {
        cmsGBDFree(gdbHandle);
        cmsDictFree(dictHandle);
        cmsDictFree(dictDupHandle);
        cmsStageFree(identityTransform);
        cmsCloseProfile(inputProfile);
        cmsDeleteContext(context);
        return 0;
    }

    // Cleanup and release resources
    cmsGBDFree(gdbHandle);
    cmsDictFree(dictHandle);
    cmsDictFree(dictDupHandle);
    cmsStageFree(identityTransform);
    cmsCloseProfile(inputProfile);
    cmsDeleteContext(context);

    return 0;
}