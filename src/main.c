#include "allocator.h"

#include "logger.h"

#include <string.h>

enum {
	ProgramMaxAllocatedBytes = 4096,
};

void program(void) {
	//...
}

int main(int argc, char **argv) {
	init_vdisk(ProgramMaxAllocatedBytes);

	program_arguments.argc = argc;
	program_arguments.argv = argv;

	program();

	free_vdisk();

	return had_error(); 
}
