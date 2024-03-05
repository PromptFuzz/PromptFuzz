#include "FDSan.h"
#include <tiffio.hxx>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 1120
//<Prompt> ["TIFFStreamOpen","TIFFVStripSize64","TIFFSwabArrayOfFloat","TIFFDeferStrileArrayWriting","TIFFIsUpSampled","TIFFWarningExt","TIFFFieldSetGetSize","TIFFFieldWithName","TIFFOpenExt","TIFFSetMode","TIFFGetMode","TIFFClose"]
/*<Combination>: [TIFF *TIFFStreamOpen(const char * , std::ostream * ),
    uint64_t TIFFVStripSize64(TIFF * tif, uint32_t nrows),
    void TIFFSwabArrayOfFloat(float * fp, tmsize_t n),
    int TIFFDeferStrileArrayWriting(TIFF * ),
    int TIFFIsUpSampled(TIFF * ),
    void TIFFWarningExt(thandle_t , const char * , const char * ),
    int TIFFFieldSetGetSize(const TIFFField * ),
    const TIFFField *TIFFFieldWithName(TIFF * , const char * ),
    TIFF *TIFFOpenExt(const char * filename, const char * mode, TIFFOpenOptions * opts),
    int TIFFSetMode(TIFF * , int ),
    int TIFFGetMode(TIFF * ),
    void TIFFClose(TIFF * tif)
*/
//<score> 10, nr_unique_branch: 0
//<Quality> {"density":10,"unique_branches":{},"library_calls":["TIFFStreamOpen","TIFFVStripSize64","TIFFSwabArrayOfFloat","TIFFDeferStrileArrayWriting","TIFFIsUpSampled","TIFFWarningExt","TIFFFieldWithName","TIFFFieldSetGetSize","TIFFSetMode","TIFFGetMode","TIFFClose"],"critical_calls":["TIFFStreamOpen","TIFFVStripSize64","TIFFSwabArrayOfFloat","TIFFDeferStrileArrayWriting","TIFFIsUpSampled","TIFFWarningExt","TIFFFieldWithName","TIFFFieldSetGetSize","TIFFSetMode","TIFFGetMode","TIFFClose"],"visited":0}
/**/
extern "C" int LLVMFuzzerTestOneInput_54(const uint8_t* data, size_t size) {
	if(size<0) return 0;

	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);

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
    TIFF *in_tif = TIFFStreamOpen("MemTIFF", &s);
    if (!in_tif) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Call libtiff APIs to achieve the desired event

    // 1. TIFFVStripSize64
    uint32_t nrows = 10;
    uint64_t vStripSize = TIFFVStripSize64(in_tif, nrows);

    // 2. TIFFSwabArrayOfFloat
    tmsize_t arraySize = 5;
    float *floatArray = (float*)malloc(sizeof(float) * arraySize);
    TIFFSwabArrayOfFloat(floatArray, arraySize);

    // 3. TIFFDeferStrileArrayWriting
    TIFFDeferStrileArrayWriting(in_tif);

    // 4. TIFFIsUpSampled
    int isUpSampled = TIFFIsUpSampled(in_tif);

    // 5. TIFFWarningExt
    const char *module = "Fuzzer";
    const char *format = "Warning message";
    TIFFWarningExt(in_tif, module, format);

    // 6. TIFFFieldSetGetSize
    const TIFFField *field = TIFFFieldWithName(in_tif, "Field");
    int fieldSize = TIFFFieldSetGetSize(field);

    // 7. TIFFSetMode
    int mode = 1; // EXAMPLE: Set mode to 1
    TIFFSetMode(in_tif, mode);

    // 8. TIFFGetMode
    int currentMode = TIFFGetMode(in_tif);

    // Close input TIFF
    TIFFClose(in_tif);

    // Release allocated resources
    free(floatArray);

    assert_file_closed(&in_file);
	return 0;
}