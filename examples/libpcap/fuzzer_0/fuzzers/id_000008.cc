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
//<ID> 868
//<Prompt> ["pcap_next_ex","pcap_fopen_offline","pcap_lib_version","pcap_alloc_option","pcap_get_option_int","pcap_dump_file","pcap_compile","bpf_image","pcap_tstamp_type_val_to_name","pcap_freecode","pcap_close","pcap_free_option"]
/*<Combination>: [int pcap_next_ex(pcap_t * , struct pcap_pkthdr ** , const u_char ** ),
    pcap_t *pcap_fopen_offline(FILE * , char * ),
    const char *pcap_lib_version(),
    pcap_options *pcap_alloc_option(),
    int pcap_get_option_int(pcap_options * po, enum pcap_option_name pon),
    FILE *pcap_dump_file(pcap_dumper_t * ),
    int pcap_compile(pcap_t * , struct bpf_program * , const char * , int , bpf_u_int32 ),
    char *bpf_image(const struct bpf_insn * , int ),
    const char *pcap_tstamp_type_val_to_name(int ),
    void pcap_freecode(struct bpf_program * ),
    void pcap_close(pcap_t * ),
    void pcap_free_option(pcap_options * po)
*/
//<score> 10, nr_unique_branch: 3
//<Quality> {"density":15,"unique_branches":{"pcap_tstamp_type_val_to_name":[[3472,14,3472,49,0,0,4,0],[3473,7,3473,49,0,0,4,0],[3473,7,3473,49,0,0,4,1]]},"library_calls":["pcap_fopen_offline","pcap_lib_version","pcap_alloc_option","pcap_close","pcap_get_option_int","pcap_dump_open","pcap_free_option","pcap_close","pcap_dump_file","pcap_dump_close","pcap_free_option","pcap_close","pcap_compile","pcap_dump_close","pcap_free_option","pcap_close","bpf_image","pcap_freecode","pcap_tstamp_type_val_to_name","pcap_dump_flush","pcap_dump_close","pcap_free_option","pcap_close"],"critical_calls":["pcap_fopen_offline","pcap_lib_version","pcap_alloc_option","pcap_get_option_int","pcap_dump_open","pcap_dump_file","pcap_compile","bpf_image","pcap_freecode","pcap_tstamp_type_val_to_name","pcap_dump_flush","pcap_dump_close","pcap_free_option","pcap_close"],"visited":2}
/**/
#define _CRT_SECURE_NO_WARNINGS


extern "C" int LLVMFuzzerTestOneInput_8(const uint8_t* data, size_t size) {
	if(size<0) return 0;

    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (in_file == NULL) {
        assert_file_closed(&in_file);
	return 0;
    }

    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t* pcap_handle = pcap_fopen_offline(in_file, errbuf);
    if (pcap_handle == NULL) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    const char* lib_version = pcap_lib_version();
    std::cout << "Libpcap version: " << lib_version << std::endl;

    pcap_options* pcap_opts = pcap_alloc_option();
    if (pcap_opts == NULL) {
        pcap_close(pcap_handle);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    int tstamp_precision = pcap_get_option_int(pcap_opts, PON_TSTAMP_PRECISION);
    std::cout << "Timestamp Precision: " << tstamp_precision << std::endl;

    pcap_dumper_t* pcap_dumper = pcap_dump_open(pcap_handle, "output_file");
    if (pcap_dumper == NULL) {
        pcap_free_option(pcap_opts);
        pcap_close(pcap_handle);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
    }

    FILE* out_file = pcap_dump_file(pcap_dumper);
    int out_file_fd = fuzz_fileno(out_file);
    if (out_file_fd == -1) {
        pcap_dump_close(pcap_dumper);
        pcap_free_option(pcap_opts);
        pcap_close(pcap_handle);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(out_file_fd);
	return 0;
    }

    struct bpf_program prog;
    char filter_exp[] = "port 80";
    if (pcap_compile(pcap_handle, &prog, filter_exp, 0, PCAP_NETMASK_UNKNOWN) == -1) {
        pcap_dump_close(pcap_dumper);
        pcap_free_option(pcap_opts);
        pcap_close(pcap_handle);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(out_file_fd);
	return 0;
    }

    char* bpf_image_str = bpf_image(prog.bf_insns, prog.bf_len);
    std::cout << "BPF Image: " << bpf_image_str << std::endl;
    pcap_freecode(&prog);

    const char* tstamp_name = pcap_tstamp_type_val_to_name(1);
    std::cout << "Timestamp Type Name: " << tstamp_name << std::endl;

    pcap_dump_flush(pcap_dumper);
    pcap_dump_close(pcap_dumper);
    pcap_free_option(pcap_opts);
    pcap_close(pcap_handle);
    assert_file_closed(&in_file);;

    assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(out_file_fd);
	return 0;
}