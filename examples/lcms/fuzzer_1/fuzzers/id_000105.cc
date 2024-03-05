#include "FDSan.h"
#include "FuzzedDataProvider.h"
#include <lcms2.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
//<ID> 14136
//<Prompt> ["cmsDeleteContext","cmsGetContextUserData","cmsAllocNamedColorList","cmsWriteRawTag","cmsIT8GetData"]
/*<Combination>: [void cmsDeleteContext(cmsContext ContextID),
    void *cmsGetContextUserData(cmsContext ContextID),
    cmsNAMEDCOLORLIST *cmsAllocNamedColorList(cmsContext ContextID, cmsUInt32Number n, cmsUInt32Number ColorantCount, const char * Prefix, const char * Suffix),
    cmsBool cmsWriteRawTag(cmsHPROFILE hProfile, cmsTagSignature sig, const void * data, cmsUInt32Number Size),
    const char *cmsIT8GetData(cmsHANDLE hIT8, const char * cPatch, const char * cSample)
*/
//<score> 0.90000004, nr_unique_branch: 2, p_cov: 0.6
//<Quality> {"diversity":4,"density":2,"covered_api":["cmsDeleteContext","cmsGetContextUserData","cmsAllocNamedColorList"],"uncovered_api":["cmsWriteRawTag","cmsIT8GetData"],"unique_branches":{"cmsDeleteContext":[[962,9,962,26,0],[965,13,965,42,1]]},"library_calls":["cmsDeleteContext","cmsGetContextUserData","cmsAllocNamedColorList","cmsFreeNamedColorList"]}
/**/


extern "C" int LLVMFuzzerTestOneInput_105(const uint8_t* f_data, size_t f_size) {
	if(f_size<=18) return 0;

	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(f_data, sizeof(uint8_t), f_size, input_file_ptr);
	fclose(input_file_ptr);

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
	//fuzzer shim end}




    // Step 4: Open the input data as a file
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Step 5: Get the file descriptor for reading
    int in_fd = fuzz_fileno(in_file);

    // Step 6: Use the file name as needed
    const char *input_file = "input_file";

    // Step 1: Delete the context
    cmsDeleteContext(NULL);

    // Step 2: Get the user data from the context
    cmsGetContextUserData(NULL);

    // Step 3: Allocate a named color list
    cmsNAMEDCOLORLIST *named_color_list = cmsAllocNamedColorList(NULL, 10, 3, fuzz_str_1, fuzz_str_2);

    // Step 7: Clean up resources
    cmsFreeNamedColorList(named_color_list);
    assert_file_closed(&in_file);;

    assert_file_name_closed("input_file");
	assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
}