#include "core/common.h"

int main(int argc, char **argv) {
	program = (Program) {
		.title = "Title",
		.argc = (usize)argc,
		.argv = argv,
	};

	if (open_logger() != Ok)
		quit(Err);

	log_msg("Hello, world!");

	if (close_logger() != Ok)
		quit(Err);

	quit(Ok);
}