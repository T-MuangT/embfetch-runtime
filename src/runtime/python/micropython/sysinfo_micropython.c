#include "py/obj.h"
#include "py/runtime.h"
#include "py/mphal.h"
#include "runtime_sysinfo.h"
#include "logo_micropython.h"

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

// Static info — implementation name and version via MicroPython sys module
void sysinfo_fetch_static(sysinfo_runtime_static_t *dst) {
    // sys.implementation.name
    dst->implementation = "MicroPython";

    // sys.version — MicroPython exposes this as a string
    mp_obj_t version_obj = mp_load_attr(
        mp_import_name(MP_QSTR_sys, mp_const_none, MP_OBJ_NEW_SMALL_INT(0)),
        MP_QSTR_version
    );
    dst->version    = mp_obj_str_get_str(version_obj);
    dst->environment = "MicroPython"; // No venv concept on MicroPython
    dst->build_date  = __DATE__ " " __TIME__;
}

// Dynamic info — heap via gc module, no threading on MicroPython
void sysinfo_fetch_dynamic(sysinfo_runtime_dynamic_t *dst) {
    // Uptime via mp_hal_ticks_ms()
    uint32_t ticks_ms = mp_hal_ticks_ms();
    uint32_t uptime_s = ticks_ms / 1000;
    dst->uptime_m = uptime_s / 60;
    dst->uptime_s = uptime_s % 60;

    // Memory via gc_info — MicroPython exposes heap directly
    gc_info_t info;
    gc_info(&info);
    format_size(dst->memory, sizeof(dst->memory), info.used);

    // GC objects — MicroPython reports bytes not object count
    char free_str[16];
    format_size(free_str, sizeof(free_str), info.free);
    snprintf(dst->gc_objects, sizeof(dst->gc_objects), "%s free", free_str);

    // No threading on MicroPython
    snprintf(dst->threads, sizeof(dst->threads), "1");
}

// stdout putline wrapper
static void mp_putline(void *ctx, const char *line) {
    (void)ctx;
    mp_printf(&mp_plat_print, "%s\n", line);
}

// MicroPython callable — embfetch.print_info()
STATIC mp_obj_t embfetch_print_info(void) {
    compiled_mode = 0;
    sysinfo_print(mp_putline, NULL);
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_0(embfetch_print_info_obj, embfetch_print_info);

// Module globals table
STATIC const mp_rom_map_elem_t embfetch_module_globals_table[] = {
    { MP_ROM_QSTR(MP_QSTR___name__),   MP_ROM_QSTR(MP_QSTR_embfetch) },
    { MP_ROM_QSTR(MP_QSTR_print_info), MP_ROM_PTR(&embfetch_print_info_obj) },
};
STATIC MP_DEFINE_CONST_DICT(embfetch_module_globals, embfetch_module_globals_table);

// Module definition
const mp_obj_module_t embfetch_user_cmodule = {
    .base    = { &mp_type_module },
    .globals = (mp_obj_dict_t *)&embfetch_module_globals,
};

MP_REGISTER_MODULE(MP_QSTR_embfetch, embfetch_user_cmodule);
// sysinfo_micropython.c