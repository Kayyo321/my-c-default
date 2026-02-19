#include "logger.h"

#include <stdarg.h>
#include <stdio.h>
#include <time.h>

static boolean did_error = False;

static void print_timestamp(FILE *stream) {
	time_t now = time(null());
	struct tm tm_now = {0};
	char buffer[32] = {0};

	if (localtime_r(&now, &tm_now) != null()) {
		strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &tm_now);
		fprintf(stream, "[%s] ", buffer);
		return;
	}

	fprintf(stream, "[time-error] ");
}

static void vlog_with_timestamp(FILE *stream, const char *format, va_list args) {
	print_timestamp(stream);
	vfprintf(stream, format, args);
	fputc('\n', stream);
	fflush(stream);
}

void log(const char *format, ...) {
	va_list args;
	va_start(args, format);
	vlog_with_timestamp(stdout, format, args);
	va_end(args);
}

void error(const char *format, ...) {
	va_list args;
	did_error = True;
	va_start(args, format);
	vlog_with_timestamp(stderr, format, args);
	va_end(args);
}

boolean had_error(void) {
	return did_error;
}
