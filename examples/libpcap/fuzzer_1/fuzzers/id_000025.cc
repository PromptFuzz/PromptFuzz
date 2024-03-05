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
//<ID> 1595
//<Prompt> ["pcap_dump_open_append","pcap_sendpacket","pcap_fopen_offline","pcap_list_tstamp_types","pcap_free_tstamp_types","pcap_set_buffer_size","bpf_dump","pcap_lookupnet","pcap_setfilter","pcap_freecode","pcap_list_datalinks","pcap_free_datalinks","pcap_setnonblock","pcap_close"]
/*<Combination>: [pcap_dumper_t *pcap_dump_open_append(pcap_t * , const char * ),
    int pcap_sendpacket(pcap_t * , const u_char * , int ),
    pcap_t *pcap_fopen_offline(FILE * , char * ),
    int pcap_list_tstamp_types(pcap_t * , int ** ),
    void pcap_free_tstamp_types(int * ),
    int pcap_set_buffer_size(pcap_t * , int ),
    void bpf_dump(const struct bpf_program * , int ),
    int pcap_lookupnet(const char * , bpf_u_int32 * , bpf_u_int32 * , char * ),
    int pcap_setfilter(pcap_t * , struct bpf_program * ),
    void pcap_freecode(struct bpf_program * ),
    int pcap_list_datalinks(pcap_t * , int ** ),
    void pcap_free_datalinks(int * ),
    int pcap_setnonblock(pcap_t * , int , char * ),
    void pcap_close(pcap_t * )
*/
//<score> 17, nr_unique_branch: 0
//<Quality> {"density":17,"unique_branches":{},"library_calls":["pcap_fopen_offline_with_tstamp_precision","pcap_list_tstamp_types","pcap_free_tstamp_types","pcap_set_buffer_size","pcap_compile","pcap_close","pcap_setfilter","pcap_freecode","pcap_close","pcap_freecode","pcap_list_datalinks","pcap_free_datalinks","pcap_dump_open","pcap_close","pcap_sendpacket","pcap_close","pcap_dump_close"],"critical_calls":["pcap_fopen_offline_with_tstamp_precision","pcap_list_tstamp_types","pcap_free_tstamp_types","pcap_set_buffer_size","pcap_compile","pcap_setfilter","pcap_freecode","pcap_list_datalinks","pcap_free_datalinks","pcap_dump_open","pcap_sendpacket","pcap_close","pcap_dump_close"],"visited":0}
/**/

extern "C" int LLVMFuzzerTestOneInput_25(const uint8_t* f_data, size_t f_size) {
	if(f_size<40) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_5, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_6, fuzz_str_sz_6, fdp);
	//fuzzer shim end}




    pcap_t* pcap_handler = nullptr;
    pcap_dumper_t* pcap_dumper = nullptr;
    struct bpf_program bpf_prog;
    char errbuf[PCAP_ERRBUF_SIZE];

    // Step 1: Open a device for capturing or reading packets
    const char* device = "eth0";
	FILE* fuzz_file_0 = fmemopen((void*)data, size, "rb");

    pcap_handler = pcap_fopen_offline_with_tstamp_precision(fuzz_file_0, fuzz_uint32_t_1, errbuf);
    if (!pcap_handler) {
        assert_file_closed(&fuzz_file_0);
	return 0;
    }

    // Step 2: Get the list of available time stamp types
    int* tstamp_types;
    int tstamp_type_count = pcap_list_tstamp_types(pcap_handler, &tstamp_types);
    pcap_free_tstamp_types(tstamp_types);

    // Step 3: Set the buffer size
    pcap_set_buffer_size(pcap_handler, fuzz_int32_t_2);

    // Step 4: Compile and set a BPF filter
    const char* filter = "port 80";
    if (pcap_compile(pcap_handler, &bpf_prog, fuzz_str_3, fuzz_int32_t_4, fuzz_uint32_t_5) == -1) {
        pcap_close(pcap_handler);
        assert_file_closed(&fuzz_file_0);
	return 0;
    }
    if (pcap_setfilter(pcap_handler, &bpf_prog) == -1) {
        pcap_freecode(&bpf_prog);
        pcap_close(pcap_handler);
        assert_file_closed(&fuzz_file_0);
	return 0;
    }
    pcap_freecode(&bpf_prog);

    // Step 5: Get the list of available data link types
    int* datalink_types;
    int datalink_type_count = pcap_list_datalinks(pcap_handler, &datalink_types);
    pcap_free_datalinks(datalink_types);

    // Step 6: Open a file for writing packets
    pcap_dumper = pcap_dump_open(pcap_handler, fuzz_str_6);
    if (!pcap_dumper) {
        pcap_close(pcap_handler);
        assert_file_closed(&fuzz_file_0);
	return 0;
    }

    // Step 7: Send a packet
    u_char packet_data[100];
    int packet_size = 100;
    pcap_sendpacket(pcap_handler, packet_data, packet_size);

    // Step 8: Close the handler and release resources
    pcap_close(pcap_handler);
    pcap_dump_close(pcap_dumper);

    assert_file_closed(&fuzz_file_0);
	return 0;
}