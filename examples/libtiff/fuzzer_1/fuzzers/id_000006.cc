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
//<ID> 73
//<Prompt> ["TIFFTileSize64","TIFFErrorExt","TIFFForceStrileArrayWriting","TIFFAccessTagMethods","LogLuv32fromXYZ","TIFFFileName"]
/*<Combination>: [uint64_t TIFFTileSize64(TIFF * tif),
    void TIFFErrorExt(thandle_t , const char * , const char * ),
    int TIFFForceStrileArrayWriting(TIFF * ),
    TIFFTagMethods *TIFFAccessTagMethods(TIFF * ),
    uint32_t LogLuv32fromXYZ(float * , int ),
    const char *TIFFFileName(TIFF * )
*/
//<score> 7, nr_unique_branch: 0
//<Quality> {"density":7,"unique_branches":{},"library_calls":["TIFFStreamOpen","TIFFTileSize64","TIFFErrorExt","TIFFForceStrileArrayWriting","TIFFAccessTagMethods","LogLuv32fromXYZ","TIFFFileName","TIFFClose"],"critical_calls":["TIFFStreamOpen","TIFFTileSize64","TIFFErrorExt","TIFFForceStrileArrayWriting","TIFFAccessTagMethods","LogLuv32fromXYZ","TIFFFileName","TIFFClose"],"visited":0}
/*Here is the complete implementation of the LLVMFuzzerTestOneInput_6 function:

*/


extern "C" int LLVMFuzzerTestOneInput_6(const uint8_t* f_data, size_t f_size) {
	if(f_size<23) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
	//fuzzer shim end}




    // write data into input_file
    FILE *in_file = fopen("input_file", "wb");
    if (in_file == NULL) {
        assert_file_closed(&in_file);
	return 0;
    }
    fwrite(data, sizeof(uint8_t), size, in_file);
    assert_file_closed(&in_file);;

    // open input tiff in memory
    std::istringstream s(std::string(data, data + size));
    TIFF* in_tif = TIFFStreamOpen(fuzz_str_1, &s);
    if (!in_tif) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Use the libtiff APIs to achieve the desired event
    uint64_t tileSize = TIFFTileSize64(in_tif);
    TIFFErrorExt((thandle_t)in_tif, fuzz_str_2, "this is a test error");
    TIFFForceStrileArrayWriting(in_tif);
    TIFFTagMethods* tagMethods = TIFFAccessTagMethods(in_tif);
    float xyz[3] = {1.0, 1.0, 1.0};
    uint32_t logLuv = LogLuv32fromXYZ(xyz, 3);
    const char* fileName = TIFFFileName(in_tif);

    // Release resources
    TIFFClose(in_tif);
    
    assert_file_closed(&in_file);
	return 0;
}