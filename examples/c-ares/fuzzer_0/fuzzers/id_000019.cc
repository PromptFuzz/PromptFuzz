#include "FDSan.h"
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
//<ID> 1015
//<Prompt> ["ares_cancel","ares_init","ares_query","ares_parse_srv_reply","ares_getnameinfo","ares_parse_uri_reply","ares_expand_string","ares_inet_pton","ares_freeaddrinfo","ares_free_string","ares_destroy"]
/*<Combination>: [void ares_cancel(ares_channel channel),
    int ares_init(ares_channel * channelptr),
    void ares_query(ares_channel channel, const char * name, int dnsclass, int type, ares_callback callback, void * arg),
    int ares_parse_srv_reply(const unsigned char * abuf, int alen, struct ares_srv_reply ** srv_out),
    void ares_getnameinfo(ares_channel channel, const struct sockaddr * sa, ares_socklen_t salen, int flags, ares_nameinfo_callback callback, void * arg),
    int ares_parse_uri_reply(const unsigned char * abuf, int alen, struct ares_uri_reply ** uri_out),
    int ares_expand_string(const unsigned char * encoded, const unsigned char * abuf, int alen, unsigned char ** s, long * enclen),
    int ares_inet_pton(int af, const char * src, void * dst),
    void ares_freeaddrinfo(struct ares_addrinfo * ai),
    void ares_free_string(void * str),
    void ares_destroy(ares_channel channel)
*/
//<score> 10, nr_unique_branch: 26
//<Quality> {"density":5,"unique_branches":{"end_aquery":[[176,7,176,11,0,0,4,0]],"ares_create_query":[[135,7,135,29,0,0,4,0],[140,10,140,15,0,0,4,1],[142,11,142,23,0,0,4,0],[151,15,151,25,0,0,4,0],[151,29,151,42,0,0,4,0],[151,29,151,42,0,0,4,1],[155,11,155,25,0,0,4,0],[164,15,164,25,0,0,4,0],[164,29,164,42,0,0,4,0],[164,29,164,42,0,0,4,1],[197,7,198,50,0,0,4,0]]," id_000019_nameinfo_callback":[[208,7,208,29,0,0,4,0],[211,11,211,49,0,0,4,1],[225,11,225,42,0,0,4,1]],"ares_getnameinfo":[[96,7,96,33,0,0,4,0],[97,7,97,44,0,0,4,0],[115,7,115,39,0,0,4,0],[115,43,115,72,0,0,4,0],[119,7,119,38,0,0,4,1],[130,7,130,35,0,0,4,0],[133,10,133,39,0,0,4,1],[171,13,171,21,0,0,4,1],[180,13,180,37,0,0,4,0]],"ares_query_qid":[[78,7,78,29,0,0,4,0],[80,11,80,23,0,0,4,1]]},"library_calls":["ares_init","ares_strerror","ares_destroy","ares_query","ares_getnameinfo","ares_destroy"],"critical_calls":["ares_init","ares_query","ares_getnameinfo","ares_destroy"],"visited":2}
/*Here is the implementation of the fuzz driver program using c-ares library APIs:

*/


// Callback function for ares_getnameinfo
void  id_000019_nameinfo_callback(void *arg, int status, int timeouts, char *node, char *service) {
    if (status == ARES_SUCCESS) {
        printf("Name: %s\n", node);
        printf("Service: %s\n", service);
    } else {
        printf("Failed to resolve nameinfo: %s\n", ares_strerror(status));
    }
}

// Callback function for ares_query
void  id_000019_query_callback(void *arg, int status, int timeouts, unsigned char *abuf, int alen) {
    if (status == ARES_SUCCESS) {
        struct ares_srv_reply *srv_reply;
        int result = ares_parse_srv_reply(abuf, alen, &srv_reply);
        if (result == ARES_SUCCESS) {
            struct ares_srv_reply *iter = srv_reply;
            while (iter) {
                printf("Host: %s\n", iter->host);
                printf("Priority: %hu\n", iter->priority);
                printf("Weight: %hu\n", iter->weight);
                printf("Port: %hu\n", iter->port);
                iter = iter->next;
            }
            ares_free_data(srv_reply);
        } else {
            printf("Failed to parse SRV reply: %s\n", ares_strerror(result));
        }
    } else {
        printf("Failed to query: %s\n", ares_strerror(status));
    }
}

extern "C" int LLVMFuzzerTestOneInput_19(const uint8_t *data, size_t size) {
	if(size<0) return 0;

    // Create ares channel
    ares_channel channel;
    int result = ares_init(&channel);
    if (result != ARES_SUCCESS) {
        printf("Failed to initialize ares library: %s\n", ares_strerror(result));
        return 0;
    }

    // Create a file stream from input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        printf("Failed to open input file\n");
        ares_destroy(channel);
        assert_file_closed(&in_file);
	return 0;
    }

    // Read input data and use it in c-ares APIs
    // Example: Read a domain name from input file
    char domain[256];
    fscanf(in_file, "%255s", domain);

    // Query SRV records
    ares_query(channel, domain, C_IN, T_SRV,  id_000019_query_callback, nullptr);

    // Create a socket address
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(80);
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    // Get name info for the socket address
    ares_getnameinfo(channel, (struct sockaddr *)&addr, sizeof(addr), 0,  id_000019_nameinfo_callback, nullptr);

    // Release allocated resources
    assert_file_closed(&in_file);;
    ares_destroy(channel);

    assert_file_closed(&in_file);
	return 0;
}