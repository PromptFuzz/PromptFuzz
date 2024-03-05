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
//<ID> 520
//<Prompt> ["TIFFCurrentRow","TIFFDefaultTileSize","TIFFReadRGBATile","TIFFCurrentTile","TIFFFileno","TIFFGetSeekProc"]
/*<Combination>: [uint32_t TIFFCurrentRow(TIFF * ),
    void TIFFDefaultTileSize(TIFF * , uint32_t * , uint32_t * ),
    int TIFFReadRGBATile(TIFF * , uint32_t , uint32_t , uint32_t * ),
    uint32_t TIFFCurrentTile(TIFF * tif),
    int TIFFFileno(TIFF * ),
    TIFFSeekProc TIFFGetSeekProc(TIFF * )
*/
//<score> 10, nr_unique_branch: 34
//<Quality> {"density":8,"unique_branches":{"_TIFFReadTileAndAllocBuffer":[[986,35,986,66,0,0,4,0]],"gtTileSeparate":[[883,9,883,31,0,0,4,0],[885,13,885,31,0,0,4,1],[1017,15,1017,39,0,0,4,0],[1020,9,1020,33,0,0,4,0],[1024,24,1024,32,0,0,4,0],[1024,24,1024,32,0,0,4,1],[1029,20,1029,32,0,0,4,1]],"gtTileContig":[[740,9,740,31,0,0,4,0],[742,13,742,31,0,0,4,1],[813,15,813,39,0,0,4,0],[817,9,817,33,0,0,4,0],[821,24,821,32,0,0,4,0],[821,24,821,32,0,0,4,1],[826,20,826,32,0,0,4,1]],"TIFFReadRGBATileExt":[[3276,9,3276,26,0,0,4,0],[3276,9,3276,26,0,0,4,1],[3285,9,3285,32,0,0,4,0],[3285,9,3285,32,0,0,4,1],[3285,36,3285,59,0,0,4,0],[3285,36,3285,59,0,0,4,1],[3297,9,3297,36,0,0,4,1],[3298,9,3298,60,0,0,4,0],[3298,9,3298,60,0,0,4,1],[3311,9,3311,38,0,0,4,0],[3311,9,3311,38,0,0,4,1],[3316,9,3316,37,0,0,4,1],[3340,9,3340,33,0,0,4,0],[3340,37,3340,61,0,0,4,0],[3340,37,3340,61,0,0,4,1],[3343,21,3343,39,0,0,4,0],[3343,21,3343,39,0,0,4,1],[3353,30,3353,48,0,0,4,0],[3353,30,3353,48,0,0,4,1]]},"library_calls":["TIFFStreamOpen","TIFFCurrentRow","TIFFDefaultTileSize","TIFFCurrentTile","TIFFReadRGBATile","TIFFFileno","TIFFGetSeekProc","TIFFClose"],"critical_calls":["TIFFStreamOpen","TIFFCurrentRow","TIFFDefaultTileSize","TIFFCurrentTile","TIFFReadRGBATile","TIFFFileno","TIFFGetSeekProc","TIFFClose"],"visited":3}
/*Here is the complete implementation of the fuzz driver based on the provided instructions:

*/


extern "C" int LLVMFuzzerTestOneInput_34(const uint8_t* f_data, size_t f_size) {
	if(f_size<10) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
	//fuzzer shim end}




    // Write data into input_file.
    FILE* in_file = fopen("input_file", "wb");
    if (in_file == nullptr) {
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

    // 1. Call the APIs to achieve the event
    uint32_t currentRow = TIFFCurrentRow(in_tif);
    uint32_t tileSizeX, tileSizeY;
    TIFFDefaultTileSize(in_tif, &tileSizeX, &tileSizeY);
    uint32_t currentTile = TIFFCurrentTile(in_tif);
    uint32_t* rgbaTile = new uint32_t[tileSizeX * tileSizeY];
    int readRGBATile = TIFFReadRGBATile(in_tif, currentTile, currentRow, rgbaTile);
    int fileNo = TIFFFileno(in_tif);
    TIFFSeekProc seekProc = TIFFGetSeekProc(in_tif);

    // 2. Use the results of the APIs if necessary
    // ...

    // 3. Consume the input data and size if needed
    // ...

    // 4. Release resources
    TIFFClose(in_tif);
    delete[] rgbaTile;

    assert_file_closed(&in_file);
	return 0;
}