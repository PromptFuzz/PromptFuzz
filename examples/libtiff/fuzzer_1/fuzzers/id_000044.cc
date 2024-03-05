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
//<ID> 622
//<Prompt> ["TIFFStreamOpen","TIFFCurrentStrip","_TIFFmalloc","_TIFFmemset","_TIFFfree","_TIFFmemset","TIFFSetField","TIFFOpen","TIFFGetSizeProc","_TIFFmalloc","TIFFReadRawTile","TIFFWriteTile","_TIFFfree","TIFFClose"]
/*<Combination>: [TIFF *TIFFStreamOpen(const char * , std::ostream * ),
    uint32_t TIFFCurrentStrip(TIFF * ),
    void *_TIFFmalloc(tmsize_t s),
    void _TIFFmemset(void * p, int v, tmsize_t c),
    void _TIFFfree(void * p),
    void _TIFFmemset(void * p, int v, tmsize_t c),
    int TIFFSetField(TIFF * , uint32_t ),
    TIFF *TIFFOpen(const char * filename, const char * mode),
    TIFFSizeProc TIFFGetSizeProc(TIFF * ),
    void *_TIFFmalloc(tmsize_t s),
    tmsize_t TIFFReadRawTile(TIFF * tif, uint32_t tile, void * buf, tmsize_t size),
    tmsize_t TIFFWriteTile(TIFF * tif, void * buf, uint32_t x, uint32_t y, uint32_t z, uint16_t s),
    void _TIFFfree(void * p),
    void TIFFClose(TIFF * tif)
*/
//<score> 8, nr_unique_branch: 1
//<Quality> {"density":8,"unique_branches":{"TIFFCheckTile":[[81,9,81,32,0,0,4,0]]},"library_calls":["TIFFStreamOpen","TIFFNumberOfStrips","TIFFStripSize","_TIFFmalloc","TIFFReadRawTile","TIFFWriteTile","_TIFFfree","TIFFClose"],"critical_calls":["TIFFStreamOpen","TIFFNumberOfStrips","TIFFStripSize","_TIFFmalloc","TIFFReadRawTile","TIFFWriteTile","_TIFFfree","TIFFClose"],"visited":1}
/**/
extern "C" int LLVMFuzzerTestOneInput_44(const uint8_t* f_data, size_t f_size) {
	if(f_size<28) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_4, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_5, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_6, fdp);
	//fuzzer shim end}




    // write data into input_file
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

    // Get the number of strips in the input TIFF
    uint32_t num_strips = TIFFNumberOfStrips(in_tif);
    
    // Create an array to store the strip data
    tmsize_t strip_size = TIFFStripSize(in_tif);
    void* strip_data = _TIFFmalloc(strip_size);
    
    // Read each strip from the input TIFF and write it to the output TIFF
    for (uint32_t strip = 0; strip < num_strips; strip++) {
        // Read raw tile from input TIFF strip
        tmsize_t strip_read_size = TIFFReadRawTile(in_tif, fuzz_uint32_t_2, strip_data, strip_size);
        
        // Write the raw tile data to the output TIFF
        TIFFWriteTile(in_tif, strip_data, fuzz_uint32_t_3, fuzz_uint32_t_4, fuzz_uint32_t_5, fuzz_uint16_t_6);
    }
    
    // Free allocated resources
    _TIFFfree(strip_data);
    TIFFClose(in_tif);
    
    assert_file_closed(&in_file);
	return 0;
}