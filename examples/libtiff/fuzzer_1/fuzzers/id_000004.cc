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
//<ID> 31
//<Prompt> ["_TIFFmemset","TIFFCurrentStrip","TIFFUnlinkDirectory","TIFFFieldPassCount","TIFFWriteTile","TIFFGetSizeProc"]
/*<Combination>: [void _TIFFmemset(void * p, int v, tmsize_t c),
    uint32_t TIFFCurrentStrip(TIFF * ),
    int TIFFUnlinkDirectory(TIFF * , tdir_t ),
    int TIFFFieldPassCount(const TIFFField * ),
    tmsize_t TIFFWriteTile(TIFF * tif, void * buf, uint32_t x, uint32_t y, uint32_t z, uint16_t s),
    TIFFSizeProc TIFFGetSizeProc(TIFF * )
*/
//<score> 22.4, nr_unique_branch: 13
//<Quality> {"density":16,"unique_branches":{"_tiffSizeProc":[[144,9,144,39,0,0,4,0]],"TIFFCheckTile":[[75,9,75,31,0,0,4,0]],"TIFFWriteTile":[[390,9,390,40,0,0,4,0]],"_TIFFmalloc":[[323,9,323,15,0,0,4,0]],"TIFFUnlinkDirectory":[[2193,9,2193,34,0,0,4,0]],"TIFFReadRawTile":[[1153,9,1153,31,0,0,4,0],[1153,9,1153,31,0,0,4,1],[1155,9,1155,31,0,0,4,1],[1162,9,1162,40,0,0,4,0],[1162,9,1162,40,0,0,4,1],[1170,9,1170,31,0,0,4,0],[1170,35,1170,64,0,0,4,0],[1174,9,1174,24,0,0,4,0]]},"library_calls":["TIFFStreamOpen","TIFFCurrentStrip","TIFFUnlinkDirectory","TIFFFindField","TIFFFieldPassCount","_TIFFmemset","TIFFSetField","TIFFOpen","TIFFClose","TIFFGetSizeProc","_TIFFmalloc","TIFFReadRawTile","TIFFWriteTile","_TIFFfree","TIFFClose","TIFFClose"],"critical_calls":["TIFFStreamOpen","TIFFCurrentStrip","TIFFUnlinkDirectory","TIFFFindField","TIFFFieldPassCount","_TIFFmemset","TIFFSetField","TIFFOpen","TIFFGetSizeProc","_TIFFmalloc","TIFFReadRawTile","TIFFWriteTile","_TIFFfree","TIFFClose","TIFFClose"],"visited":4}
/**/
extern "C" int LLVMFuzzerTestOneInput_4(const uint8_t* f_data, size_t f_size) {
	if(f_size<36) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_4, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_5, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_6, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_7, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_8, fdp);
	//fuzzer shim end}




    // Write data into input_file.
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

    // Call APIs to achieve the desired event
    uint32_t current_strip = TIFFCurrentStrip(in_tif);
    TIFFUnlinkDirectory(in_tif, current_strip);
    const TIFFField* field = TIFFFindField(in_tif, fuzz_uint32_t_2, TIFF_ANY);
    if (field) {
        int pass_count = TIFFFieldPassCount(field);
        // Assuming the field is an ASCII string
        char* str_value = new char[pass_count + 1];
        _TIFFmemset(str_value, 'A', pass_count);
        // Set the value of the field
        TIFFSetField(in_tif, TIFFTAG_PLANARCONFIG, str_value);
        delete[] str_value;
    }

    // Create an output tiff file
    TIFF* out_tif = TIFFOpen("output_file", fuzz_str_3);
    if (!out_tif) {
        TIFFClose(in_tif);
        assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
    }

    // Set the output tiff size proc
    TIFFSizeProc size_proc = TIFFGetSizeProc(out_tif);

    // Get the size of the input tiff
    tmsize_t input_tiff_size = size_proc(in_tif);

    // Allocate a buffer for the input tiff
    void* input_tiff_buffer = _TIFFmalloc(input_tiff_size);

    // Read the input tiff into the buffer
    tmsize_t read_size = TIFFReadRawTile(in_tif, fuzz_uint32_t_4, input_tiff_buffer, input_tiff_size);

    // Write the input tiff buffer to the output tiff
    TIFFWriteTile(out_tif, input_tiff_buffer, fuzz_uint32_t_5, fuzz_uint32_t_6, fuzz_uint32_t_7, fuzz_uint16_t_8);

    // Clean up resources
    _TIFFfree(input_tiff_buffer);
    TIFFClose(out_tif);
    TIFFClose(in_tif);

    assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
}