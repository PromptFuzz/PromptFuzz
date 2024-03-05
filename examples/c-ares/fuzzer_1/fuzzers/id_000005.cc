#include "FDSan.h"
#include "FuzzedDataProvider.h"
#include <ares.h>
#include <ares_nameser.h>
#include <ares_dns.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 272
//<Prompt> ["ares_set_local_dev","ares_set_sortlist","ares_expand_string","ares_getnameinfo","ares_parse_soa_reply"]
/*<Combination>: [void ares_set_local_dev(ares_channel channel, const char * local_dev_name),
    int ares_set_sortlist(ares_channel channel, const char * sortstr),
    int ares_expand_string(const unsigned char * encoded, const unsigned char * abuf, int alen, unsigned char ** s, long * enclen),
    void ares_getnameinfo(ares_channel channel, const struct sockaddr * sa, ares_socklen_t salen, int flags, ares_nameinfo_callback callback, void * arg),
    int ares_parse_soa_reply(const unsigned char * abuf, int alen, struct ares_soa_reply ** soa_out)
*/
//<score> 15, nr_unique_branch: 50
//<Quality> {"density":5,"unique_branches":{"config_sortlist":[[2085,37,2085,45,0,0,4,1],[290,21,290,57,12,0,4,0]],"ares_free_data":[[133,9,133,37,0,0,4,0],[134,15,134,41,0,0,4,0],[134,15,134,41,0,0,4,1],[136,15,136,45,0,0,4,0],[136,15,136,45,0,0,4,1]],"ares_malloc_data":[[247,7,247,35,0,0,4,0]],"ares_parse_soa_reply":[[59,7,59,22,0,0,4,0],[59,7,59,22,0,0,4,1],[66,7,66,19,0,0,4,0],[66,7,66,19,0,0,4,1],[68,7,68,19,0,0,4,0],[68,7,68,19,0,0,4,1],[75,7,75,29,0,0,4,0],[75,7,75,29,0,0,4,1],[78,7,78,33,0,0,4,0],[78,7,78,33,0,0,4,1],[85,7,85,36,0,0,4,0],[85,7,85,36,0,0,4,1],[90,7,90,22,0,0,4,0],[90,7,90,22,0,0,4,1],[90,26,90,37,0,0,4,0],[90,26,90,37,0,0,4,1],[94,15,94,26,0,0,4,0],[94,15,94,26,0,0,4,1],[98,9,98,31,0,0,4,0],[98,9,98,31,0,0,4,1],[105,10,105,40,0,0,4,0],[105,10,105,40,0,0,4,1],[115,9,115,36,0,0,4,0],[115,9,115,36,0,0,4,1],[121,10,121,26,0,0,4,0],[121,10,121,26,0,0,4,1],[121,30,121,46,0,0,4,0],[121,30,121,46,0,0,4,1],[125,11,125,15,0,0,4,1],[135,11,135,33,0,0,4,0],[135,11,135,33,0,0,4,1],[145,11,145,33,0,0,4,0],[145,11,145,33,0,0,4,1],[153,11,153,37,0,0,4,0],[153,11,153,37,0,0,4,1],[175,9,175,27,0,0,4,1],[185,7,185,10,0,0,4,0],[185,7,185,10,0,0,4,1],[187,7,187,12,0,0,4,0],[187,7,187,12,0,0,4,1]],"ares_expand_string":[[56,7,56,27,0,0,4,1],[60,7,60,37,0,0,4,0]]},"library_calls":["ares_library_init","ares_init","ares_library_cleanup","ares_set_local_dev","ares_set_sortlist","ares_expand_string","ares_getnameinfo","ares_parse_soa_reply","ares_free_data","ares_free_data","ares_destroy","ares_library_cleanup"],"critical_calls":["ares_library_init","ares_init","ares_set_local_dev","ares_set_sortlist","ares_expand_string","ares_getnameinfo","ares_parse_soa_reply","ares_free_data","ares_free_data","ares_destroy","ares_library_cleanup"],"visited":1}
/*Here is the step-by-step implementation of the C++ program using c-ares library APIs:

*/


// Callback function for ares_getnameinfo
void  id_000005_nameinfo_callback(void *arg, int status, int timeouts, char *node, char *service) {
    if (status == ARES_SUCCESS) {
        printf("Node: %s, Service: %s\n", node, service);
    } else {
        printf("Failed to get name info\n");
    }
}

extern "C" int LLVMFuzzerTestOneInput_5(const uint8_t* f_data, size_t f_size) {
	if(f_size<11) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
	//fuzzer shim end}




    // Initialize c-ares library
    if (ares_library_init(ARES_LIB_INIT_ALL) != ARES_SUCCESS) {
        printf("Failed to initialize c-ares library\n");
        return 0;
    }

    // Create a channel
    ares_channel channel;
    if (ares_init(&channel) != ARES_SUCCESS) {
        printf("Failed to create ares channel\n");
        ares_library_cleanup();
        return 0;
    }

    // Set local device name
    ares_set_local_dev(channel, fuzz_str_1);

    // Set sortlist
    ares_set_sortlist(channel, "192.168.0.0/16 10.0.0.0/8");

    // Expand string
    unsigned char *expanded_str;
    long enclen;
    ares_expand_string((const unsigned char *)data, nullptr, 0, &expanded_str, &enclen);

    // Get name information using expanded string
    struct sockaddr sa;
    ares_getnameinfo(channel, &sa, sizeof(sa), fuzz_int32_t_2,  id_000005_nameinfo_callback, nullptr);

    // Parse SOA reply
    struct ares_soa_reply *soa_reply;
    ares_parse_soa_reply(data, size, &soa_reply);

    // Cleanup and release resources
    ares_free_data(expanded_str);
    ares_free_data(soa_reply);
    ares_destroy(channel);
    ares_library_cleanup();

    return 0;
}