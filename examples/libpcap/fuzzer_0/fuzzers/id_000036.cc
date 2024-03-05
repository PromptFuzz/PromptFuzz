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
//<ID> 2023
//<Prompt> ["pcap_compile_nopcap","pcap_dump_open","bpf_filter","pcap_inject","pcap_loop"]
/*<Combination>: [int pcap_compile_nopcap(int , int , struct bpf_program * , const char * , int , bpf_u_int32 ),
    pcap_dumper_t *pcap_dump_open(pcap_t * , const char * ),
    u_int bpf_filter(const struct bpf_insn * , const u_char * , u_int , u_int ),
    int pcap_inject(pcap_t * , const void * , size_t ),
    int pcap_loop(pcap_t * , int , pcap_handler , u_char * )
*/
//<score> 11, nr_unique_branch: 0
//<Quality> {"density":11,"unique_branches":{},"library_calls":["pcap_compile_nopcap","pcap_geterr","pcap_close","pcap_setfilter","pcap_geterr","pcap_freecode","pcap_close","pcap_dump_open","pcap_geterr","pcap_freecode","pcap_close","pcap_dump","pcap_dump","pcap_loop","pcap_freecode","pcap_close","pcap_dump_close"],"critical_calls":["pcap_compile_nopcap","pcap_setfilter","pcap_dump_open","pcap_dump","pcap_dump","pcap_loop","pcap_freecode","pcap_close","pcap_dump_close"],"visited":0}
/**/

extern "C" int LLVMFuzzerTestOneInput_36(const uint8_t* data, size_t size) {
	if(size<0) return 0;

	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);

    pcap_t* handle;
    char errbuf[PCAP_ERRBUF_SIZE];
    struct pcap_pkthdr header;
    const u_char* packet;
    
    // Create pcap handler
    handle = pcap_open_offline("input_file", errbuf);
    
    if (handle == NULL) {
        fprintf(stderr, "Error opening pcap file: %s\n", errbuf);
        assert_file_name_closed("input_file");
	return 0;
    }
    
    // Compile pcap filter
    struct bpf_program filter;
    if (pcap_compile_nopcap(BPF_ALIGNMENT, DLT_EN10MB, &filter, "tcp port 80", 1, PCAP_NETMASK_UNKNOWN) == -1) {
        fprintf(stderr, "Error compiling pcap filter: %s\n", pcap_geterr(handle));
        pcap_close(handle);
        assert_file_name_closed("input_file");
	return 0;
    }
    
    // Set pcap filter
    if (pcap_setfilter(handle, &filter) == -1) {
        fprintf(stderr, "Error setting pcap filter: %s\n", pcap_geterr(handle));
        pcap_freecode(&filter);
        pcap_close(handle);
        assert_file_name_closed("input_file");
	return 0;
    }
    
    // Create pcap dumper
    pcap_dumper_t* dumper = pcap_dump_open(handle, "output_file");
    if (dumper == NULL) {
        fprintf(stderr, "Error creating pcap dumper: %s\n", pcap_geterr(handle));
        pcap_freecode(&filter);
        pcap_close(handle);
        assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	return 0;
    }
    
    // Loop through pcap packets
    pcap_handler packet_handler = [](u_char* user, const struct pcap_pkthdr* header, const u_char* packet) {
        pcap_dumper_t* dumper = (pcap_dumper_t*)user;
        pcap_dump((u_char*)dumper, header, packet);
    };
    
    pcap_loop(handle, 0, packet_handler, (u_char*)dumper);
    
    // Cleanup
    pcap_freecode(&filter);
    pcap_close(handle);
    pcap_dump_close(dumper);
    
    assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	return 0;
}