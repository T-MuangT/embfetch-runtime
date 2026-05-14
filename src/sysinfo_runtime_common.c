#include <stdio.h>
#include "runtime_sysinfo.h"

// This file provides common output wrapper functions for embfetch-runtime.
// The actual fetching and sysinfo_print() are implemented in language-family
// common files (e.g. sysinfo_python_common.c) and runtime-specific files
// (e.g. sysinfo_cpython.c).

static void file_putline(void *ctx, const char *line) {
    fprintf((FILE *)ctx, "%s\n", line);
}

void sysinfo_print_file(FILE *f) {
    sysinfo_print(file_putline, f);
}

static void puts_putline(void *ctx, const char *line) {
    ((void (*)(const char *))ctx)(line);
}

void sysinfo_print_puts(void (*puts_fn)(const char *)) {
    sysinfo_print(puts_putline, (void *)puts_fn);
} // sysinfo_runtime_common.c