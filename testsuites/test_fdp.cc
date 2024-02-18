#include "FuzzedDataProvider.h"
#include <stdint.h>
#include <stdlib.h>

int main() {
    const uint8_t f_data[] = {0xCA, 0xFE, 0xBA};
    FuzzedDataProvider fdp(f_data, 3);
	FDPConsumeRawBytes(const uint8_t *, data, data_sz, fdp);
    if (data != NULL)
        abort();
    FDPConsumeCharArray(fuzz_char_array, fuzz_char_array_sz, fdp);
    if (fuzz_char_array != NULL)
        abort();
    FDPConsumeDoublingArray(fuzz_double_array, fuzz_double_array_sz, fdp);
    if (fuzz_double_array != NULL)
        abort();
    FDPConsumeFloatingArray(fuzz_float_array, fuzz_float_array_sz, fdp);
    if (fuzz_float_array != NULL)
        abort();
    FDPConsumeIntegralArray(int, fuzz_int_array, fuzz_init_array_sz, fdp);
    if (fuzz_int_array != NULL)
        abort();
    FDPConsumeRandomLengthString(fuzz_str, fdp);
    if (fuzz_str != NULL)
        abort();
    return 0;
}