#include "py/obj.h"
#include "py/runtime.h"
#include "py/mphal.h"
#include "supervisor/shared/translate/translate.h"
#include "supervisor/memory.h"
#include "runtime_sysinfo.h"
#include "logo_circuitpython.h"

// Compiled mode flag
static int compiled_mode = 0;

int embfetch_is_compiled_mode(void) {
    return compiled_mode;
}

// Byte formatting
static void format_size(char *dst, size_t len, size_t bytes) {
    if (bytes >= 1024 * 1024) {
        snprintf(dst, len, "%.1fMB", (double)bytes / (1024.0 * 1024.0));
    } else if (bytes >= 1024) {
        snprintf(dst, len, "%.1fKB", (double)bytes / 1024.0);
    } else {
        snprintf(dst, len, "%uB", (unsigned)bytes);
    }
}

// Static info — CircuitPython specific
void sysinfo_fetch_static(sysinfo_runtime_static_t *dst) {
    dst->implementation = "CircuitPython";

    // sys.version via MicroPython compatible API
    mp_obj_t version_obj = mp_load_attr(
        mp_import_name(MP_QSTR_sys, mp_const_none, MP_OBJ_NEW_SMALL_INT(0)),
        MP_QSTR_version
    );
    dst->version     = mp_obj_str_get_str(version_obj);
    dst->environment = "CircuitPython";  // No venv, always bare metal
    dst->build_date  = __DATE__ " " __TIME__;
}

// Dynamic info — CircuitPython has supervisor memory API on top of MicroPython gc
void sysinfo_fetch_dynamic(sysinfo_runtime_dynamic_t *dst) {
    // Uptime via mp_hal_ticks_ms()
    uint32_t ticks_ms = mp_hal_ticks_ms();
    uint32_t uptime_s = ticks_ms / 1000;
    dst->uptime_m = uptime_s / 60;
    dst->uptime_s = uptime_s % 60;

    // Memory — CircuitPython supervisor exposes Python heap separately
    // from the supervisor heap, giving more accurate Python memory usage
    gc_info_t info;
    gc_info(&info);
    format_size(dst->memory, sizeof(dst->memory), info.used);

    char free_str[16];
    format_size(free_str, sizeof(free_str), info.free);
    snprintf(dst->gc_objects, sizeof(dst->gc_objects), "%s free", free_str);

    // CircuitPython is always single-threaded
    snprintf(dst->threads, sizeof(dst->threads), "1");
}

// stdout putline via CircuitPython's mp_printf
static void cp_putline(void *ctx, const char *line) {
    (void)ctx;
    mp_printf(&mp_plat_print, "%s\n", line);
}

// CircuitPython callable — embfetch.print_info()
STATIC mp_obj_t embfetch_print_info(void) {
    compiled_mode = 0;
    sysinfo_print(cp_putline, NULL);
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_0(embfetch_print_info_obj, embfetch_print_info);

// Module globals table
STATIC const mp_rom_map_elem_t embfetch_module_globals_table[] = {
    { MP_ROM_QSTR(MP_QSTR___name__),   MP_ROM_QSTR(MP_QSTR_embfetch) },
    { MP_ROM_QSTR(MP_QSTR_print_info), MP_ROM_PTR(&embfetch_print_info_obj) },
};
STATIC MP_DEFINE_CONST_DICT(embfetch_module_globals, embfetch_module_globals_table);

// Module definition — CircuitPython uses CIRCUITPY_MODULE registration
const mp_obj_module_t embfetch_module = {
    .base    = { &mp_type_module },
    .globals = (mp_obj_dict_t *)&embfetch_module_globals,
};

// CircuitPython built-in registration — add to circuitpy_mpconfig.h:
// extern const mp_obj_module_t embfetch_module;
// #define CIRCUITPY_EMBFETCH (1)
// And in mpconfigport.h EXTRA_BUILT_IN_MODULE_WEAK_LINKS add embfetch_module
MP_REGISTER_MODULE(MP_QSTR_embfetch, embfetch_module);
// sysinfo_circuitpython.c