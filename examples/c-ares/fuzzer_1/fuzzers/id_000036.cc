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
//<ID> 1515
//<Prompt> ["ares_parse_a_reply","ares_gethostbyname_file","ares_strerror","ares_get_servers_ports","ares_gethostbyaddr","ares_library_init_mem","ares_fds","ares_save_options","ares_dup"]
/*<Combination>: [int ares_parse_a_reply(const unsigned char * abuf, int alen, struct hostent ** host, struct ares_addrttl * addrttls, int * naddrttls),
    int ares_gethostbyname_file(ares_channel channel, const char * name, int family, struct hostent ** host),
    const char *ares_strerror(int code),
    int ares_get_servers_ports(ares_channel channel, struct ares_addr_port_node ** servers),
    void ares_gethostbyaddr(ares_channel channel, const void * addr, int addrlen, int family, ares_host_callback callback, void * arg),
    int ares_library_init_mem(int flags, void *(*)(size_t) amalloc, void (*)(void *) afree, void *(*)(void *, size_t) arealloc),
    int ares_fds(ares_channel channel, fd_set * read_fds, fd_set * write_fds),
    int ares_save_options(ares_channel channel, struct ares_options * options, int * optmask),
    int ares_dup(ares_channel * dest, ares_channel src)
*/
//<score> 10, nr_unique_branch: 1
//<Quality> {"density":5,"unique_branches":{"ares_strerror":[[44,5,44,24,0,0,4,0]]},"library_calls":["ares_library_init_mem","ares_strerror","ares_init","ares_strerror","ares_library_cleanup","ares_gethostbyname_file","ares_strerror","ares_destroy","ares_library_cleanup","ares_free_hostent","ares_destroy","ares_library_cleanup"],"critical_calls":["ares_library_init_mem","ares_init","ares_gethostbyname_file","ares_free_hostent","ares_destroy","ares_library_cleanup"],"visited":0}
/**/

extern "C" int LLVMFuzzerTestOneInput_36(const uint8_t* f_data, size_t f_size) {
	if(f_size<17) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    // Step 4: Create a FILE pointer to read input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Step 5: Get the file descriptor of the input file
    int in_fd = fuzz_fileno(in_file);
    
    // Step 6: Create a string for the input file name
    const char *in_file_name = "input_file";
    
    // Step 1: Initialize c-ares library
    ares_channel channel;
    struct ares_options options;
    int optmask = 0;
    int status = ares_library_init_mem(ARES_LIB_INIT_ALL, malloc, free, realloc);
    if (status != ARES_SUCCESS) {
        fprintf(stderr, "Failed to initialize c-ares library: %s\n", ares_strerror(status));
        assert_file_closed(&in_file);;
        assert_file_name_closed("input_file");
	assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    status = ares_init(&channel);
    if (status != ARES_SUCCESS) {
        fprintf(stderr, "Failed to initialize ares_channel: %s\n", ares_strerror(status));
        assert_file_closed(&in_file);;
        ares_library_cleanup();
        assert_file_name_closed("input_file");
	assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Step 2: Call c-ares APIs to achieve the desired event
    struct hostent *host = NULL;
    struct ares_addrttl addrttls[10];
    int naddrttls = 0;
    status = ares_gethostbyname_file(channel, fuzz_str_1, fuzz_int32_t_2, &host);
    if (status != ARES_SUCCESS) {
        fprintf(stderr, "Failed to get host by name: %s\n", ares_strerror(status));
        assert_file_closed(&in_file);;
        ares_destroy(channel);
        ares_library_cleanup();
        assert_file_name_closed("input_file");
	assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Step 3: Consume the input data
    
    // Step 7: Release allocated resources
    assert_file_closed(&in_file);;
    ares_free_hostent(host);
    ares_destroy(channel);
    ares_library_cleanup();
    
    assert_file_name_closed("input_file");
	assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
}