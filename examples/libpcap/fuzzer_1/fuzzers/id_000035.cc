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
//<ID> 2011
//<Prompt> ["pcap_tstamp_type_val_to_description","pcap_create","pcap_set_promisc","pcap_activate","pcap_inject","pcap_close","pcap_freecode"]
/*<Combination>: [const char *pcap_tstamp_type_val_to_description(int ),
    pcap_t *pcap_create(const char * , char * ),
    int pcap_set_promisc(pcap_t * , int ),
    int pcap_activate(pcap_t * ),
    int pcap_inject(pcap_t * , const void * , size_t ),
    void pcap_close(pcap_t * ),
    void pcap_freecode(struct bpf_program * )
*/
//<score> 6, nr_unique_branch: 0
//<Quality> {"density":6,"unique_branches":{},"library_calls":["pcap_tstamp_type_val_to_description","pcap_create","pcap_set_promisc","pcap_activate","pcap_inject","pcap_close"],"critical_calls":["pcap_tstamp_type_val_to_description","pcap_create","pcap_set_promisc","pcap_activate","pcap_inject","pcap_close"],"visited":0}
/**/
extern "C" int LLVMFuzzerTestOneInput_35(const uint8_t* f_data, size_t f_size) {
	if(f_size<21) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    // Step 4: Create an input file pointer
    FILE* in_file = fmemopen((void*)data, size, "rb");

    // Step 6: Define input file name
    const char* in_filename = "input_file";

    // Step 1: Define the event
    const char* event_description = pcap_tstamp_type_val_to_description(fuzz_int32_t_1);
    
    // Step 2: Create a pcap handler
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t* handle = pcap_create(fuzz_str_2, errbuf);
    if (!handle) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
    }

    // Step 3: Set promiscuous mode
    pcap_set_promisc(handle, fuzz_int32_t_3);

    // Step 5: Get file descriptor for input file
    int in_fd = fuzz_fileno(in_file);

    // Process the event
    pcap_activate(handle);
    pcap_inject(handle, &event_description, sizeof(event_description));

    // Clean up 
    pcap_close(handle);
    assert_file_closed(&in_file);;

    assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_fd_closed(in_fd);
	return 0;
}