#ifndef LOGGER_H
#define LOGGER_H

#include "common.h"

#if defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wbuiltin-declaration-mismatch"
#endif

void log(const char *format, ...);
void error(const char *format, ...);

boolean had_error(void);

#if defined(__GNUC__)
#pragma GCC diagnostic pop
#endif

#endif
