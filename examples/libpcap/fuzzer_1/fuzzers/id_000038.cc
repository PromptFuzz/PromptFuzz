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
//<ID> 2326
//<Prompt> ["pcap_list_datalinks","pcap_sendpacket","pcap_dump","pcap_setdirection","pcap_set_tstamp_precision","pcap_init","pcap_list_tstamp_types"]
/*<Combination>: [int pcap_list_datalinks(pcap_t * , int ** ),
    int pcap_sendpacket(pcap_t * , const u_char * , int ),
    void pcap_dump(u_char * , const struct pcap_pkthdr * , const u_char * ),
    int pcap_setdirection(pcap_t * , pcap_direction_t ),
    int pcap_set_tstamp_precision(pcap_t * , int ),
    int pcap_init(unsigned int , char * ),
    int pcap_list_tstamp_types(pcap_t * , int ** )
*/
//<score> 9, nr_unique_branch: 0
//<Quality> {"density":9,"unique_branches":{},"library_calls":["pcap_fopen_offline_with_tstamp_precision","pcap_datalink_ext","pcap_close","pcap_list_datalinks","pcap_close","pcap_free_datalinks","pcap_next_ex","pcap_dump","pcap_close"],"critical_calls":["pcap_fopen_offline_with_tstamp_precision","pcap_datalink_ext","pcap_list_datalinks","pcap_free_datalinks","pcap_next_ex","pcap_dump","pcap_close"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_38(const uint8_t* f_data, size_t f_size) {
	if(f_size<4) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_1, fdp);
	//fuzzer shim end}




    FILE* in_file = fmemopen((void*)data, size, "rb");
    FILE* out_file = fopen("output_file", "wb");
    if (in_file == nullptr || out_file == nullptr) {
        // handle error
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t* handle = pcap_fopen_offline_with_tstamp_precision(in_file, fuzz_uint32_t_1, errbuf);
    if (handle == nullptr) {
        // handle error
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    int link_layer;
    int result = pcap_datalink_ext(handle);
    if (result == -1) {
        // handle error
        pcap_close(handle);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    link_layer = result;
    
    int* datalink_list;
    int datalink_count = pcap_list_datalinks(handle, &datalink_list);
    if (datalink_count == -1) {
        // handle error
        pcap_close(handle);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    pcap_free_datalinks(datalink_list);
    
    pcap_pkthdr* header;
    const u_char* packet_data;
    while (pcap_next_ex(handle, &header, &packet_data) == 1) {
        pcap_dump(reinterpret_cast<u_char*>(out_file), header, packet_data);
    }
    
    pcap_close(handle);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}