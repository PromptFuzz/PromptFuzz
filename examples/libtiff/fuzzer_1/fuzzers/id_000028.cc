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
//<ID> 471
//<Prompt> ["TIFFStreamOpen","TIFFTileSize64","TIFFErrorExt","TIFFReadDirectory","TIFFForceStrileArrayWriting","TIFFSwabFloat","TIFFAccessTagMethods","LogLuv32fromXYZ","TIFFFieldPassCount","TIFFFileName","TIFFClose"]
/*<Combination>: [TIFF *TIFFStreamOpen(const char * , std::ostream * ),
    uint64_t TIFFTileSize64(TIFF * tif),
    void TIFFErrorExt(thandle_t , const char * , const char * ),
    int TIFFReadDirectory(TIFF * tif),
    int TIFFForceStrileArrayWriting(TIFF * ),
    void TIFFSwabFloat(float * ),
    TIFFTagMethods *TIFFAccessTagMethods(TIFF * ),
    uint32_t LogLuv32fromXYZ(float * , int ),
    int TIFFFieldPassCount(const TIFFField * ),
    const char *TIFFFileName(TIFF * ),
    void TIFFClose(TIFF * tif)
*/
//<score> 15, nr_unique_branch: 2
//<Quality> {"density":10,"unique_branches":{"_TIFFCheckFieldIsValidForCodec":[[1334,9,1334,34,0,0,4,0]],"TIFFReadDirectory":[[4128,9,4133,50,0,0,4,0]]},"library_calls":["TIFFStreamOpen","TIFFTileSize64","TIFFErrorExt","TIFFReadDirectory","TIFFForceStrileArrayWriting","TIFFSwabFloat","TIFFAccessTagMethods","LogLuv32fromXYZ","TIFFFieldWithTag","TIFFFieldPassCount","TIFFFileName","TIFFClose"],"critical_calls":["TIFFStreamOpen","TIFFTileSize64","TIFFErrorExt","TIFFReadDirectory","TIFFForceStrileArrayWriting","TIFFSwabFloat","TIFFAccessTagMethods","LogLuv32fromXYZ","TIFFFieldWithTag","TIFFFieldPassCount","TIFFFileName","TIFFClose"],"visited":1}
/**/
extern "C" int LLVMFuzzerTestOneInput_28(const uint8_t* f_data, size_t f_size) {
	if(f_size<18) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
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

    // call TIFFTileSize64 function
    uint64_t tileSize = TIFFTileSize64(in_tif);

    // call TIFFErrorExt function
    TIFFErrorExt((thandle_t)in_tif, fuzz_str_2, "Test Error");

    // call TIFFReadDirectory function
    int readDirectory = TIFFReadDirectory(in_tif);

    // call TIFFForceStrileArrayWriting function
    int forceStrileArrayWriting = TIFFForceStrileArrayWriting(in_tif);

    // call TIFFSwabFloat function
    float floatNumber = 3.14;
    TIFFSwabFloat(&floatNumber);

    // call TIFFAccessTagMethods function
    TIFFTagMethods *tagMethods = TIFFAccessTagMethods(in_tif);

    // call LogLuv32fromXYZ function
    float XYZ[3] = {1.0, 1.0, 1.0};
    uint32_t logLuv = LogLuv32fromXYZ(XYZ, 1);

    // call TIFFFieldPassCount function
    const TIFFField *field = TIFFFieldWithTag(in_tif, 256);
    int passCount = TIFFFieldPassCount(field);

    // call TIFFFileName function
    const char *fileName = TIFFFileName(in_tif);

    // call TIFFClose function
    TIFFClose(in_tif);

    // release allocated resources
    assert_file_closed(&in_file);
	return 0;
}