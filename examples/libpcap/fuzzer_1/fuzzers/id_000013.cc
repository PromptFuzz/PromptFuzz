#include "FDSan.h"
#include "FuzzedDataProvider.h"
#include <pcap/pcap.h>
#include <pcap/can_socketcan.h>
#include <pcap/bluetooth.h>
#include <pcap/ipnet.h>
#include <pcap/usb.h>
#include <pcap/vlan.h>
#include <pcap/sll.h>
#include <pcap/nflog.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 1174
//<Prompt> ["pcap_is_swapped","bpf_dump","pcap_activate","pcap_get_option_int","pcap_datalink_ext","pcap_statustostr"]
/*<Combination>: [int pcap_is_swapped(pcap_t * ),
    void bpf_dump(const struct bpf_program * , int ),
    int pcap_activate(pcap_t * ),
    int pcap_get_option_int(pcap_options * po, enum pcap_option_name pon),
    int pcap_datalink_ext(pcap_t * ),
    const char *pcap_statustostr(int )
*/
//<score> 5, nr_unique_branch: 0
//<Quality> {"density":5,"unique_branches":{},"library_calls":["pcap_create","pcap_is_swapped","bpf_dump","pcap_activate","pcap_alloc_option","pcap_get_option_int","pcap_datalink_ext","pcap_statustostr","pcap_close","pcap_free_option"],"critical_calls":["pcap_create","pcap_is_swapped","bpf_dump","pcap_activate","pcap_alloc_option","pcap_get_option_int","pcap_datalink_ext","pcap_statustostr","pcap_close","pcap_free_option"],"visited":0}
/**/
extern "C" int LLVMFuzzerTestOneInput_13(const uint8_t* f_data, size_t f_size) {
	if(f_size<21) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    // Step 4: Create input file
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (in_file == nullptr) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Step 6: Define input file name
    const char* input_file = "input_file";
    
    // Step 5: Get file descriptor for input file
    int in_fd = fuzz_fileno(in_file);
    
    // Step 4: Create output file
    FILE* out_file = fopen("output_file", "wb");
    if (out_file == nullptr) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Step 6: Define output file name
    const char* output_file = "output_file";
    
    // Step 5: Get file descriptor for output file
    int out_fd = fuzz_fileno(out_file);
    
    // Step 1: Check if pcap is swapped
    pcap_t* pcap = pcap_create(fuzz_str_1, nullptr);
    int is_swapped = pcap_is_swapped(pcap);
    
    // Step 2: Dump BPF program
    struct bpf_program program;
    bpf_dump(&program, fuzz_int32_t_2);
    
    // Step 1: Activate pcap handler
    pcap_activate(pcap);
    
    // Step 1: Get option int value
    pcap_options* options = pcap_alloc_option();
    int option_value = pcap_get_option_int(options, PON_TSTAMP_PRECISION);
    
    // Step 1: Get datalink extension
    int datalink_ext = pcap_datalink_ext(pcap);
    
    // Step 1: Convert status to string
    const char* status_str = pcap_statustostr(fuzz_int32_t_3);
    
    // Step 7: Release resources
    pcap_close(pcap);
    pcap_free_option(options);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}