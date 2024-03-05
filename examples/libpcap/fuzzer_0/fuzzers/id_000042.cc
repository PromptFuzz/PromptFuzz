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
//<ID> 2379
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
//<score> 8, nr_unique_branch: 6
//<Quality> {"density":4,"unique_branches":{"pcap_dump_open_append":[[1038,8,1038,61,0,0,4,0],[1046,3,1046,31,0,0,4,0],[1047,3,1047,36,0,0,4,0],[1053,3,1053,31,0,0,4,0],[1062,3,1062,10,0,0,4,0],[1073,7,1073,45,0,0,4,0]]},"library_calls":["pcap_findalldevs","pcap_lib_version","pcap_freealldevs","pcap_dump_open_append","pcap_close","pcap_freealldevs","pcap_dump_close","pcap_close","pcap_freealldevs"],"critical_calls":["pcap_findalldevs","pcap_lib_version","pcap_dump_open_append","pcap_dump_close","pcap_close","pcap_freealldevs"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_42(const uint8_t* data, size_t size) {
	if(size<0) return 0;

	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);

    // Step 4: Create a FILE* variable for reading input data
    FILE* in_file = fmemopen((void*)data, size, "rb");
    
    // Step 5: Get file descriptor for reading input data
    int in_fd = fuzz_fileno(in_file);
    
    // Step 6: Set input file name
    const char* input_file = "input_file";
    
    // Step 1: pcap_findalldevs
    pcap_if_t* devices;
    char errbuf[PCAP_ERRBUF_SIZE];
    
    if (pcap_findalldevs(&devices, errbuf) == -1) {
        // Error handling
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_fd_closed(in_fd);
	return 1;
    }
    
    // Loop through the devices and print their names
    pcap_if_t* device;
    for (device = devices; device != NULL; device = device->next) {
        printf("Device: %s\n", device->name);
    }
    
    // Step 2: pcap_lib_version
    const char* version = pcap_lib_version();
    printf("Libpcap version: %s\n", version);
    
    // Step 3: pcap_dump_open_append
    pcap_t* pcap = pcap_open_offline(input_file, errbuf);
    if (pcap == NULL) {
        // Error handling
        pcap_freealldevs(devices);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_fd_closed(in_fd);
	return 1;
    }
    
    pcap_dumper_t* dumper = pcap_dump_open_append(pcap, input_file);
    if (dumper == NULL) {
        // Error handling
        pcap_close(pcap);
        pcap_freealldevs(devices);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_fd_closed(in_fd);
	return 1;
    }
    
    // Step 7: Release all allocated resources
    pcap_dump_close(dumper);
    pcap_close(pcap);
    pcap_freealldevs(devices);
    assert_file_closed(&in_file);;
    
    assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_fd_closed(in_fd);
	return 0;
}