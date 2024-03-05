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
//<ID> 583
//<Prompt> ["TIFFStreamOpen","TIFFNumberOfDirectories","TIFFSetDirectory","TIFFErrorExtR","TIFFGetSeekProc","TIFFGetField","TIFFSwabLong","_TIFFmalloc","TIFFRasterScanlineSize64","TIFFReadScanline","TIFFUnlinkDirectory","_TIFFfree","TIFFClose"]
/*<Combination>: [TIFF *TIFFStreamOpen(const char * , std::ostream * ),
    tdir_t TIFFNumberOfDirectories(TIFF * ),
    int TIFFSetDirectory(TIFF * , tdir_t ),
    void TIFFErrorExtR(TIFF * , const char * , const char * ),
    TIFFSeekProc TIFFGetSeekProc(TIFF * ),
    int TIFFGetField(TIFF * tif, uint32_t tag),
    void TIFFSwabLong(uint32_t * ),
    void *_TIFFmalloc(tmsize_t s),
    uint64_t TIFFRasterScanlineSize64(TIFF * tif),
    int TIFFReadScanline(TIFF * tif, void * buf, uint32_t row, uint16_t sample),
    int TIFFUnlinkDirectory(TIFF * , tdir_t ),
    void _TIFFfree(void * p),
    void TIFFClose(TIFF * tif)
*/
//<score> 8.333333, nr_unique_branch: 0
//<Quality> {"density":25,"unique_branches":{},"library_calls":["TIFFStreamOpen","TIFFNumberOfDirectories","TIFFSetDirectory","TIFFErrorExtR","TIFFClose","TIFFGetSeekProc","TIFFGetField","TIFFErrorExtR","TIFFClose","TIFFSwabLong","TIFFRasterScanlineSize64","_TIFFmalloc","TIFFErrorExtR","TIFFClose","TIFFReadScanline","TIFFErrorExtR","_TIFFfree","TIFFClose","TIFFUnlinkDirectory","TIFFCurrentDirectory","TIFFErrorExtR","_TIFFfree","TIFFClose","_TIFFfree","TIFFClose"],"critical_calls":["TIFFStreamOpen","TIFFNumberOfDirectories","TIFFSetDirectory","TIFFGetSeekProc","TIFFGetField","TIFFSwabLong","TIFFRasterScanlineSize64","_TIFFmalloc","TIFFReadScanline","TIFFUnlinkDirectory","TIFFCurrentDirectory","TIFFErrorExtR","_TIFFfree","TIFFClose"],"visited":2}
/**/
extern "C" int LLVMFuzzerTestOneInput_42(const uint8_t* f_data, size_t f_size) {
	if(f_size<250) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_5, fuzz_str_sz_5, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_6, fuzz_str_sz_6, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_7, fuzz_str_sz_7, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_8, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_9, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_10, fuzz_str_sz_10, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_11, fuzz_str_sz_11, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_12, fuzz_str_sz_12, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_13, fuzz_str_sz_13, fdp);
	//fuzzer shim end}




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
    TIFF* in_tif = TIFFStreamOpen(fuzz_str_1, &s);
    if (!in_tif) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Get the number of directories in the TIFF file
    tdir_t num_directories = TIFFNumberOfDirectories(in_tif);

    // Set the first directory as the current directory
    if (!TIFFSetDirectory(in_tif, fuzz_uint32_t_2)) {
        TIFFErrorExtR(in_tif, fuzz_str_3, "Failed to set directory");
        TIFFClose(in_tif);
        assert_file_closed(&in_file);
	return 0;
    }

    // Get the seek function for the TIFF file
    TIFFSeekProc seek_proc = TIFFGetSeekProc(in_tif);

    // Get a field from the current directory
    uint32_t tag = TIFFTAG_IMAGEWIDTH;
    uint32_t value = 0;
    if (!TIFFGetField(in_tif, tag, &value)) {
        TIFFErrorExtR(in_tif, fuzz_str_4, fuzz_str_5);
        TIFFClose(in_tif);
        assert_file_closed(&in_file);
	return 0;
    }

    // Swab the value
    TIFFSwabLong(&value);

    // Allocate memory for a scanline
    tmsize_t scanline_size = TIFFRasterScanlineSize64(in_tif);
    void* scanline_buffer = _TIFFmalloc(scanline_size);
    if (!scanline_buffer) {
        TIFFErrorExtR(in_tif, fuzz_str_6, fuzz_str_7);
        TIFFClose(in_tif);
        assert_file_closed(&in_file);
	return 0;
    }

    // Read a scanline from the TIFF file
    uint32_t row = 0;
    uint16_t sample = 0;
    if (TIFFReadScanline(in_tif, scanline_buffer, fuzz_uint32_t_8, fuzz_uint16_t_9) == -1) {
        TIFFErrorExtR(in_tif, fuzz_str_10, fuzz_str_11);
        _TIFFfree(scanline_buffer);
        TIFFClose(in_tif);
        assert_file_closed(&in_file);
	return 0;
    }

    // Unlink the current directory
    if (!TIFFUnlinkDirectory(in_tif, TIFFCurrentDirectory(in_tif))) {
        TIFFErrorExtR(in_tif, fuzz_str_12, fuzz_str_13);
        _TIFFfree(scanline_buffer);
        TIFFClose(in_tif);
        assert_file_closed(&in_file);
	return 0;
    }

    // Free memory and close the TIFF file
    _TIFFfree(scanline_buffer);
    TIFFClose(in_tif);

    assert_file_closed(&in_file);
	return 0;
}