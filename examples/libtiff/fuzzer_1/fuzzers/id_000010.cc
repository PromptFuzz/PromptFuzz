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
//<ID> 83
//<Prompt> ["TIFFStreamOpen","TIFFWarning","TIFFSwabFloat","TIFFFieldTag","TIFFFindField","TIFFMergeFieldInfo","TIFFGetField","TIFFStripSize","TIFFRawStripSize","TIFFReadTile","TIFFWriteCheck","TIFFRewriteDirectory","TIFFClose"]
/*<Combination>: [TIFF *TIFFStreamOpen(const char * , std::ostream * ),
    void TIFFWarning(const char * , const char * ),
    void TIFFSwabFloat(float * ),
    uint32_t TIFFFieldTag(const TIFFField * ),
    const TIFFField *TIFFFindField(TIFF * , uint32_t , TIFFDataType ),
    int TIFFMergeFieldInfo(TIFF * , const TIFFFieldInfo * , uint32_t ),
    int TIFFGetField(TIFF * tif, uint32_t tag),
    tmsize_t TIFFStripSize(TIFF * tif),
    tmsize_t TIFFRawStripSize(TIFF * tif, uint32_t strip),
    tmsize_t TIFFReadTile(TIFF * tif, void * buf, uint32_t x, uint32_t y, uint32_t z, uint16_t s),
    int TIFFWriteCheck(TIFF * , int , const char * ),
    int TIFFRewriteDirectory(TIFF * ),
    void TIFFClose(TIFF * tif)
*/
//<score> 88, nr_unique_branch: 29
//<Quality> {"density":11,"unique_branches":{"Fax3Decode1D":[[337,21,337,39,33,0,4,0],[170,21,170,35,50,0,4,0],[268,13,268,38,57,0,4,1]],"Fax3DecodeRLE":[[414,17,414,31,3,0,4,0],[414,35,414,49,3,0,4,1]],"PixarLogDecode":[[879,9,879,60,0,0,4,0]],"Fax3Decode2D":[[337,21,337,39,40,0,4,0],[326,13,326,22,96,0,4,0],[331,20,331,30,96,0,4,0],[331,34,331,46,96,0,4,0],[268,13,268,38,126,0,4,1],[170,21,170,35,145,0,4,1],[170,21,170,35,155,0,4,0],[268,13,268,38,184,0,4,1],[117,21,117,31,243,0,4,0]],"Fax4Decode":[[518,33,518,40,3,0,4,0],[553,33,553,46,3,0,4,0],[333,17,333,27,65,0,4,1],[341,22,341,32,65,0,4,1],[150,21,150,35,78,0,4,0],[170,21,170,35,90,0,4,1],[170,21,170,35,95,0,4,0],[170,21,170,35,100,0,4,0],[117,21,117,31,146,0,4,0],[117,21,117,31,150,0,4,0],[117,21,117,31,154,0,4,0]],"TIFFFillStrip":[[743,31,743,67,0,0,4,0]],"ThunderDecode":[[97,12,97,18,0,0,4,1],[159,23,159,42,0,0,4,0]]},"library_calls":["TIFFStreamOpen","TIFFFindField","TIFFGetField","TIFFFieldTag","TIFFFindField","TIFFGetField","TIFFFieldTag","TIFFStripSize","TIFFNumberOfStrips","TIFFReadEncodedStrip","TIFFClose"],"critical_calls":["TIFFStreamOpen","TIFFFindField","TIFFGetField","TIFFFieldTag","TIFFFindField","TIFFGetField","TIFFFieldTag","TIFFStripSize","TIFFNumberOfStrips","TIFFReadEncodedStrip","TIFFClose"],"visited":0}
/**/
extern "C" int LLVMFuzzerTestOneInput_10(const uint8_t* f_data, size_t f_size) {
	if(f_size<14) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
	//fuzzer shim end}




    // Write data into input_file
    FILE* in_file = fopen("input_file", "wb");
    if (in_file == NULL) {
        assert_file_closed(&in_file);
	return 0;
    }
    fwrite(data, sizeof(uint8_t), size, in_file);
    assert_file_closed(&in_file);;
    
    // Open input TIFF in memory
    std::istringstream s(std::string(data, data + size));
    TIFF* in_tif = TIFFStreamOpen(fuzz_str_1, &s);
    if (!in_tif) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Read TIFF fields
    int width = 0;
    int height = 0;
    const TIFFField* field = TIFFFindField(in_tif, TIFFTAG_IMAGEWIDTH, TIFF_NOTYPE);
    if (field) {
        TIFFGetField(in_tif, TIFFFieldTag(field), &width);
    }
    field = TIFFFindField(in_tif, TIFFTAG_IMAGELENGTH, TIFF_NOTYPE);
    if (field) {
        TIFFGetField(in_tif, TIFFFieldTag(field), &height);
    }
    
    // Check if TIFF is valid
    if (width > 0 && height > 0) {
        // Read TIFF strips
        tmsize_t strip_size = TIFFStripSize(in_tif);
        uint8_t* strip_buffer = (uint8_t*)malloc(strip_size);
        if (strip_buffer) {
            for (uint32_t strip = 0; strip < TIFFNumberOfStrips(in_tif); strip++) {
                tmsize_t read_size = TIFFReadEncodedStrip(in_tif, fuzz_uint32_t_2, strip_buffer, strip_size);
                if (read_size < 0) {
                    break;
                }
                // Process strip data (e.g. analyze or write to output file)
            }
            free(strip_buffer);
        }
    }
    
    // Perform additional operations here using other libtiff APIs
    
    // Cleanup
    TIFFClose(in_tif);
    
    assert_file_closed(&in_file);
	return 0;
}