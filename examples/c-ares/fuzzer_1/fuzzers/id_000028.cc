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
//<ID> 1314
//<Prompt> ["ares_library_init_mem","ares_version","ares_init_options","ares_set_servers_ports_csv","ares_search","ares_parse_txt_reply","ares_parse_ns_reply","ares_init","ares_query","ares_getnameinfo","ares_free_hostent","ares_free_string"]
/*<Combination>: [int ares_library_init_mem(int flags, void *(*)(size_t) amalloc, void (*)(void *) afree, void *(*)(void *, size_t) arealloc),
    const char *ares_version(int * version),
    int ares_init_options(ares_channel * channelptr, struct ares_options * options, int optmask),
    int ares_set_servers_ports_csv(ares_channel channel, const char * servers),
    void ares_search(ares_channel channel, const char * name, int dnsclass, int type, ares_callback callback, void * arg),
    int ares_parse_txt_reply(const unsigned char * abuf, int alen, struct ares_txt_reply ** txt_out),
    int ares_parse_ns_reply(const unsigned char * abuf, int alen, struct hostent ** host),
    int ares_init(ares_channel * channelptr),
    void ares_query(ares_channel channel, const char * name, int dnsclass, int type, ares_callback callback, void * arg),
    void ares_getnameinfo(ares_channel channel, const struct sockaddr * sa, ares_socklen_t salen, int flags, ares_nameinfo_callback callback, void * arg),
    void ares_free_hostent(struct hostent * host),
    void ares_free_string(void * str)
*/
//<score> 2, nr_unique_branch: 0
//<Quality> {"density":2,"unique_branches":{},"library_calls":["ares_library_init","ares_version","ares_init","ares_library_cleanup","ares_destroy","ares_library_cleanup"],"critical_calls":["ares_library_init","ares_version","ares_init","ares_destroy","ares_library_cleanup"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_28(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    // Step 4: Open input file
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Step 5: Get file descriptor
    int in_fd = fuzz_fileno(in_file);
    
    // Step 6: Get file name
    const char *in_filename = "input_file";
    
    // Step 1: Initialize c-ares library
    int res = ares_library_init(ARES_LIB_INIT_ALL);
    if (res != ARES_SUCCESS) {
        // Handle error
        assert_file_closed(&in_file);;
        assert_file_name_closed("input_file");
	assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Step 2: Get c-ares version
    int version;
    const char *ares_version_str = ares_version(&version);
    
    // Step 3: Initialize c-ares channel
    ares_channel channel;
    res = ares_init(&channel);
    if (res != ARES_SUCCESS) {
        // Handle error
        ares_library_cleanup();
        assert_file_closed(&in_file);;
        assert_file_name_closed("input_file");
	assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Step 7: Cleanup and release resources
    ares_destroy(channel);
    ares_library_cleanup();
    assert_file_closed(&in_file);;
    
    assert_file_name_closed("input_file");
	assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
}