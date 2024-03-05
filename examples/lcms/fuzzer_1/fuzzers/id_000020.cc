#include "FDSan.h"
#include "FuzzedDataProvider.h"
#include <lcms2.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
//<ID> 2217
//<Prompt> ["cmsDupProfileSequenceDescription","cmsAdaptToIlluminant","cmsIT8SetDataRowColDbl","cmsCreate_OkLabProfile","cmsReadRawTag"]
/*<Combination>: [cmsSEQ *cmsDupProfileSequenceDescription(const cmsSEQ * pseq),
    cmsBool cmsAdaptToIlluminant(cmsCIEXYZ * Result, const cmsCIEXYZ * SourceWhitePt, const cmsCIEXYZ * Illuminant, const cmsCIEXYZ * Value),
    cmsBool cmsIT8SetDataRowColDbl(cmsHANDLE hIT8, int row, int col, cmsFloat64Number Val),
    cmsHPROFILE cmsCreate_OkLabProfile(cmsContext ctx),
    cmsUInt32Number cmsReadRawTag(cmsHPROFILE hProfile, cmsTagSignature sig, void * Buffer, cmsUInt32Number BufferSize)
*/
//<score> 1.6, nr_unique_branch: 1, p_cov: 0.8
//<Quality> {"diversity":26,"density":26,"covered_api":["cmsAdaptToIlluminant","cmsIT8SetDataRowColDbl","cmsCreate_OkLabProfile","cmsReadRawTag"],"uncovered_api":["cmsDupProfileSequenceDescription"],"unique_branches":{"cmsAdaptToIlluminant":[[341,9,341,74,1]]},"library_calls":["cmsCreateContext","cmsAllocProfileSequenceDescription","cmsAdaptToIlluminant","cmsIT8Alloc","cmsIT8SetDataRowColDbl","cmsCreate_OkLabProfile","cmsReadRawTag","cmsIT8Free","cmsFreeProfileSequenceDescription","cmsCloseProfile","cmsDeleteContext"]}
/**/

extern "C" int LLVMFuzzerTestOneInput_20(const uint8_t* f_data, size_t f_size) {
	if(f_size<=12) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
	//fuzzer shim end}




    // Step 1: Create a context
    cmsContext ctx = cmsCreateContext(NULL, NULL);

    // Step 2: Create a profile sequence description
    cmsSEQ *seq = cmsAllocProfileSequenceDescription(ctx, fuzz_uint32_t_1);
    if (seq == NULL) {
        cmsDeleteContext(ctx);
        return 0;
    }

    // Step 3: Create a source white point
    cmsCIEXYZ srcWhitePoint;
    srcWhitePoint.X = 0.9642;
    srcWhitePoint.Y = 1.0000;
    srcWhitePoint.Z = 0.8249;

    // Step 4: Create an illuminant
    cmsCIEXYZ illuminant;
    illuminant.X = 0.3127;
    illuminant.Y = 0.3290;
    illuminant.Z = 0.3583;

    // Step 5: Create a value
    cmsCIEXYZ value;
    value.X = 0.5000;
    value.Y = 0.5000;
    value.Z = 0.5000;

    // Step 6: Adapt the value to the illuminant
    cmsBool success = cmsAdaptToIlluminant(&value, &srcWhitePoint, &illuminant, &value);
    if (!success) {
        cmsFreeProfileSequenceDescription(seq);
        cmsDeleteContext(ctx);
        return 0;
    }

    // Step 7: Set the data row and column value in an IT8 handle
    cmsHANDLE it8Handle = cmsIT8Alloc(ctx);
    if (it8Handle == NULL) {
        cmsFreeProfileSequenceDescription(seq);
        cmsDeleteContext(ctx);
        return 0;
    }

    double val = value.X;
    cmsBool setSuccess = cmsIT8SetDataRowColDbl(it8Handle, fuzz_int32_t_2, fuzz_int32_t_3, val);
    if (!setSuccess) {
        cmsIT8Free(it8Handle);
        cmsFreeProfileSequenceDescription(seq);
        cmsDeleteContext(ctx);
        return 0;
    }

    // Step 8: Create an OkLab profile
    cmsHPROFILE profile = cmsCreate_OkLabProfile(ctx);
    if (profile == NULL) {
        cmsIT8Free(it8Handle);
        cmsFreeProfileSequenceDescription(seq);
        cmsDeleteContext(ctx);
        return 0;
    }

    // Step 9: Read a raw tag from the profile
    unsigned char buffer[1024];
    cmsUInt32Number bufferSize = sizeof(buffer);
    cmsTagSignature tagSig = cmsSigMediaWhitePointTag;
    cmsUInt32Number bytesRead = cmsReadRawTag(profile, tagSig, buffer, bufferSize);
    if (bytesRead == 0) {
        cmsIT8Free(it8Handle);
        cmsFreeProfileSequenceDescription(seq);
        cmsCloseProfile(profile);
        cmsDeleteContext(ctx);
        return 0;
    }

    // Step 10: Cleanup
    cmsIT8Free(it8Handle);
    cmsFreeProfileSequenceDescription(seq);
    cmsCloseProfile(profile);
    cmsDeleteContext(ctx);

    return 0;
}