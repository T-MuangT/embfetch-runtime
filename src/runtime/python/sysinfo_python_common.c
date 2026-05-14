#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include "runtime_sysinfo.h"
#include "logo.h"

// Extern — implemented by runtime-specific file (sysinfo_cpython.c, sysinfo_micropython.c, etc.)
extern void sysinfo_fetch_static(sysinfo_runtime_static_t *dst);
extern void sysinfo_fetch_dynamic(sysinfo_runtime_dynamic_t *dst);

// Extern — compiled mode flag, set by print_info_compiled()
extern int embfetch_is_compiled_mode(void);

// Print logo and info — shared across all Python family runtimes
void sysinfo_print(sysinfo_putline_fn putline, void *ctx) {
    sysinfo_runtime_static_t  st;
    sysinfo_runtime_dynamic_t dyn;

    sysinfo_fetch_static(&st);
    sysinfo_fetch_dynamic(&dyn);

    char header[64], separator[32];
    char impl_line[64], ver_line[64], uptime_line[64];
    char env_line[64], threads_line[64], gc_line[64], mem_line[64];

    snprintf(header,       sizeof(header),       "%s", st.implementation);
    snprintf(separator,    sizeof(separator),     "----------------");
    snprintf(impl_line,    sizeof(impl_line),     "Implementation:  %s", st.implementation);
    snprintf(ver_line,     sizeof(ver_line),      "Version:         %s", st.version);
    if (embfetch_is_compiled_mode()) {
        snprintf(uptime_line, sizeof(uptime_line), "Uptime:          Compiled");
    } else {
        snprintf(uptime_line, sizeof(uptime_line), "Uptime:          %" PRIu32 "h %" PRIu32 "m %" PRIu32 "s",
                 dyn.uptime_h, dyn.uptime_m, dyn.uptime_s);
    }
    snprintf(env_line,     sizeof(env_line),      "Environment:     %s", st.environment);
    snprintf(threads_line, sizeof(threads_line),  "Threads:         %s active", dyn.threads);
    snprintf(gc_line,      sizeof(gc_line),       "GC Objects:      %s", dyn.gc_objects);
    snprintf(mem_line,     sizeof(mem_line),      "Memory:          %s RSS", dyn.memory);

    const char *info[] = {
        header, separator, impl_line, ver_line,
        uptime_line, env_line, threads_line,
        gc_line, mem_line
    };

    int logo_count = 0;
    while (logo[logo_count] != NULL) logo_count++;
    int info_count = sizeof(info) / sizeof(info[0]);
    int total_lines = (logo_count > info_count) ? logo_count : info_count;

    for (int i = 0; i < total_lines; i++) {
        const char *logo_part = (i < logo_count) ? logo[i] : "";
        const char *info_part = (i < info_count) ? info[i] : "";

        char line[256];
        if (info_part[0] != '\0')
            snprintf(line, sizeof(line), "%s" LOGO_INFO_COLUMN "%s", logo_part, info_part);
        else
            snprintf(line, sizeof(line), "%s", logo_part);
        putline(ctx, line);
    }
} // sysinfo_python_common.c