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
//<ID> 743
//<Prompt> ["ares_save_options","ares_dup","ares_freeaddrinfo","ares_gethostbyname","ares_destroy_options","ares_query","ares_gethostbyname_file"]
/*<Combination>: [int ares_save_options(ares_channel channel, struct ares_options * options, int * optmask),
    int ares_dup(ares_channel * dest, ares_channel src),
    void ares_freeaddrinfo(struct ares_addrinfo * ai),
    void ares_gethostbyname(ares_channel channel, const char * name, int family, ares_host_callback callback, void * arg),
    void ares_destroy_options(struct ares_options * options),
    void ares_query(ares_channel channel, const char * name, int dnsclass, int type, ares_callback callback, void * arg),
    int ares_gethostbyname_file(ares_channel channel, const char * name, int family, struct hostent ** host)
*/
//<score> 5, nr_unique_branch: 0
//<Quality> {"density":10,"unique_branches":{},"library_calls":["ares_init","ares_save_options","ares_dup","ares_destroy","ares_freeaddrinfo","ares_gethostbyname","ares_destroy_options","ares_query","ares_gethostbyname_file","ares_destroy","ares_destroy"],"critical_calls":["ares_init","ares_save_options","ares_dup","ares_freeaddrinfo","ares_gethostbyname","ares_destroy_options","ares_query","ares_gethostbyname_file","ares_destroy","ares_destroy"],"visited":1}
/**/

extern "C" int LLVMFuzzerTestOneInput_12(const uint8_t* f_data, size_t f_size) {
	if(f_size<36) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
	//fuzzer shim end}




    // Create ares channel
    ares_channel channel;
    if (ares_init(&channel) != ARES_SUCCESS) {
        return 0;
    }

    // Create options struct
    struct ares_options options;
    memset(&options, 0, sizeof(options));

    // Save options
    int optmask;
    ares_save_options(channel, &options, &optmask);

    // Duplicate channel
    ares_channel dup_channel;
    if (ares_dup(&dup_channel, channel) != ARES_SUCCESS) {
        ares_destroy(channel);
        return 0;
    }

    // Create addrinfo struct
    struct ares_addrinfo *ai = nullptr;

    // Free addrinfo struct
    ares_freeaddrinfo(ai);

    // Get host by name
    ares_host_callback callback = [](void *arg, int status, int timeouts, struct hostent *host) {};
    ares_gethostbyname(channel, fuzz_str_1, fuzz_int32_t_2, callback, nullptr);

    // Destroy options
    ares_destroy_options(&options);

    // Query
    ares_callback query_callback = [](void *arg, int status, int timeouts, unsigned char *abuf, int alen) {};
    ares_query(channel, fuzz_str_3, ns_c_in, ns_t_a, query_callback, nullptr);

    // Get host by name file
    struct hostent *host;
    ares_gethostbyname_file(channel, "example.com", fuzz_int32_t_4, &host);

    // Release resources
    ares_destroy(dup_channel);
    ares_destroy(channel);

    return 0;
}