#include "FDSan.h"
#include "FuzzedDataProvider.h"
#include <lcms2.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
//<ID> 2294
//<Prompt> ["cmsDictAlloc","cmsAdaptToIlluminant","cmsIT8SetDataRowColDbl","cmsIsToneCurveMultisegment","cmsCreate_OkLabProfile","cmsReadRawTag"]
/*<Combination>: [cmsHANDLE cmsDictAlloc(cmsContext ContextID),
    cmsBool cmsAdaptToIlluminant(cmsCIEXYZ * Result, const cmsCIEXYZ * SourceWhitePt, const cmsCIEXYZ * Illuminant, const cmsCIEXYZ * Value),
    cmsBool cmsIT8SetDataRowColDbl(cmsHANDLE hIT8, int row, int col, cmsFloat64Number Val),
    cmsBool cmsIsToneCurveMultisegment(const cmsToneCurve * InGamma),
    cmsHPROFILE cmsCreate_OkLabProfile(cmsContext ctx),
    cmsUInt32Number cmsReadRawTag(cmsHPROFILE hProfile, cmsTagSignature sig, void * Buffer, cmsUInt32Number BufferSize)
*/
//<score> 2.7857141, nr_unique_branch: 2, p_cov: 1
//<Quality> {"diversity":14,"density":13,"covered_api":["cmsDictAlloc","cmsAdaptToIlluminant","cmsIT8SetDataRowColDbl","cmsIsToneCurveMultisegment","cmsCreate_OkLabProfile","cmsReadRawTag"],"uncovered_api":[],"unique_branches":{"cmsDictFree":[[905,12,905,25,1]],"cmsDictAlloc":[[888,9,888,21,1]]},"library_calls":["cmsCreateContext","cmsDictAlloc","cmsCreate_OkLabProfile","cmsReadRawTag","cmsIT8Alloc","cmsIT8SetDataRowColDbl","cmsAdaptToIlluminant","cmsBuildGamma","cmsIsToneCurveMultisegment","cmsFreeToneCurve","cmsIT8Free","cmsCloseProfile","cmsDictFree","cmsDeleteContext"]}
/**/

extern "C" int LLVMFuzzerTestOneInput_21(const uint8_t* f_data, size_t f_size) {
	if(f_size<=0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Create a context
    cmsContext context = cmsCreateContext(NULL, NULL);

    // Create a dictionary handle
    cmsHANDLE dictHandle = cmsDictAlloc(context);

    // Create profiles
    cmsHPROFILE profile1 = cmsCreate_OkLabProfile(context);

    // Read raw tag from the profile
    cmsTagSignature tagSig = cmsSigProfileDescriptionTag;
    void *buffer = malloc(100);
    cmsUInt32Number bufferSize = cmsReadRawTag(profile1, tagSig, buffer, 100);

    // Set data row and column in IT8 handle
    cmsHANDLE it8Handle = cmsIT8Alloc(context);
    int row = 0, col = 0;
    cmsFloat64Number val = 1.0;
    cmsBool setDataRowColSuccess = cmsIT8SetDataRowColDbl(it8Handle, row, col, val);

    // Adapt to illuminant
    cmsCIEXYZ sourceWhitePt = {0.5, 0.5, 0.5};
    cmsCIEXYZ illuminant = {0.6, 0.6, 0.6};
    cmsCIEXYZ result;
    cmsBool adaptSuccess = cmsAdaptToIlluminant(&result, &sourceWhitePt, &illuminant, &sourceWhitePt);

    // Check if tone curve is multisegment
    cmsToneCurve *toneCurve = cmsBuildGamma(context, 2.2);
    cmsBool isMultisegment = cmsIsToneCurveMultisegment(toneCurve);

    // Cleanup
    cmsFreeToneCurve(toneCurve);
    cmsIT8Free(it8Handle);
    cmsCloseProfile(profile1);
    cmsDictFree(dictHandle);
    cmsDeleteContext(context);
    free(buffer);

    return 0;
}