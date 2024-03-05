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
//<ID> 1424
//<Prompt> ["pcap_create","pcap_set_datalink","pcap_datalink_name_to_val","pcap_setfilter","pcap_tstamp_type_name_to_val","pcap_alloc_option","pcap_get_option_string","pcap_dump_open_append","pcap_close","pcap_freecode","pcap_loop","pcap_dump_close","pcap_free_option"]
/*<Combination>: [pcap_t *pcap_create(const char * , char * ),
    int pcap_set_datalink(pcap_t * , int ),
    int pcap_datalink_name_to_val(const char * ),
    int pcap_setfilter(pcap_t * , struct bpf_program * ),
    int pcap_tstamp_type_name_to_val(const char * ),
    pcap_options *pcap_alloc_option(),
    const char *pcap_get_option_string(pcap_options * po, enum pcap_option_name pon),
    pcap_dumper_t *pcap_dump_open_append(pcap_t * , const char * ),
    void pcap_close(pcap_t * ),
    void pcap_freecode(struct bpf_program * ),
    int pcap_loop(pcap_t * , int , pcap_handler , u_char * ),
    void pcap_dump_close(pcap_dumper_t * ),
    void pcap_free_option(pcap_options * po)
*/
//<score> 27, nr_unique_branch: 2
//<Quality> {"density":9,"unique_branches":{"pcap_datalink_name_to_val":[[3393,14,3393,41,0,0,4,1]],"pcap_get_option_string":[[112,9,112,34,0,0,4,0]]},"library_calls":["pcap_create","pcap_geterr","pcap_set_datalink","pcap_datalink_name_to_val","pcap_setfilter","pcap_tstamp_type_name_to_val","pcap_alloc_option","pcap_get_option_string","pcap_dump_open_append","pcap_close","pcap_freecode","pcap_dump_close","pcap_free_option"],"critical_calls":["pcap_create","pcap_geterr","pcap_set_datalink","pcap_datalink_name_to_val","pcap_setfilter","pcap_tstamp_type_name_to_val","pcap_alloc_option","pcap_get_option_string","pcap_dump_open_append","pcap_close","pcap_freecode","pcap_dump_close","pcap_free_option"],"visited":0}
/**/

extern "C" int LLVMFuzzerTestOneInput_21(const uint8_t* data, size_t size) {
	if(size<0) return 0;

    pcap_t* handle = pcap_create("interface", pcap_geterr(handle));
    if (handle == nullptr) {
        return 0;
    }

    pcap_set_datalink(handle, pcap_datalink_name_to_val("datalink"));
    struct bpf_program fp;
    pcap_setfilter(handle, &fp);

    pcap_tstamp_type_name_to_val("tstamp_type");

    pcap_options* options = pcap_alloc_option();

    const char* optionString = pcap_get_option_string(options, PON_TSTAMP_PRECISION);

    pcap_dumper_t* dumper = pcap_dump_open_append(handle, "output_file");

    pcap_close(handle);
    pcap_freecode(&fp);
    pcap_dump_close(dumper);
    pcap_free_option(options);

    assert_file_name_closed("output_file");
	return 0;
}