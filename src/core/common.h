#ifndef COMMON_H
#define COMMON_H

#include <stdint.h>
#include <stddef.h>

enum {
	True = 1,
	False = 0,
};

typedef struct {
	char **argv;
	int argc;
} program_arguments_t;

extern program_arguments_t program_arguments;

typedef uint8_t boolean;
typedef size_t usize;

void *null(void);

#endif
