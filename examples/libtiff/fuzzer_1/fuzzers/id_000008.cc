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
//<ID> 77
//<Prompt> ["TIFFTileSize64","TIFFErrorExt","TIFFForceStrileArrayWriting","TIFFAccessTagMethods","LogLuv32fromXYZ","TIFFFileName"]
/*<Combination>: [uint64_t TIFFTileSize64(TIFF * tif),
    void TIFFErrorExt(thandle_t , const char * , const char * ),
    int TIFFForceStrileArrayWriting(TIFF * ),
    TIFFTagMethods *TIFFAccessTagMethods(TIFF * ),
    uint32_t LogLuv32fromXYZ(float * , int ),
    const char *TIFFFileName(TIFF * )
*/
//<score> 10.5, nr_unique_branch: 3
//<Quality> {"density":7,"unique_branches":{"LogL16fromY":[[807,9,807,26,0,0,4,1],[809,9,809,27,0,0,4,1]],"LogLuv32fromXYZ":[[1197,9,1197,12,0,0,4,0]]},"library_calls":["TIFFStreamOpen","TIFFTileSize64","TIFFErrorExt","TIFFForceStrileArrayWriting","TIFFAccessTagMethods","LogLuv32fromXYZ","TIFFFileName","TIFFClose"],"critical_calls":["TIFFStreamOpen","TIFFTileSize64","TIFFErrorExt","TIFFForceStrileArrayWriting","TIFFAccessTagMethods","LogLuv32fromXYZ","TIFFFileName","TIFFClose"],"visited":1}
/*Here is the complete implementation of the LLVMFuzzerTestOneInput_8 function using the libtiff APIs:

*/

extern "C" int LLVMFuzzerTestOneInput_8(const uint8_t* f_data, size_t f_size) {
	if(f_size<34) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
	//fuzzer shim end}




    // write data into input_file.
    FILE* in_file = fopen("input_file", "wb");
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

    // Call TIFFTileSize64
    uint64_t tileSize = TIFFTileSize64(in_tif);
    
    // Call TIFFErrorExt
    TIFFErrorExt(reinterpret_cast<thandle_t>(in_tif), fuzz_str_2, fuzz_str_3);

    // Call TIFFForceStrileArrayWriting
    int forceStrileArrayWriting = TIFFForceStrileArrayWriting(in_tif);

    // Call TIFFAccessTagMethods
    TIFFTagMethods* tagMethods = TIFFAccessTagMethods(in_tif);

    // Call LogLuv32fromXYZ
    float xyz[3];
    int logluv32 = LogLuv32fromXYZ(xyz, 3);

    // Call TIFFFileName
    const char* fileName = TIFFFileName(in_tif);
    
    // Release allocated resources
    TIFFClose(in_tif);

    assert_file_closed(&in_file);
	return 0;
}