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
//<ID> 449
//<Prompt> ["ares_parse_naptr_reply","ares_parse_txt_reply_ext","ares_inet_pton","ares_getsock","ares_getaddrinfo","ares_create_query"]
/*<Combination>: [int ares_parse_naptr_reply(const unsigned char * abuf, int alen, struct ares_naptr_reply ** naptr_out),
    int ares_parse_txt_reply_ext(const unsigned char * abuf, int alen, struct ares_txt_ext ** txt_out),
    int ares_inet_pton(int af, const char * src, void * dst),
    int ares_getsock(ares_channel channel, ares_socket_t * socks, int numsocks),
    void ares_getaddrinfo(ares_channel channel, const char * node, const char * service, const struct ares_addrinfo_hints * hints, ares_addrinfo_callback callback, void * arg),
    int ares_create_query(const char * name, int dnsclass, int type, unsigned short id, int rd, unsigned char ** buf, int * buflen, int max_udp_size)
*/
//<score> 20, nr_unique_branch: 58
//<Quality> {"density":4,"unique_branches":{"ares_parse_naptr_reply":[[64,7,64,22,0,0,4,0],[64,7,64,22,0,0,4,1],[70,7,70,19,0,0,4,0],[70,7,70,19,0,0,4,1],[72,7,72,19,0,0,4,0],[72,7,72,19,0,0,4,1],[78,7,78,29,0,0,4,0],[78,7,78,29,0,0,4,1],[81,7,81,42,0,0,4,0],[81,7,81,42,0,0,4,1],[89,15,89,26,0,0,4,0],[89,15,89,26,0,0,4,1],[93,11,93,33,0,0,4,0],[93,11,93,33,0,0,4,1],[98,11,98,41,0,0,4,0],[98,11,98,41,0,0,4,1],[107,11,107,38,0,0,4,0],[107,11,107,38,0,0,4,1],[114,11,114,27,0,0,4,0],[114,11,114,27,0,0,4,1],[114,31,114,49,0,0,4,0],[114,31,114,49,0,0,4,1],[119,15,119,25,0,0,4,0],[119,15,119,25,0,0,4,1],[127,15,127,26,0,0,4,1],[132,15,132,25,0,0,4,0],[132,15,132,25,0,0,4,1],[149,15,149,37,0,0,4,0],[149,15,149,37,0,0,4,1],[154,15,154,37,0,0,4,0],[154,15,154,37,0,0,4,1],[159,15,159,37,0,0,4,0],[159,15,159,37,0,0,4,1],[164,15,164,37,0,0,4,0],[164,15,164,37,0,0,4,1],[176,7,176,15,0,0,4,0],[178,7,178,14,0,0,4,0],[178,7,178,14,0,0,4,1],[182,7,182,29,0,0,4,0],[182,7,182,29,0,0,4,1],[184,11,184,21,0,0,4,0],[184,11,184,21,0,0,4,1]],"ares_free_data":[[99,9,99,35,0,0,4,0],[119,9,119,39,0,0,4,0],[121,15,121,41,0,0,4,0],[121,15,121,41,0,0,4,1],[123,15,123,42,0,0,4,0],[123,15,123,42,0,0,4,1],[125,15,125,44,0,0,4,0],[125,15,125,44,0,0,4,1],[127,15,127,43,0,0,4,0],[127,15,127,43,0,0,4,1],[129,15,129,48,0,0,4,0],[129,15,129,48,0,0,4,1]],"ares__parse_txt_reply":[[142,43,142,45,0,0,4,0],[159,19,159,21,0,0,4,0]],"ares_malloc_data":[[203,7,203,33,0,0,4,0],[237,7,237,37,0,0,4,0]]},"library_calls":["ares_parse_naptr_reply","ares_parse_txt_reply_ext","ares_free_data","ares_free_data"],"critical_calls":["ares_parse_naptr_reply","ares_parse_txt_reply_ext","ares_free_data","ares_free_data"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_8(const uint8_t *data, size_t size) {
	if(size<0) return 0;

	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);

  // Step 4: Create input and output file streams
  FILE *in_file = fmemopen((void *)data, size, "rb");
  FILE *out_file = fopen("output_file", "wb");

  if (!in_file || !out_file) {
    // Handle error
    if (in_file) assert_file_closed(&in_file);;
    if (out_file) assert_file_closed(&out_file);;
    assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
  }

  // Step 6: File names
  const char *input_file = "input_file";
  const char *output_file = "output_file";

  // Step 1: Define the event we want to achieve
  // Parse NAPTR reply and TXT reply extension data
  struct ares_naptr_reply *naptr_reply = nullptr;
  struct ares_txt_ext *txt_ext = nullptr;

  // Step 2: Call the c-ares APIs
  unsigned char *abuf = nullptr;
  int alen = 0;
  int af = AF_INET;  // Assuming IP address family is AF_INET

  // Read input data from file
  fseek(in_file, 0, SEEK_END);
  long file_size = ftell(in_file);
  rewind(in_file);
  if (file_size > 0) {
    abuf = (unsigned char *)malloc(file_size);
    if (!abuf) {
      // Handle allocation error
      assert_file_closed(&in_file);;
      assert_file_closed(&out_file);;
      assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    fread(abuf, file_size, 1, in_file);
    alen = file_size;
  }

  ares_parse_naptr_reply(abuf, alen, &naptr_reply);
  ares_parse_txt_reply_ext(abuf, alen, &txt_ext);

  // Step 3: Consume the input data

  // Step 5: Get the file descriptor
  int in_fd = fuzz_fileno(in_file);
  int out_fd = fuzz_fileno(out_file);

  // Step 7: Release resources
  if (abuf) free(abuf);
  if (in_file) assert_file_closed(&in_file);;
  if (out_file) assert_file_closed(&out_file);;
  if (naptr_reply) ares_free_data(naptr_reply);
  if (txt_ext) ares_free_data(txt_ext);

  assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}