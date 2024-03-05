#include "FDSan.h"
#include <lcms2.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
//<ID> 15029
//<Prompt> ["cmsIT8GetPatchName","cmsStageType","cmsGetStageContextID","_cmsICCcolorSpace","cmsIT8GetDataRowCol"]
/*<Combination>: [const char *cmsIT8GetPatchName(cmsHANDLE hIT8, int nPatch, char * buffer),
    cmsStageSignature cmsStageType(const cmsStage * mpe),
    cmsContext cmsGetStageContextID(const cmsStage * mpe),
    cmsColorSpaceSignature _cmsICCcolorSpace(int OurNotation),
    const char *cmsIT8GetDataRowCol(cmsHANDLE hIT8, int row, int col)
*/
//<score> 1.882353, nr_unique_branch: 1, p_cov: 1
//<Quality> {"diversity":17,"density":16,"covered_api":["cmsIT8GetPatchName","cmsStageType","cmsGetStageContextID","_cmsICCcolorSpace","cmsIT8GetDataRowCol"],"uncovered_api":[],"unique_branches":{"_cmsICCcolorSpace":[[771,8,771,14,0]]},"library_calls":["cmsCreateContext","cmsIT8Alloc","cmsIT8SetDataFormat","cmsIT8GetPatchName","cmsStageAllocCLutFloat","cmsStageType","cmsGetStageContextID","_cmsICCcolorSpace","cmsIT8GetDataRowCol","cmsStageFree","cmsIT8Free","cmsDeleteContext"]}
/**/


extern "C" int LLVMFuzzerTestOneInput_118(const uint8_t *data, size_t size) {
	if(size<=0) return 0;

	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);

    // Create lcms context
    cmsContext context = cmsCreateContext(NULL, NULL);

    // Create an IT8 handle
    cmsHANDLE hIT8 = cmsIT8Alloc(context);

    if (hIT8 == NULL) {
        // Clean up resources
        cmsDeleteContext(context);
        return 0;
    }

    // Set the data format for the IT8 handle
    cmsIT8SetDataFormat(hIT8, 0, "Sample");

    // Read the input data using a FILE pointer
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file == NULL) {
        // Clean up resources
        cmsIT8Free(hIT8);
        cmsDeleteContext(context);
        assert_file_closed(&in_file);
	return 0;
    }

    // Get the patch name from the IT8 handle
    char patchName[256];
    cmsIT8GetPatchName(hIT8, 0, patchName);

    // Close the input file
    assert_file_closed(&in_file);;

    // Create a stage
    cmsStage *stage = cmsStageAllocCLutFloat(context, 3, 3, 3, NULL);
    if (stage == NULL) {
        // Clean up resources
        cmsIT8Free(hIT8);
        cmsDeleteContext(context);
        assert_file_closed(&in_file);
	return 0;
    }

    // Get the stage type signature
    cmsStageSignature stageType = cmsStageType(stage);

    // Get the context ID for the stage
    cmsContext stageContextID = cmsGetStageContextID(stage);

    // Get the color space from ICC notation
    cmsColorSpaceSignature colorSpace = _cmsICCcolorSpace(2);

    // Create a file name string
    const char *fileName = "input_file";

    // Get the IT8 data for a specific row and column
    const char *rowDataCol = cmsIT8GetDataRowCol(hIT8, 0, 0);

    // Print the results
    printf("Patch Name: %s\n", patchName);
    printf("Stage Type: %d\n", stageType);
    printf("Stage Context ID: %p\n", stageContextID);
    printf("Color Space: %d\n", colorSpace);
    printf("File Name: %s\n", fileName);
    printf("Row Data Col: %s\n", rowDataCol);

    // Clean up resources
    cmsStageFree(stage);
    cmsIT8Free(hIT8);
    cmsDeleteContext(context);

    assert_file_name_closed("input_file");
	assert_file_closed(&in_file);
	return 0;
}