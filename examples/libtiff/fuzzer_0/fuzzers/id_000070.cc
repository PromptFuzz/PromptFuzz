#include "FDSan.h"
#include <tiffio.hxx>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 2102
//<Prompt> ["TIFFStreamOpen","TIFFStreamOpen","LogLuv24toXYZ","TIFFSwabArrayOfFloat","TIFFOpen","TIFFNumberOfStrips","TIFFReadEncodedStrip","TIFFWriteRawStrip","TIFFFlushData","_TIFFfree","TIFFClose"]
/*<Combination>: [TIFF *TIFFStreamOpen(const char * , std::ostream * ),
    TIFF *TIFFStreamOpen(const char * , std::ostream * ),
    void LogLuv24toXYZ(uint32_t , float * ),
    void TIFFSwabArrayOfFloat(float * fp, tmsize_t n),
    TIFF *TIFFOpen(const char * filename, const char * mode),
    uint32_t TIFFNumberOfStrips(TIFF * ),
    tmsize_t TIFFReadEncodedStrip(TIFF * tif, uint32_t strip, void * buf, tmsize_t size),
    tmsize_t TIFFWriteRawStrip(TIFF * tif, uint32_t strip, void * data, tmsize_t cc),
    int TIFFFlushData(TIFF * tif),
    void _TIFFfree(void * p),
    void TIFFClose(TIFF * tif)
*/
//<score> 40, nr_unique_branch: 8
//<Quality> {"density":20,"unique_branches":{"Fax3Decode2D":[[328,13,328,24,40,0,4,1],[328,13,328,24,96,0,4,1],[333,17,333,27,96,0,4,1],[341,22,341,32,96,0,4,1],[170,21,170,35,119,0,4,0],[150,21,150,35,138,0,4,0],[170,21,170,35,160,0,4,0],[117,21,117,31,252,0,4,0]]},"library_calls":["TIFFStreamOpen","TIFFNumberOfStrips","TIFFStripSize","_TIFFmalloc","TIFFClose","TIFFOpen","_TIFFfree","TIFFClose","TIFFReadEncodedStrip","TIFFWriteRawStrip","_TIFFfree","TIFFClose","TIFFClose","TIFFFlushData","_TIFFfree","TIFFClose","TIFFClose","_TIFFfree","TIFFClose","TIFFClose"],"critical_calls":["TIFFStreamOpen","TIFFNumberOfStrips","TIFFStripSize","_TIFFmalloc","TIFFOpen","TIFFReadEncodedStrip","TIFFWriteRawStrip","TIFFFlushData","_TIFFfree","TIFFClose","TIFFClose"],"visited":0}
/**/
extern "C" int LLVMFuzzerTestOneInput_70(const uint8_t* data, size_t size) {
	if(size<0) return 0;

	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);

    // Write data into input_file
    FILE* in_file = fopen("input_file", "wb");
    if (in_file == NULL) {
        assert_file_closed(&in_file);
	return 0;
    }
    fwrite(data, sizeof(uint8_t), size, in_file);
    assert_file_closed(&in_file);;

    // Open input tiff in memory
    std::istringstream s(std::string(data, data + size));
    TIFF* in_tif = TIFFStreamOpen("MemTIFF", &s);
    if (!in_tif) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Get the number of strips in the TIFF file
    uint32_t num_strips = TIFFNumberOfStrips(in_tif);

    // Allocate memory for reading a strip
    tmsize_t strip_size = TIFFStripSize(in_tif);
    void* strip_buf = _TIFFmalloc(strip_size);
    if (!strip_buf) {
        TIFFClose(in_tif);
        assert_file_closed(&in_file);
	return 0;
    }

    // Read each strip and write it to the output TIFF file
    TIFF* out_tif = TIFFOpen("output_file", "wb");
    if (!out_tif) {
        _TIFFfree(strip_buf);
        TIFFClose(in_tif);
        assert_file_name_closed("output_file");
	assert_file_closed(&in_file);
	return 0;
    }
    for (uint32_t strip = 0; strip < num_strips; strip++) {
        tmsize_t bytes_read = TIFFReadEncodedStrip(in_tif, strip, strip_buf, strip_size);
        tmsize_t bytes_written = TIFFWriteRawStrip(out_tif, strip, strip_buf, bytes_read);
        if (bytes_read != bytes_written) {
            _TIFFfree(strip_buf);
            TIFFClose(in_tif);
            TIFFClose(out_tif);
            assert_file_name_closed("output_file");
	assert_file_closed(&in_file);
	return 0;
        }
    }

    // Flush any remaining data in the output TIFF file
    int flush_result = TIFFFlushData(out_tif);
    if (flush_result != 1) {
        _TIFFfree(strip_buf);
        TIFFClose(in_tif);
        TIFFClose(out_tif);
        assert_file_name_closed("output_file");
	assert_file_closed(&in_file);
	return 0;
    }

    // Free allocated memory and close TIFF files
    _TIFFfree(strip_buf);
    TIFFClose(in_tif);
    TIFFClose(out_tif);

    assert_file_name_closed("output_file");
	assert_file_closed(&in_file);
	return 0;
}