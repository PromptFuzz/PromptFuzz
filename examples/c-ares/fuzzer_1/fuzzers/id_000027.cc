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
//<ID> 1236
//<Prompt> ["ares_cancel","ares_process","ares_gethostbyname_file","ares_dup","ares_send","ares_set_servers_csv"]
/*<Combination>: [void ares_cancel(ares_channel channel),
    void ares_process(ares_channel channel, fd_set * read_fds, fd_set * write_fds),
    int ares_gethostbyname_file(ares_channel channel, const char * name, int family, struct hostent ** host),
    int ares_dup(ares_channel * dest, ares_channel src),
    void ares_send(ares_channel channel, const unsigned char * qbuf, int qlen, ares_callback callback, void * arg),
    int ares_set_servers_csv(ares_channel channel, const char * servers)
*/
//<score> 10, nr_unique_branch: 1
//<Quality> {"density":10,"unique_branches":{"ares_set_servers_ports":[[218,7,218,49,0,0,4,0]]},"library_calls":["ares_init","ares_set_socket_functions","ares_gethostbyname_file","ares_dup","ares_destroy","ares_send","ares_set_servers_csv","ares_process","ares_destroy","ares_destroy"],"critical_calls":["ares_init","ares_set_socket_functions","ares_gethostbyname_file","ares_dup","ares_send","ares_set_servers_csv","ares_process","ares_destroy","ares_destroy"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_27(const uint8_t* f_data, size_t f_size) {
	if(f_size<22) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
	//fuzzer shim end}




    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file == nullptr) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Create ares channel
    ares_channel channel;
    if (ares_init(&channel) != ARES_SUCCESS) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Set the file descriptor for reading
    int fd = fuzz_fileno(in_file);
    ares_set_socket_functions(channel, nullptr, (void *)fd);
    
    // Read input data from the file
    struct hostent *host = nullptr;
    int family = AF_INET;
    ares_gethostbyname_file(channel, "example.com", fuzz_int32_t_1, &host);
    
    // Duplicate the ares channel
    ares_channel duplicate;
    if (ares_dup(&duplicate, channel) != ARES_SUCCESS) {
        assert_file_closed(&in_file);;
        ares_destroy(channel);
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // Send a DNS query
    unsigned char query[256];
    int query_len = 256;
    ares_send(channel, query, query_len, [](void *, int, int, unsigned char *, int){}, nullptr);
    
    // Set the servers using a CSV string
    const char *servers = "8.8.8.8,8.8.4.4";
    ares_set_servers_csv(channel, fuzz_str_2);
    
    // Process DNS responses
    fd_set read_fds;
    fd_set write_fds;
    ares_process(channel, &read_fds, &write_fds);
    
    // Clean up
    assert_file_closed(&in_file);;
    ares_destroy(duplicate);
    ares_destroy(channel);
    
    assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
}