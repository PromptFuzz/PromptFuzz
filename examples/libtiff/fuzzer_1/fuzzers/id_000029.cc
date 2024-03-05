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
//<ID> 500
//<Prompt> ["TIFFFieldWriteCount","_TIFFfree","TIFFSetWriteOffset","TIFFIsByteSwapped","TIFFIsMSB2LSB","TIFFWarning","XYZtoRGB24","TIFFGetMode","TIFFCheckpointDirectory"]
/*<Combination>: [int TIFFFieldWriteCount(const TIFFField * ),
    void _TIFFfree(void * p),
    void TIFFSetWriteOffset(TIFF * tif, toff_t off),
    int TIFFIsByteSwapped(TIFF * ),
    int TIFFIsMSB2LSB(TIFF * ),
    void TIFFWarning(const char * , const char * ),
    void XYZtoRGB24(float * , uint8_t * ),
    int TIFFGetMode(TIFF * ),
    int TIFFCheckpointDirectory(TIFF * )
*/
//<score> 4, nr_unique_branch: 0
//<Quality> {"density":4,"unique_branches":{},"library_calls":["TIFFStreamOpen","TIFFFieldWriteCount","TIFFFindField","TIFFClose"],"critical_calls":["TIFFStreamOpen","TIFFFieldWriteCount","TIFFFindField","TIFFClose"],"visited":0}
/**/
extern "C" int LLVMFuzzerTestOneInput_29(const uint8_t* f_data, size_t f_size) {
	if(f_size<10) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
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

    // Get the number of fields in the TIFF directory
    uint32_t fieldCount = TIFFFieldWriteCount(TIFFFindField(in_tif, TIFFTAG_IMAGEWIDTH, TIFF_ANY));
    printf("Field count: %u\n", fieldCount);

    // Free allocated resources
    TIFFClose(in_tif);

    assert_file_closed(&in_file);
	return 0;
}