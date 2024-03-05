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
//<ID> 120
//<Prompt> ["ares_set_local_ip6","ares_parse_srv_reply","ares_freeaddrinfo","ares_getsock","ares_inet_pton","ares_parse_uri_reply","ares_expand_string"]
/*<Combination>: [void ares_set_local_ip6(ares_channel channel, const unsigned char * local_ip6),
    int ares_parse_srv_reply(const unsigned char * abuf, int alen, struct ares_srv_reply ** srv_out),
    void ares_freeaddrinfo(struct ares_addrinfo * ai),
    int ares_getsock(ares_channel channel, ares_socket_t * socks, int numsocks),
    int ares_inet_pton(int af, const char * src, void * dst),
    int ares_parse_uri_reply(const unsigned char * abuf, int alen, struct ares_uri_reply ** uri_out),
    int ares_expand_string(const unsigned char * encoded, const unsigned char * abuf, int alen, unsigned char ** s, long * enclen)
*/
//<score> 6, nr_unique_branch: 2
//<Quality> {"density":4,"unique_branches":{"ares_parse_uri_reply":[[70,7,70,22,0,0,4,0]],"ares_expand_string":[[56,7,56,27,0,0,4,0]]},"library_calls":["ares_init","ares_destroy","ares_parse_srv_reply","ares_parse_uri_reply","ares_expand_string","ares_inet_pton","ares_freeaddrinfo","ares_free_string","ares_destroy"],"critical_calls":["ares_init","ares_parse_srv_reply","ares_parse_uri_reply","ares_expand_string","ares_inet_pton","ares_freeaddrinfo","ares_free_string","ares_destroy"],"visited":1}
/*Here is an example implementation of the LLVMFuzzerTestOneInput_2 function using the c-ares library APIs:

*/


extern "C" int LLVMFuzzerTestOneInput_2(const uint8_t* f_data, size_t f_size) {
	if(f_size<16) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
	//fuzzer shim end}




    // Create a c-ares channel
    ares_channel channel;
    ares_init(&channel);

    // Create a file to read the input data
    FILE *in_file = fmemopen((void *) data, size, "rb");
    if (!in_file) {
        // Failed to create the file, return
        ares_destroy(channel);
        assert_file_closed(&in_file);
	return 0;
    }

    // Read the input data from the file
    unsigned char *abuf = nullptr;
    int alen = 0;
    fseek(in_file, 0, SEEK_END);
    long file_size = ftell(in_file);
    fseek(in_file, 0, SEEK_SET);
    abuf = new unsigned char[file_size];
    fread(abuf, 1, file_size, in_file);
    assert_file_closed(&in_file);;

    // Example event: Parsing SRV reply
    struct ares_srv_reply *srv_out = nullptr;
    ares_parse_srv_reply(abuf, alen, &srv_out);

    // Example event: Parsing URI reply
    struct ares_uri_reply *uri_out = nullptr;
    ares_parse_uri_reply(abuf, alen, &uri_out);

    // Example event: Expanding string
    unsigned char *s = nullptr;
    long enclen;
    ares_expand_string(abuf, abuf, alen, &s, &enclen);

    // Example event: Converting IP address
    struct sockaddr_in sa;
    ares_inet_pton(fuzz_int32_t_2, fuzz_str_1, &(sa.sin_addr));

    // Cleanup
    ares_freeaddrinfo(nullptr);
    ares_free_string(s);
    ares_destroy(channel);
    delete[] abuf;

    assert_file_closed(&in_file);
	return 0;
}