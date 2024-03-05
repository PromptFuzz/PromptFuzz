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
//<ID> 2503
//<Prompt> ["pcap_compile_nopcap","pcap_get_option_int","pcap_dump","pcap_set_option_int","pcap_offline_filter","pcap_lookupnet","bpf_validate","pcap_list_tstamp_types","pcap_loop"]
/*<Combination>: [int pcap_compile_nopcap(int , int , struct bpf_program * , const char * , int , bpf_u_int32 ),
    int pcap_get_option_int(pcap_options * po, enum pcap_option_name pon),
    void pcap_dump(u_char * , const struct pcap_pkthdr * , const u_char * ),
    int pcap_set_option_int(pcap_options * po, enum pcap_option_name pon, const int value),
    int pcap_offline_filter(const struct bpf_program * , const struct pcap_pkthdr * , const u_char * ),
    int pcap_lookupnet(const char * , bpf_u_int32 * , bpf_u_int32 * , char * ),
    int bpf_validate(const struct bpf_insn * f, int len),
    int pcap_list_tstamp_types(pcap_t * , int ** ),
    int pcap_loop(pcap_t * , int , pcap_handler , u_char * )
*/
//<score> 7, nr_unique_branch: 0
//<Quality> {"density":7,"unique_branches":{},"library_calls":["pcap_create","pcap_alloc_option","pcap_set_option_int","pcap_set_option_int","pcap_set_option_int","pcap_compile_nopcap","pcap_free_option","pcap_setfilter","pcap_freecode","pcap_free_option","pcap_loop","pcap_freecode","pcap_free_option","pcap_close"],"critical_calls":["pcap_create","pcap_alloc_option","pcap_set_option_int","pcap_set_option_int","pcap_set_option_int","pcap_compile_nopcap","pcap_setfilter","pcap_loop","pcap_freecode","pcap_free_option","pcap_close"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_46(const uint8_t* f_data, size_t f_size) {
	if(f_size<39) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_6, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_7, fdp);
	//fuzzer shim end}




    // Create pcap handler
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t* handle = pcap_create(fuzz_str_1, errbuf);
    if (handle == NULL) {
        return 0;
    }
    
    // Set options
    pcap_options* options = pcap_alloc_option();
    pcap_set_option_int(options, PON_TSTAMP_PRECISION, fuzz_int32_t_2);
    pcap_set_option_int(options, PON_IO_READ_PLUGIN, fuzz_int32_t_3);
    pcap_set_option_int(options, PON_IO_WRITE_PLUGIN, fuzz_int32_t_4);
    
    // Compile BPF filter
    struct bpf_program filter;
    const char* filter_string = "tcp";
    if (pcap_compile_nopcap(1, 0, &filter, filter_string, fuzz_int32_t_5, fuzz_uint32_t_6) == -1) {
        pcap_free_option(options);
        return 0;
    }
    
    // Set filter
    if (pcap_setfilter(handle, &filter) == -1) {
        pcap_freecode(&filter);
        pcap_free_option(options);
        return 0;
    }
    
    // Loop through packets
    int packet_count = 0;
    pcap_handler callback = [](u_char *, const struct pcap_pkthdr *, const u_char *) {
        // Do nothing
    };
    pcap_loop(handle, fuzz_int32_t_7, callback, NULL);
    
    // Free resources
    pcap_freecode(&filter);
    pcap_free_option(options);
    pcap_close(handle);
    
    return 0;
}