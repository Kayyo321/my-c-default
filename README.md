# my-c-default

Starter C project template with:
- Recursive `Makefile` build.
- Centralized diagnostics/logging utilities.
- Simple heap allocation tracking with leak scanning on exit.

## Project structure

```text
.
├── Makefile
├── src/
│   ├── main.c
│   └── core/
│       ├── common.c
│       └── common.h
├── obj/         # build artifacts
├── bin/         # executable output
└── logs/        # rotating log files
```

## Build

```sh
make
```

This compiles all `src/**/*.c` files into `obj/**/*.o` and links `bin/program`.

## Run

```sh
./bin/program
```

Example output includes diagnostic counters and exit code, and logs are written to `logs/`.

## Clean

```sh
make clean
```

Removes `obj/` and `bin/`.

## Core utilities (`src/core/common.*`)

`common.h` and `common.c` provide shared project utilities:

- Program metadata via global `program` (`title`, `argc`, `argv`).
- Logging API:
	- `open_logger()` / `close_logger()`
	- `log_msg(...)`, `log_warn(...)`, `log_err(...)`
	- `get_warn_count()`, `get_error_count()`
- Heap helpers:
	- `allocate(count, bytes)`
	- `reallocate(heap, new_size)`
	- `deallocate(heap)`
	- `scan_and_deallocate()`
- Process shutdown helper:
	- `quit(result)` prints diagnostics, scans for leaks, and exits.

## Logging behavior

- Current run writes to `logs/log0-last.log`.
- Older logs are rotated to numbered files like `logs/log1-<timestamp>.log`.
- Messages are mirrored to both log file and `stderr`.

## Notes

- The codebase intentionally discourages direct `malloc`/`realloc`/`free` in most translation units via macros in `common.h`.
- `Version` is currently defined in `src/core/common.h` as `"0.1.0"`.
