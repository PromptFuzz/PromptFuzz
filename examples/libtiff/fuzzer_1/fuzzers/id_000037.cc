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
//<ID> 549
//<Prompt> ["TIFFSwabArrayOfTriples","TIFFDefaultStripSize","TIFFOpenOptionsAlloc","_TIFFrealloc","TIFFFieldWriteCount","TIFFScanlineSize64","TIFFGetWriteProc","TIFFFieldIsAnonymous"]
/*<Combination>: [void TIFFSwabArrayOfTriples(uint8_t * tp, tmsize_t n),
    uint32_t TIFFDefaultStripSize(TIFF * tif, uint32_t request),
    TIFFOpenOptions *TIFFOpenOptionsAlloc(),
    void *_TIFFrealloc(void * p, tmsize_t s),
    int TIFFFieldWriteCount(const TIFFField * ),
    uint64_t TIFFScanlineSize64(TIFF * tif),
    TIFFReadWriteProc TIFFGetWriteProc(TIFF * ),
    int TIFFFieldIsAnonymous(const TIFFField * )
*/
//<score> 8, nr_unique_branch: 2
//<Quality> {"density":8,"unique_branches":{"TIFFSwabArrayOfTriples":[[105,12,105,19,0,0,4,0],[105,12,105,19,0,0,4,1]]},"library_calls":["TIFFStreamOpen","TIFFSwabArrayOfTriples","TIFFDefaultStripSize","TIFFOpenOptionsAlloc","_TIFFrealloc","TIFFGetWriteProc","TIFFFieldWithTag","TIFFFieldIsAnonymous","TIFFFieldWriteCount","TIFFScanlineSize64","TIFFClose","TIFFOpenOptionsFree","_TIFFfree"],"critical_calls":["TIFFStreamOpen","TIFFSwabArrayOfTriples","TIFFDefaultStripSize","TIFFOpenOptionsAlloc","_TIFFrealloc","TIFFGetWriteProc","TIFFFieldWithTag","TIFFFieldIsAnonymous","TIFFFieldWriteCount","TIFFScanlineSize64","TIFFClose","TIFFOpenOptionsFree","_TIFFfree"],"visited":1}
/*Here is the completed program:

*/


extern "C" int LLVMFuzzerTestOneInput_37(const uint8_t* f_data, size_t f_size) {
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

    // Open input tiff in memory
    std::istringstream s(std::string(data, data + size));
    TIFF* in_tif = TIFFStreamOpen(fuzz_str_1, &s);
    if (!in_tif) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Swab array of triples
    uint8_t arr[9] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    TIFFSwabArrayOfTriples(arr, 3);

    // Get default strip size
    uint32_t stripSize = TIFFDefaultStripSize(in_tif, fuzz_uint32_t_2);

    // Allocate TIFF open options
    TIFFOpenOptions* opts = TIFFOpenOptionsAlloc();

    // Reallocate memory
    void* mem = _TIFFrealloc(NULL, size);

    // Get write proc
    TIFFReadWriteProc writeProc = TIFFGetWriteProc(in_tif);

    // Check if field is anonymous
    const TIFFField* field = TIFFFieldWithTag(in_tif, TIFFTAG_IMAGEWIDTH);
    int isAnonymous = TIFFFieldIsAnonymous(field);

    // Write count of field
    int count = TIFFFieldWriteCount(field);

    // Get scanline size
    uint64_t scanlineSize = TIFFScanlineSize64(in_tif);

    // Cleanup
    TIFFClose(in_tif);
    TIFFOpenOptionsFree(opts);
    _TIFFfree(mem);

    assert_file_closed(&in_file);
	return 0;
}