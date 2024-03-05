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
//<ID> 1270
//<Prompt> ["pcap_fopen_offline_with_tstamp_precision","pcap_major_version","pcap_snapshot","pcap_set_snaplen","pcap_compile","pcap_freecode","pcap_lookupnet","pcap_datalink_name_to_val","bpf_filter","pcap_offline_filter","pcap_set_immediate_mode","pcap_dump_open","pcap_dump_ftell64","pcap_dump_close","pcap_close"]
/*<Combination>: [pcap_t *pcap_fopen_offline_with_tstamp_precision(FILE * , u_int , char * ),
    int pcap_major_version(pcap_t * ),
    int pcap_snapshot(pcap_t * ),
    int pcap_set_snaplen(pcap_t * , int ),
    int pcap_compile(pcap_t * , struct bpf_program * , const char * , int , bpf_u_int32 ),
    void pcap_freecode(struct bpf_program * ),
    int pcap_lookupnet(const char * , bpf_u_int32 * , bpf_u_int32 * , char * ),
    int pcap_datalink_name_to_val(const char * ),
    u_int bpf_filter(const struct bpf_insn * , const u_char * , u_int , u_int ),
    int pcap_offline_filter(const struct bpf_program * , const struct pcap_pkthdr * , const u_char * ),
    int pcap_set_immediate_mode(pcap_t * , int ),
    pcap_dumper_t *pcap_dump_open(pcap_t * , const char * ),
    int64_t pcap_dump_ftell64(pcap_dumper_t * ),
    void pcap_dump_close(pcap_dumper_t * ),
    void pcap_close(pcap_t * )
*/
//<score> 68, nr_unique_branch: 3
//<Quality> {"density":17,"unique_branches":{"and_pullup":[[2001,7,2001,18,0,0,4,0]],"eq_slist":[[2322,33,2322,49,0,0,4,0]],"or_pullup":[[1918,7,1918,18,0,0,4,0]]},"library_calls":["pcap_fopen_offline_with_tstamp_precision","pcap_major_version","pcap_snapshot","pcap_set_snaplen","pcap_compile","pcap_close","pcap_freecode","pcap_lookupnet","pcap_close","pcap_datalink_name_to_val","bpf_filter","pcap_next","pcap_offline_filter","pcap_set_immediate_mode","pcap_dump_open","pcap_close","pcap_dump_ftell64","pcap_dump_close","pcap_close"],"critical_calls":["pcap_fopen_offline_with_tstamp_precision","pcap_major_version","pcap_snapshot","pcap_set_snaplen","pcap_compile","pcap_freecode","pcap_lookupnet","pcap_datalink_name_to_val","bpf_filter","pcap_next","pcap_offline_filter","pcap_set_immediate_mode","pcap_dump_open","pcap_dump_ftell64","pcap_dump_close","pcap_close"],"visited":0}
/**/

extern "C" int LLVMFuzzerTestOneInput_18(const uint8_t* data, size_t size) {
	if(size<0) return 0;

    FILE* in_file = fmemopen((void *)data, size, "rb");
    FILE* out_file = fopen("output_file", "wb");
  
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);
  
    char errbuf[PCAP_ERRBUF_SIZE];
  
    pcap_t* pcap_handle = pcap_fopen_offline_with_tstamp_precision(in_file, PCAP_TSTAMP_PRECISION_MICRO, errbuf);
  
    if (pcap_handle == NULL) {
        // handle pcap open error
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
  
    int major_version = pcap_major_version(pcap_handle);
    int snapshot_len = pcap_snapshot(pcap_handle);
  
    pcap_set_snaplen(pcap_handle, 65535); // Set snapshot length to maximum
  
    struct bpf_program filter_program;
    if (pcap_compile(pcap_handle, &filter_program, "tcp", 1, PCAP_NETMASK_UNKNOWN) == -1) {
        // handle filter compilation error
        pcap_close(pcap_handle);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
  
    pcap_freecode(&filter_program);
  
    bpf_u_int32 net, mask;
    if (pcap_lookupnet("eth0", &net, &mask, errbuf) == -1) {
        // handle network lookup error
        pcap_close(pcap_handle);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
  
    int datalink = pcap_datalink_name_to_val("EN10MB");
  
    struct bpf_insn filter_insns[] = {
        { 0x28, 0, 0, 0x0000000c },
        { 0x15, 0, 1, 0x000086dd },
        { 0x30, 0, 0, 0x00000014 },
        { 0x15, 0, 15, 0x00000011 },
        { 0x28, 0, 0, 0x00000036 },
        { 0x15, 12, 0, 0x00000800 },
        { 0x30, 0, 0, 0x00000017 },
        { 0x15, 0, 10, 0x00000006 },
        { 0x28, 0, 0, 0x00000038 },
        { 0x15, 8, 9, 0x000086dd },
        { 0x30, 0, 0, 0x00000014 },
        { 0x15, 6, 7, 0x00000011 },
        { 0x28, 0, 0, 0x00000014 },
        { 0x45, 4, 0, 0x00001fff },
        { 0xb1, 0, 0, 0x0000000e },
        { 0x50, 0, 0, 0x0000001c },
        { 0x45, 2, 0, 0x00001fff },
        { 0xb1, 0, 0, 0x0000000c },
        { 0x50, 0, 0, 0x00000020 },
        { 0x45, 0, 0, 0x00001fff },
        { 0x50, 0, 0, 0x00000024 },
        { 0x54, 0, 0, 0x00000010 },
        { 0x15, 0, 1, 0x00000001 },
        { 0x54, 0, 0, 0x0000002c },
        { 0x15, 0, 1, 0x00000002 },
        { 0x6, 0, 0, 0x00040000 },
        { 0x6, 0, 0, 0x00000000 },
    };
  
    u_int filter_result = bpf_filter(filter_insns, NULL, 0, 0);
  
    struct pcap_pkthdr header;
    const u_char* packet_data = pcap_next(pcap_handle, &header);
  
    int offline_filter_result = pcap_offline_filter(&filter_program, &header, packet_data);
  
    pcap_set_immediate_mode(pcap_handle, 1); // Enable immediate mode
  
    pcap_dumper_t* dumper = pcap_dump_open(pcap_handle, "output.pcap");
    if (dumper == NULL) {
        // handle pcap dump open error
        pcap_close(pcap_handle);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
  
    int64_t dump_file_position = pcap_dump_ftell64(dumper);
  
    pcap_dump_close(dumper);
  
    pcap_close(pcap_handle);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
  
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}