#include "FDSan.h"
#include "FuzzedDataProvider.h"
#include <lcms2.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
//<ID> 6116
//<Prompt> ["cmsGetNamedColorList","cmsCreateLab4ProfileTHR","cmsGetProfileInfo","cmsReadRawTag","cmsSetHeaderRenderingIntent"]
/*<Combination>: [cmsNAMEDCOLORLIST *cmsGetNamedColorList(cmsHTRANSFORM xform),
    cmsHPROFILE cmsCreateLab4ProfileTHR(cmsContext ContextID, const cmsCIExyY * WhitePoint),
    cmsUInt32Number cmsGetProfileInfo(cmsHPROFILE hProfile, cmsInfoType Info, const char LanguageCode[3], const char CountryCode[3], wchar_t * Buffer, cmsUInt32Number BufferSize),
    cmsUInt32Number cmsReadRawTag(cmsHPROFILE hProfile, cmsTagSignature sig, void * Buffer, cmsUInt32Number BufferSize),
    void cmsSetHeaderRenderingIntent(cmsHPROFILE hProfile, cmsUInt32Number RenderingIntent)
*/
//<score> 3.2, nr_unique_branch: 3, p_cov: 0.8
//<Quality> {"diversity":11,"density":11,"covered_api":["cmsGetNamedColorList","cmsCreateLab4ProfileTHR","cmsReadRawTag","cmsSetHeaderRenderingIntent"],"uncovered_api":["cmsGetProfileInfo"],"unique_branches":{"cmsGetNamedColorList":[[772,9,772,20,0]],"cmsReadRawTag":[[1981,9,1981,21,0],[2008,9,2008,96,1]]},"library_calls":["cmsCreateContext","cmsCreateLab4ProfileTHR","cmsD50_xyY","cmsReadRawTag","cmsReadRawTag","cmsSetHeaderRenderingIntent","cmsCreateTransformTHR","cmsGetNamedColorList","cmsDeleteTransform","cmsCloseProfile","cmsDeleteContext"]}
/*Please find the C++ program to achieve the desired event using the lcms library APIs:

*/


extern "C" int LLVMFuzzerTestOneInput_50(const uint8_t* f_data, size_t f_size) {
	if(f_size<=0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Create a context
    cmsContext ctx = cmsCreateContext(nullptr, nullptr);

    // Create a LAB profile
    cmsHPROFILE labProfile = cmsCreateLab4ProfileTHR(ctx, cmsD50_xyY());

    // Read raw tag from profile
    cmsTagSignature sig = cmsSigMediaWhitePointTag;
    void *buffer = nullptr;
    cmsUInt32Number bufferSize = cmsReadRawTag(labProfile, sig, buffer, 0);
    if (bufferSize > 0) {
        buffer = new unsigned char[bufferSize];
        cmsReadRawTag(labProfile, sig, buffer, bufferSize);
    }

    // Set header rendering intent
    cmsSetHeaderRenderingIntent(labProfile, INTENT_ABSOLUTE_COLORIMETRIC);

    // Create a transform using the LAB profile
    cmsHTRANSFORM xform = cmsCreateTransformTHR(ctx, labProfile, TYPE_Lab_16, labProfile, TYPE_Lab_16, INTENT_PERCEPTUAL, 0);

    // Get the named color list from the transform
    cmsNAMEDCOLORLIST *namedColorList = cmsGetNamedColorList(xform);

    // Free allocated resources
    if (buffer) {
        delete[] static_cast<unsigned char*>(buffer);
    }
    cmsDeleteTransform(xform);
    cmsCloseProfile(labProfile);
    cmsDeleteContext(ctx);
    
    return 0;
}