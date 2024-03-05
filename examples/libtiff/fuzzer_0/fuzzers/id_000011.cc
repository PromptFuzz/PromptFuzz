#include "FDSan.h"
#include <tiffio.hxx>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 130
//<Prompt> ["TIFFVStripSize64","TIFFFieldSetGetCountSize","TIFFVGetField","TIFFSetSubDirectory","LogL10fromY"]
/*<Combination>: [uint64_t TIFFVStripSize64(TIFF * tif, uint32_t nrows),
    int TIFFFieldSetGetCountSize(const TIFFField * ),
    int TIFFVGetField(TIFF * tif, uint32_t tag, __va_list_tag * ap),
    int TIFFSetSubDirectory(TIFF * , uint64_t ),
    int LogL10fromY(double , int )
*/
//<score> 84, nr_unique_branch: 88
//<Quality> {"density":6,"unique_branches":{"_TIFFCheckDirNumberAndOffset":[[5433,9,5433,50,0,0,4,1],[5445,9,5445,50,0,0,4,1],[5471,9,5471,19,0,0,4,0],[5473,13,5473,42,0,0,4,0],[5473,13,5473,42,0,0,4,1],[5492,9,5492,19,0,0,4,0],[5494,13,5494,41,0,0,4,0],[5505,17,5505,30,0,0,4,0],[5512,17,5512,30,0,0,4,0],[5520,17,5520,33,0,0,4,1],[5528,17,5528,80,0,0,4,1],[5535,17,5535,80,0,0,4,1]],"TIFFNumberOfDirectories":[[2010,9,2010,41,0,0,4,0],[2016,12,2016,27,0,0,4,0],[2016,12,2016,27,0,0,4,1],[2017,12,2017,69,0,0,4,0],[2017,12,2017,69,0,0,4,1]],"_TIFFSetupFields":[[524,55,524,80,0,0,4,0]],"TIFFHashSetGetNewListElt":[[192,9,192,29,0,0,4,0]],"TIFFHashSetRemoveInternal":[[520,9,520,38,0,0,4,1],[538,12,538,15,0,0,4,0],[540,13,540,46,0,0,4,0],[540,13,540,46,0,0,4,1],[542,17,542,21,0,0,4,0],[542,17,542,21,0,0,4,1],[547,17,547,35,0,0,4,0],[547,17,547,35,0,0,4,1]],"TIFFHashSetReturnListElt":[[210,9,210,38,0,0,4,0]],"LogL10fromY":[[881,9,881,20,0,0,4,1],[883,14,883,28,0,0,4,1]],"TIFFSetSubDirectory":[[2126,9,2126,20,0,0,4,1],[2133,13,2133,63,0,0,4,0],[2140,13,2140,24,0,0,4,0],[2147,9,2147,16,0,0,4,0],[2147,20,2147,31,0,0,4,0],[2149,13,2149,60,0,0,4,0],[2149,13,2149,60,0,0,4,1],[2154,9,2154,15,0,0,4,1]],"TIFFHashSetLookup":[[506,9,506,13,0,0,4,0]],"TIFFAdvanceDirectory":[[1827,9,1827,69,0,0,4,1],[1922,13,1922,45,0,0,4,0],[1926,17,1926,24,0,0,4,0],[1926,17,1926,24,0,0,4,1],[1927,17,1927,24,0,0,4,0],[1927,17,1927,24,0,0,4,1],[1933,17,1933,43,0,0,4,1],[1935,17,1935,28,0,0,4,1],[1939,17,1939,24,0,0,4,0],[1939,17,1939,24,0,0,4,1],[1945,17,1945,43,0,0,4,1],[1982,9,1982,25,0,0,4,0],[1982,9,1982,25,0,0,4,1],[1987,13,1987,73,0,0,4,0],[1987,13,1987,73,0,0,4,1],[251,23,251,62,1,0,4,1]],"_TIFFGetDirNumberFromOffset":[[5593,9,5593,20,0,0,4,1],[5600,9,5600,50,0,0,4,1],[5609,9,5609,19,0,0,4,1],[5620,9,5620,19,0,0,4,1]],"TIFFHashSetFindPtr":[[413,12,413,15,0,0,4,0],[415,13,415,46,0,0,4,0],[415,13,415,46,0,0,4,1]],"TIFFFieldSetGetCountSize":[[758,9,758,20,0,0,4,1],[763,9,763,35,0,0,4,1],[764,9,764,35,0,0,4,1],[765,9,765,35,0,0,4,1],[766,9,766,36,0,0,4,1],[767,9,767,36,0,0,4,1],[768,9,768,36,0,0,4,1],[769,9,769,36,0,0,4,1],[770,9,770,35,0,0,4,1],[771,9,771,36,0,0,4,1],[772,9,772,36,0,0,4,1],[773,9,773,36,0,0,4,1],[774,9,774,34,0,0,4,1],[776,9,776,35,0,0,4,1],[777,9,777,35,0,0,4,1],[778,9,778,35,0,0,4,1],[779,9,779,36,0,0,4,1],[780,9,780,36,0,0,4,1],[781,9,781,36,0,0,4,1],[782,9,782,36,0,0,4,1],[783,9,783,35,0,0,4,1],[784,9,784,36,0,0,4,1],[785,9,785,36,0,0,4,1],[786,9,786,36,0,0,4,1],[787,9,787,34,0,0,4,1],[789,9,789,16,0,0,4,0]]},"library_calls":["TIFFStreamOpen","TIFFVStripSize64","TIFFFindField","TIFFFieldSetGetCountSize","TIFFSetSubDirectory","LogL10fromY","TIFFClose"],"critical_calls":["TIFFStreamOpen","TIFFVStripSize64","TIFFFindField","TIFFFieldSetGetCountSize","TIFFSetSubDirectory","LogL10fromY","TIFFClose"],"visited":0}
/*The completion of the fuzz driver code is provided below:

*/

extern "C" int LLVMFuzzerTestOneInput_11(const uint8_t* data, size_t size) {
	if(size<0) return 0;

	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);

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
    TIFF* in_tif = TIFFStreamOpen("MemTIFF", &s);
    if (!in_tif) {
        assert_file_closed(&in_file);
	return 0;
    }

    uint32_t nrows = 10;
    uint64_t strip_size = TIFFVStripSize64(in_tif, nrows);
    // Process strip_size further if needed

    const TIFFField* field = TIFFFindField(in_tif, TIFFTAG_ROWSPERSTRIP, TIFF_ANY);
    int count_size = TIFFFieldSetGetCountSize(field);
    // Process count_size further if needed

    int tag = TIFFTAG_SUBFILETYPE;
    uint64_t sub_dir_offset = 123;
    int set_sub_dir = TIFFSetSubDirectory(in_tif, sub_dir_offset);
    // Process set_sub_dir further if needed

    double y = 1.23;
    int l_f = 4;
    int log_val = LogL10fromY(y, l_f);
    // Process log_val further if needed

    TIFFClose(in_tif);

    assert_file_closed(&in_file);
	return 0;
}