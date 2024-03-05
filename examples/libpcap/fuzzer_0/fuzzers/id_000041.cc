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
//<ID> 2377
//<Prompt> ["pcap_fopen_offline","pcap_lib_version","pcap_dump_open_append","pcap_findalldevs","pcap_alloc_option","pcap_dump_open","pcap_dump_file","pcap_setnonblock","pcap_compile","pcap_freecode","pcap_tstamp_type_val_to_name","pcap_dump_close","pcap_free_option","pcap_close"]
/*<Combination>: [pcap_t *pcap_fopen_offline(FILE * , char * ),
    const char *pcap_lib_version(),
    pcap_dumper_t *pcap_dump_open_append(pcap_t * , const char * ),
    int pcap_findalldevs(pcap_if_t ** , char * ),
    pcap_options *pcap_alloc_option(),
    pcap_dumper_t *pcap_dump_open(pcap_t * , const char * ),
    FILE *pcap_dump_file(pcap_dumper_t * ),
    int pcap_setnonblock(pcap_t * , int , char * ),
    int pcap_compile(pcap_t * , struct bpf_program * , const char * , int , bpf_u_int32 ),
    void pcap_freecode(struct bpf_program * ),
    const char *pcap_tstamp_type_val_to_name(int ),
    void pcap_dump_close(pcap_dumper_t * ),
    void pcap_free_option(pcap_options * po),
    void pcap_close(pcap_t * )
*/
//<score> 17, nr_unique_branch: 0
//<Quality> {"density":17,"unique_branches":{},"library_calls":["pcap_lib_version","pcap_fopen_offline","pcap_dump_open_append","pcap_close","pcap_findalldevs","pcap_dump_close","pcap_close","pcap_alloc_option","pcap_freealldevs","pcap_dump_close","pcap_close","pcap_compile","pcap_free_option","pcap_freealldevs","pcap_dump_close","pcap_close","pcap_freecode","pcap_tstamp_type_val_to_name","pcap_dump_close","pcap_free_option","pcap_close","pcap_freealldevs"],"critical_calls":["pcap_lib_version","pcap_fopen_offline","pcap_dump_open_append","pcap_findalldevs","pcap_alloc_option","pcap_compile","pcap_freecode","pcap_tstamp_type_val_to_name","pcap_dump_close","pcap_free_option","pcap_close","pcap_freealldevs"],"visited":0}
/**/

extern "C" int LLVMFuzzerTestOneInput_41(const uint8_t* data, size_t size) {
	if(size<0) return 0;

    FILE* in_file = fmemopen((void*)data, size, "rb"); // create input file
    FILE* out_file = fopen("output_file", "wb"); // create output file

    if (in_file == nullptr || out_file == nullptr) {
        // handle error
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    pcap_t* pcap_handle;
    pcap_dumper_t* pcap_dumper;
    pcap_if_t* devices;
    char errbuf[PCAP_ERRBUF_SIZE];

    // get libpcap version
    const char* lib_version = pcap_lib_version();

    // open pcap handler for offline reading
    pcap_handle = pcap_fopen_offline(in_file, errbuf);
    if (pcap_handle == nullptr) {
        // handle error
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // open pcap dumper for appending packets
    pcap_dumper = pcap_dump_open_append(pcap_handle, "output_file");
    if (pcap_dumper == nullptr) {
        // handle error
        pcap_close(pcap_handle);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
    }

    // find all available devices
    if (pcap_findalldevs(&devices, errbuf) == -1) {
        // handle error
        pcap_dump_close(pcap_dumper);
        pcap_close(pcap_handle);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
    }

    // allocate pcap options
    pcap_options* options = pcap_alloc_option();
    if (options == nullptr) {
        // handle error
        pcap_freealldevs(devices);
        pcap_dump_close(pcap_dumper);
        pcap_close(pcap_handle);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
    }

    // compile packet filter
    struct bpf_program filter;
    if (pcap_compile(pcap_handle, &filter, "tcp", 1, PCAP_NETMASK_UNKNOWN) == -1) {
        // handle error
        pcap_free_option(options);
        pcap_freealldevs(devices);
        pcap_dump_close(pcap_dumper);
        pcap_close(pcap_handle);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
    }

    // free filter program
    pcap_freecode(&filter);

    // get timestamp type name
    const char* tstamp_type_name = pcap_tstamp_type_val_to_name(0);

    // close pcap dumper
    pcap_dump_close(pcap_dumper);

    // free pcap options
    pcap_free_option(options);

    // close pcap handle
    pcap_close(pcap_handle);

    // free device list
    pcap_freealldevs(devices);

    // close input and output files
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
}