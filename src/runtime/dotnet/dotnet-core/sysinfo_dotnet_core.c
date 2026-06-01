#include <stdio.h>
#include <string.h>
#include <time.h>
#include <inttypes.h>
#include <stdint.h>
#include "runtime_sysinfo.h"
#include "logo.h"

// VM start time — captured at library load
static double vm_start_time = 0.0;

// Compiled mode flag
static int compiled_mode = 0;

int embfetch_is_compiled_mode(void) {
    return compiled_mode;
}

// Byte formatting
static void format_size(char *dst, size_t len, size_t bytes) {
    if (bytes >= 1024UL * 1024UL * 1024UL) {
        snprintf(dst, len, "%.1fGB", (double)bytes / (1024.0 * 1024.0 * 1024.0));
    } else if (bytes >= 1024UL * 1024UL) {
        snprintf(dst, len, "%.1fMB", (double)bytes / (1024.0 * 1024.0));
    } else if (bytes >= 1024UL) {
        snprintf(dst, len, "%.1fKB", (double)bytes / 1024.0);
    } else {
        snprintf(dst, len, "%zuB", bytes);
    }
}

// Static info defaults — overridden by .NET side via P/Invoke
void sysinfo_fetch_static(sysinfo_runtime_static_t *dst) {
    dst->implementation = ".NET";
    dst->version        = "Unknown";
    dst->environment    = "Unknown";
    dst->build_date     = __DATE__ " " __TIME__;
}

// Dynamic info defaults — overridden by .NET side via P/Invoke
void sysinfo_fetch_dynamic(sysinfo_runtime_dynamic_t *dst) {
    double now      = (double)clock() / CLOCKS_PER_SEC;
    double uptime_s = now - vm_start_time;
    dst->uptime_m = (uint32_t)(uptime_s / 60.0);
    dst->uptime_s = (uint32_t)((uint64_t)uptime_s % 60);

    snprintf(dst->threads,    sizeof(dst->threads),    "Unknown");
    snprintf(dst->gc_objects, sizeof(dst->gc_objects), "Unknown");
    snprintf(dst->memory,     sizeof(dst->memory),     "Unknown");
}

// P/Invoke output wrapper
static void dotnet_putline(void *ctx, const char *line) {
    (void)ctx;
    printf("%s\n", line);
    fflush(stdout);
}

// Platform-specific export macro
#if defined(_WIN32)
#  define EMBFETCH_EXPORT __declspec(dllexport)
#else
#  define EMBFETCH_EXPORT __attribute__((visibility("default")))
#endif

// P/Invoke export — called from Embfetch.cs PrintInfo()
// All runtime info passed from .NET side via GC.GetTotalMemory(),
// GC.CollectionCount(), Environment.Version, etc.
EMBFETCH_EXPORT void embfetch_print_info(
    const char *version,
    const char *implementation,
    const char *environment,
    int64_t     heap_used,
    int64_t     heap_max,
    int64_t     gc_count,
    int32_t     thread_count)
{
    compiled_mode = 0;

    sysinfo_runtime_static_t  st;
    sysinfo_runtime_dynamic_t dyn;

    sysinfo_fetch_static(&st);
    sysinfo_fetch_dynamic(&dyn);

    // Apply .NET-side values
    st.implementation = implementation ? implementation : ".NET";
    st.version        = version        ? version        : "Unknown";
    st.environment    = environment    ? environment    : "Unknown";

    // Format heap
    char heap_used_str[16], heap_max_str[16];
    format_size(heap_used_str, sizeof(heap_used_str), (size_t)heap_used);
    format_size(heap_max_str,  sizeof(heap_max_str),  (size_t)heap_max);
    snprintf(dyn.memory, sizeof(dyn.memory), "%s used / %s max",
             heap_used_str, heap_max_str);

    snprintf(dyn.gc_objects, sizeof(dyn.gc_objects), "%" PRId64, gc_count);
    snprintf(dyn.threads,    sizeof(dyn.threads),    "%" PRId32, thread_count);

    sysinfo_print(dotnet_putline, NULL);
}

// P/Invoke export — compiled variant, uptime shows "Compiled"
EMBFETCH_EXPORT void embfetch_print_info_compiled(
    const char *version,
    const char *implementation,
    const char *environment,
    int64_t     heap_used,
    int64_t     heap_max,
    int64_t     gc_count,
    int32_t     thread_count)
{
    compiled_mode = 1;
    embfetch_print_info(version, implementation, environment,
                        heap_used, heap_max, gc_count, thread_count);
    compiled_mode = 0;
}

// Library init — capture start time
// Called automatically on shared library load via constructor attribute
#if defined(_WIN32)
BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
    (void)hinstDLL; (void)lpvReserved;
    if (fdwReason == DLL_PROCESS_ATTACH)
        vm_start_time = (double)clock() / CLOCKS_PER_SEC;
    return TRUE;
}
#else
__attribute__((constructor))
static void embfetch_init(void) {
    vm_start_time = (double)clock() / CLOCKS_PER_SEC;
}
#endif
// sysinfo_dotnet_core.c