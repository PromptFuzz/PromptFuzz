#include "FDSan.h"
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
//<ID> 2491
//<Prompt> ["pcap_set_option_string","pcap_dispatch","pcap_init","pcap_offline_filter","pcap_close"]
/*<Combination>: [int pcap_set_option_string(pcap_options * po, enum pcap_option_name pon, const char * value),
    int pcap_dispatch(pcap_t * , int , pcap_handler , u_char * ),
    int pcap_init(unsigned int , char * ),
    int pcap_offline_filter(const struct bpf_program * , const struct pcap_pkthdr * , const u_char * ),
    void pcap_close(pcap_t * )
*/
//<score> 8, nr_unique_branch: 2
//<Quality> {"density":4,"unique_branches":{"pcap_init":[[279,2,279,26,0,0,4,0],[281,7,281,18,0,0,4,1]]},"library_calls":["pcap_alloc_option","pcap_set_option_string","pcap_init","pcap_free_option","pcap_free_option"],"critical_calls":["pcap_alloc_option","pcap_set_option_string","pcap_init","pcap_free_option"],"visited":0}
/**/
extern "C" int LLVMFuzzerTestOneInput_45(const uint8_t* data, size_t size) {
	if(size<0) return 0;

	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);

    pcap_t* pcap_handle = nullptr;
    char errbuf[PCAP_ERRBUF_SIZE];

    // Step 4: create a FILE pointer to read from the input data
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (in_file == nullptr) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Step 6: specify the input file name
    const char* input_file = "input_file";

    // Step 1: set the pcap options
    pcap_options* options = pcap_alloc_option();
    pcap_set_option_string(options, PON_IO_READ_PLUGIN, "input_plugin");

    // Step 2: initialize pcap with the options
    if (pcap_init(PCAP_TSTAMP_PRECISION_NANO, errbuf) == -1) {
        assert_file_closed(&in_file);;
        pcap_free_option(options);
        assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
    }

    // Step 7: release the allocated resources before return
    pcap_free_option(options);
    assert_file_closed(&in_file);;

    assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
}