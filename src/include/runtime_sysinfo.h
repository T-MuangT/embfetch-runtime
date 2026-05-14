#ifndef RUNTIME_SYSINFO_H
#define RUNTIME_SYSINFO_H

#include <stdint.h>
#include <stddef.h>
#include <stdio.h>

typedef void (*sysinfo_putline_fn)(void *ctx, const char *line);

// Static runtime info — implementation name, version, environment
typedef struct {
    const char *implementation;   // "CPython", "MicroPython", "CircuitPython", "OpenJDK", etc.
    const char *version;          // runtime version string
    const char *environment;      // "venv", "Global", "conda", etc.
    const char *build_date;       // __DATE__ " " __TIME__ where available
} sysinfo_runtime_static_t;

// Dynamic runtime info — uptime, threads, GC, memory
typedef struct {
    uint32_t uptime_m;            // VM uptime minutes
    uint32_t uptime_s;            // VM uptime seconds
    char     threads[16];         // active thread count
    char     gc_objects[32];      // GC tracked object count where available
    char     memory[16];          // RSS or equivalent memory usage
} sysinfo_runtime_dynamic_t;

// Public API — implemented by platform-specific file
extern void sysinfo_fetch_static(sysinfo_runtime_static_t *dst);
extern void sysinfo_fetch_dynamic(sysinfo_runtime_dynamic_t *dst);

// sysinfo_print() implemented by language-family common file
void sysinfo_print(sysinfo_putline_fn putline, void *ctx);

// Output wrappers — implemented in sysinfo_runtime_common.c
void sysinfo_print_file(FILE *f);
void sysinfo_print_puts(void (*puts_fn)(const char *));

#endif /* RUNTIME_SYSINFO_H */