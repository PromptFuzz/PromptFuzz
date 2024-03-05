#include "FDSan.h"
#include "FuzzedDataProvider.h"
#include <lcms2.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
//<ID> 4666
//<Prompt> ["cmsPipelineEval16","cmsSaveProfileToMem","cmsGetAlarmCodes","cmsIT8DefineDblFormat","cmsCreateNULLProfileTHR"]
/*<Combination>: [void cmsPipelineEval16(const cmsUInt16Number * In, cmsUInt16Number * Out, const cmsPipeline * lut),
    cmsBool cmsSaveProfileToMem(cmsHPROFILE hProfile, void * MemPtr, cmsUInt32Number * BytesNeeded),
    void cmsGetAlarmCodes(cmsUInt16Number NewAlarm[16]),
    void cmsIT8DefineDblFormat(cmsHANDLE hIT8, const char * Formatter),
    cmsHPROFILE cmsCreateNULLProfileTHR(cmsContext ContextID)
*/
//<score> 37.846153, nr_unique_branch: 40, p_cov: 1
//<Quality> {"diversity":13,"density":12,"covered_api":["cmsPipelineEval16","cmsSaveProfileToMem","cmsGetAlarmCodes","cmsIT8DefineDblFormat","cmsCreateNULLProfileTHR"],"uncovered_api":[],"unique_branches":{"WriteMatrix":[[2623,17,2623,22,0],[2623,17,2623,22,1],[2625,13,2625,56,1],[2628,9,2628,26,1],[2636,21,2636,44,0],[2636,21,2636,44,1],[2638,17,2638,49,1]],"cmsIT8DefineDblFormat":[[2949,9,2949,26,1]],"Type_LUTB2A_Write":[[2954,9,2954,79,0],[2955,13,2955,142,1],[2966,9,2966,62,1],[2967,9,2967,63,1],[2968,9,2968,38,1],[2972,9,2972,38,1],[2973,9,2973,38,1],[2974,9,2974,38,1],[2975,9,2975,38,1],[2976,9,2976,38,1],[2978,9,2978,18,1],[2984,9,2984,21,1],[2989,9,2989,18,0],[2992,13,2992,70,1],[2995,9,2995,23,0],[2997,13,2997,43,1],[3000,9,3000,18,0],[3003,13,3003,70,1],[3008,9,3008,37,1],[3010,9,3010,44,1],[3011,9,3011,46,1],[3012,9,3012,44,1],[3013,9,3013,44,1],[3014,9,3014,44,1],[3016,9,3016,35,1]],"WriteSetOfCurves":[[2666,13,2666,41,0],[2677,13,2677,33,0],[2678,21,2678,62,1],[2681,13,2681,43,1]],"MemoryWrite":[[212,9,212,49,1]],"Type_Curve_Write":[[1197,9,1197,31,1],[1208,9,1208,53,1]]},"library_calls":["cmsCreateContext","cmsCreateNULLProfileTHR","cmsIT8LoadFromFile","cmsIT8DefineDblFormat","cmsIT8Free","cmsSaveProfileToMem","cmsSaveProfileToMem","cmsPipelineAlloc","cmsPipelineEval16","cmsPipelineFree","cmsGetAlarmCodes","cmsCloseProfile","cmsDeleteContext"]}
/**/


extern "C" int LLVMFuzzerTestOneInput_39(const uint8_t* f_data, size_t f_size) {
	if(f_size<=0) return 0;

	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(f_data, sizeof(uint8_t), f_size, input_file_ptr);
	fclose(input_file_ptr);

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (in_file == nullptr) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Create a NULL profile
    cmsContext context = cmsCreateContext(nullptr, nullptr);
    cmsHPROFILE nullProfile = cmsCreateNULLProfileTHR(context);

    // Define the double format for IT8 handling
    cmsHANDLE it8Handle = cmsIT8LoadFromFile(context, "input_file");
    if (it8Handle != nullptr) {
        cmsIT8DefineDblFormat(it8Handle, "Formatter");
        cmsIT8Free(it8Handle);
    }

    // Save the profile to memory
    cmsUInt32Number bytesNeeded;
    cmsSaveProfileToMem(nullProfile, nullptr, &bytesNeeded);
    void* profileData = malloc(bytesNeeded);
    cmsSaveProfileToMem(nullProfile, profileData, &bytesNeeded);

    // Evaluate the pipeline
    cmsPipeline* pipeline = cmsPipelineAlloc(context, 3, 3);
    cmsUInt16Number inData[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
    cmsUInt16Number outData[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
    cmsPipelineEval16(inData, outData, pipeline);
    cmsPipelineFree(pipeline);

    // Get alarm codes
    cmsUInt16Number alarmCodes[16];
    cmsGetAlarmCodes(alarmCodes);

    // Cleanup
    free(profileData);
    cmsCloseProfile(nullProfile);
    cmsDeleteContext(context);
    assert_file_closed(&in_file);;

    assert_file_name_closed("input_file");
	assert_file_closed(&in_file);
	return 0;
}