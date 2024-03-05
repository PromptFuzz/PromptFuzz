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
//<ID> 1152
//<Prompt> ["pcap_list_datalinks","pcap_set_tstamp_precision","pcap_setnonblock","pcap_set_rfmon","pcap_set_datalink"]
/*<Combination>: [int pcap_list_datalinks(pcap_t * , int ** ),
    int pcap_set_tstamp_precision(pcap_t * , int ),
    int pcap_setnonblock(pcap_t * , int , char * ),
    int pcap_set_rfmon(pcap_t * , int ),
    int pcap_set_datalink(pcap_t * , int )
*/
//<score> 8, nr_unique_branch: 2
//<Quality> {"density":8,"unique_branches":{"pcap_set_tstamp_precision":[[2730,15,2730,44,0,0,4,1],[2731,8,2731,62,0,0,4,1]]},"library_calls":["pcap_create","pcap_list_datalinks","pcap_close","pcap_set_tstamp_precision","pcap_close","pcap_setnonblock","pcap_close","pcap_close"],"critical_calls":["pcap_create","pcap_list_datalinks","pcap_set_tstamp_precision","pcap_setnonblock","pcap_close"],"visited":1}
/**/
extern "C" int LLVMFuzzerTestOneInput_12(const uint8_t* data, size_t size) {
	if(size<0) return 0;

	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);

    // Step 4: Create a FILE pointer for reading the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file == nullptr) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Step 5: Get the file descriptor for reading
    int in_fd = fuzz_fileno(in_file);
    
    // Step 6: Define the input file name
    const char *input_file = "input_file";
    
    // Step 1: List available data link types
    pcap_t *handle = pcap_create(input_file, nullptr);
    if (handle == nullptr) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_fd_closed(in_fd);
	return 0;
    }
    
    int *datalinks;
    int num_datalinks = pcap_list_datalinks(handle, &datalinks);
    if (num_datalinks == -1) {
        pcap_close(handle);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Step 2: Set the time stamp precision
    int tstamp_precision = pcap_set_tstamp_precision(handle, 3);
    if (tstamp_precision == -1) {
        pcap_close(handle);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Step 3: Set the file descriptor in non-blocking mode
    int nonblock = pcap_setnonblock(handle, in_fd, nullptr);
    if (nonblock == -1) {
        pcap_close(handle);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Step 7: Release allocated resources
    pcap_close(handle);
    assert_file_closed(&in_file);;
    assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_fd_closed(in_fd);
	return 0;
}