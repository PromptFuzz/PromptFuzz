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
//<ID> 320
//<Prompt> ["LogL16fromY","TIFFGetReadProc","TIFFSetClientInfo","TIFFRegisterCODEC","TIFFSetFileno"]
/*<Combination>: [int LogL16fromY(double , int ),
    TIFFReadWriteProc TIFFGetReadProc(TIFF * ),
    void TIFFSetClientInfo(TIFF * , void * , const char * ),
    TIFFCodec *TIFFRegisterCODEC(uint16_t , const char * , TIFFInitMethod ),
    int TIFFSetFileno(TIFF * , int )
*/
//<score> 10, nr_unique_branch: 4
//<Quality> {"density":5,"unique_branches":{"TIFFRegisterCODEC":[[213,9,213,19,0,0,4,0]],"TIFFSetClientInfo":[[87,12,87,26,0,0,4,1],[90,9,90,23,0,0,4,1]],"TIFFCleanup":[[59,12,59,31,0,0,4,0]]},"library_calls":["TIFFStreamOpen","LogL16fromY","TIFFGetReadProc","TIFFSetClientInfo","TIFFRegisterCODEC","TIFFSetFileno","TIFFClose"],"critical_calls":["TIFFStreamOpen","LogL16fromY","TIFFGetReadProc","TIFFSetClientInfo","TIFFRegisterCODEC","TIFFSetFileno","TIFFClose"],"visited":1}
/**/
extern "C" int LLVMFuzzerTestOneInput_14(const uint8_t* f_data, size_t f_size) {
	if(f_size<34) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
	//fuzzer shim end}




    // write data into input_file.
    FILE *in_file = fopen("input_file", "wb");
    if (in_file == NULL) {
        assert_file_closed(&in_file);
	return 0;
    }
    fwrite(data, sizeof(uint8_t), size, in_file);
    assert_file_closed(&in_file);;

    // open input tiff in memory
    std::istringstream s(std::string(data, data + size));
    TIFF *in_tif = TIFFStreamOpen(fuzz_str_1, &s);
    if (!in_tif) {
        assert_file_closed(&in_file);
	return 0;
    }

    int result = 0;

    // Call the libtiff APIs to achieve the desired event

    // 1. int LogL16fromY(double, int);
    double y = 0.5;
    int l16_value = LogL16fromY(y, fuzz_int32_t_2);

    // 2. TIFFReadWriteProc TIFFGetReadProc(TIFF *);
    TIFFReadWriteProc read_proc = TIFFGetReadProc(in_tif);

    // 3. void TIFFSetClientInfo(TIFF *, void *, const char *);
    void *client_info = nullptr;
    const char *client_name = "LibTIFF";

    TIFFSetClientInfo(in_tif, client_info, fuzz_str_3);

    // 4. TIFFCodec *TIFFRegisterCODEC(uint16_t, const char *, TIFFInitMethod);
    uint16_t codec_scheme = 123;
    const char *codec_name = "MyCodec";
    TIFFInitMethod codec_init = nullptr;

    TIFFCodec *codec = TIFFRegisterCODEC(codec_scheme, fuzz_str_4, codec_init);

    // 5. int TIFFSetFileno(TIFF *, int);
    int file_descriptor = fuzz_fileno(in_file);
    int set_fileno_result = TIFFSetFileno(in_tif, file_descriptor);

    // Release all allocated resources
    TIFFClose(in_tif);

    assert_file_closed(&in_file);
	assert_fd_closed(file_descriptor);
	return result;
}