#include<stdint.h>
#include<unistd.h>
#include<stdio.h>
#include<fcntl.h>

extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    int fd = open("/dev/null", O_RDONLY);
    if (fd == -1) {
        return -1;
    }
    if (size == 1)
        close(fd);
    close(fd);
    return 0;
}