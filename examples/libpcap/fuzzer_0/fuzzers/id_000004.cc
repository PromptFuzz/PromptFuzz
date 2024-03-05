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
//<ID> 674
//<Prompt> ["pcap_bufsize","pcap_set_tstamp_type","pcap_dump_flush","pcap_tstamp_type_val_to_description","bpf_validate","pcap_datalink_ext","pcap_dump_ftell","pcap_next_ex"]
/*<Combination>: [int pcap_bufsize(pcap_t * ),
    int pcap_set_tstamp_type(pcap_t * , int ),
    int pcap_dump_flush(pcap_dumper_t * ),
    const char *pcap_tstamp_type_val_to_description(int ),
    int bpf_validate(const struct bpf_insn * f, int len),
    int pcap_datalink_ext(pcap_t * ),
    long pcap_dump_ftell(pcap_dumper_t * ),
    int pcap_next_ex(pcap_t * , struct pcap_pkthdr ** , const u_char ** )
*/
//<score> 11, nr_unique_branch: 1
//<Quality> {"density":11,"unique_branches":{"pcap_bufsize":[[3526,6,3526,19,0,0,4,1]]},"library_calls":["pcap_fopen_offline_with_tstamp_precision","pcap_bufsize","pcap_set_tstamp_type","pcap_dump_open","pcap_close","pcap_dump_ftell","pcap_next_ex","pcap_dump","pcap_dump_flush","pcap_dump_close","pcap_close"],"critical_calls":["pcap_fopen_offline_with_tstamp_precision","pcap_bufsize","pcap_set_tstamp_type","pcap_dump_open","pcap_dump_ftell","pcap_next_ex","pcap_dump","pcap_dump_flush","pcap_dump_close","pcap_close"],"visited":1}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_4(const uint8_t* data, size_t size) {
	if(size<0) return 0;

	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);

  // Step 4: Create a FILE pointer for reading input data
  FILE* in_file = fmemopen((void*)data, size, "rb");
  if (in_file == nullptr) {
    assert_file_closed(&in_file);
	return 0;
  }

  // Step 5: Get the file descriptor for reading input data
  int in_fd = fuzz_fileno(in_file);

  // Step 6: Specify the input file name
  const char* in_file_name = "input_file";

  // Step 7: Allocate memory for error buffer
  char errbuf[PCAP_ERRBUF_SIZE];

  // Step 8: Use libpcap APIs to achieve the desired event
  pcap_t* pcap_handle = pcap_fopen_offline_with_tstamp_precision(in_file, PCAP_TSTAMP_PRECISION_MICRO, errbuf);
  if (pcap_handle == nullptr) {
    assert_file_closed(&in_file);;
    assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_fd_closed(in_fd);
	return 0;
  }

  int bufsize = pcap_bufsize(pcap_handle);
  pcap_set_tstamp_type(pcap_handle, PCAP_TSTAMP_ADAPTER);

  pcap_dumper_t* dumper = pcap_dump_open(pcap_handle, "output_file");
  if (dumper == nullptr) {
    pcap_close(pcap_handle);
    assert_file_closed(&in_file);;
    assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_fd_closed(in_fd);
	return 0;
  }
  pcap_dump_ftell(dumper);

  struct pcap_pkthdr* header;
  const uint8_t* packet_data;
  int ret;
  while ((ret = pcap_next_ex(pcap_handle, &header, &packet_data)) == 1) {
    pcap_dump((u_char*)dumper, header, packet_data);
  }

  if (ret == -1) {
    pcap_dump_flush(dumper);
  }

  pcap_dump_close(dumper);
  pcap_close(pcap_handle);
  assert_file_closed(&in_file);;

  assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_fd_closed(in_fd);
	return 0;
}