#include "FDSan.h"
#include "FuzzedDataProvider.h"
#include <lcms2.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
//<ID> 3714
//<Prompt> ["cmsEvalToneCurveFloat","cmsFreeToneCurveTriple","cmsGetProfileIOhandler","cmsGetToneCurveEstimatedTable","cmsGDBCompute"]
/*<Combination>: [cmsFloat32Number cmsEvalToneCurveFloat(const cmsToneCurve * Curve, cmsFloat32Number v),
    void cmsFreeToneCurveTriple(cmsToneCurve Curve[3]),
    cmsIOHANDLER *cmsGetProfileIOhandler(cmsHPROFILE hProfile),
    const cmsUInt16Number *cmsGetToneCurveEstimatedTable(const cmsToneCurve * t),
    cmsBool cmsGDBCompute(cmsHANDLE hGDB, cmsUInt32Number dwFlags)
*/
//<score> 29.466667, nr_unique_branch: 50, p_cov: 0.8
//<Quality> {"diversity":18,"density":13,"covered_api":["cmsEvalToneCurveFloat","cmsGetProfileIOhandler","cmsGetToneCurveEstimatedTable","cmsGDBCompute"],"uncovered_api":["cmsFreeToneCurveTriple"],"unique_branches":{"InterpolateMissingSector":[[487,9,487,51,0],[487,9,487,51,1],[510,15,510,32,0],[510,15,510,32,1],[512,22,512,39,0],[512,22,512,39,1],[529,18,529,36,0],[529,18,529,36,1],[530,18,530,54,0],[530,18,530,54,1],[531,18,531,58,0],[532,18,532,54,0],[533,18,533,58,0]],"cmsGDBCompute":[[558,21,558,36,0],[558,21,558,36,1],[560,13,560,53,1],[564,21,564,36,0],[564,21,564,36,1],[566,13,566,61,1],[571,21,571,36,0],[571,21,571,36,1],[572,25,572,40,0],[572,25,572,40,1],[574,17,574,61,1]],"ClosestLineToLine":[[236,9,236,34,0],[236,9,236,34,1],[248,13,248,21,0],[248,13,248,21,1],[254,18,254,25,0],[254,18,254,25,1],[261,9,261,17,0],[261,9,261,17,1],[265,13,265,21,0],[265,13,265,21,1],[267,18,267,24,1],[274,14,274,21,0],[274,14,274,21,1],[279,13,279,27,0],[279,13,279,27,1],[281,18,281,30,1],[289,11,289,42,0],[289,11,289,42,1]],"FindNearSectors":[[447,15,447,25,0],[447,15,447,25,1],[457,13,457,18,0],[457,13,457,18,1],[458,13,458,18,0],[458,13,458,18,1],[462,13,462,35,0],[462,13,462,35,1]]},"library_calls":["cmsCreateContext","cmsOpenProfileFromMem","cmsGetProfileIOhandler","cmsBuildGamma","cmsGetToneCurveEstimatedTable","cmsEvalToneCurveFloat","cmsFreeToneCurve","cmsGBDAlloc","cmsGDBCompute","cmsGBDFree","cmsCloseProfile","cmsDeleteContext"]}
/*Here is the C++ program that achieves the given event using the lcms library APIs:

*/


extern "C" int LLVMFuzzerTestOneInput_32(const uint8_t* f_data, size_t f_size) {
	if(f_size<=4) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_1, fdp);
	//fuzzer shim end}




    cmsContext ctx = cmsCreateContext(NULL, NULL);

    // Open profile from memory
    cmsHPROFILE hProfile = cmsOpenProfileFromMem(data, static_cast<cmsUInt32Number>(size));
    if (hProfile == NULL) {
        cmsDeleteContext(ctx);
        return 0;
    }

    // Get profile IO handler
    cmsIOHANDLER *ioHandler = cmsGetProfileIOhandler(hProfile);
    if (ioHandler == NULL) {
        cmsCloseProfile(hProfile);
        cmsDeleteContext(ctx);
        return 0;
    }

    // Read tone curve estimated table
    cmsToneCurve *toneCurve = cmsBuildGamma(ctx, 2.2);
    const cmsUInt16Number *table = cmsGetToneCurveEstimatedTable(toneCurve);
    if (table == NULL) {
        cmsFreeToneCurve(toneCurve);
        cmsCloseProfile(hProfile);
        cmsDeleteContext(ctx);
        return 0;
    }

    // Evaluate tone curve
    cmsFloat32Number result = cmsEvalToneCurveFloat(toneCurve, 0.5);
    cmsFreeToneCurve(toneCurve);

    // GDB compute
    cmsHANDLE hGDB = cmsGBDAlloc(ctx);
    if (hGDB != NULL) {
        cmsGDBCompute(hGDB, fuzz_uint32_t_1);
        cmsGBDFree(hGDB);
    }

    // Clean up
    cmsCloseProfile(hProfile);
    cmsDeleteContext(ctx);

    return 0;
}