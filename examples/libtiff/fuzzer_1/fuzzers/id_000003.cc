#include "FDSan.h"
#include "FuzzedDataProvider.h"
#include <tiffio.hxx>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 18
//<Prompt> ["uv_encode","TIFFIsBigEndian","_TIFFmemcpy","TIFFOpenOptionsSetWarningHandlerExtR","TIFFIsMSB2LSB","TIFFGetTagListCount"]
/*<Combination>: [int uv_encode(double , double , int ),
    int TIFFIsBigEndian(TIFF * ),
    void _TIFFmemcpy(void * d, const void * s, tmsize_t c),
    void TIFFOpenOptionsSetWarningHandlerExtR(TIFFOpenOptions * opts, TIFFErrorHandlerExtR handler, void * warnhandler_user_data),
    int TIFFIsMSB2LSB(TIFF * ),
    int TIFFGetTagListCount(TIFF * )
*/
//<score> 12, nr_unique_branch: 1
//<Quality> {"density":6,"unique_branches":{"uv_encode":[[966,9,966,21,0,0,4,0]]},"library_calls":["TIFFStreamOpen","uv_encode","TIFFIsBigEndian","_TIFFmalloc","TIFFClose","_TIFFmemcpy","TIFFOpenOptionsAlloc","TIFFOpenOptionsSetWarningHandlerExtR","TIFFIsMSB2LSB","TIFFGetTagListCount","TIFFClose","_TIFFfree","TIFFOpenOptionsFree"],"critical_calls":["TIFFStreamOpen","uv_encode","TIFFIsBigEndian","_TIFFmalloc","_TIFFmemcpy","TIFFOpenOptionsAlloc","TIFFOpenOptionsSetWarningHandlerExtR","TIFFIsMSB2LSB","TIFFGetTagListCount","TIFFClose","_TIFFfree","TIFFOpenOptionsFree"],"visited":0}
/**/
extern "C" int LLVMFuzzerTestOneInput_3(const uint8_t* f_data, size_t f_size) {
	if(f_size<14) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
	//fuzzer shim end}




     // Write data into input_file.
     FILE* in_file = fopen("input_file", "wb");
     if (in_file == NULL) {
         assert_file_closed(&in_file);
	return 0;
     }
     fwrite(data, sizeof(uint8_t), size, in_file);
     assert_file_closed(&in_file);;

     // Open input tiff in memory
     std::istringstream s(std::string(data, data + size));
     TIFF* in_tif = TIFFStreamOpen(fuzz_str_1, &s);
     if (!in_tif) {
         assert_file_closed(&in_file);
	return 0;
     }

     // Call libtiff APIs to achieve the desired event
     int uv_encode_result = uv_encode(10.0, 20.0, fuzz_int32_t_2);
     int is_big_endian = TIFFIsBigEndian(in_tif);
     
     // Create a buffer to store the data when using _TIFFmemcpy
     tmsize_t buffer_size = size * sizeof(uint8_t);
     void* buffer = _TIFFmalloc(buffer_size);
     if (buffer == NULL) {
         TIFFClose(in_tif);
         assert_file_closed(&in_file);
	return 0;
     }
     _TIFFmemcpy(buffer, data, buffer_size);
     
     TIFFOpenOptions* options = TIFFOpenOptionsAlloc();
     TIFFOpenOptionsSetWarningHandlerExtR(options, NULL, NULL);
     
     int is_msb2lsb = TIFFIsMSB2LSB(in_tif);
     int tag_list_count = TIFFGetTagListCount(in_tif);
     
     // Release allocated resources
     TIFFClose(in_tif);
     _TIFFfree(buffer);
     TIFFOpenOptionsFree(options);

     assert_file_closed(&in_file);
	return 0;
 }