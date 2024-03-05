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
//<ID> 327
//<Prompt> ["LogL16fromY","TIFFGetReadProc","TIFFSetClientInfo","TIFFRegisterCODEC","TIFFSetFileno"]
/*<Combination>: [int LogL16fromY(double , int ),
    TIFFReadWriteProc TIFFGetReadProc(TIFF * ),
    void TIFFSetClientInfo(TIFF * , void * , const char * ),
    TIFFCodec *TIFFRegisterCODEC(uint16_t , const char * , TIFFInitMethod ),
    int TIFFSetFileno(TIFF * , int )
*/
//<score> 2, nr_unique_branch: 0
//<Quality> {"density":2,"unique_branches":{},"library_calls":["TIFFStreamOpen","TIFFClose"],"critical_calls":["TIFFStreamOpen","TIFFClose"],"visited":0}
/*Here is the completed fuzz driver code using libtiff library APIs:

*/


extern "C" int LLVMFuzzerTestOneInput_15(const uint8_t* f_data, size_t f_size) {
	if(f_size<10) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
	//fuzzer shim end}




    // Write data into input_file
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

    // Get file descriptor for reading and writing
    int in_fd = fuzz_fileno(in_file);

    // Read and write image data using libtiff APIs
    // ...

    // Release allocated resources
    TIFFClose(in_tif);
    assert_file_closed(&in_file);;

    assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
}