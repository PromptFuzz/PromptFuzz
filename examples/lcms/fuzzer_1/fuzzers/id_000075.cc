#include "FDSan.h"
#include "FuzzedDataProvider.h"
#include <lcms2.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
//<ID> 8965
//<Prompt> ["cmsDoTransformLineStride","cmsCreateLab4ProfileTHR","cmsFloat2LabEncodedV2","cmsIT8SetTableByLabel","_cmsICCcolorSpace"]
/*<Combination>: [void cmsDoTransformLineStride(cmsHTRANSFORM Transform, const void * InputBuffer, void * OutputBuffer, cmsUInt32Number PixelsPerLine, cmsUInt32Number LineCount, cmsUInt32Number BytesPerLineIn, cmsUInt32Number BytesPerLineOut, cmsUInt32Number BytesPerPlaneIn, cmsUInt32Number BytesPerPlaneOut),
    cmsHPROFILE cmsCreateLab4ProfileTHR(cmsContext ContextID, const cmsCIExyY * WhitePoint),
    void cmsFloat2LabEncodedV2(cmsUInt16Number wLab[3], const cmsCIELab * Lab),
    int cmsIT8SetTableByLabel(cmsHANDLE hIT8, const char * cSet, const char * cField, const char * ExpectedType),
    cmsColorSpaceSignature _cmsICCcolorSpace(int OurNotation)
*/
//<score> 7.5, nr_unique_branch: 8, p_cov: 1
//<Quality> {"diversity":12,"density":10,"covered_api":["cmsDoTransformLineStride","cmsCreateLab4ProfileTHR","cmsFloat2LabEncodedV2","cmsIT8SetTableByLabel","_cmsICCcolorSpace"],"uncovered_api":[],"unique_branches":{"cmsDeleteTransform":[[156,9,156,17,1]],"AllocEmptyTransform":[[871,12,871,26,1],[969,13,969,46,0]],"cmsCreateExtendedTransform":[[1137,9,1137,41,0]],"NullXFORM":[[381,17,381,30,0],[381,17,381,30,1],[386,24,386,41,0],[386,24,386,41,1]]},"library_calls":["cmsCreateContext","cmsCreateLab4ProfileTHR","cmsCreateTransformTHR","cmsDoTransformLineStride","cmsFloat2LabEncodedV2","cmsIT8Alloc","cmsIT8SetTableByLabel","_cmsICCcolorSpace","cmsIT8Free","cmsCloseProfile","cmsDeleteTransform","cmsDeleteContext"]}
/**/


extern "C" int LLVMFuzzerTestOneInput_75(const uint8_t* f_data, size_t f_size) {
	if(f_size<=0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Create a context
    cmsContext context = cmsCreateContext(NULL, NULL);
  
    // Read the input data into a memory file
    FILE *in_file = fmemopen((void *)data, size, "rb");
    
    // Create a Lab4 profile with a white point
    cmsCIExyY whitePoint = { 0.34567, 0.54321, 0.98765 };
    cmsHPROFILE profile = cmsCreateLab4ProfileTHR(context, &whitePoint);
    
    // Create a transform using the Lab4 profile
    cmsHTRANSFORM transform = cmsCreateTransformTHR(context, profile, TYPE_Lab_8, profile, TYPE_Lab_8, INTENT_PERCEPTUAL, cmsFLAGS_NULLTRANSFORM);
    
    // Allocate memory for the input and output buffers
    cmsUInt32Number pixelsPerLine = 128;
    cmsUInt32Number lineCount = 10;
    cmsUInt32Number bytesPerLineIn = pixelsPerLine * 4;
    cmsUInt32Number bytesPerLineOut = pixelsPerLine * 4;
    cmsUInt32Number bytesPerPlaneIn = bytesPerLineIn * lineCount;
    cmsUInt32Number bytesPerPlaneOut = bytesPerLineOut * lineCount;
    void *inputBuffer = malloc(bytesPerPlaneIn);
    void *outputBuffer = malloc(bytesPerPlaneOut);
    
    // Perform the transform on the input data
    cmsDoTransformLineStride(transform, inputBuffer, outputBuffer, pixelsPerLine, lineCount, bytesPerLineIn, bytesPerLineOut, bytesPerPlaneIn, bytesPerPlaneOut);
    
    // Convert the Lab values to encoded V2
    cmsCIELab lab = { 30.0, 40.0, 50.0 };
    cmsUInt16Number wLab[3];
    cmsFloat2LabEncodedV2(wLab, &lab);
    
    // Set a table by label in an IT8 handle
    cmsHANDLE it8Handle = cmsIT8Alloc(context);
    const char *set = "set1";
    const char *field = "field1";
    const char *expectedType = "type1";
    int result = cmsIT8SetTableByLabel(it8Handle, set, field, expectedType);
    
    // Convert an ICC color space to our notation
    int ourNotation = 12345;
    cmsColorSpaceSignature colorSpace = _cmsICCcolorSpace(ourNotation);
    
    // Close the input file and free resources
    assert_file_closed(&in_file);;
    free(inputBuffer);
    free(outputBuffer);
    cmsIT8Free(it8Handle);
    cmsCloseProfile(profile);
    cmsDeleteTransform(transform);
    cmsDeleteContext(context);
  
    assert_file_closed(&in_file);
	return 0;
}