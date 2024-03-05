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
//<ID> 592
//<Prompt> ["TIFFReadEncodedStrip","TIFFTileRowSize64","TIFFReadFromUserBuffer","TIFFScanlineSize64","TIFFFieldIsAnonymous","TIFFSwabShort","_TIFFrealloc","TIFFWriteRawTile"]
/*<Combination>: [tmsize_t TIFFReadEncodedStrip(TIFF * tif, uint32_t strip, void * buf, tmsize_t size),
    uint64_t TIFFTileRowSize64(TIFF * tif),
    int TIFFReadFromUserBuffer(TIFF * tif, uint32_t strile, void * inbuf, tmsize_t insize, void * outbuf, tmsize_t outsize),
    uint64_t TIFFScanlineSize64(TIFF * tif),
    int TIFFFieldIsAnonymous(const TIFFField * ),
    void TIFFSwabShort(uint16_t * ),
    void *_TIFFrealloc(void * p, tmsize_t s),
    tmsize_t TIFFWriteRawTile(TIFF * tif, uint32_t tile, void * data, tmsize_t cc)
*/
//<score> 5.3333335, nr_unique_branch: 0
//<Quality> {"density":16,"unique_branches":{},"library_calls":["TIFFStreamOpen","TIFFGetField","TIFFGetField","TIFFScanlineSize64","_TIFFmalloc","TIFFClose","TIFFReadEncodedStrip","TIFFOpen","_TIFFfree","TIFFClose","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFWriteRawTile","TIFFClose","_TIFFfree","TIFFClose"],"critical_calls":["TIFFStreamOpen","TIFFGetField","TIFFGetField","TIFFScanlineSize64","_TIFFmalloc","TIFFReadEncodedStrip","TIFFOpen","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFWriteRawTile","TIFFClose","_TIFFfree","TIFFClose"],"visited":2}
/*Here is the complete implementation of the fuzz driver:

*/


extern "C" int LLVMFuzzerTestOneInput_43(const uint8_t* f_data, size_t f_size) {
	if(f_size<22) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_4, fdp);
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    // Write data into input_file
    std::ofstream in_file("input_file", std::ios::binary);
    if (!in_file) {
        assert_file_name_closed("input_file");
	return 0;
    }
    in_file.write(reinterpret_cast<const char *>(data), size);
    in_file.close();

    // Open input TIFF in memory
    std::ifstream s("input_file", std::ios::binary);
    if (!s) {
        assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
    }
    TIFF *in_tif = TIFFStreamOpen(fuzz_str_1, &s);
    if (!in_tif) {
        s.close();
        assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
    }

    // Read input data
    uint32_t width, height;
    TIFFGetField(in_tif, TIFFTAG_IMAGEWIDTH, &width);
    TIFFGetField(in_tif, TIFFTAG_IMAGELENGTH, &height);
    tmsize_t bufsize = TIFFScanlineSize64(in_tif) * height;
    uint8_t *buf = static_cast<uint8_t *>(_TIFFmalloc(bufsize));
    if (!buf) {
        TIFFClose(in_tif);
        s.close();
        assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
    }
    TIFFReadEncodedStrip(in_tif, fuzz_uint32_t_2, buf, bufsize);
    
    // Process the input data using libtiff APIs
    // ...

    // Write output data into output_file
    TIFF *out_tif = TIFFOpen("output_file", fuzz_str_3);
    if (!out_tif) {
        _TIFFfree(buf);
        TIFFClose(in_tif);
        s.close();
        assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
    }
    TIFFSetField(out_tif, TIFFTAG_IMAGEWIDTH, width);
    TIFFSetField(out_tif, TIFFTAG_IMAGELENGTH, height);
    TIFFSetField(out_tif, TIFFTAG_SAMPLESPERPIXEL, 1);
    TIFFSetField(out_tif, TIFFTAG_BITSPERSAMPLE, 8);
    TIFFWriteRawTile(out_tif, fuzz_uint32_t_4, buf, bufsize);
    TIFFClose(out_tif);

    // Clean up
    _TIFFfree(buf);
    TIFFClose(in_tif);
    s.close();

    assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
}