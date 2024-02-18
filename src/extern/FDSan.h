#ifndef LLM_FUZZ_EXTERN_FDSAN_H_
#define LLM_FUZZ_EXTERN_FDSAN_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <stdint.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <libgen.h>
#include <sys/mman.h>
#include <string.h>
#include <sys/resource.h>

#if defined(__clang__) || defined (__GNUC__)
# define ATTRIBUTE_NO_SANITIZE_ADDRESS __attribute__((no_sanitize_address))
#else
# define ATTRIBUTE_NO_SANITIZE_ADDRESS
#endif

#ifndef _LIBIO_H
#define _IO_MAGIC         0xFBAD0000 /* Magic number */
#define _IO_MAGIC_MASK    0xFFFF0000
#endif


extern "C" {
static int fuzz_fd_count = 0;

#define FUZZ_FILE_INIT() \
	std::vector<int> fuzz_fd_vector; \
    std::vector<FILE **> fuzz_file_vector; \
    std::vector<const char*> fuzz_filename_vector; 

#define FUZZ_FD_PUSH(fd) \
	fuzz_fd_vector.push_back(fd);

#define FUZZ_FILE_PUSH(file) \
	fuzz_file_vector.push_back(&file);

#define FUZZ_FILENAME_PUSH(filename) \
	fuzz_filename_vector.push_back(filename);

#define FUZZ_FILE_CLOSE() \
	for(int fd : fuzz_fd_vector) { \
		assert_fd_closed(fd); \
	} \
	for(FILE **file :fuzz_file_vector) { \
		assert_file_closed(file); \
	} \
	for(const char* filename: fuzz_filename_vector) { \
		assert_file_name_closed(filename); \
	}


// Instread of using abort() directly, i trigger a segment fault which is able to report ASAN's stack trace.
static void fd_err_abort() {
	struct rlimit rlim;
	if (getrlimit(RLIMIT_NOFILE, &rlim) != 0) {
		perror("Error getting file descriptor limit");
		fd_err_abort();
	}
	// increase the FILE limit, let ASAN open sockets to report
	rlim.rlim_cur += 5;
	if (setrlimit(RLIMIT_NOFILE, &rlim) != 0) {
		perror("Error getting file descriptor limit");
		fd_err_abort();
	}
	int *ptr = NULL;
	*(ptr + 1) = 0;
}

static int fuzz_fileno(FILE* file) {
	if (file == NULL || file == nullptr) 
		return -1;
	// this file is obtained by fmemopen
	int fd = fileno(file);
	if (fd != -1)
		return fd;
	
	 // Get the size of the file
    fseek(file, 0, SEEK_END);
    size_t size = ftell(file);
    rewind(file);

	// Read the file into a buffer
    void *buffer = malloc(size);
	if (buffer == NULL) {
		return -1;
	}

    if (fread(buffer, 1, size, file) != size) {
        perror("fread");
		free(buffer);
        return -1;
    }
	rewind(file);

	char fuzz_name[30];
	if (fuzz_fd_count >= INT32_MAX)
		fuzz_fd_count = 0;
	sprintf(fuzz_name, "fuzz_memory_file_%d", fuzz_fd_count++);

	// Create an anonymous file in memory
    fd = memfd_create(fuzz_name, MFD_CLOEXEC);
    if (fd == -1) {
        perror("memfd_create: Found a file descriptor leak.");
		fd_err_abort();
        return -1;
    }

	    // Set the size of the memory file
    if (ftruncate(fd, size) == -1) {
        perror("ftruncate");
        close(fd);
		free(buffer);
        return -1;
    }

	    // Write the buffer to the memory file
    if (write(fd, buffer, size) != size) {
        perror("write");
        close(fd);
		free(buffer);
        return -1;
    }
	free(buffer);
	return fd;
}

static int is_fd_closed(int fd) {
    int flags = fcntl(fd, F_GETFD);
    return (flags == -1) ? 1 : 0;
}

static void assert_fd_closed(int fd) {
	if (fd == -1)
		return;
    if (is_fd_closed(fd) != 1) {
        //abort();
		close(fd);
    }
}

ATTRIBUTE_NO_SANITIZE_ADDRESS
static void assert_file_closed(FILE **file) {
	if (file == NULL || file == nullptr || *file == nullptr || *file == NULL)
		return;
	if(((*file)->_flags & _IO_MAGIC_MASK) != _IO_MAGIC) {
		// already freed
		return;
	}
	fclose(*file);
	*file = NULL;
}

static int is_file_name_closed(const char* file_name) {
	DIR *dir = opendir("/proc/self/fd");
	if (!dir) {
		return 0;
	} 
	struct dirent *entry;
	while ((entry = readdir(dir)) != NULL) {
		// DT_LNK = 10
		if (entry->d_type == 10) {
			char procfile[256];
    		snprintf(procfile, sizeof(procfile), "/proc/self/fd/%s", entry->d_name);
			char actual_path[255 + 1];

			ssize_t len = readlink(procfile, actual_path, 255);
			if (len == -1) {
				perror("readlink");
				continue;
			}
			actual_path[len] = '\0';
			char *base = basename(actual_path);
			printf("%s\n", base);
			if (strcmp(file_name, base) == 0) {
				return 0;
			}
		}
    }
	closedir(dir);
    return 1;
}

static void assert_file_name_closed(const char* file_name) {
    if (is_file_name_closed(file_name) != 1) 
        fd_err_abort();
}
}

#endif // LLM_FUZZ_EXTERN_FDSAN_H_
