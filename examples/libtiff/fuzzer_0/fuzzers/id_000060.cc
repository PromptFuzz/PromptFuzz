#include "FDSan.h"
#include <tiffio.hxx>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 1353
//<Prompt> ["TIFFStreamOpen","TIFFNumberOfStrips","TIFFStripSize","_TIFFmalloc","TIFFReadRawStrip","TIFFSwabArrayOfDouble","TIFFFlush","TIFFReadRGBAImageOriented","TIFFOpen","TIFFWriteRawStrip","TIFFClose","_TIFFfree"]
/*<Combination>: [TIFF *TIFFStreamOpen(const char * , std::ostream * ),
    uint32_t TIFFNumberOfStrips(TIFF * ),
    tmsize_t TIFFStripSize(TIFF * tif),
    void *_TIFFmalloc(tmsize_t s),
    tmsize_t TIFFReadRawStrip(TIFF * tif, uint32_t strip, void * buf, tmsize_t size),
    void TIFFSwabArrayOfDouble(double * dp, tmsize_t n),
    int TIFFFlush(TIFF * tif),
    int TIFFReadRGBAImageOriented(TIFF * , uint32_t , uint32_t , uint32_t * , int , int ),
    TIFF *TIFFOpen(const char * filename, const char * mode),
    tmsize_t TIFFWriteRawStrip(TIFF * tif, uint32_t strip, void * data, tmsize_t cc),
    void TIFFClose(TIFF * tif),
    void _TIFFfree(void * p)
*/
//<score> 13, nr_unique_branch: 3
//<Quality> {"density":13,"unique_branches":{"TIFFFlush":[[32,9,32,34,0,0,4,0]],"TIFFSwabArrayOfDouble":[[228,12,228,19,0,0,4,0],[228,12,228,19,0,0,4,1]]},"library_calls":["TIFFStreamOpen","TIFFNumberOfStrips","TIFFStripSize","_TIFFmalloc","TIFFClose","TIFFReadRawStrip","_TIFFfree","TIFFClose","TIFFSwabArrayOfDouble","TIFFWriteRawStrip","_TIFFfree","TIFFFlush","TIFFClose"],"critical_calls":["TIFFStreamOpen","TIFFNumberOfStrips","TIFFStripSize","_TIFFmalloc","TIFFReadRawStrip","TIFFSwabArrayOfDouble","TIFFWriteRawStrip","_TIFFfree","TIFFFlush","TIFFClose"],"visited":2}
/**/
extern "C" int LLVMFuzzerTestOneInput_60(const uint8_t* data, size_t size) {
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
    
    uint32_t num_strips = TIFFNumberOfStrips(in_tif);
    for (uint32_t strip = 0; strip < num_strips; strip++) {
        tmsize_t strip_size = TIFFStripSize(in_tif);
        void* strip_buffer = _TIFFmalloc(strip_size);
        if (strip_buffer == NULL) {
            TIFFClose(in_tif);
            assert_file_closed(&in_file);
	return 0;
        }
        
        tmsize_t read_size = TIFFReadRawStrip(in_tif, strip, strip_buffer, strip_size);
        if (read_size < 0) {
            _TIFFfree(strip_buffer);
            TIFFClose(in_tif);
            assert_file_closed(&in_file);
	return 0;
        }
        
        // Perform some operation on the strip buffer
        
        TIFFSwabArrayOfDouble((double*)strip_buffer, read_size / sizeof(double));
        
        // Write the modified strip buffer to output tiff
        TIFFWriteRawStrip(in_tif, strip, strip_buffer, read_size);
        
        _TIFFfree(strip_buffer);
    }
    
    // Flush and close the input tiff
    TIFFFlush(in_tif);
    TIFFClose(in_tif);
    
    // Perform some other operations
    
    assert_file_closed(&in_file);
	return 0;
}