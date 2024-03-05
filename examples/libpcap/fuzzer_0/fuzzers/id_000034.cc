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
//<ID> 1979
//<Prompt> ["pcap_set_datalink","pcap_statustostr","pcap_fopen_offline_with_tstamp_precision","bpf_validate","pcap_tstamp_type_val_to_description"]
/*<Combination>: [int pcap_set_datalink(pcap_t * , int ),
    const char *pcap_statustostr(int ),
    pcap_t *pcap_fopen_offline_with_tstamp_precision(FILE * , u_int , char * ),
    int bpf_validate(const struct bpf_insn * f, int len),
    const char *pcap_tstamp_type_val_to_description(int )
*/
//<score> 39, nr_unique_branch: 2
//<Quality> {"density":13,"unique_branches":{"pcapint_validate_filter":[[514,9,514,35,0,0,4,0]],"pcap_statustostr":[[3715,2,3715,27,0,0,4,0]]},"library_calls":["pcap_fopen_offline_with_tstamp_precision","pcap_datalink","pcap_set_datalink","pcap_close","pcap_statustostr","pcap_activate","bpf_validate","pcap_close","pcap_datalink","pcap_set_datalink","pcap_close","pcap_close","pcap_close","pcap_statustostr","pcap_activate","bpf_validate","pcap_close","pcap_tstamp_type_val_to_description","pcap_close","pcap_close"],"critical_calls":["pcap_fopen_offline_with_tstamp_precision","pcap_datalink","pcap_set_datalink","pcap_statustostr","pcap_activate","bpf_validate","pcap_datalink","pcap_set_datalink","pcap_statustostr","pcap_activate","bpf_validate","pcap_tstamp_type_val_to_description","pcap_close"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_34(const uint8_t* data, size_t size) {
	if(size<0) return 0;

	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);

    // Step 4: create input file
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (in_file == nullptr) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Step 5: get the file descriptor for input file
    int in_fd = fuzz_fileno(in_file);
    if (in_fd < 0) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
    }

    // Step 6: set the input file name
    const char* in_file_name = "input_file";

    // Step 7: create pcap handler
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t* pcap_handle = pcap_fopen_offline_with_tstamp_precision(in_file, PCAP_TSTAMP_PRECISION_MICRO, errbuf);
    if (pcap_handle == nullptr) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_fd_closed(in_fd);
	return 0;
    }
  
    // Step 1: set datalink type
    int datalink = pcap_datalink(pcap_handle);
    if (pcap_set_datalink(pcap_handle, datalink) != 0) {
        pcap_close(pcap_handle);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_fd_closed(in_fd);
	return 0;
    }
  
    // Step 2: print pcap status
    printf("PCAP Status: %s\n", pcap_statustostr(pcap_activate(pcap_handle)));
  
    // Step 3: validate BPF filter instructions
    struct bpf_insn filter[] = {
        {0x6, 0, 0, 0x000003e8},
        {0x28, 0, 0, 0x0000000c},
        {0x15, 5, 0, 0x00000000},
        {0x6, 0, 0, 0x00000000}
    };
    if (bpf_validate(filter, sizeof(filter) / sizeof(filter[0])) < 0) {
        pcap_close(pcap_handle);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_fd_closed(in_fd);
	return 0;
    }
  
    // Step 1: set datalink type
    int datalink2 = pcap_datalink(pcap_handle);
    if (pcap_set_datalink(pcap_handle, datalink2) != 0) {
        pcap_close(pcap_handle);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_fd_closed(in_fd);
	return 0;
    }
  
    // Step 4: create output file
    FILE* out_file = fopen("output_file", "wb");
    if (out_file == nullptr) {
        pcap_close(pcap_handle);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_fd_closed(in_fd);
	return 0;
    }
  
    // Step 5: get the file descriptor for output file
    int out_fd = fuzz_fileno(out_file);
    if (out_fd < 0) {
        pcap_close(pcap_handle);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
  
    // Step 6: set the output file name
    const char* out_file_name = "output_file";

    // Step 2: print pcap status
    printf("PCAP Status: %s\n", pcap_statustostr(pcap_activate(pcap_handle)));
  
    // Step 3: validate BPF filter instructions
    struct bpf_insn filter2[] = {
        {0x6, 0, 0, 0x000003e8},
        {0x28, 0, 0, 0x0000000c},
        {0x15, 5, 0, 0x00000000},
        {0x6, 0, 0, 0x00000000}
    };
    if (bpf_validate(filter2, sizeof(filter2) / sizeof(filter2[0])) < 0) {
        pcap_close(pcap_handle);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
  
    // Step 4: get timestamp type description
    const char* timestamp_desc = pcap_tstamp_type_val_to_description(PCAP_TSTAMP_ADAPTER);
    if (timestamp_desc == nullptr) {
        pcap_close(pcap_handle);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }

    // Step 7: release resources
    pcap_close(pcap_handle);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}