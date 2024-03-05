#include "FDSan.h"
#include "FuzzedDataProvider.h"
#include <lcms2.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
//<ID> 1884
//<Prompt> ["cmsCreateLinearizationDeviceLinkTHR","cmsSetColorSpace","cmsIsCLUT","cmsFreeToneCurve","cmsLab2LCh","cmsLinkTag","cmsSaveProfileToIOhandler"]
/*<Combination>: [cmsHPROFILE cmsCreateLinearizationDeviceLinkTHR(cmsContext ContextID, cmsColorSpaceSignature ColorSpace, cmsToneCurve *const * TransferFunctions),
    void cmsSetColorSpace(cmsHPROFILE hProfile, cmsColorSpaceSignature sig),
    cmsBool cmsIsCLUT(cmsHPROFILE hProfile, cmsUInt32Number Intent, cmsUInt32Number UsedDirection),
    void cmsFreeToneCurve(cmsToneCurve * Curve),
    void cmsLab2LCh(cmsCIELCh * LCh, const cmsCIELab * Lab),
    cmsBool cmsLinkTag(cmsHPROFILE hProfile, cmsTagSignature sig, cmsTagSignature dest),
    cmsUInt32Number cmsSaveProfileToIOhandler(cmsHPROFILE hProfile, cmsIOHANDLER * io)
*/
//<score> 34.153847, nr_unique_branch: 36, p_cov: 1
//<Quality> {"diversity":13,"density":12,"covered_api":["cmsCreateLinearizationDeviceLinkTHR","cmsSetColorSpace","cmsIsCLUT","cmsFreeToneCurve","cmsLab2LCh","cmsLinkTag","cmsSaveProfileToIOhandler"],"uncovered_api":[],"unique_branches":{"WritePositionTable":[[287,9,287,31,1],[290,9,290,29,1],[296,15,296,24,0],[296,15,296,24,1],[298,13,298,42,1],[299,13,299,42,1],[303,15,303,24,0],[303,15,303,24,1],[309,13,309,54,1],[317,9,317,37,1],[319,15,319,25,0],[319,15,319,25,1],[320,13,320,58,1],[321,13,321,56,1],[324,9,324,35,1],[326,9,326,31,0],[327,9,327,29,0]],"WriteSeqID":[[3530,9,3530,56,1],[3533,9,3533,61,1]],"_cmsWriteUInt64Number":[[331,9,331,60,1]],"Type_MLU_Write":[[1587,9,1587,20,0],[1590,15,1590,44,1],[1591,15,1591,45,1]],"Type_ProfileSequenceDesc_Write":[[3417,9,3417,43,1],[3419,15,3419,26,0],[3419,15,3419,26,1],[3423,13,3423,56,1],[3424,13,3424,58,1],[3425,13,3425,58,1],[3426,13,3426,57,1],[3428,13,3428,59,1],[3429,13,3429,52,1]],"SaveDescription":[[3399,9,3399,38,1],[3405,13,3405,68,1]],"Type_ProfileSequenceId_Write":[[3550,9,3550,44,1],[3553,9,3553,79,1]]},"library_calls":["cmsCreateContext","cmsBuildGamma","cmsCreateLinearizationDeviceLinkTHR","cmsSetColorSpace","cmsIsCLUT","cmsFreeToneCurve","cmsLab2LCh","cmsLinkTag","cmsOpenIOhandlerFromMem","cmsSaveProfileToIOhandler","cmsCloseIOhandler","cmsCloseProfile","cmsDeleteContext"]}
/*Here is the C++ program that implements the steps mentioned above:

*/


extern "C" int LLVMFuzzerTestOneInput_17(const uint8_t* f_data, size_t f_size) {
	if(f_size<=0) return 0;

	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(f_data, sizeof(uint8_t), f_size, input_file_ptr);
	fclose(input_file_ptr);

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    cmsContext context = cmsCreateContext(NULL, NULL); // Create a context for the lcms library
    
    // Step 4: Create a FILE object for input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Step 5: Get a file descriptor for input data
    int in_fd = fuzz_fileno(in_file);
    
    // Step 6: Specify the input and output file names
    const char *input_file = "input_file";
    const char *output_file = "output_file";
    
    // Step 1: Create a linearization device link profile
    cmsColorSpaceSignature colorSpace = cmsSigGrayData;
    cmsToneCurve *transferFunction[1];
    transferFunction[0] = cmsBuildGamma(context, 1.0);
    cmsHPROFILE profile = cmsCreateLinearizationDeviceLinkTHR(context, colorSpace, transferFunction);
    
    // Step 2: Set the color space of the profile and check if it is a CLUT
    cmsSetColorSpace(profile, cmsSigXYZData);
    
    cmsUInt32Number intent = 0;
    cmsUInt32Number direction = 0;
    cmsBool isCLUT = cmsIsCLUT(profile, intent, direction);
    
    // Step 3: Free the tone curve and convert from Lab to LCh
    cmsFreeToneCurve(transferFunction[0]);
    cmsCIELab Lab;
    cmsCIELCh LCh;
    cmsLab2LCh(&LCh, &Lab);
    
    // Step 8: Link two tags in the profile and save it to an IO handler
    cmsTagSignature tag1 = cmsSigMediaWhitePointTag;
    cmsTagSignature tag2 = cmsSigChromaticAdaptationTag;
    cmsLinkTag(profile, tag1, tag2);
    
    cmsIOHANDLER *ioHandler = cmsOpenIOhandlerFromMem(context, NULL, 0, "wb");
    cmsSaveProfileToIOhandler(profile, ioHandler);
    cmsCloseIOhandler(ioHandler);
    
    // Step 7: Release allocated resources
    assert_file_closed(&in_file);;
    cmsCloseProfile(profile);
    cmsDeleteContext(context);
    
    assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
}