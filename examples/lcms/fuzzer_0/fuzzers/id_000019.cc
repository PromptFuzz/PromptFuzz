#include "FDSan.h"
#include <lcms2.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
//<ID> 2162
//<Prompt> ["cmsDupProfileSequenceDescription","cmsAdaptToIlluminant","cmsIT8SetDataRowColDbl","cmsCreate_OkLabProfile","cmsReadRawTag"]
/*<Combination>: [cmsSEQ *cmsDupProfileSequenceDescription(const cmsSEQ * pseq),
    cmsBool cmsAdaptToIlluminant(cmsCIEXYZ * Result, const cmsCIEXYZ * SourceWhitePt, const cmsCIEXYZ * Illuminant, const cmsCIEXYZ * Value),
    cmsBool cmsIT8SetDataRowColDbl(cmsHANDLE hIT8, int row, int col, cmsFloat64Number Val),
    cmsHPROFILE cmsCreate_OkLabProfile(cmsContext ctx),
    cmsUInt32Number cmsReadRawTag(cmsHPROFILE hProfile, cmsTagSignature sig, void * Buffer, cmsUInt32Number BufferSize)
*/
//<score> 5.4545455, nr_unique_branch: 11, p_cov: 1
//<Quality> {"diversity":11,"density":5,"covered_api":["cmsDupProfileSequenceDescription","cmsAdaptToIlluminant","cmsIT8SetDataRowColDbl","cmsCreate_OkLabProfile","cmsReadRawTag"],"uncovered_api":[],"unique_branches":{"cmsReadRawTag":[[2008,9,2008,96,0]],"cmsAdaptToIlluminant":[[341,9,341,74,0]],"Type_LUTA2B_Write":[[2767,13,2767,83,0],[2768,17,2768,146,0],[2769,21,2769,146,0],[2770,25,2771,98,0]],"SetData":[[1691,9,1691,29,1],[1691,33,1691,41,1],[1696,9,1696,30,1],[1696,34,1696,44,1]],"ComputeChromaticAdaptation":[[218,9,218,58,0]]},"library_calls":["cmsCreateContext","cmsDupProfileSequenceDescription","cmsAdaptToIlluminant","cmsIT8Alloc","cmsIT8SetDataRowColDbl","cmsCreate_OkLabProfile","cmsReadRawTag","cmsIT8Free","cmsCloseProfile","cmsFreeProfileSequenceDescription","cmsDeleteContext"]}
/*Here's an example implementation of the LLVMFuzzerTestOneInput_19 function using the lcms library APIs:

*/


extern "C" int LLVMFuzzerTestOneInput_19(const uint8_t *data, size_t size) {
	if(size<=0) return 0;

    // Create a context
    cmsContext ctx = cmsCreateContext(nullptr, nullptr);

    // Create a profile sequence description
    cmsSEQ *pseq = cmsDupProfileSequenceDescription(nullptr);

    // Adapt to illuminant
    cmsCIEXYZ whitePt, illuminant, value;
    cmsBool result = cmsAdaptToIlluminant(&whitePt, &whitePt, &illuminant, &value);

    // Create an IT8 handle
    cmsHANDLE hIT8 = cmsIT8Alloc(nullptr);

    // Set a data row and column value
    cmsBool it8Result = cmsIT8SetDataRowColDbl(hIT8, 0, 0, 1.0);

    // Create an OkLab profile
    cmsHPROFILE profile = cmsCreate_OkLabProfile(ctx);

    // Read a raw tag from the profile
    uint8_t buffer[1024];
    cmsUInt32Number bytesRead = cmsReadRawTag(profile, cmsSigAToB0Tag, buffer, sizeof(buffer));

    // Clean up resources
    cmsIT8Free(hIT8);
    cmsCloseProfile(profile);
    cmsFreeProfileSequenceDescription(pseq);
    cmsDeleteContext(ctx);

    return 0;
}