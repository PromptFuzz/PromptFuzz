#include "FDSan.h"
#include "FuzzedDataProvider.h"
#include <lcms2.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
//<ID> 1964
//<Prompt> ["cmsCreateLinearizationDeviceLinkTHR","cmsCreateRGBProfile","cmsSetColorSpace","cmsFreeToneCurve","cmsGetHeaderCreator","cmsLab2LCh","cmsLinkTag","cmsSaveProfileToIOhandler"]
/*<Combination>: [cmsHPROFILE cmsCreateLinearizationDeviceLinkTHR(cmsContext ContextID, cmsColorSpaceSignature ColorSpace, cmsToneCurve *const * TransferFunctions),
    cmsHPROFILE cmsCreateRGBProfile(const cmsCIExyY * WhitePoint, const cmsCIExyYTRIPLE * Primaries, cmsToneCurve *const TransferFunction[3]),
    void cmsSetColorSpace(cmsHPROFILE hProfile, cmsColorSpaceSignature sig),
    void cmsFreeToneCurve(cmsToneCurve * Curve),
    cmsUInt32Number cmsGetHeaderCreator(cmsHPROFILE hProfile),
    void cmsLab2LCh(cmsCIELCh * LCh, const cmsCIELab * Lab),
    cmsBool cmsLinkTag(cmsHPROFILE hProfile, cmsTagSignature sig, cmsTagSignature dest),
    cmsUInt32Number cmsSaveProfileToIOhandler(cmsHPROFILE hProfile, cmsIOHANDLER * io)
*/
//<score> 3.28125, nr_unique_branch: 4, p_cov: 0.875
//<Quality> {"diversity":12,"density":9,"covered_api":["cmsCreateLinearizationDeviceLinkTHR","cmsCreateRGBProfile","cmsSetColorSpace","cmsGetHeaderCreator","cmsLab2LCh","cmsLinkTag","cmsSaveProfileToIOhandler"],"uncovered_api":["cmsFreeToneCurve"],"unique_branches":{"cmsWriteTag":[[1798,9,1798,21,0],[1802,14,1802,20,1]],"cmsCreateRGBProfileTHR":[[183,13,183,79,0],[212,9,212,13,0]]},"library_calls":["cmsCreateContext","cmsCreateLinearizationDeviceLinkTHR","cmsSetColorSpace","cmsCreateRGBProfile","cmsGetHeaderCreator","cmsLab2LCh","cmsLinkTag","cmsOpenIOhandlerFromNULL","cmsSaveProfileToIOhandler","cmsCloseProfile","cmsCloseProfile","cmsCloseIOhandler"]}
/**/

extern "C" int LLVMFuzzerTestOneInput_18(const uint8_t* f_data, size_t f_size) {
	if(f_size<=0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Step 4: Create FILE pointers for input and output
    FILE* in_file = fmemopen((void*)data, size, "rb");
    FILE* out_file = fopen("output_file", "wb");

    if (in_file == nullptr || out_file == nullptr) {
        // Error handling
        std::cerr << "Error opening input or output file" << std::endl;
        if (in_file != nullptr)
            assert_file_closed(&in_file);;
        if (out_file != nullptr)
            assert_file_closed(&out_file);;
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Step 5: Get file descriptors for reading and writing
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);

    // Step 2: Use lcms APIs to achieve the desired event
    cmsContext ctx = cmsCreateContext(nullptr, nullptr); // Create lcms context
    cmsHPROFILE profile = cmsCreateLinearizationDeviceLinkTHR(ctx, cmsSigRgbData, nullptr); // Create linearization device link profile

    // Set color space
    cmsSetColorSpace(profile, cmsSigRgbData);

    // Create RGB profile
    cmsCIExyY whitePoint = {0.3457, 0.3585, 1.0};
    cmsCIExyYTRIPLE primaries = {{0.6400, 0.3300, 1.0}, {0.3000, 0.6000, 1.0}, {0.1500, 0.0600, 1.0}};
    cmsToneCurve* transferFunctions[3] = {nullptr, nullptr, nullptr};
    cmsHPROFILE rgbProfile = cmsCreateRGBProfile(&whitePoint, &primaries, transferFunctions);

    // Get profile header creator
    cmsUInt32Number creator = cmsGetHeaderCreator(profile);

    // Convert from Lab to LCh
    cmsCIELab Lab = {50.0, 0.0, 0.0};
    cmsCIELCh LCh;
    cmsLab2LCh(&LCh, &Lab);

    // Link tags
    cmsTagSignature sig = cmsSigMediaBlackPointTag;
    cmsTagSignature dest = cmsSigMediaWhitePointTag;
    cmsLinkTag(profile, sig, dest);

    // Save profile to IO handler
    cmsIOHANDLER* ioHandler = cmsOpenIOhandlerFromNULL(ctx);
    cmsSaveProfileToIOhandler(profile, ioHandler);

    // Step 6: Release resources
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    cmsCloseProfile(profile);
    cmsCloseProfile(rgbProfile);
    cmsCloseIOhandler(ioHandler);

    // Step 7: Return 0 to indicate successful execution
    assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}