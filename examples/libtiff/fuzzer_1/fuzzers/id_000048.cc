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
//<ID> 761
//<Prompt> ["TIFFStreamOpen","TIFFGetWriteProc","TIFFGetField","TIFFScanlineSize64","_TIFFmalloc","TIFFSetupStrips","TIFFOpen","TIFFClose","TIFFCurrentRow","_TIFFfree"]
/*<Combination>: [TIFF *TIFFStreamOpen(const char * , std::ostream * ),
    TIFFReadWriteProc TIFFGetWriteProc(TIFF * ),
    int TIFFGetField(TIFF * tif, uint32_t tag),
    uint64_t TIFFScanlineSize64(TIFF * tif),
    void *_TIFFmalloc(tmsize_t s),
    int TIFFSetupStrips(TIFF * ),
    TIFF *TIFFOpen(const char * filename, const char * mode),
    void TIFFClose(TIFF * tif),
    uint32_t TIFFCurrentRow(TIFF * ),
    void _TIFFfree(void * p)
*/
//<score> 9, nr_unique_branch: 1
//<Quality> {"density":9,"unique_branches":{"TIFFReadEncodedStripGetStripSize":[[487,9,487,32,0,0,4,0]]},"library_calls":["TIFFStreamOpen","TIFFCurrentRow","TIFFGetWriteProc","TIFFScanlineSize64","_TIFFmalloc","TIFFReadEncodedStrip","TIFFComputeStrip","TIFFClose","_TIFFfree"],"critical_calls":["TIFFStreamOpen","TIFFCurrentRow","TIFFGetWriteProc","TIFFScanlineSize64","_TIFFmalloc","TIFFReadEncodedStrip","TIFFComputeStrip","TIFFClose","_TIFFfree"],"visited":1}
/**/
extern "C" int LLVMFuzzerTestOneInput_48(const uint8_t* f_data, size_t f_size) {
	if(f_size<12) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_2, fdp);
	//fuzzer shim end}




  // Write data into input_file.
  FILE* in_file = fopen("input_file", "wb");
  if (in_file == NULL) {
    assert_file_closed(&in_file);
	return 0;
  }
  fwrite(data, sizeof(uint8_t), size, in_file);
  assert_file_closed(&in_file);;

  // Open input TIFF in memory
  std::istringstream s(std::string(data, data + size));
  TIFF* in_tif = TIFFStreamOpen(fuzz_str_1, &s);
  if (!in_tif) {
    assert_file_closed(&in_file);
	return 0;
  }

  // Get file descriptor for reading
  int in_fd = fuzz_fileno(in_file);

  // Get current row
  uint32_t current_row = TIFFCurrentRow(in_tif);

  // Get write proc
  TIFFReadWriteProc write_proc = TIFFGetWriteProc(in_tif);

  // Get scanline size
  uint64_t scanline_size = TIFFScanlineSize64(in_tif);

  // Allocate memory
  void* buffer = _TIFFmalloc(scanline_size);

  // Read data from file
  TIFFReadEncodedStrip(in_tif, TIFFComputeStrip(in_tif, current_row, fuzz_uint16_t_2), buffer, scanline_size);

  // Process the data
  // ...

  // Close input TIFF
  TIFFClose(in_tif);

  // Free memory
  _TIFFfree(buffer);

  // Release all allocated resources
  assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
}