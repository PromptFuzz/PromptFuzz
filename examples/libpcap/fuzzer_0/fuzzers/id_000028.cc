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
//<ID> 1702
//<Prompt> ["pcap_set_tstamp_type","pcap_getnonblock","pcap_findalldevs","pcap_next_ex","pcap_tstamp_type_val_to_description","pcap_sendpacket","pcap_list_tstamp_types"]
/*<Combination>: [int pcap_set_tstamp_type(pcap_t * , int ),
    int pcap_getnonblock(pcap_t * , char * ),
    int pcap_findalldevs(pcap_if_t ** , char * ),
    int pcap_next_ex(pcap_t * , struct pcap_pkthdr ** , const u_char ** ),
    const char *pcap_tstamp_type_val_to_description(int ),
    int pcap_sendpacket(pcap_t * , const u_char * , int ),
    int pcap_list_tstamp_types(pcap_t * , int ** )
*/
//<score> 12, nr_unique_branch: 7
//<Quality> {"density":8,"unique_branches":{"pcap_create":[[2387,7,2387,16,0,0,4,0],[2397,8,2397,17,0,0,4,1]],"netfilter_create":[[736,6,736,59,0,0,4,0],[738,11,738,68,0,0,4,0],[750,6,750,16,0,0,4,0],[750,20,750,31,0,0,4,1],[760,6,760,15,0,0,4,1]]},"library_calls":["pcap_findalldevs","pcap_create","pcap_set_tstamp_type","pcap_getnonblock","pcap_sendpacket","pcap_geterr","pcap_close","pcap_tstamp_type_val_to_description","pcap_close","pcap_freealldevs"],"critical_calls":["pcap_findalldevs","pcap_create","pcap_set_tstamp_type","pcap_getnonblock","pcap_sendpacket","pcap_geterr","pcap_close","pcap_tstamp_type_val_to_description","pcap_close","pcap_freealldevs"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_28(const uint8_t* data, size_t size) {
	if(size<0) return 0;

    pcap_if_t* alldevs;
    char errbuf[PCAP_ERRBUF_SIZE];
    
    // Step 1: Find all available devices
    if (pcap_findalldevs(&alldevs, errbuf) == -1) {
        std::cerr << "Error finding devices: " << errbuf << std::endl;
        return 0;
    }
    
    // Step 2: Iterate over the available devices
    for (pcap_if_t* d = alldevs; d != nullptr; d = d->next) {
        // Step 3: Open the capture device
        pcap_t* handle = pcap_create(d->name, errbuf);
        if (handle == nullptr) {
            std::cerr << "Error creating pcap handle: " << errbuf << std::endl;
            continue;
        }
        
        // Step 4: Set the time stamp type
        pcap_set_tstamp_type(handle, PCAP_TSTAMP_ADAPTER_UNSYNCED);
        
        // Step 5: Set the non-blocking mode
        int arg1 = pcap_getnonblock(handle, errbuf);
        
        // Step 6: Send a packet through the device
        if (pcap_sendpacket(handle, data, size) != 0) {
            std::cerr << "Error sending packet: " << pcap_geterr(handle) << std::endl;
            pcap_close(handle);
            continue;
        }
        
        // Step 7: Get the time stamp type description
        const char* description = pcap_tstamp_type_val_to_description(PCAP_TSTAMP_ADAPTER_UNSYNCED);
        std::cout << "Time stamp description: " << description << std::endl;
        
        pcap_close(handle);
    }
    
    pcap_freealldevs(alldevs);
    
    return 0;
}