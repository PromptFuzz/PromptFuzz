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
//<ID> 1443
//<Prompt> ["pcap_getnonblock","pcap_set_option_int","pcap_init","pcap_set_rfmon","pcap_findalldevs","pcap_lookupdev"]
/*<Combination>: [int pcap_getnonblock(pcap_t * , char * ),
    int pcap_set_option_int(pcap_options * po, enum pcap_option_name pon, const int value),
    int pcap_init(unsigned int , char * ),
    int pcap_set_rfmon(pcap_t * , int ),
    int pcap_findalldevs(pcap_if_t ** , char * ),
    char *pcap_lookupdev(char * )
*/
//<score> 5, nr_unique_branch: 0
//<Quality> {"density":5,"unique_branches":{},"library_calls":["pcap_init","pcap_alloc_option","pcap_set_option_int","pcap_free_option","pcap_findalldevs","pcap_free_option","pcap_free_option","pcap_freealldevs"],"critical_calls":["pcap_init","pcap_alloc_option","pcap_set_option_int","pcap_findalldevs","pcap_free_option","pcap_freealldevs"],"visited":0}
/**/

extern "C" int LLVMFuzzerTestOneInput_23(const uint8_t* data, size_t size) {
	if(size<0) return 0;

	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);

    // Step 4: Create input file
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (in_file == NULL) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Step 5: Get input file descriptor
    int in_fd = fuzz_fileno(in_file);
    if (in_fd < 0) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
    }

    // Step 6: Set input file name
    const char* input_file = "input_file";

    // Step 1: Initialize pcap
    unsigned int linktype = 0;
    char errbuf[PCAP_ERRBUF_SIZE];
    int res = pcap_init(linktype, errbuf);
    if (res != 0) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_fd_closed(in_fd);
	return 0;
    }

    // Step 2: Set pcap option
    pcap_options* options = pcap_alloc_option();
    res = pcap_set_option_int(options, PON_TSTAMP_PRECISION, 3);
    if (res != 0) {
        pcap_free_option(options);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_fd_closed(in_fd);
	return 0;
    }

    // Step 3: Find all available devices
    pcap_if_t* devices;
    res = pcap_findalldevs(&devices, errbuf);
    if (res != 0) {
        pcap_free_option(options);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_fd_closed(in_fd);
	return 0;
    }

    // Step 7: Clean up resources
    pcap_free_option(options);
    pcap_freealldevs(devices);
    assert_file_closed(&in_file);;

    assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_fd_closed(in_fd);
	return 0;
}