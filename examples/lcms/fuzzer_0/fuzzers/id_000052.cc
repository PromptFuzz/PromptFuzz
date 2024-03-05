#include "FDSan.h"
#include <lcms2.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
//<ID> 6119
//<Prompt> ["cmsGetNamedColorList","cmsCreateLab4ProfileTHR","cmsGetProfileInfo","cmsReadRawTag","cmsSetHeaderRenderingIntent"]
/*<Combination>: [cmsNAMEDCOLORLIST *cmsGetNamedColorList(cmsHTRANSFORM xform),
    cmsHPROFILE cmsCreateLab4ProfileTHR(cmsContext ContextID, const cmsCIExyY * WhitePoint),
    cmsUInt32Number cmsGetProfileInfo(cmsHPROFILE hProfile, cmsInfoType Info, const char LanguageCode[3], const char CountryCode[3], wchar_t * Buffer, cmsUInt32Number BufferSize),
    cmsUInt32Number cmsReadRawTag(cmsHPROFILE hProfile, cmsTagSignature sig, void * Buffer, cmsUInt32Number BufferSize),
    void cmsSetHeaderRenderingIntent(cmsHPROFILE hProfile, cmsUInt32Number RenderingIntent)
*/
//<score> 0.8, nr_unique_branch: 0, p_cov: 0.8
//<Quality> {"diversity":10,"density":10,"covered_api":["cmsGetNamedColorList","cmsCreateLab4ProfileTHR","cmsReadRawTag","cmsSetHeaderRenderingIntent"],"uncovered_api":["cmsGetProfileInfo"],"unique_branches":{},"library_calls":["cmsCreateContext","cmsCreateLab4ProfileTHR","cmsReadRawTag","cmsReadRawTag","cmsCreateTransform","cmsGetNamedColorList","cmsSetHeaderRenderingIntent","cmsCloseProfile","cmsDeleteTransform","cmsDeleteContext"]}
/**/

extern "C" int LLVMFuzzerTestOneInput_52(const uint8_t *data, size_t size) {
	if(size<=0) return 0;

    // Create a context
    cmsContext context = cmsCreateContext(NULL, NULL);
  
    // Create a LAB4 profile
    cmsCIExyY whitePoint = {0.3457, 0.3585, 1.0};
    cmsHPROFILE labProfile = cmsCreateLab4ProfileTHR(context, &whitePoint);
  
    // Read the raw tag from the profile
    cmsTagSignature tagSig = cmsSigProfileDescriptionTag;
    cmsUInt32Number bufferSize = cmsReadRawTag(labProfile, tagSig, NULL, 0);
    uint8_t *buffer = new uint8_t[bufferSize];
    cmsReadRawTag(labProfile, tagSig, buffer, bufferSize);

    // Get the named color list from the transform
    cmsHTRANSFORM transform = cmsCreateTransform(labProfile, TYPE_Lab_16, labProfile, TYPE_Lab_16, INTENT_PERCEPTUAL, 0);
    cmsNAMEDCOLORLIST *namedColorList = cmsGetNamedColorList(transform);

    // Set the header rendering intent
    cmsUInt32Number intent = INTENT_PERCEPTUAL;
    cmsSetHeaderRenderingIntent(labProfile, intent);

    // Cleanup
    delete[] buffer;
    cmsCloseProfile(labProfile);
    cmsDeleteTransform(transform);
    cmsDeleteContext(context);

    return 0;
}