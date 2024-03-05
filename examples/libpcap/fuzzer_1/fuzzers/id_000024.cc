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
//<ID> 1527
//<Prompt> ["pcap_list_tstamp_types","pcap_loop","pcap_set_buffer_size","bpf_filter","pcap_list_datalinks","pcap_setdirection","pcap_lookupnet","pcap_setnonblock"]
/*<Combination>: [int pcap_list_tstamp_types(pcap_t * , int ** ),
    int pcap_loop(pcap_t * , int , pcap_handler , u_char * ),
    int pcap_set_buffer_size(pcap_t * , int ),
    u_int bpf_filter(const struct bpf_insn * , const u_char * , u_int , u_int ),
    int pcap_list_datalinks(pcap_t * , int ** ),
    int pcap_setdirection(pcap_t * , pcap_direction_t ),
    int pcap_lookupnet(const char * , bpf_u_int32 * , bpf_u_int32 * , char * ),
    int pcap_setnonblock(pcap_t * , int , char * )
*/
//<score> 19, nr_unique_branch: 1
//<Quality> {"density":19,"unique_branches":{"pcap_setnonblock":[[3641,6,3641,15,0,0,4,0]]},"library_calls":["pcap_fopen_offline","pcap_list_tstamp_types","pcap_free_tstamp_types","pcap_loop","pcap_set_buffer_size","pcap_lookupnet","pcap_close","pcap_compile","pcap_close","pcap_setfilter","pcap_freecode","pcap_close","pcap_freecode","pcap_list_datalinks","pcap_free_datalinks","pcap_setdirection","pcap_setnonblock","pcap_close","pcap_close"],"critical_calls":["pcap_fopen_offline","pcap_list_tstamp_types","pcap_free_tstamp_types","pcap_loop","pcap_set_buffer_size","pcap_lookupnet","pcap_compile","pcap_setfilter","pcap_freecode","pcap_list_datalinks","pcap_free_datalinks","pcap_setdirection","pcap_setnonblock","pcap_close"],"visited":1}
/**/

extern "C" int LLVMFuzzerTestOneInput_24(const uint8_t* f_data, size_t f_size) {
	if(f_size<29) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
	//fuzzer shim end}




    FILE *in_file = fmemopen((void *)data, size, "rb");
    FILE *out_file = fopen("output_file", "wb");

    if (!in_file || !out_file) {
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *handle = pcap_fopen_offline(in_file, errbuf);

    if (!handle) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Get the available timestamp types
    int *tstamp_types = NULL;
    int num_tstamp_types = pcap_list_tstamp_types(handle, &tstamp_types);
    if (num_tstamp_types > 0 && tstamp_types != NULL) {
        // Do something with the timestamp types
        // ...

        pcap_free_tstamp_types(tstamp_types);
    }

    // Loop through the packets
    pcap_loop(handle, fuzz_int32_t_1, [](u_char *user, const struct pcap_pkthdr *pkthdr, const u_char *packet) {}, NULL);

    // Set the buffer size
    pcap_set_buffer_size(handle, fuzz_int32_t_2);

    // Compile and apply BPF filter
    struct bpf_insn bpf_filter_instr;
    bpf_u_int32 net, mask;
    char dev[PCAP_ERRBUF_SIZE];

    if (pcap_lookupnet(fuzz_str_3, &net, &mask, dev) == -1) {
        pcap_close(handle);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    struct bpf_program filter;
    char filter_exp[] = "port 80";

    if (pcap_compile(handle, &filter, fuzz_str_4, fuzz_int32_t_5, net) == -1) {
        pcap_close(handle);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    if (pcap_setfilter(handle, &filter) == -1) {
        pcap_freecode(&filter);
        pcap_close(handle);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    pcap_freecode(&filter);

    // Get the available datalink types
    int *datalink_types = NULL;
    int num_datalink_types = pcap_list_datalinks(handle, &datalink_types);
    if (num_datalink_types > 0 && datalink_types != NULL) {
        // Do something with the datalink types
        // ...

        pcap_free_datalinks(datalink_types);
    }

    // Set the capture direction
    pcap_setdirection(handle, PCAP_D_IN);

    // Set non-blocking mode
    char nonblock_errbuf[PCAP_ERRBUF_SIZE];
    int nonblock_status = pcap_setnonblock(handle, 1, nonblock_errbuf);
    if (nonblock_status == -1) {
        pcap_close(handle);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Release resources
    pcap_close(handle);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}