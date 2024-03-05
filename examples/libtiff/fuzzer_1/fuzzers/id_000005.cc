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
//<ID> 70
//<Prompt> ["TIFFTileSize64","TIFFErrorExt","TIFFForceStrileArrayWriting","TIFFAccessTagMethods","LogLuv32fromXYZ","TIFFFileName"]
/*<Combination>: [uint64_t TIFFTileSize64(TIFF * tif),
    void TIFFErrorExt(thandle_t , const char * , const char * ),
    int TIFFForceStrileArrayWriting(TIFF * ),
    TIFFTagMethods *TIFFAccessTagMethods(TIFF * ),
    uint32_t LogLuv32fromXYZ(float * , int ),
    const char *TIFFFileName(TIFF * )
*/
//<score> 15, nr_unique_branch: 12
//<Quality> {"density":6,"unique_branches":{"TIFFErrorExt":[[66,9,66,26,0,0,4,0],[72,9,72,29,0,0,4,1]],"LogLuv32fromXYZ":[[1197,9,1197,12,0,0,4,1],[1197,16,1197,23,0,0,4,1],[1207,9,1207,16,0,0,4,1],[1211,9,1211,17,0,0,4,1],[1213,9,1213,16,0,0,4,1],[1217,9,1217,17,0,0,4,1]],"LogL16fromY":[[803,9,803,26,0,0,4,1],[805,9,805,27,0,0,4,1],[807,9,807,26,0,0,4,0]],"TIFFForceStrileArrayWriting":[[81,9,81,34,0,0,4,0]]},"library_calls":["TIFFStreamOpen","TIFFTileSize64","TIFFErrorExt","TIFFForceStrileArrayWriting","TIFFAccessTagMethods","LogLuv32fromXYZ","TIFFFileName","TIFFClose"],"critical_calls":["TIFFStreamOpen","TIFFTileSize64","TIFFErrorExt","TIFFForceStrileArrayWriting","TIFFAccessTagMethods","LogLuv32fromXYZ","TIFFFileName","TIFFClose"],"visited":1}
/**/
extern "C" int LLVMFuzzerTestOneInput_5(const uint8_t* f_data, size_t f_size) {
	if(f_size<18) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
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

    // 1. Use TIFFTileSize64 to get the size of a tile in bytes
    uint64_t tileSize = TIFFTileSize64(in_tif);
    printf("Tile Size: %lu\n", tileSize);

    // 2. Use TIFFErrorExt to print an error message
    TIFFErrorExt(NULL, fuzz_str_2, "An error occurred");

    // 3. Use TIFFForceStrileArrayWriting to force writing of the strip/tile byte counts array
    int result = TIFFForceStrileArrayWriting(in_tif);
    if (result != 0) {
        printf("Failed to force writing of strip/tile byte counts array\n");
    }

    // 4. Use TIFFAccessTagMethods to access the tag methods of the TIFF file
    TIFFTagMethods* tagMethods = TIFFAccessTagMethods(in_tif);
    if (tagMethods != NULL) {
        printf("Tag Methods Accessed\n");
    }

    // 5. Use LogLuv32fromXYZ to convert XYZ color values to LogLuv32 format
    float xyzValues[3] = {0.5, 0.3, 0.1};
    uint32_t logLuvValue = LogLuv32fromXYZ(xyzValues, 1);
    printf("LogLuv32 Value: %u\n", logLuvValue);

    // 6. Use TIFFFileName to get the file name of the TIFF file
    const char* fileName = TIFFFileName(in_tif);
    printf("File Name: %s\n", fileName);

    // Clean up resources
    TIFFClose(in_tif);
    assert_file_closed(&in_file);
	return 0;
}