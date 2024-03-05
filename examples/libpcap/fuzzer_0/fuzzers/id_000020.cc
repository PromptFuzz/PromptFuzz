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
//<ID> 1382
//<Prompt> ["pcap_set_option_string","pcap_fopen_offline_with_tstamp_precision","pcap_lookupdev","pcap_free_datalinks","pcap_loop","pcap_dump_open_append","pcap_dump_close","pcap_close"]
/*<Combination>: [int pcap_set_option_string(pcap_options * po, enum pcap_option_name pon, const char * value),
    pcap_t *pcap_fopen_offline_with_tstamp_precision(FILE * , u_int , char * ),
    char *pcap_lookupdev(char * ),
    void pcap_free_datalinks(int * ),
    int pcap_loop(pcap_t * , int , pcap_handler , u_char * ),
    pcap_dumper_t *pcap_dump_open_append(pcap_t * , const char * ),
    void pcap_dump_close(pcap_dumper_t * ),
    void pcap_close(pcap_t * )
*/
//<score> 44, nr_unique_branch: 6
//<Quality> {"density":11,"unique_branches":{"pcapint_offline_read":[[687,8,687,16,0,0,4,1],[464,42,464,56,1,0,4,0]],"pcap_dump_open_append":[[1038,8,1038,61,0,0,4,1]],"pcap_loop":[[2963,7,2963,23,0,0,4,0],[2977,7,2977,13,0,0,4,1],[2979,7,2979,33,0,0,4,1]]},"library_calls":["pcap_alloc_option","pcap_set_option_string","pcap_fopen_offline_with_tstamp_precision","pcap_free_option","pcap_lookupdev","pcap_close","pcap_free_option","pcap_set_datalink","pcap_close","pcap_free_option","pcap_datalink","pcap_dump","pcap_dump","pcap_dump_open_append","pcap_close","pcap_free_option","pcap_loop","pcap_dump_close","pcap_close","pcap_free_option"],"critical_calls":["pcap_alloc_option","pcap_set_option_string","pcap_fopen_offline_with_tstamp_precision","pcap_lookupdev","pcap_set_datalink","pcap_datalink","pcap_dump","pcap_dump","pcap_dump_open_append","pcap_loop","pcap_dump_close","pcap_close","pcap_free_option"],"visited":0}
/**/
extern "C" int LLVMFuzzerTestOneInput_20(const uint8_t* data, size_t size) {
	if(size<0) return 0;

    pcap_t* handle; // pcap handle
    pcap_dumper_t* dumper; // pcap dumper
    pcap_handler callback; // pcap callback function
    char errbuf[PCAP_ERRBUF_SIZE]; // error buffer

    // Step 1: Set pcap options
    pcap_options* options = pcap_alloc_option();
    pcap_set_option_string(options, PON_TSTAMP_PRECISION, "nano");

    // Step 2: Open pcap file for reading
    FILE* in_file = fmemopen((void*)data, size, "rb");
    handle = pcap_fopen_offline_with_tstamp_precision(in_file, PCAP_TSTAMP_PRECISION_NANO, errbuf);

    if (handle == nullptr) {
        assert_file_closed(&in_file);;
        pcap_free_option(options);
        assert_file_closed(&in_file);
	return 0;
    }

    // Step 3: Lookup default device
    char* device = pcap_lookupdev(errbuf);
    if (device == nullptr) {
        pcap_close(handle);
        assert_file_closed(&in_file);;
        pcap_free_option(options);
        assert_file_closed(&in_file);
	return 0;
    }

    // Step 4: Set data link for pcap handle
    if (pcap_set_datalink(handle, DLT_EN10MB) != 0) {
        pcap_close(handle);
        assert_file_closed(&in_file);;
        pcap_free_option(options);
        assert_file_closed(&in_file);
	return 0;
    }

    // Step 5: Get data link for pcap handle
    int datalink = pcap_datalink(handle);

    // Step 6: Loop and process packets
    callback = [](u_char* user, const struct pcap_pkthdr* header, const u_char* packet) {
        pcap_dump((u_char*)user, header, packet);
    };
    dumper = pcap_dump_open_append(handle, "output_file");

    if (dumper == nullptr) {
        pcap_close(handle);
        assert_file_closed(&in_file);;
        pcap_free_option(options);
        assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
    }

    pcap_loop(handle, -1, callback, (u_char*)dumper);

    // Step 7: Close pcap handle and cleanup
    pcap_dump_close(dumper);
    pcap_close(handle);
    assert_file_closed(&in_file);;
    pcap_free_option(options);

    assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
}