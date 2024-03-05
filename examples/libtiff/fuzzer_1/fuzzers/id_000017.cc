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
//<ID> 344
//<Prompt> ["TIFFVTileSize","TIFFStreamOpen","TIFFCurrentStrip","TIFFUnlinkDirectory","TIFFFindField","TIFFFieldPassCount","_TIFFmalloc","TIFFFieldWithTag","TIFFYCbCrtoRGB","_TIFFmemset","TIFFStripSize","TIFFReadRawTile","TIFFWriteTile","_TIFFfree","TIFFClose"]
/*<Combination>: [tmsize_t TIFFVTileSize(TIFF * tif, uint32_t nrows),
    TIFF *TIFFStreamOpen(const char * , std::ostream * ),
    uint32_t TIFFCurrentStrip(TIFF * ),
    int TIFFUnlinkDirectory(TIFF * , tdir_t ),
    const TIFFField *TIFFFindField(TIFF * , uint32_t , TIFFDataType ),
    int TIFFFieldPassCount(const TIFFField * ),
    void *_TIFFmalloc(tmsize_t s),
    const TIFFField *TIFFFieldWithTag(TIFF * , uint32_t ),
    void TIFFYCbCrtoRGB(TIFFYCbCrToRGB * , uint32_t , int32_t , int32_t , uint32_t * , uint32_t * , uint32_t * ),
    void _TIFFmemset(void * p, int v, tmsize_t c),
    tmsize_t TIFFStripSize(TIFF * tif),
    tmsize_t TIFFReadRawTile(TIFF * tif, uint32_t tile, void * buf, tmsize_t size),
    tmsize_t TIFFWriteTile(TIFF * tif, void * buf, uint32_t x, uint32_t y, uint32_t z, uint16_t s),
    void _TIFFfree(void * p),
    void TIFFClose(TIFF * tif)
*/
//<score> 18, nr_unique_branch: 9
//<Quality> {"density":9,"unique_branches":{"_TIFFGetOffsetFromDirNumber":[[5639,9,5639,50,0,0,4,1],[5648,9,5648,19,0,0,4,0]],"TIFFSetDirectory":[[2034,9,2034,45,0,0,4,1],[2044,9,2044,46,0,0,4,0],[2045,9,2045,60,0,0,4,0],[2099,9,2099,29,0,0,4,0],[2099,9,2099,29,0,0,4,1]],"TIFFReadDirectory":[[4129,39,4130,75,0,0,4,1],[4149,9,4149,56,0,0,4,1]]},"library_calls":["TIFFStreamOpen","TIFFNumberOfDirectories","TIFFSetDirectory","TIFFCurrentStrip","TIFFStripSize","_TIFFmalloc","TIFFReadRawTile","_TIFFfree","TIFFClose"],"critical_calls":["TIFFStreamOpen","TIFFNumberOfDirectories","TIFFSetDirectory","TIFFCurrentStrip","TIFFStripSize","_TIFFmalloc","TIFFReadRawTile","_TIFFfree","TIFFClose"],"visited":1}
/*Here is the complete LLVMFuzzerTestOneInput_17 function that uses the libtiff library APIs to achieve the event:

*/


extern "C" int LLVMFuzzerTestOneInput_17(const uint8_t* f_data, size_t f_size) {
	if(f_size<14) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
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

    // Get the number of directories in the TIFF file
    tdir_t num_directories = TIFFNumberOfDirectories(in_tif);

    // Iterate over each directory
    for (tdir_t dir_num = 0; dir_num < num_directories; dir_num++) {
        // Set the current directory
        if (!TIFFSetDirectory(in_tif, fuzz_uint32_t_2)) {
            continue;
        }

        // Get the current strip
        uint32_t current_strip = TIFFCurrentStrip(in_tif);

        // Get the strip size
        tmsize_t strip_size = TIFFStripSize(in_tif);

        // Allocate memory for reading the raw tile
        void* raw_tile_buf = _TIFFmalloc(strip_size);

        // Read the raw tile
        tmsize_t read_size = TIFFReadRawTile(in_tif, current_strip, raw_tile_buf, strip_size);

        if (read_size > 0) {
            // Write the raw tile to output file
            FILE* out_file = fopen("output_file", "ab");
            if (out_file) {
                fwrite(raw_tile_buf, sizeof(uint8_t), read_size, out_file);
                assert_file_closed(&out_file);;
            }
        }

        // Free the allocated memory for raw tile
        _TIFFfree(raw_tile_buf);
    }

    // Close the TIFF file
    TIFFClose(in_tif);

    assert_file_closed(&in_file);
	return 0;
}