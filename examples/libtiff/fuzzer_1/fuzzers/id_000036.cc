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
//<ID> 542
//<Prompt> ["TIFFStreamOpen","TIFFNumberOfStrips","TIFFReadBufferSetup","TIFFStripSize","_TIFFmalloc","TIFFReadRawStrip","TIFFOpen","TIFFCurrentDirOffset","TIFFWriteRawStrip","TIFFClientOpen","TIFFClose","_TIFFfree"]
/*<Combination>: [TIFF *TIFFStreamOpen(const char * , std::ostream * ),
    uint32_t TIFFNumberOfStrips(TIFF * ),
    int TIFFReadBufferSetup(TIFF * tif, void * bp, tmsize_t size),
    tmsize_t TIFFStripSize(TIFF * tif),
    void *_TIFFmalloc(tmsize_t s),
    tmsize_t TIFFReadRawStrip(TIFF * tif, uint32_t strip, void * buf, tmsize_t size),
    TIFF *TIFFOpen(const char * filename, const char * mode),
    uint64_t TIFFCurrentDirOffset(TIFF * ),
    tmsize_t TIFFWriteRawStrip(TIFF * tif, uint32_t strip, void * data, tmsize_t cc),
    TIFF *TIFFClientOpen(const char * , const char * , thandle_t , TIFFReadWriteProc , TIFFReadWriteProc , TIFFSeekProc , TIFFCloseProc , TIFFSizeProc , TIFFMapFileProc , TIFFUnmapFileProc ),
    void TIFFClose(TIFF * tif),
    void _TIFFfree(void * p)
*/
//<score> 3.6666667, nr_unique_branch: 0
//<Quality> {"density":11,"unique_branches":{},"library_calls":["TIFFStreamOpen","TIFFNumberOfStrips","TIFFStripSize","_TIFFmalloc","TIFFClose","TIFFReadRawStrip","_TIFFfree","TIFFClose","TIFFWriteRawStrip","_TIFFfree","TIFFClose"],"critical_calls":["TIFFStreamOpen","TIFFNumberOfStrips","TIFFStripSize","_TIFFmalloc","TIFFReadRawStrip","TIFFWriteRawStrip","_TIFFfree","TIFFClose"],"visited":2}
/**/
extern "C" int LLVMFuzzerTestOneInput_36(const uint8_t* f_data, size_t f_size) {
	if(f_size<18) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
	//fuzzer shim end}




  // write data into input_file.
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

  // get the number of strips in the input tiff
  uint32_t num_strips = TIFFNumberOfStrips(in_tif);

  // allocate memory for reading strip data
  tmsize_t strip_size = TIFFStripSize(in_tif);
  void* strip_data = _TIFFmalloc(strip_size);
  if (!strip_data) {
    TIFFClose(in_tif);
    assert_file_closed(&in_file);
	return 0;
  }

  // read and write each strip
  for (uint32_t strip = 0; strip < num_strips; strip++) {
    // read strip data from input tiff
    tmsize_t read_size = TIFFReadRawStrip(in_tif, fuzz_uint32_t_2, strip_data, strip_size);
    if (read_size < 0) {
      _TIFFfree(strip_data);
      TIFFClose(in_tif);
      assert_file_closed(&in_file);
	return 0;
    }

    // write strip data to output tiff
    TIFFWriteRawStrip(in_tif, fuzz_uint32_t_3, strip_data, read_size);
  }

  // free memory and close tiffs
  _TIFFfree(strip_data);
  TIFFClose(in_tif);

  assert_file_closed(&in_file);
	return 0;
}